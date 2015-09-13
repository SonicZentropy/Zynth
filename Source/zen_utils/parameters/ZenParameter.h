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
#pragma warning(disable:4100)

#ifndef ZEN_PARAMETER_H_INCLUDED
#define ZEN_PARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include <sstream>

#include "../utilities/DecibelConversions.h"
#include "../utilities/ZenParamUtils.h"


namespace Zen
{



class ZenParameter : public AudioProcessorParameter
{

public:

	explicit ZenParameter(const String &inName) :
		defaultValue(0.0), minValue(0.0), maxValue(1.0), precision(2), name(inName),
		unitLabel(""), description("")
	{
		
	}

	ZenParameter(const String &inName, const float& inDefaultValue) :
		defaultValue(inDefaultValue), minValue(0.0), maxValue(1.0), precision(2), name(inName),
		unitLabel(""), description("")
	{
	}
	ZenParameter(const String &inName,
		float inMinValue,
		float inMaxValue,
		float inDefaultValue) :
		defaultValue(inDefaultValue), minValue(inMinValue), maxValue(inMaxValue), precision(2),
		name(inName), unitLabel(""), description("")
	{
	}
	virtual ~ZenParameter() {};

	virtual void setValue(float inValue) override
	{
		DBG("In ZenParameter::setValue() with inValue: " + String(inValue));
		value = inValue;
		requestUIUpdate = true;
	}

	//Juce's AudioProcessorParameter method changed to virtual
	virtual void setValueNotifyingHost(float newValue) override
	{
		// This method can't be used until the parameter has been attached to a processor!
		jassert(processor != nullptr && getParameterIndex() >= 0);
		processor->setParameterNotifyingHost(getParameterIndex(), newValue);
		requestUIUpdate = false;  //set this to false because change came from GUI

	}

	virtual float getMinValue() const
	{
		return minValue;
	}

	/**
	* @return Get the parameter's maximum value
	*/
	virtual float getMaxValue() const
	{
		return maxValue;
	}

	virtual float getDefaultValue() const override
	{
		return defaultValue;
	}

	virtual bool getBoolFromValue() const
	{
		DecibelConversions::convertDecibelsToLinear(0.5);
		return ZenParamUtils::convertFloatToBoolean(value);
	}

	/**
	* Get the number of decimal places for displaying floating-point parameter values.
	*/
	virtual unsigned int getDisplayPrecision() const
	{
		return precision;
	}

	/**
	* Number of floating point digits to be displayed, for parameters which support
	* display precision.
	*
	* @param inPrecision Number of decimal digits to display
	*/
	virtual void setDisplayPrecision(unsigned int inPrecision)
	{
		this->precision = inPrecision;
	}

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

	virtual String getText(float inValue) const
	{
		return getText(inValue, 50000);
	}

	virtual float getValueForGUIComponent() const
	{
		return value;
	}

	virtual float getValue() const override { return value; };
	virtual String getName(int maximumStringLength) const override { return name; };
	virtual String getLabel() const override { return unitLabel; };
	virtual float getValueForText(const String& text) const override
	{
		return text.getFloatValue();
	}
protected:
	float defaultValue, minValue, maxValue;
	unsigned int precision;
	bool requestUIUpdate = true;
	String name, unitLabel, description;


};
} // Namespace Zen
#endif // ZEN_PARAMETER_H_INCLUDED
