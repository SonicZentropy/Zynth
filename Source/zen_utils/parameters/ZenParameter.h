/*==============================================================================
//  ZenParameter.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Base Audio Parameter class (Virtual only)
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/
#pragma warning(disable:4100 4305)

#ifndef ZEN_PARAMETER_H_INCLUDED
#define ZEN_PARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include <sstream>

#include "../utilities/ZenParamUtils.h"
#include "LinearSmoothedValue.h"
#include "../../ZynthAudioProcessor.h"

namespace Zen {
using namespace juce;
class ZenParameter : public AudioProcessorParameter
{
	
public:
	// #TODO: add copy constructors
	// #TODO: change parameter Value to a Value object that refersTo(the parameter value obj)
	// #TODO: Add lsValue.reset to prepareToPlay

	// #TODO: Add smoothing from DrowAudio parameter
	ZenParameter()
	{
		throw std::logic_error("ZenParameter Default Constructor should never be called");
	}

	ZenParameter(const String &inName, const float& inStep = 0.01f, const String& inLabel = "") :
		value(0.5), defaultValue(0.0), minValue(0.0), maxValue(1.0), 
		precision(2), name(inName),	unitLabel(inLabel), description(""), 
		smoothValue(0.5), smoothTime(0.95), intervalStep(inStep)
	{
		lsValue.setValue(0.5);
		lsValue.reset(processor->getCurrentSampleRate(), 5.0);
	}

	ZenParameter(const String &inName, const float& inDefaultValue, const float& inStep = 0.01f, const String& inLabel = "") :
		value(inDefaultValue), defaultValue(inDefaultValue), minValue(0.0), maxValue(1.0), 
		intervalStep(inStep), precision(2), name(inName), unitLabel(inLabel), 
		description(""), smoothValue(inDefaultValue), smoothTime(0.95)
	{
		lsValue.setValue(0.5);
		lsValue.reset(44100, 5.0);
	}

	ZenParameter(const String &inName, float inMinValue, float inMaxValue, float inDefaultValue, const String& inLabel = "", const float& inStep = 0.01f) :
		value(inDefaultValue), defaultValue(inDefaultValue), minValue(inMinValue), maxValue(inMaxValue),
		precision(2), name(inName), unitLabel(inLabel), description(""),
		smoothValue(inDefaultValue), smoothTime(0.95), intervalStep(inStep)
	{
		lsValue.setValue(0.5);
		lsValue.reset(44100, 5.0);
	}

	

	virtual ~ZenParameter() {};

	virtual void setValue(float inValue) override
	{
		value = inValue;
		setSmoothedValue(inValue);
		//resetSmoothedValue(processor->getCurrentSampleRate(), 10.0);
		requestUIUpdate = true;
	}

	//Juce's AudioProcessorParameter method changed to virtual
	virtual void setValueNotifyingHost(float inValue) override
	{
		// This method can't be used until the parameter has been attached to a processor!
		jassert(processor != nullptr && getParameterIndex() >= 0);
		processor->setParameterNotifyingHost(getParameterIndex(), inValue);
		//setSmoothValue(inValue);
		//resetSmoothedValue(processor->getCurrentSampleRate(), 10.0);
		requestUIUpdate = false;  //set this to false because change came from GUI
	}

	/*void smooth()
	{
		if (smoothValue != getValue())
		{
			if ((smoothCoeff == 1.0) || almostEqual(smoothValue, getValue()))
				smoothValue = getValue();
			else
				smoothValue = ((getValue() - smoothValue) * smoothCoeff) + smoothValue;
		}
	}

	float getSmoothedValue()
	{
		return smoothValue;
	}*/

	virtual bool needsUIUpdate()
	{
		return requestUIUpdate;
	}

	virtual bool checkUIUpdateAndReset()
	{
		bool updateSet = requestUIUpdate;
		requestUIUpdate = false;
		return updateSet;
	}

	virtual void resetUIUpdate()
	{
		requestUIUpdate = false;
	}


	virtual String getText(float inValue, int maxStringLength) const override
	{
		jassert(maxStringLength >= 0);
		std::stringstream numberFormatter;
		numberFormatter.precision(getDisplayPrecision());
		numberFormatter << std::fixed << inValue;
		String result = numberFormatter.str();
		result.append(getLabel(), 20);
		
		return result;
	}

