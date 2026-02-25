/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DSP2_Project4AudioProcessor::DSP2_Project4AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    /*
        Initialize audio parameters variables.
    */
    gainKnobParameter = new juce::AudioParameterFloat(juce::ParameterID("1", 1), "GainSlider", -12.f, 12.f, 0);
	fcenterSliderParameter = new juce::AudioParameterInt(juce::ParameterID("2", 2), "FcenterSlider", 35, 18000, 4000);
	qSliderParameter = new juce::AudioParameterFloat(juce::ParameterID("3", 3), "QSlider", 0.1f, 10.f, 1.0f);


    /*
        Add audio parameters to parameter tree.
    */

	addParameter(gainKnobParameter);
	addParameter(fcenterSliderParameter);
	addParameter(qSliderParameter);


    // Set current variables to use for checking.
    currentGain = getParameter(0);
	currentFCenter = getParameter(1);
	currentQ = getParameter(2);

    // Update/compute coefficients.

    // Initialize arrays with zeros.


    /*
        Initialize any properties of effect classes.
        (This is more for Final Project than Project 4.)
    */
    eq.reset();
	eq.setGain(currentGain);
	eq.setCenterFrequency(currentFCenter);
	eq.setQFactor(currentQ);
	eq.setSampleRate(getSampleRate());
	eq.updateCoefficients();
	/*
		Initialize any other variables.
	*/
	// ...

}

DSP2_Project4AudioProcessor::~DSP2_Project4AudioProcessor()
{/*
		Delete any pointers to avoid memory leaks.
	*/
	/*
		Delete any other pointers.
	*/
	// ...
}

//==============================================================================
const juce::String DSP2_Project4AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DSP2_Project4AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DSP2_Project4AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DSP2_Project4AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DSP2_Project4AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DSP2_Project4AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DSP2_Project4AudioProcessor::getCurrentProgram()
{
    return 0;
}

void DSP2_Project4AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DSP2_Project4AudioProcessor::getProgramName (int index)
{
    return {};
}

void DSP2_Project4AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DSP2_Project4AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need...

    /*
        Initialize any variables or effect class properties.
    */
	eq.setGain(currentGain);
	eq.setCenterFrequency(currentFCenter);
	eq.setQFactor(currentQ);
	eq.setSampleRate(sampleRate);
}

void DSP2_Project4AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DSP2_Project4AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DSP2_Project4AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Check to see if parameters have changed.
    // ...
    float newGain = gainKnobParameter->get();
    int newFreq = fcenterSliderParameter->get();
    float newQ = qSliderParameter->get();

    if (newGain != currentGain || newFreq != currentFCenter || newQ != currentQ)
    {
        currentGain = newGain;
        currentFCenter = newFreq;
        currentQ = newQ;

        eq.setGain(currentGain);
        eq.setCenterFrequency(currentFCenter);
        eq.setQFactor(currentQ);
        eq.updateCoefficients();
    }



    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        /*
            Place the difference equation here.
        */
		for (int i = 0; i < buffer.getNumSamples(); ++i)
		{
			// Apply the filter to the input sample
			channelData[i] = eq.processSample(channelData[i]);
		}
    }
}

//==============================================================================
bool DSP2_Project4AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DSP2_Project4AudioProcessor::createEditor()
{
    return new DSP2_Project4AudioProcessorEditor (*this);
}

//==============================================================================
void DSP2_Project4AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DSP2_Project4AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DSP2_Project4AudioProcessor();
}



void DSP2_Project4AudioProcessor::updateCoefficients()
{
    // ...
    
}

void DSP2_Project4AudioProcessor::resetDelays()
{
    // ...
    
}