/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ParametricEQ.h"
//==============================================================================
/**
*/
class DSP2_Project4AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DSP2_Project4AudioProcessor();
    ~DSP2_Project4AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Declare two methods, updateCoefficients() and resetDelays() here.
    void updateCoefficients();
    
    void resetDelays();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSP2_Project4AudioProcessor)

    /*
        Declare Audio Parameters Variables
    */
	
	juce::AudioParameterFloat* gainKnobParameter;
	juce::AudioParameterInt* fcenterSliderParameter;
	juce::AudioParameterFloat* qSliderParameter;

    float currentGain;
    int currentFCenter;
    float currentQ;

    /*
        Declare Effect Classes
        (This is more for Final Project than Project 4.)
    */

    ParametricEQ eq;

};
