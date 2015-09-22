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
#include <memory>
#include "zen_utils\utilities\ZenUtils.h"
// #ENHANCE:  add JUCE_TRACK_OBJECT macro to bottom of my params/components
// #ENHANCE: add copy constructors


//==============================================================================
	ZynthAudioProcessor::ZynthAudioProcessor()
		:rootTree("Root"), parametersTree("Parameters"), componentsTree("Components")
	{			
		addParameter(audioGainParam = new DecibelParameter("Gain", true, 0.01f, -96.0f, 12.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.01f, "dB"));
 		addParameter(muteParam = new BooleanParameter("Mute", false));
		addParameter(bypassParam = new BooleanParameter("Bypass", false));	

		initializeValueTree();

		
	}

	ZynthAudioProcessor::~ZynthAudioProcessor()
	{
		audioGainParam = nullptr;
		muteParam = nullptr;
		bypassParam = nullptr;
	}
	
	//==============================================================================
	/**Note that if you have more outputs than inputs, then only those channels that 
	correspond to an input channel are guaranteed to contain sensible data - 
	e.g. in the case of 2 inputs and 4 outputs, the first two channels contain the input, 
	but the last two channels may contain garbage, so you should be careful NOT to let 
	this pass through without being overwritten or cleared.*/
	void ZynthAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
	{
		setCurrentSampleRate(getSampleRate());
		jassert(currentSampleRate >= 0);

		if (bypassParam->isOn()) return;

		float* leftData = buffer.getWritePointer(0);  //leftData references left channel now
		float* rightData = buffer.getWritePointer(1); //right data references right channel now		

		if (muteParam->isOn())
		{
			
			for (int i = 0; i < buffer.getNumSamples() ; i++)
			{
				leftData[i] = rightData[i] = 0;
			}
			return;
		}
		
		//Main Processing Loop
		for (long i = 0; i < buffer.getNumSamples(); i++)
		{
			auto audioGainRaw = getClamped(audioGainParam->getSmoothedRawDecibelGainValue(), 0, 4.0f); //Make sure screwups don't blow up speakers
			jassert(audioGainRaw >= 0);
			BufferSampleProcesses::processGain(&leftData[i], &rightData[i], audioGainRaw);
		}
		
	}
	
	void ZynthAudioProcessor::getStateInformation(MemoryBlock& destData)
	{
		// You should use this method to store your parameterSet in the memory block.
		// You could do that either as raw data, or use the XML or ValueTree classes
		// as intermediaries to make it easy to save and load complex data.
		// You should use this method to store your parameters in the memory block.
		// You could do that either as raw data, or use the XML or ValueTree classes
		// as intermediaries to make it easy to save and load complex data.

		XmlElement rootXML(*(rootTree.createXml()));
	//	rootTree.writeToStream()
	//	rootTree.readFromData()
		DBG("getState: Root Tree XMLString:\n" + rootTree.toXmlString());
		//XmlElement root("Root");
	//	XmlElement* el;
		
/*
		for (auto &theParam : managedParameters)
		{
			dynamic_cast<ZenParameter*>(theParam)->writeXML(root);
		}*/

		/*
		el = root.createNewChildElement("Bypass");
		el->addTextElement(String(masterBypassParam->getValue()));
		el = root.createNewChildElement("StereoWidth");
		el->addTextElement(String(stereoWidthParam->getValue()));
		el = root.createNewChildElement("MuteAudio");
		el->addTextElement(String(muteAudioParam->getValue()));
		el = root.createNewChildElement("Gain");
		el->addTextElement(String(audioGainParam->getValue()));
		el = root.createNewChildElement("InvertLeft");
		el->addTextElement(String(invertLeftParam->getValue()));
		el = root.createNewChildElement("InvertRight");
		el->addTextElement(String(invertRightParam->getValue()));
		el = root.createNewChildElement("LockGain");
		el->addTextElement(String(lockGainParam->getValue()));
		el = root.createNewChildElement("StereoPan");
		el->addTextElement(String(stereoPanParam->getValue()));
		*/
		copyXmlToBinary(rootXML, destData);
	}

	void ZynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
	{
		// You should use this method to restore your parameters from this memory block,
		// whose contents will have been created by the getStateInformation() call.

		ScopedPointer<XmlElement> theXML = this->getXmlFromBinary(data, sizeInBytes);
		DBG("getState: XMLString:\n" + theXML->createDocument("any", false, true, "UTF-8", 100));
		if (theXML != nullptr) 
			rootTree.fromXml(*theXML);
	}

	void ZynthAudioProcessor::initializeValueTree()
	{
		createParametersTree();
		rootTree.setProperty("name", "Root", nullptr);
		
		rootTree.addChild(parametersTree, 0, nullptr);
	//	rootTree.addChild(componentsTree, 1, nullptr);
	}

	void ZynthAudioProcessor::createParametersTree()
	{
		parametersTree.setProperty("name", "Parameters", nullptr);
		for (auto &param : getParameters())
		{
			ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
			parametersTree.addChild(zenParam->getAssociatedValueTree(), -1, nullptr);
		}
	}

	


#pragma region overrides
	//==============================================================================
	void ZynthAudioProcessor::prepareToPlay(double inSampleRate, int samplesPerBlock)
	{
		// Use this method as the place to do any pre-playback
		// initialisation that you need..
	
		// Iterates over parameters and resets Smooth for the ones who need it
		for (auto &param : getParameters())
		{			
			ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
			if (zenParam->checkShouldBeSmoothed())
			{				
				zenParam->resetSmoothedValue(inSampleRate);
			}
		}

	}

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

