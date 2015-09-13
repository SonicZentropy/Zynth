/* ==============================================================================
//  ZynthAudioProcessor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Main DSP Processing Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/
#pragma warning(disable:4100)

#include "ZynthAudioProcessor.h"
#include "ZynthAudioProcessorEditor.h"

using namespace Zen;

//==============================================================================
	ZynthAudioProcessor::ZynthAudioProcessor()
	{
		addParameter(audioGainParam = new DecibelParameter("Gain", -96.0f, 12.0f, 0.0f));
		addParameter(muteParam = new BooleanParameter("Mute", false));
		addParameter(bypassParam = new BooleanParameter("Bypass", false));
	}

	ZynthAudioProcessor::~ZynthAudioProcessor()
	{
		audioGainParam = nullptr;
		muteParam = nullptr;
		bypassParam = nullptr;
	}
	
	//==============================================================================

	void ZynthAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
	{
		if (bypassParam->isOn()) return;

		float* leftData = buffer.getWritePointer(0);  //leftData references left channel now
		float* rightData = buffer.getWritePointer(1); //right data references right channel now		

		if (muteParam->isOn())
		{
			TIMEDPRINT("Mute is on");
			for (int i = 0; i < buffer.getNumSamples() ; i++)
			{
				leftData[i] = rightData[i] = 0;
			}
			return;
		}
													  
		// #TODO: getRawDecGainValue is the issue
		// Turn 0.0->1.0 decibel range (scaled) into raw decibel gain
		float audioGainRaw = audioGainParam->getRawDecibelGainValue();
		for (long i = 0; i < buffer.getNumSamples(); i++)
		{
			leftData[i] *= audioGainRaw;
			rightData[i] *= audioGainRaw;
			
			//TIMEDPRINT("Value in processBlock is: " + String(audioGainRaw) + " In decibels: " + String(DecibelConversions::mapNormalizedValueToDecibels<float>(audioGainParam->getValue(), 0.0, 1.0, 0.5, -96.0, 12.0, 0.0)) );			
		}
		
	}
	
	void ZynthAudioProcessor::getStateInformation(MemoryBlock& destData)
	{
		// You should use this method to store your parameterSet in the memory block.
		// You could do that either as raw data, or use the XML or ValueTree classes
		// as intermediaries to make it easy to save and load complex data.
	}

	void ZynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
	{
		// You should use this method to restore your parameters from this memory block,
		// whose contents will have been created by the getStateInformation() call.
	}

	


#pragma region overrides
	//==============================================================================
	const String ZynthAudioProcessor::getName() const
	{
		return JucePlugin_Name;
	}

	const String ZynthAudioProcessor::getInputChannelName(int channelIndex) const
	{
		return String(channelIndex + 1);
	}

	const String ZynthAudioProcessor::getOutputChannelName(int channelIndex) const
	{
		return String(channelIndex + 1);
	}

	bool ZynthAudioProcessor::isInputChannelStereoPair(int index) const
	{
		return true;
	}

	bool ZynthAudioProcessor::isOutputChannelStereoPair(int index) const
	{
		return true;
	}

	bool ZynthAudioProcessor::acceptsMidi() const
	{
#if JucePlugin_WantsMidiInput
		return true;
#else
		return false;
#endif
	}

	bool ZynthAudioProcessor::producesMidi() const
	{
#if JucePlugin_ProducesMidiOutput
		return true;
#else
		return false;
#endif
	}

	bool ZynthAudioProcessor::silenceInProducesSilenceOut() const
	{
		return false;
	}

	double ZynthAudioProcessor::getTailLengthSeconds() const
	{
		return 0.0;
	}

	int ZynthAudioProcessor::getNumPrograms()
	{
		return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
					// so this should be at least 1, even if you're not really implementing programs.
	}

	int ZynthAudioProcessor::getCurrentProgram()
	{
		return 0;
	}

	void ZynthAudioProcessor::setCurrentProgram(int index)
	{
	}

	const String ZynthAudioProcessor::getProgramName(int index)
	{
		return String();
	}

	void ZynthAudioProcessor::changeProgramName(int index, const String& newName)
	{
	}

	//==============================================================================
	void ZynthAudioProcessor::prepareToPlay(double inSampleRate, int samplesPerBlock)
	{
		// Use this method as the place to do any pre-playback
		// initialisation that you need..
	}

	void ZynthAudioProcessor::releaseResources()
	{
		// When playback stops, you can use this as an opportunity to free up any
		// spare memory, etc.
	}



	//==============================================================================
	bool ZynthAudioProcessor::hasEditor() const
	{
		return true; // (change this to false if you choose to not supply an editor)
	}

	AudioProcessorEditor* ZynthAudioProcessor::createEditor()
	{
		return new ZynthAudioProcessorEditor(*this);
	}

#pragma endregion
	//==============================================================================
	// This creates new instances of the plugin..
	AudioProcessor* JUCE_CALLTYPE createPluginFilter()
	{
		return new ZynthAudioProcessor();
	}

