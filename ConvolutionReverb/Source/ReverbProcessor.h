/*
  ==============================================================================

    ReverbProccesor.h
    Created: 26 Mar 2018 5:04:01pm
    Author:  Erik Whipp

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "FileBuffer.h"
#include <fftw3.h>


class ReverbProcessor
{
public:
    
    /*
     * 1) Init the sample rate and partition size and thus, the double partition size
     * 2) Allocate memory for fft routines and buffer for current block
     * 3) Setting up FFT plan
     */
    ReverbProcessor (int partitionSize1, int sampleRate1)
    {
        // [1]
        partitionSize    = partitionSize1;
        doubleWindowSize = partitionSize * 2;
        pluginSampleRate = sampleRate1;
        scaleOutputGain  = 1.0f / doubleWindowSize;

        
       // [2]
        fftCurrentBlockOutput = (fftwf_complex*) fftwf_alloc_complex(partitionSize + 1);
        fftLiveValues         = (fftwf_complex*) fftwf_alloc_complex (partitionSize + 1);
        inputInverseFFTBuffer = (fftwf_complex*) fftwf_alloc_complex (partitionSize + 1);
        
        currentAudio     = new float[doubleWindowSize];
        outputIFFT       = new float[doubleWindowSize];
        nonOverlapOutput = new float[2 * doubleWindowSize];
        convolutedOutput = new float[partitionSize1];
        
        // init overlap to 0
        for (int i = 0; i < 2 * doubleWindowSize; i++)
            nonOverlapOutput[i] = 0.0;
        
        // [3]
        fftCurrentProcessBlockPlan = fftwf_plan_dft_r2c_1d (doubleWindowSize, currentAudio, fftCurrentBlockOutput, FFTW_ESTIMATE);
        fftFinalFreqToTimePlan     = fftwf_plan_dft_c2r_1d (doubleWindowSize, inputInverseFFTBuffer, outputIFFT, FFTW_ESTIMATE);
    }
    
    ~ReverbProcessor()
    {
        fftwf_free (fftIRResult);
        fftwf_free (fftCurrentBlockOutput);
        fftwf_free (liveFrequencyDomData);
        fftwf_free (fftLiveValues);
        fftwf_free (inputInverseFFTBuffer);

        fftwf_destroy_plan(fftCurrentProcessBlockPlan);
        fftwf_destroy_plan(fftFinalFreqToTimePlan);
        
        delete[] currentAudio;
        currentAudio = nullptr;
        delete[] outputIFFT;
        outputIFFT = nullptr;
        delete[] convolutedOutput;
        convolutedOutput = nullptr;
        delete[] nonOverlapOutput;
        nonOverlapOutput = nullptr;
    }
    
    /*
     * 1) Get pointer to the file from the file buffer
     * 2) Set the number of partitions and the overlap
     * 3) Calculate the sample overlap for the convolution preparation
     * 4) Prepare the FFT
     * 5) Init the input array after the plan has been defined
     * 6) Execute the FFT
     * 7) Get the samples from the file
     * 8) Zero pad the fftInput input
     * 9) Move the values to the output FFT array
     */
    void getImpulseResponseFileFFT (FileBuffer::Ptr currentBuffer)
    {
        // [1]
        AudioSampleBuffer* currentFileBuffer = currentBuffer->getAudioSampleBuffer ();
        int FILESIZE = currentFileBuffer->getNumSamples();
        
        // [2]
        setNumPartitons (FILESIZE);
        sampleOverlapSize = 0;
        
        // [3]
        sampleOverlapSize = (getNumPartitions() * doubleWindowSize);
        
        // [4]
        fftIRResult           = (fftwf_complex*) fftwf_alloc_complex (getNumPartitions() * (partitionSize + 1));
        liveFrequencyDomData  = (fftwf_complex*) fftwf_alloc_complex (getNumPartitions() * (partitionSize + 1));

        fftIROutput   = (fftwf_complex*) fftwf_alloc_complex (partitionSize + 1);
        fftIRInput    = new float[doubleWindowSize];
        fftIRFilePlan = fftwf_plan_dft_r2c_1d (doubleWindowSize, fftIRInput, fftIROutput, FFTW_ESTIMATE);
        
        // [5]
        for (int i = 0; i < doubleWindowSize; i++)
            fftIRInput[i] = 0.0;
        
        // [6]
        for (int currentPartition = 0; currentPartition < getNumPartitions(); currentPartition++)
        {
            int IRoffset = currentPartition * (partitionSize + 1);
            
            // [7]
            fillSampleBasedOnPartition (currentFileBuffer, fftIRInput, IRoffset);
            
            fftwf_execute (fftIRFilePlan);
            
            // [9]
            for (int j = 0; j < (partitionSize + 1); j++)
            {
                fftIRResult[IRoffset + j][0] = fftIROutput[j][0];
                fftIRResult[IRoffset + j][1] = fftIROutput[j][1];
            }
        }
        
        delete []fftIRInput;
        fftIRInput = nullptr;
        fftwf_destroy_plan (fftIRFilePlan);
        fftwf_free (fftIROutput);
    }
    
    /*
     * 1) Only begin analyzing if the IR has been analyzed and the current audio buffer is ready
     * 2) Initialize the FFT plan
     * 3) Zero pad the second half of the current audio's buffer
     * 4) Execute the FFT plan created in the constructor
     * 5) Save the current complex values in the currentBlock's FFT array
     */
    void computeRealTimeFFT ()
    {
        // [1]
        if (isNextBlockReady == true && numPartitions > 0)
        {
            // [2]
            fftCurrentProcessBlockPlan = fftwf_plan_dft_r2c_1d (doubleWindowSize, currentAudio,                                                            fftCurrentBlockOutput, FFTW_ESTIMATE);
            
            // [3]
            for (int i = partitionSize; i < doubleWindowSize; i++)
                currentAudio[i] = 0.0;
            
            // [4]
            fftwf_execute (fftCurrentProcessBlockPlan);
            
            // [5]
            for (int j = 0; j < partitionSize + 1; j++)
            {
                
                fftLiveValues[j][0] = fftCurrentBlockOutput[j][0];
                fftLiveValues[j][1] = fftCurrentBlockOutput[j][1];
            }
            
            isNextBlockReady = false;
            beginAccumulation = true;
        }
    }

    /*
     * 1) Begin iteration through each partition
     * 2) Accumulate the values of the IR + Current live block in liveFrequencyDomData buffer
     * 3) Copy to our inverse FFT buffer.
     * 4) Execute the IFFT plan
     */
    void accumulateComplexValues()
    {
        if (beginAccumulation == true)
        {
            // [1]
            for (int currentPartition = 0; currentPartition < getNumPartitions(); currentPartition++)
            {
                int operationOffset = currentPartition * (partitionSize + 1);
                
                // [2]
                for (int i = 0; i < (partitionSize + 1); i++)
                {
                    float realCB, imaginaryCB, realIR, imaginaryIR, realAcc, imaginaryAcc;
                    
                    realCB      = fftLiveValues[i][0];
                    imaginaryCB = fftLiveValues[i][1];
                    
                    realIR      = fftIRResult[operationOffset + i][0];
                    imaginaryIR = fftIRResult[operationOffset + i][1];
                    
                    realAcc      = (realCB * realIR) - (imaginaryCB * imaginaryIR);
                    imaginaryAcc = (imaginaryCB * realIR) + (realCB * imaginaryIR);

                    liveFrequencyDomData[operationOffset + i][0] += realAcc;
                    liveFrequencyDomData[operationOffset + i][1] += imaginaryAcc;
                }
            }
            
            // [3]
            for (int i = 0; i < (partitionSize + 1); i++)
            {
                inputInverseFFTBuffer[i][0] = liveFrequencyDomData[i][0];
                inputInverseFFTBuffer[i][1] = liveFrequencyDomData[i][1];
            }
            
            fftwf_execute (fftFinalFreqToTimePlan);
            beginOutputConvolution = true;
        }
    }
    
    /*
     * 1) Copy the latest IFFT result into the second block of the nonOverlapped array, first block contains most recent output.
     * 2) Write to the output array and reduce the gain
     * 3) Push the live frequency domain buffer contents backwards
     * 4) Init the new chunk of live frequency domain buffer
     * 5) Push remaining output buffer backwards
     * 6) Init the new output buffer at the end
     */
    float* outputConvolution()
    {
        // [1]
        if (beginOutputConvolution == true)
        {
            for (int i = 0; i < doubleWindowSize; i++)
                nonOverlapOutput[i + doubleWindowSize] = outputIFFT[i];
            
            // [2]
            for (int i = 0; i < partitionSize; i++)
            {
                convolutedOutput[i] = nonOverlapOutput[partitionSize + i] + nonOverlapOutput[doubleWindowSize + i];
                convolutedOutput[i] *= scaleOutputGain;
            }
            
            // [3]
            for (int i = 0; i < ((getNumPartitions() * (partitionSize + 1)) - (partitionSize + 1)); i++)
            {
                liveFrequencyDomData[i][0] = liveFrequencyDomData[(partitionSize + 1) + i][0];
                liveFrequencyDomData[i][1] = liveFrequencyDomData[(partitionSize + 1) + i][1];
            }
            
            // [4]
            for (int i = 0; i < (getNumPartitions() * (partitionSize + 1)); i++)
            {
                liveFrequencyDomData[i][0] = 0.0;
                liveFrequencyDomData[i][1] = 0.0;
            }
            
            // [6]
            for (int i = 0; i < doubleWindowSize; i++)
                nonOverlapOutput[i] = nonOverlapOutput[doubleWindowSize  + i];
            
            for (int i = doubleWindowSize; i < (2 * doubleWindowSize); i++)
                nonOverlapOutput[i] = 0.0;
            
            return convolutedOutput;
        }
        
        return currentAudio;
    }
    
    // Set the number of partitions for the plugin
    void setNumPartitons (int numSamples)
    {
        numPartitions = 0;
        
        while ((numPartitions * partitionSize) < numSamples)
            numPartitions++;
    }
    
    /*
     * We calculate our convolution based on the current partition. This takes the current
     * buffer and offsets where we start filling the array based on our current partition.
     */
    void fillSampleBasedOnPartition (AudioSampleBuffer* buffer, float* arrayToFill,
                                     int currentPartition)
    {
        int i = 0;
        int j = 0;
        
        for (i = 0; i < buffer->getNumChannels(); i++)
            for (j = 0; j < partitionSize; j++)
                if (currentPartition + j < buffer->getNumSamples())
                    arrayToFill[j] = buffer->getSample (i, currentPartition + j);
        
        for (; j < doubleWindowSize; j++)
            arrayToFill[j] = 0.0;
    }
    
    // Fill an array with the current buffer of audio
    void setCurrentAudio(float currentSample)
    {
        currentAudio[currentIndex++] = currentSample;
        
        if (currentIndex == partitionSize)
        {
            isNextBlockReady = true;
            currentIndex = 0;
        }
    }
    
    /* GETTERS AND SETTERS */
    void setPartitionSize (int N)
    {
        partitionSize = N;
    }
    
    void setSampleRate (int N)
    {
        pluginSampleRate = N;
    }
    
    int getPartitionSize()
    {
        return partitionSize;
    }
    
    int getSampleRate()
    {
        return pluginSampleRate;
    }
    
    int getNumPartitions()
    {
        return numPartitions;
    }
    