#pragma region Getters/Setters

	virtual String getName() const { return name; };

	virtual float getValue() const override { return value.getValue(); }

	virtual float getMinValue() const {	return minValue; }

	virtual float getMaxValue() const {	return maxValue; }

	virtual float getDefaultValue() const override { return defaultValue; }

	virtual bool getBoolFromValue() const
	{		
		return convertFloatToBoolean(getValue());
	}

	virtual void setDisplayPrecision(unsigned int inPrecision) { precision = inPrecision; }

	virtual unsigned int getDisplayPrecision() const { return precision; }

	virtual String getText(float inValue) const
	{
		return getText(inValue, 50000);
	}

	virtual float getValueForGUIComponent() const
	{
		return getValue();
	}

	
	virtual String getName(int maximumStringLength) const override { return name; };
	virtual String getLabel() const override { return unitLabel; };
	virtual float getValueForText(const String& text) const override
	{
		return text.getFloatValue();
	}
	//float getSmoothCoeff() const { return smoothCoeff; }
	//void setSmoothCoeff(float inValue) { smoothCoeff = inValue; }

/*  float getSmoothValue() const { return smoothValue; }
	void setSmoothValue(float inValue) { smoothValue = inValue; }*/


	float getIntervalStep() const { return intervalStep; }
	void setIntervalStep(float inValue) { intervalStep = inValue; }

	void resetSmoothedValue(float inSampleRate, float inSmoothTime)
	{
		lsValue.reset(inSampleRate, inSmoothTime);
	}

	void setSmoothedValue(float newValue)
	{
		lsValue.setValue(newValue);
	}

	float getNextSmoothedValue() noexcept
	{
		return lsValue.getNextValue();
	}

#pragma endregion
protected:
	Value value;
	
	float defaultValue, minValue, maxValue, intervalStep;
	unsigned int precision;
	bool requestUIUpdate = true;
	String name, unitLabel, description;
	float smoothCoeff = 1.0f;  //Coeff of 1 yields no smoothing, value instantly changes.  Lower coeffs make the smoothing slower per-smooth call
	float smoothValue, smoothTime;
	LinearSmoothedValue lsValue;

	//==============================================================================
	JUCE_LEAK_DETECTOR(ZenParameter);
};

/// <summary> Values that represent parameter units. </summary>
enum ParameterUnit
{
	UnitGeneric = 0,	/* untyped value generally between 0.0 and 1.0 */
	UnitIndexed = 1,	/* takes an integer value (good for menu selections) */
	UnitBoolean = 2,	/* 0.0 means FALSE, non-zero means TRUE */
	UnitPercent = 3,	/* usually from 0 -> 100, sometimes -50 -> +50 */
	UnitSeconds = 4,	/* absolute or relative time */
	UnitSampleFrames = 5,	/* one sample frame equals (1.0/sampleRate) seconds */
	UnitPhase = 6,	/* -180 to 180 degrees */
	UnitRate = 7,	/* rate multiplier, for playback speed, etc. (e.g. 2.0 == twice as fast) */
	UnitHertz = 8,	/* absolute frequency/pitch in cycles/second */
	UnitCents = 9,	/* unit of relative pitch */
	UnitRelativeSemiTones = 10,	/* useful for coarse detuning */
	UnitMIDINoteNumber = 11,	/* absolute pitch as defined in the MIDI spec (exact freq may depend on tuning table) */
	UnitMIDIController = 12,	/* a generic MIDI controller value from 0 -> 127 */
	UnitDecibels = 13,	/* logarithmic relative gain */
	UnitLinearGain = 14,	/* linear relative gain */
	UnitDegrees = 15,	/* -180 to 180 degrees, similar to phase but more general (good for 3D coord system) */
	UnitEqualPowerCrossfade = 16,	/* 0 -> 100, crossfade mix two sources according to sqrt(x) and sqrt(1.0 - x) */
	UnitMixerFaderCurve1 = 17,	/* 0.0 -> 1.0, pow(x, 3.0) -> linear gain to simulate a reasonable mixer channel fader response */
	UnitPan = 18,	/* standard left to right mixer pan */
	UnitMeters = 19,	/* distance measured in meters */
	UnitAbsoluteCents = 20,	/* absolute frequency measurement : if f is freq in hertz then 	*/
							/* absoluteCents = 1200 * log2(f / 440) + 6900					*/
	UnitOctaves = 21,	/* octaves in relative pitch where a value of 1 is equal to 1200 cents*/
	UnitBPM = 22,	/* beats per minute, ie tempo */
	UnitBeats = 23,	/* time relative to tempo, ie. 1.0 at 120 BPM would equal 1/2 a second */
	UnitMilliseconds = 24,	/* parameter is expressed in milliseconds */
	UnitRatio = 25,	/* for compression, expansion ratio, etc. */

	UnitCustomUnit = 26	/* this is the parameter unit type for parameters that present a custom unit name */
};

} // Namespace Zen
#endif // ZEN_PARAMETER_H_INCLUDED
