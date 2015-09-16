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

class DecibelParameter : public FloatParameter
{

public:

	DecibelParameter()
	{
		throw std::logic_error("Decibel Parameter Default Constructor should never be called");
	}

	DecibelParameter(const String& paramName, 
		const float& inMinDecibels, const float& inMaxDecibels, const float& inUnityDecibels, 
		const float& minFloat, const float& maxFloat, const float& inMidFloat, const float& defaultFloat, const String& inLabel = "")
		: FloatParameter(paramName,	minFloat, maxFloat, defaultFloat, inLabel), 
		minDecibels(inMinDecibels),
		maxDecibels(inMaxDecibels),
		unityDecibels(inUnityDecibels),
		midFloat(inMidFloat),
		range(inMaxDecibels - inMinDecibels)
	{
		requestUIUpdate = true;
	}

	DecibelParameter(const String& paramName, const float& inMinDecibels, const float& inMaxDecibels, const float& inUnityDecibels, const String& inLabel = "")
		: FloatParameter(paramName, 0.0f, 1.0f, 0.5f, inLabel),
		minDecibels(inMinDecibels),
		maxDecibels(inMaxDecibels),
		unityDecibels(inUnityDecibels),
		midFloat(0.5f),
		range(inMaxDecibels - inMinDecibels)
	{	
		requestUIUpdate = true;
	}



	virtual ~DecibelParameter()
	{
	}	
	
	virtual float convertDecibelsToLinearWithSetMidpoint(const float& decibels)
	{
		return DecibelConversions::mapDecibelsToProperNormalizedValue(decibels, minDecibels, maxDecibels, unityDecibels);
	}

	virtual float convertLinearWithSetMidpointToDecibels(const float& inValue)
	{
		return DecibelConversions::mapNormalizedValueToDecibels(inValue, minDecibels, maxDecibels);
	}

	virtual float getRawDecibelGainValue() const
	{
		return DecibelConversions::decibelRangeGainToRawDecibelGain(this->getValue(), minDecibels, maxDecibels);		
	}

	virtual float getSmoothedRawDecibelGainValue()
	{
		return DecibelConversions::decibelRangeGainToRawDecibelGain(lsValue.getNextValue(), minDecibels, maxDecibels);
	}

	virtual void setValueNotifyingHost(float newValue) override
	{			
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

	float getMidFloat() const { return midFloat; }

	void setMidFloat(const float inMidFloat) { midFloat = inMidFloat; }

	float getRange() const { return range; }

	void setRange(const float inRange) { range = inRange; }

#pragma endregion

protected:

	float minDecibels, maxDecibels;	
	float unityDecibels;
	float midFloat;
	float range;
};
}

#endif // ZEN_DECIBEL_PARAMETER_H_INCLUDED
