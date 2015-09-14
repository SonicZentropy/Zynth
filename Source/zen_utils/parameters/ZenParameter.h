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

#include "../utilities/ZenParamUtils.h"


namespace Zen
{

class ZenParameter : public AudioProcessorParameter
{

public:

	// #TODO: Add smoothing from DrowAudio parameter
	ZenParameter()
	{
		throw std::logic_error("ZenParameter Default Constructor should never be called");
	}

	explicit ZenParameter(const String &inName, const String& inLabel = "") :
		value(0.5), defaultValue(0.0), minValue(0.0), maxValue(1.0), 
		precision(2), name(inName),	unitLabel(inLabel), description("")
	{	}

	ZenParameter(const String &inName, const float& inDefaultValue, const String& inLabel = "") :
		value(inDefaultValue), defaultValue(inDefaultValue), minValue(0.0), maxValue(1.0), 
		precision(2), name(inName), unitLabel(inLabel), description("")
	{	}

	ZenParameter(const String &inName, float inMinValue, float inMaxValue, float inDefaultValue, const String& inLabel = "") :
		value(inDefaultValue), defaultValue(inDefaultValue), minValue(inMinValue), maxValue(inMaxValue), 
		precision(2), name(inName), unitLabel(inLabel), description("")
	{	}

	virtual ~ZenParameter() {};

	virtual void setValue(float inValue) override
	{
		DBG("In ZenParameter::setValue() with inValue: " + String(inValue));
		value = inValue;
		//processor->setParameter(getParameterIndex(), inValue);
		requestUIUpdate = true;
	}

	//Juce's AudioProcessorParameter method changed to virtual
	virtual void setValueNotifyingHost(float inValue) override
	{
		// This method can't be used until the parameter has been attached to a processor!
		jassert(processor != nullptr && getParameterIndex() >= 0);
		processor->setParameterNotifyingHost(getParameterIndex(), inValue);
		requestUIUpdate = false;  //set this to false because change came from GUI
	}

	/*virtual float getValueForGUIComponent() const
	{
		return value;
	}*/

	String getName() const { return name; };


	virtual float getValue() const override { return value; }

	virtual float getMinValue() const {	return minValue; }

	virtual float getMaxValue() const {	return maxValue; }

	virtual float getDefaultValue() const override { return defaultValue; }

	virtual bool getBoolFromValue() const
	{		
		return ZenParamUtils::convertFloatToBoolean(getValue());
	}

	virtual unsigned int getDisplayPrecision() const { return precision; }

	virtual void setDisplayPrecision(unsigned int inPrecision) { precision = inPrecision; }

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
protected:
	float value, defaultValue, minValue, maxValue;
	unsigned int precision;
	bool requestUIUpdate = true;
	String name, unitLabel, description;


};
} // Namespace Zen
#endif // ZEN_PARAMETER_H_INCLUDED
