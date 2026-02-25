/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DSP2_Project4AudioProcessorEditor::DSP2_Project4AudioProcessorEditor (DSP2_Project4AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);



    /*
        The constructor of the AudioProcessorEditor class is used to define the properties of the GUI components that you want to add in
        These components can include Sliders, Knobs, TextBoxes, SubWindows etc.
    */

    /*
        Step 1 : You have to push your GUI components onto the Component Tree and make them visible
        To do that we use the addAndMakeVisible() function.
    */
	addAndMakeVisible(gainKnob);
	addAndMakeVisible(gainLabel);
	addAndMakeVisible(fcenterSlider);
	addAndMakeVisible(fcenterLabel);
	addAndMakeVisible(qSlider); 
	addAndMakeVisible(qLabel);

    /*
        Step 2: Use the functions defined for each GUI component class to stylise your components in the way you want it to be
    */

	gainKnob.setSliderStyle(juce::Slider::Rotary);
    gainKnob.setColour(juce::Slider::thumbColourId, juce::Colours::darksalmon);
	gainKnob.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::white);
	gainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 50);
    gainLabel.setText("dB", juce::dontSendNotification);
	gainLabel.setFont(juce::Font(15.0f, juce::Font::bold));
	gainLabel.attachToComponent(&gainKnob, false);
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	gainLabel.setJustificationType(juce::Justification::centredBottom);

	fcenterSlider.setSliderStyle(juce::Slider::LinearVertical);
	fcenterSlider.setColour(juce::Slider::thumbColourId, juce::Colours::darksalmon);
	fcenterSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::white);
	fcenterSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 50);
	fcenterLabel.setText("Hz", juce::dontSendNotification);
	fcenterLabel.setFont(juce::Font(15.0f, juce::Font::bold));
	fcenterLabel.attachToComponent(&fcenterSlider, false);
	fcenterLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	fcenterLabel.setJustificationType(juce::Justification::centredBottom);

	qSlider.setSliderStyle(juce::Slider::LinearVertical);
	qSlider.setColour(juce::Slider::thumbColourId, juce::Colours::darksalmon);
	qSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::white);
	qSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 50);
	qLabel.setText("Q", juce::dontSendNotification);
	qLabel.setFont(juce::Font(15.0f, juce::Font::bold));
	qLabel.attachToComponent(&qSlider, false);
	qLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	qLabel.setJustificationType(juce::Justification::centredBottom);
    /*
        Now for our Sliders and Knobs, we need to set their ranges. These ranges are dependent on the audio parameters we defined in the AudioProcessor Class
        In the AudioProcessorEditor class, we have a pointer to our AudioProcessor class.

        We will use that pointer to get information of all the parameters declared in the AudioProcessor class.
    */
    /*
        Step 3: Use the getParameters() function to get the audio parameter tree.
    */

	auto parameterTree = AudioProcessorEditor::processor.getParameters();


    /*
        Step 4: Use the function getUnchecked() to get the pointer to the specific parameter for your GUIs and we cast it to a AudioParameterFloat.
    */
	gainKnobParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(0);
	fcenterSliderParameter = (juce::AudioParameterInt*)parameterTree.getUnchecked(1);
	qSliderParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(2);
    /*
        Step 5: Use the function setRange() to set the ranges of the GUI elements.
    */
    
    gainKnob.setRange(gainKnobParameter->range.start, gainKnobParameter->range.end,gainKnobParameter->range.interval);
	auto fcenterRange = fcenterSliderParameter->getRange();
    fcenterSlider.setRange(fcenterRange.getStart(), fcenterRange.getEnd(), 1);
	qSlider.setRange(qSliderParameter->range.start,qSliderParameter->range.end, qSliderParameter->range.interval);

    /*
        Step 6: Set the default value of the slider and knob.
    */
    gainKnob.setValue(0, juce::NotificationType::dontSendNotification);
    fcenterSlider.setValue(4000, juce::NotificationType::dontSendNotification);
	qSlider.setValue(1.0f, juce::NotificationType::dontSendNotification);


    /*
        Step 7 : Use the callback function to setValue of the audio processor parameters.
    */
	gainKnob.onValueChange = [this] { *gainKnobParameter = gainKnob.getValue(); };
	fcenterSlider.onValueChange = [this] { *fcenterSliderParameter = fcenterSlider.getValue(); };
	qSlider.onValueChange = [this] { *qSliderParameter = qSlider.getValue(); };



    
}

DSP2_Project4AudioProcessorEditor::~DSP2_Project4AudioProcessorEditor()
{
}

//==============================================================================
void DSP2_Project4AudioProcessorEditor::paint (juce::Graphics& g)
{
    // Here's how to fill the background with a solid colour.
    // Change it to a colour of your preference.
    g.fillAll(juce::Colours::grey);

}

void DSP2_Project4AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any subcomponents in your editor...
    
    /*
        Code written in this function is used to position and resize the GUI components on your window.
    */

    /*
        Dividing the GUI window into a grid...
    */

    auto x = getWidth() / 10;
    auto y = getHeight() / 10;

    
    /*
        Use the setBounds() function to position the GUI Components (x,y) and set the width and height (width,height)
    */

    gainKnob.setBounds(-1* x, 2* y, 5 * x, 5 * y);
	//gainLabel.setBounds(x, y, gainKnob.getWidth(), gainKnob.getHeight());
    fcenterSlider.setBounds(x * 4, 2 * y, x * 1, 7 * y);
	
    qSlider.setBounds(6 * x, 2 * y, 1 * x, 7 * y);
	
}
