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

    ReverbProcessor (int blocksize, int sampleRate)
    {
        pluginWindowSize = blocksize;
        pluginSampleRate = sampleRate;
        doubleWindowSize = pluginWindowSize * 2;
    }
    
    ~ReverbProcessor ()
    {
        fftIRResults = nullptr;
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
     * 10) Destroy the plan
     */
    void getImpulseResponseFileFFT ()
    {
        // [1]
        FileBuffer::Ptr processAudioBuffer (currentBuffer);
        auto* temp = currentBuffer->getAudioSampleBuffer ();
        int FILESIZE = temp->getNumSamples ();
        
        // [2]
        setNumPartitons (FILESIZE);
        sampleOverlapSize = 0;
        
        // [3]
        sampleOverlapSize = (getNumPartitions () * doubleWindowSize);
        for (int i = 0; i < sampleOverlapSize; i++)
            nonSampleOverlap[i] = 0.0;
        
        // [4]
        fftIRResults = (fftwf_complex*) fftwf_alloc_complex (getNumPartitions () * pluginWindowSize + 1);
        fftOutput    = (fftwf_complex*) fftwf_alloc_complex (pluginWindowSize + 1);
        fftInput[doubleWindowSize * getNumPartitions ()];
        fftwPlan = fftwf_plan_dft_r2c_1d (doubleWindowSize, fftInput, fftOutput, FFTW_ESTIMATE);
        
        // [5]
        for (int i = 0; i < doubleWindowSize; i++)
            fftInput[i] = 0.0;
        
        // [6]
        for (int i = 0; i < getNumPartitions (); i++)
        {
            int k, j, startSpec;
            startSpec = i * pluginWindowSize + 1;
            
            // [7]
            for (j = 0; j < temp->getNumChannels (); j++)
                for (k = 0; k < temp->getNumSamples (); k++)
                    fftInput[k] = temp->getSample (j, k);
            
            // [8]
            for (; k < doubleWindowSize; k++)
                fftInput[k] = 0.0;
            
            fftwf_execute (fftwPlan);
            
            // [9]
            for (j = 0; j < pluginWindowSize + 1; j++)
            {
                fftIRResults[startSpec + j][0] = fftOutput[j][0];
                fftIRResults[startSpec + j][1] = fftOutput[j][1];
            }
        }
        
        // [10]
        fftwf_destroy_plan(fftwPlan);
        fftwf_free(fftOutput);
    }
    
    // Set the number of partitions for the plugin
    void setNumPartitons (int numSamples)
    {
        numPartitions = 0;
        while ((numPartitions * pluginWindowSize) < numSamples)
            numPartitions++;
    }
    
    // For when we want to clear a specific buffer
    void clearSampleArray (float* arrayToClear[])
    {
        int partitions = getNumPartitions ();
        int windowSize = getPartitionSize ();
        for (int i = 0; i < (partitions * (windowSize + 1)); i++)
        {
            arrayToClear[i][0] = 0.0;
            arrayToClear[i][1] = 0.0
        }
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
    int windowSize;             // numSamples of IR / block size
    int doubleWindowSize;       // Double the window size
    int pluginWindowSize;       // block size
    int pluginSampleRate;       // sample rate
    int numPartitions;          // number of partitions
    int sampleOverlapSize;      // Where the samples will overlap with each other
    float nonSampleOverlap[];   // Where the samples are in use ??
    float *fftInput;
    
    // fftwf_complex fftResults[NUMPARTITIONS][Real][Imag]
    fftwf_complex* fftOutput;                // Used for the FFTW plan
    fftwf_complex* fftIRResults;             // IR results complex array
    fftwf_complex* fftPlaceHolder;          // unknown for now
    
    fftwf_plan fftwPlan;
    
    // Impulse response will be split based on length of IR audio = Numpartitions
    
    // Partition size = Block size
    
    // fftw results is 2 * partition size with second half zeroed out
    
    // FFTresults should be (N / 2 + 1) * NumPartitions --> if its 2D array
    
    // do we want fftResults to be 2D --> 2D because it is more efficient
};