private:
    int windowSize = 0;                         // How many samples we will process at a time
    int doubleWindowSize = 0;                   // Double the window size
    int partitionSize = 0;                      // Block size
    int pluginSampleRate = 0;                   // Sample rate
    int numPartitions = 0;                      // Number of partitions
    int sampleOverlapSize = 0;                  // Where the samples will overlap with each other
    int currentIndex = 0;                       // Current index as we are filling our buffer
    int currentAudioQueueTop = 0;               // Top value of the current Queue
    
    float scaleOutputGain = 0.0;                // Scale the output gain of the plugin

    float* nonOverlapOutput;                    // Where the samples are in use ??
    float* fftIRInput;                          // Array of the IR Input's samples.
    float* currentAudio;                        // Buffer that holds the most recent block of samples
    float* outputIFFT;                          // Ultimate output data
    float* convolutedOutput;                    // Final output of the plugin
    
    bool isNextBlockReady = false;              // boolean to tell us when to perform the FFT on current samples
    bool beginAccumulation = false;             // begin accumulation
    bool beginOutputConvolution = false;        // begin outputting the convolution
    
    fftwf_complex* fftIROutput;                 // Used for the output of the IR files FFTW plan
    fftwf_complex* fftIRResult;                 // IR results complex array
    fftwf_complex* fftCurrentBlockOutput;       // Used for the output of the current blocks FFTW plan
    fftwf_complex* fftLiveValues;               // The current processing block
    fftwf_complex* liveFrequencyDomData;        // Current frequency domain data.
    fftwf_complex* inputInverseFFTBuffer;       // Inverse FFT Buffer
    
    fftwf_plan fftIRFilePlan;                   // Plan for the impulse response file
    fftwf_plan fftCurrentProcessBlockPlan;      // Plan for the most recent block of audio
    fftwf_plan fftFinalFreqToTimePlan;          // IFFT Plan
};
