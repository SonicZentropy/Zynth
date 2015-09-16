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

	BooleanParameter()
	{
		throw std::logic_error("Boolean Parameter Default Constructor should never be called");
	}
	BooleanParameter(const String& parameterName, const float& defaultParameterValue)
		: ZenParameter(parameterName, defaultParameterValue, 0.01f)
	{	}

	BooleanParameter(const String& parameterName, const bool& defaultBooleanValue)
		: ZenParameter(parameterName, convertBooleanToFloat(defaultBooleanValue), 0.01f)
	{	}

	virtual ~BooleanParameter()
	{	}

	/*virtual void BooleanParameter::setValue(float newValue) override
	{
		// #TODO: Add jassert checking for setValue everywhere
		//other values technically disallowed
		jassert(newValue >= 0.0 && newValue <= 1.0);

		//compatibility transform such that anything != 0.0 is true
		value = ((newValue == 0.0f) ? 0.0f : 1.0f);
		booleanValue = ZenParamUtils::convertFloatToBoolean(newValue);
		requestUIUpdate = true;
	}*/

	virtual void BooleanParameter::setValue(bool newBool)
	{	
		value = convertBooleanToFloat(newBool);
		requestUIUpdate = true;
	}

	virtual void BooleanParameter::setValueNotifyingHost(bool newBoolValue) 
	{
		ZenParameter::setValueNotifyingHost(convertBooleanToFloat(newBoolValue));
		requestUIUpdate = false;
		
	}

	virtual bool isOn() const
	{
		return convertFloatToBoolean(value.getValue());
	}

protected:

};
}//namespace
#endif   // ZEN_BOOLEAN_PARAMETER_H_INCLUDED