/*==============================================================================
//  DecibelParameter.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Decibel Parameter Class - Handles Log/linear scaling
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZEN_DECIBEL_PARAMETER_H_INCLUDED
#define ZEN_DECIBEL_PARAMETER_H_INCLUDED

#include "FloatParameter.h"
#include "..\utilities\DecibelConversions.h"

namespace Zen
{
using juce::String;
class DecibelParameter : public FloatParameter
{

public:

	DecibelParameter()
	{
		DBG("In DecibelParameter::DecibelParameter() SHOULD NEVER BE CALLED!");
		jassertfalse;
	}

	explicit DecibelParameter(const String& paramName, const bool& inShouldBeSmoothed = false, const float& inSmoothingTime = 0.01f,
		const float& inMinDecibels = -96.0f, const float& inMaxDecibels = 12.0f, const float& inUnityDecibels = 0.0f, 
		const float& minValue=0.0f, const float& maxValue = 1.0f, const float& inMidValue = 0.5f, 
		const float& defaultValue = 0.5f, const float& inStep = 0.01f,  const String& inLabel = "")
		: FloatParameter(paramName,	minValue, maxValue, defaultValue, inStep, inShouldBeSmoothed, inSmoothingTime, inLabel), 
		minDecibels(inMinDecibels),
		maxDecibels(inMaxDecibels),
		unityDecibels(inUnityDecibels),
		midValue(inMidValue),
		range(inMaxDecibels - inMinDecibels)
	{
		DBGM("In DecibelParameter::DecibelParameter() ");
		requestUIUpdate = true;
	}

	virtual ~DecibelParameter()
	{
	}	
	
	virtual float convertDecibelsToLinearWithSetMidpoint(const float& decibels)
	{
		DBGM("In DecibelParameter::convertDecibelsToLinearWithSetMidpoint() ");
		return DecibelConversions::mapDecibelsToProperNormalizedValue(decibels, minDecibels, maxDecibels, unityDecibels);
	}

	virtual float convertLinearWithSetMidpointToDecibels(const float& inValue)
	{
		DBGM("In DecibelParameter::convertLinearWithSetMidpointToDecibels() ");
		return DecibelConversions::mapNormalizedValueToDecibels(inValue, minDecibels, maxDecibels);
	}

	virtual float getRawDecibelGainValue() const
	{
		return DecibelConversions::decibelRangeGainToRawDecibelGain(this->getValue(), minDecibels, maxDecibels);		
	}

	/// <summary> Processes one sample worth of value smoothing and returns the raw decibel gain representing it.
	/// This should be called once for every sample's process block and the result considered 
	/// that sample's gain adjustment</summary>
	/// <returns> The smoothed raw decibel gain value. </returns>
	virtual float getSmoothedRawDecibelGainValue()
	{
		
		return DecibelConversions::decibelRangeGainToRawDecibelGain(getNextSmoothedValue(), minDecibels, maxDecibels);
	}

	virtual void setValueNotifyingHost(float newValue) override
	{		
		DBGM("In DecibelParameter::setValueNotifyingHost() ");
		float dbConvertedToLinear = convertDecibelsToLinearWithSetMidpoint(newValue);
		ZenParameter::setValueNotifyingHost(dbConvertedToLinear);

	}

	virtual float getValueInDecibels() const
	{		
		return DecibelConversions::mapNormalizedValueToDecibels(getValue(), minDecibels, maxDecibels);
	}

	virtual float getValueForGUIComponent() const override
	{
		return getValueInDecibels();
	}

	virtual String getText(float inValue, int maxStringLength) const override
	{
		DBGM("In DecibelParameter::getText() ");
		jassert(maxStringLength >= 0);
		std::stringstream numberFormatter;
		numberFormatter.precision(getDisplayPrecision());
		numberFormatter << std::fixed << inValue;
		String result = numberFormatter.str();
		result.append( " "+getLabel(), 20);
		
		return (String(getValueInDecibels()) + " " + String(unitLabel));
	}

#pragma region GET/SET
	float getMinDecibels() const { return minDecibels; }

	void setMinDecibels(const float inMinDecibels) { minDecibels = inMinDecibels; }

	float getMaxDecibels() const { return maxDecibels; }

	void setMaxDecibels(const float inMaxDecibels) { maxDecibels = inMaxDecibels; }

	float getUnityDecibels() const { return unityDecibels; }

	void setUnityDecibels(const float inUnityDecibels) { unityDecibels = inUnityDecibels; }

	float getmidValue() const { return midValue; }

	void setmidValue(const float inMidValue) { midValue = inMidValue; }

	float getRange() const { return range; }

	void setRange(const float inRange) { range = inRange; }

#pragma endregion

protected:

	float minDecibels, maxDecibels;	
	float unityDecibels;
	float midValue;
	float range;
};
}

#endif // ZEN_DECIBEL_PARAMETER_H_INCLUDED
