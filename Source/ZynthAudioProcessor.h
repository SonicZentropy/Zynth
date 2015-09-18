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



#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "JuceHeader.h"
#include "zen_utils\parameters\DecibelParameter.h"
#include "zen_utils\parameters\BooleanParameter.h"

//==============================================================================
/**
*/
class ZynthAudioProcessor : public AudioProcessor
{
public:
	//==============================================================================
	ZynthAudioProcessor();
	~ZynthAudioProcessor();

	//==============================================================================
		

	void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

		
	//DO NOT USE SCOPED POINTERS FOR PARAMETERS
	Zen::DecibelParameter* audioGainParam;		
	Zen::BooleanParameter* muteParam;
	Zen::BooleanParameter* bypassParam;
		

#pragma region overrides
	//==============================================================================
	void prepareToPlay(double inSampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const String getName() const override;

	const String getInputChannelName(int channelIndex) const override;
	const String getOutputChannelName(int channelIndex) const override;
	bool isInputChannelStereoPair(int index) const override;
	bool isOutputChannelStereoPair(int index) const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool silenceInProducesSilenceOut() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	//==============================================================================
	
#pragma endregion 
private:
	// Change to scoped pointer
	ValueTree rootTree;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZynthAudioProcessor)

};


#endif  // PLUGINPROCESSOR_H_INCLUDED
