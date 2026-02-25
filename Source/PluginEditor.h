/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DSP2_Project4AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DSP2_Project4AudioProcessorEditor (DSP2_Project4AudioProcessor&);
    ~DSP2_Project4AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DSP2_Project4AudioProcessor& audioProcessor;

    // Declare your sliders, labels, and EQ parameters here.
    // ...
    juce::Slider gainKnob;
    juce::Label gainLabel;
    juce::Slider fcenterSlider;
	juce::Label fcenterLabel;
    juce::Slider qSlider;
	juce::Label qLabel;

    // Declaring AudioProcessor Parameters
    juce::AudioParameterFloat* gainKnobParameter;
	juce::AudioParameterInt* fcenterSliderParameter;
	juce::AudioParameterFloat* qSliderParameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSP2_Project4AudioProcessorEditor)
};
