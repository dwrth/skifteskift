/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SkifteskiftAudioProcessor::SkifteskiftAudioProcessor()
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
    skiftVal = 1.0f;
    DBG("Plugin initialized with skiftVal: " << skiftVal);
}

SkifteskiftAudioProcessor::~SkifteskiftAudioProcessor()
{
    delete rubberSkifter;
}

//==============================================================================
const juce::String SkifteskiftAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SkifteskiftAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SkifteskiftAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SkifteskiftAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SkifteskiftAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SkifteskiftAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SkifteskiftAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SkifteskiftAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SkifteskiftAudioProcessor::getProgramName (int index)
{
    return {};
}

void SkifteskiftAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SkifteskiftAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    if (rubberSkifter != nullptr) {
        delete rubberSkifter;
    }

    int options = RubberBand::RubberBandStretcher::OptionProcessRealTime |
                 RubberBand::RubberBandStretcher::OptionPitchHighQuality |
                 RubberBand::RubberBandStretcher::OptionTransientsCrisp |
                 RubberBand::RubberBandStretcher::OptionWindowShort;
    
    rubberSkifter = new RubberBand::RubberBandStretcher(sampleRate, 2, options);
    rubberSkifter->setMaxProcessSize(samplesPerBlock);
    rubberSkifter->setPitchScale(skiftVal);
    rubberSkifter->reset();
}

void SkifteskiftAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    
    delete rubberSkifter;
    rubberSkifter = nullptr;
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SkifteskiftAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SkifteskiftAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (rubberSkifter == nullptr) return;

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Update pitch scale from slider
    rubberSkifter->setPitchScale(skiftVal);

    // Create input/output arrays
    std::vector<const float*> inputBuffers(totalNumInputChannels);
    std::vector<float*> outputBuffers(totalNumInputChannels);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        inputBuffers[channel] = buffer.getReadPointer(channel);
        outputBuffers[channel] = buffer.getWritePointer(channel);
    }

    // Process the audio - note the false parameter for continuous processing
    rubberSkifter->process(&inputBuffers[0], buffer.getNumSamples(), false);
    
    size_t available = rubberSkifter->available();
    if (available > 0)
    {
        rubberSkifter->retrieve(&outputBuffers[0], std::min(available, (size_t)buffer.getNumSamples()));
    }
}

//==============================================================================
bool SkifteskiftAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SkifteskiftAudioProcessor::createEditor()
{
    return new SkifteskiftAudioProcessorEditor (*this);
}

//==============================================================================
void SkifteskiftAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SkifteskiftAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SkifteskiftAudioProcessor();
}
