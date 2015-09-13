/* ==============================================================================
//  BooleanParameter.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/08/18
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Derived AudioProcessorParameter for Boolean values.  Pure
//  virtual methods that must be overridden:
//   destructor
//   getValue
//   setValue
//   getDefaultValue
//   getName
//   getLabel (units string, example "Hz" or "%")
//   getValueForText (parse a String and return appropriate value for it)
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/
#ifndef ZEN_BOOLEAN_PARAMETER_H_INCLUDED
#define ZEN_BOOLEAN_PARAMETER_H_INCLUDED

#pragma warning (disable : 4100 )

#include "JuceHeader.h"
#include "ZenParameter.h"


namespace Zen
{

class BooleanParameter : public ZenParameter
{

public:

	BooleanParameter::BooleanParameter(const String& parameterName, const float& defaultParameterValue)
		: ZenParameter(parameterName, defaultParameterValue)
	{	}

	BooleanParameter(const String& parameterName, const bool& defaultBooleanValue)
		: ZenParameter(parameterName, ZenParamUtils::convertBooleanToFloat(defaultBooleanValue))
	{
		//defaultValue = ZenParamUtils::convertBooleanToFloat(defaultBooleanValue);		
	}

	virtual BooleanParameter::~BooleanParameter()
	{
	}

	virtual void BooleanParameter::setValue(float newValue) override
	{
		// #TODO: Add jassert checking for setValue everywhere
		//other values technically disallowed
		jassert(newValue >= 0.0 && newValue <= 1.0);

		//compatibility transform such that anything != 0.0 is true
		setValue(static_cast<float>((newValue == 0.0) ? 0.0 : 1.0));
		requestUIUpdate = true;
	}

	virtual void BooleanParameter::setValue(bool newBool)
	{		
		setValue(static_cast<float>(ZenParamUtils::convertBooleanToFloat(newBool)));
		requestUIUpdate = true;
	}

	virtual void BooleanParameter::setValueNotifyingHost(bool newBoolValue) 
	{
		ZenParameter::setValueNotifyingHost(ZenParamUtils::convertBooleanToFloat(newBoolValue));
	}

	virtual bool isOn() const
	{
		return getBoolFromValue();		
	}

};
}//namespace
#endif   // ZEN_BOOLEAN_PARAMETER_H_INCLUDED