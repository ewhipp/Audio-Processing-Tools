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
    ReverbProcessor () {}
    
    ~ReverbProcessor ()
    {
        fftIRResult = nullptr;
        fftwf_destroy_plan(fftwPlan);
        fftwf_free(fftOutput);
        fftwf_free (fftIRResult);
        delete []fftIRInput;
        fftIRInput = nullptr;
    }
    
    // Get the current blocks FFT
    void computeRealTimeFFT (float currentAudio, int blocksize)
    {
        
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
        int FILESIZE = temp->getNumSamples ();
        
        // [2]
        setNumPartitons (FILESIZE);
        sampleOverlapSize = 0;
        
        // [3]
        sampleOverlapSize = (getNumPartitions () * doubleWindowSize);
        
//        std::cout << "SETTING NON SAMPLE OVERLAP\n";
//        for (int i = 0; i < sampleOverlapSize; i++)
//            nonSampleOverlap[i] = 0.0;
        
        // [4]
        fftIRResult = (fftwf_complex*) fftwf_malloc ((getNumPartitions ()) * (pluginWindowSize + 1) * (pluginWindowSize + 1) * (sizeof (fftwf_complex*)));
        fftOutput   = (fftwf_complex*) fftwf_malloc ((pluginWindowSize + 1) * sizeof (fftwf_complex*));
        fftIRInput = new float[doubleWindowSize * getNumPartitions ()];
        fftwPlan = fftwf_plan_dft_r2c_1d (doubleWindowSize, fftIRInput, fftOutput, FFTW_ESTIMATE);
        
        // [5]
        for (int i = 0; i < doubleWindowSize; i++)
            fftIRInput[i] = 0.0;
        
        // [6]
        for (int currentPartition = 0; currentPartition < getNumPartitions (); currentPartition++)
        {
            int startSpec = currentPartition * pluginWindowSize + 1;
            
            // [7]
            fillSampleBasedOnPartition(temp, fftIRInput, startSpec);
            
            fftwf_execute (fftwPlan);
            
            // [9]
            for (int j = 0; j < pluginWindowSize + 1; j++)
            {
                fftIRResult[access3DArrayPosition(currentPartition)][0] = fftOutput[j][0];
                fftIRResult[0][access3DArrayPosition(currentPartition)] = fftOutput[j][1];
                /*
                std::cout << "\nfftIR current results:: R = " << fftIRResult[access3DArrayPosition(currentPartition)][0] << "\tC = " << fftIRResult[0][access3DArrayPosition(currentPartition)] << "\n";
                std::cout << "\nReal = " << fftOutput[j][0] << "\t Complex = " << fftOutput[j][1] << "\n"; */
            }
        }
    }
    
    // Helper function for accessing a 3D array
    int access3DArrayPosition(int currentPartition)
    {
        
        int y = pluginWindowSize + 1;
        int z = pluginWindowSize + 1;
        return (z * currentPartition * y) + (y * currentPartition) + currentPartition;
    }

    
    // Set the number of partitions for the plugin
    void setNumPartitons (int numSamples)
    {
        numPartitions = 0;
        while ((numPartitions * pluginWindowSize) < numSamples)
            numPartitions++;
    }
    
    void fillSampleBasedOnPartition (AudioSampleBuffer* buffer, float* arrayToFill,
                                     int currentPartition)
    {
        int i = 0;
        int j = 0;
        for (i = 0; i < buffer->getNumChannels (); i++)
            for (j = 0; j < pluginWindowSize; j++)
                arrayToFill[j] = buffer->getSample(i, currentPartition + j);
        
        for (; j < doubleWindowSize; j++)
            arrayToFill[j] = 0.0;
    }
    
    
    void setPartitionSize (int N)
    {
        pluginWindowSize = N;
        doubleWindowSize = N * 2;
    }
    
    void setSampleRate (int N)
    {
        pluginSampleRate = N;
    }
    
    int getPartitionSize ()
    {
        return pluginWindowSize;
    }
    
    int getSampleRate ()
    {
        return pluginSampleRate;
    }
    
    int getNumPartitions ()
    {
        return numPartitions;
    }
    
private:
    int windowSize;             // NumSamples of IR / block size
    int doubleWindowSize;       // Double the window size
    int pluginWindowSize;       // Block size
    int pluginSampleRate;       // Sample rate
    int numPartitions;          // Number of partitions
    int sampleOverlapSize;      // Where the samples will overlap with each other
    float* nonSampleOverlap;    // Where the samples are in use ??
    float* fftIRInput;          // Array of the IR Input's samples.
    
    fftwf_complex* fftOutput;               // Used for the FFTW plan
    fftwf_complex* fftIRResult;             // IR results complex array
    fftwf_complex* currentProcessBlock;     // The current processing block
    fftwf_plan fftwPlan;
};
