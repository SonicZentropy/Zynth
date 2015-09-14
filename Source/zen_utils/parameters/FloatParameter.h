/* ==============================================================================
//  FloatParameter.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/08/18
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Derived AudioProcessorParameter for Float values.  Pure
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
#ifndef ZEN_FLOAT_PARAMETER_H_INCLUDED
#define ZEN_FLOAT_PARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include "ZenParameter.h"

namespace Zen
{
using namespace juce;
class FloatParameter : public ZenParameter
{
public:

	FloatParameter()
	{
		std::logic_error("Float Parameter Default Constructor should never be called");
	}

	FloatParameter(const String& paramName, const float& defaultfloat, const String& inLabel = "")
		: ZenParameter(paramName, defaultfloat, inLabel)
	{
		range = 1.0;
	}

	FloatParameter(const String& parameterName, const float& minfloat, const float& maxfloat, const float& defaultfloat, const String& inLabel = "")
		: ZenParameter(parameterName, minfloat, maxfloat, defaultfloat, inLabel)
	{
		range = maxfloat - minfloat;
	}

	virtual ~FloatParameter()
	{	}

	virtual float getScaledValue() const
	{
		return (getValue() - getMinValue()) / range;
	}

	virtual void setScaledValue(const float& inValue)
	{
		setValue(inValue * range + getMinValue());
	}

protected:
	
	float range;

};
}//namespace
#endif   // ZEN_FLOAT_PARAMETER_H_INCLUDED