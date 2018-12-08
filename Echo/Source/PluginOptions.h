/*
  ==============================================================================

    PluginOptions.h
    Created: 8 Dec 2018 10:58:50am
    Author:  Erik Whipp

  ==============================================================================
*/

#pragma once
class PluginOptions {
    
    double getSampleRate() {
        return _sampleRate;
    }
    
    double getBlockSize() {
        return _blockSize;
    }
    
    void setBlockSize(double blockSize) {
        _blockSize = blockSize;
    }
    
    void setSampleRate(double sampleRate) {
        _sampleRate = sampleRate;
    }
    
private:
    double _sampleRate;
    double _blockSize;
};
