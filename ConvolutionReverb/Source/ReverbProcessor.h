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
    
    ReverbProcessor (int partitionSize1, int sampleRate1 )
    {
        partitionSize = partitionSize1;
        doubleWindowSize = partitionSize * 2;
        fftCurrentProcessBlockPlan = fftwf_plan_dft_r2c_1d (doubleWindowSize, currentAudio,                                                            fftCurrentBlockOutput, FFTW_ESTIMATE);
        fftCurrentBlockOutput  = (fftwf_complex*) fftwf_malloc ((partitionSize + 1) * sizeof (fftwf_complex*));
        currentProcessBlockFFT = (fftwf_complex*) fftwf_malloc ((getNumPartitions()) * (partitionSize + 1) * (partitionSize + 1) * (sizeof (fftwf_complex*)));
        currentAudio = new float[doubleWindowSize];
        pluginSampleRate = sampleRate1;
    }
    
    ~ReverbProcessor()
    {
        fftwf_free (fftIROutput);
        fftwf_free (fftIRResult);
        fftwf_free (fftIRResult);
        fftwf_free (fftCurrentBlockOutput);

        fftwf_destroy_plan (fftIRFilePlan);
        fftwf_destroy_plan(fftCurrentProcessBlockPlan);
        delete[] currentAudio;
        currentAudio = nullptr;
        
    }

    // Get the current blocks FFT
    void computeRealTimeFFT ()
    {
        if (isNextBlockReady == true)
        {
            // Zero pad the buffer
            for (int i = partitionSize; i < doubleWindowSize; i++)
                currentAudio[i] = 0.0;
            
            std::cout << "Successfully padded the buffer\n";
            fftwf_execute (fftCurrentProcessBlockPlan);
            std::cout << "Successfully executed the FFT\n";
            
            for (int j = 0; j < partitionSize + 1; j++)
            {
                currentProcessBlockFFT[j][0] = fftCurrentBlockOutput[j][0];
                currentProcessBlockFFT[j][1] = fftCurrentBlockOutput[j][1];
                std::cout << "Real time FFT REAL: " << currentProcessBlockFFT[access3DArrayPosition (0)][0] << "\tImaginary: " << currentProcessBlockFFT[0][access3DArrayPosition (0)] << "\n";
            }
            isNextBlockReady = false;
            std::cout << "Computed the FFT of real time data.\n";
        }
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
        AudioSampleBuffer* temp = currentBuffer->getAudioSampleBuffer ();
        int FILESIZE = temp->getNumSamples();
        
        // [2]
        setNumPartitons (FILESIZE);
        sampleOverlapSize = 0;
        
        // [3]
        sampleOverlapSize = (getNumPartitions() * doubleWindowSize);
        
        // [4]
        fftIRResult   = (fftwf_complex*) fftwf_alloc_complex(getNumPartitions() * (partitionSize + 1));
        fftIROutput   = (fftwf_complex*) fftwf_alloc_complex(partitionSize + 1);
        fftIRInput    = new float[doubleWindowSize];
        fftIRFilePlan = fftwf_plan_dft_r2c_1d (doubleWindowSize, fftIRInput, fftIROutput, FFTW_ESTIMATE);
        
        // [5]
        for (int i = 0; i < doubleWindowSize; i++)
            fftIRInput[i] = 0.0;
        
        // [6]
        for (int currentPartition = 0; currentPartition < getNumPartitions(); currentPartition++)
        {
            int IRoffset = currentPartition * partitionSize;
            
            // [7]
            fillSampleBasedOnPartition(temp, fftIRInput, IRoffset);
            
            fftwf_execute (fftIRFilePlan);
            
            // [9]
            for (int j = 0; j < partitionSize + 1; j++)
            {
                fftIRResult[IRoffset + j][0] = fftIROutput[j][0];
                fftIRResult[IRoffset + j][1] = fftIROutput[j][1];
            }
        }
        delete []fftIRInput;
        fftIRInput = nullptr;
    }

    
    // Helper function for accessing a 3D array
    int access3DArrayPosition (int currentPartition)
    {
        int y = partitionSize + 1;
        int z = partitionSize + 1;
        return (z * currentPartition * y) + (y * currentPartition) + currentPartition;
    }

    
    // Set the number of partitions for the plugin
    void setNumPartitons (int numSamples)
    {
        numPartitions = 0;
        
        while ((numPartitions * partitionSize) < numSamples)
            numPartitions++;
    }
    
    // We calculate our convolution based on the current partition. This takes the current
    // buffer and offsets where we start filling the array based on our current partition.
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
    
    void setPartitionSize (int N)
    {
        
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
    int windowSize;                             // NumSamples of IR / block size
    int doubleWindowSize;                       // Double the window size
    int partitionSize;                          // Block size
    int pluginSampleRate;                       // Sample rate
    int numPartitions;                          // Number of partitions
    int sampleOverlapSize;                      // Where the samples will overlap with each other
    int currentIndex = 0;                       // Current index as we are filling our buffer
    int currentAudioQueueTop = 0;               // Top value of the current Queue

    float* nonSampleOverlap;                    // Where the samples are in use ??
    float* fftIRInput;                          // Array of the IR Input's samples.
    float* currentAudio;                        // Buffer that holds the most recent block of samples
    
    bool isNextBlockReady;                      // boolean to tell us when to perform the FFT on current samples
    
    fftwf_complex* fftIROutput;                 // Used for the output of the IR files FFTW plan
    fftwf_complex* fftIRResult;                 // IR results complex array
    fftwf_complex* currentProcessBlockFFT;      // The current processing block
    fftwf_complex* fftCurrentBlockOutput;       // Used for the output of the current blocks FFTW plan
    
    
    fftwf_plan fftIRFilePlan;                   // Plan for the impulse response file
    fftwf_plan fftCurrentProcessBlockPlan;      // Plan for the most recent block of audio
};
