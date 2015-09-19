/* ==============================================================================
//  AssociatedButton.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details:  Contains a Button component and its associated Parameter
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZEN_ASSOCIATED_BUTTON_H_INCLUDED
#define ZEN_ASSOCIATED_BUTTON_H_INCLUDED
#include "JuceHeader.h"

#include "../parameters/ZenParameter.h"

namespace Zen
{

class AssociatedButton : virtual public Button, public AssociatedComponent
{
public:

	AssociatedButton(const String& componentName, ZenParameter* associatedParam, const String& inLabel = "")
		: Button(componentName), AssociatedComponent(associatedParam, inLabel)
	{
		setToggleState(associatedParam->getBoolFromValue(), sendNotification);
	}
	void setAssociatedParameterValue() override
	{
		associatedParameter->setValue(getValueFromToggle());

	}

	void setAssociatedParameterValueNotifyingHost() override
	{
		associatedParameter->setValueNotifyingHost(this->getValueFromToggle());
	}

	float getAssociatedParameterValue() override
	{
		return associatedParameter->getValue();
	}
	float getValueFromToggle()
	{
		float test = (getToggleState() == true) ? 1.0 : 0.0;
		return test;
	}

	/*virtual bool isOn() const
	{
		return getToggleState();
	}*/


};
}
#endif  // ZEN_ASSOCIATED_BUTTON_H_INCLUDED