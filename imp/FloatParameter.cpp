// This is the main DLL file.


#include "FloatParameter.h"
#include <math.h>

//using namespace juce;
namespace juce
{

	float FloatParameter::testGainUtil(float inGain)
	{
		return 10.0f + inGain;
	}


	float FloatParameter::testGain(float inGain)
	{
		return 12.7f;
	}

	bool FloatParameter::isMetaParameter() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	int FloatParameter::getNumSteps() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool FloatParameter::isAutomatable() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void FloatParameter::setValue(float newValue)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	String FloatParameter::getName(int maximumStringLength) const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	float FloatParameter::getValueForText(const String& text) const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	float FloatParameter::getDefaultValue() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool FloatParameter::isOrientationInverted() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	String FloatParameter::getLabel() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	float FloatParameter::getValue() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	String FloatParameter::getText(float value, int) const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


	float FloatParameter::testFloat(float inVal)
	{
		return 10.0f + inVal;
	}

}