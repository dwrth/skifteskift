/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SkifteskiftAudioProcessorEditor::SkifteskiftAudioProcessorEditor (SkifteskiftAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (250, 250);
    
    skift.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    skift.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    skift.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentWhite);
    skift.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black);
    skift.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::transparentWhite);
    skift.setVelocityModeParameters();
    skift.setScrollWheelEnabled(true);
    skift.setDoubleClickReturnValue(true, 1.f);
    skift.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 190, 25);
    skift.setTextValueSuffix(" % skift");
    skift.setRange(0.5f, 2.0f, 0.01);  // 0.5 = one octave down, 2.0 = one octave up
    skift.setValue(1.0f);              // Start at normal pitch
    skift.addListener(this);
    addAndMakeVisible(&skift);
    
}

void SkifteskiftAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &skift)
    {
        DBG("Slider value changed to: " << skift.getValue()); // Debug output
        audioProcessor.skiftVal = skift.getValue();
    }
}

SkifteskiftAudioProcessorEditor::~SkifteskiftAudioProcessorEditor()
{
}

//==============================================================================
void SkifteskiftAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
}

void SkifteskiftAudioProcessorEditor::resized()
{
    skift.setBounds(0, 0, 180, 180);
    skift.setCentrePosition(getWidth() / 2, getHeight() / 2);
}
