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

#ifndef ZEN_ASSOCIATED_TEXT_BUTTON_H_INCLUDED
#define ZEN_ASSOCIATED_TEXT_BUTTON_H_INCLUDED
#include "JuceHeader.h"


namespace Zen
{

class AssociatedTextButton : public AssociatedButton, public TextButton
{
public:

	AssociatedTextButton(const String& componentName, ZenParameter* associatedParam, const String& inLabel = "")
		: Button(componentName), AssociatedButton(componentName, associatedParam, inLabel), TextButton(componentName)
	{
		DBG("Entered method: AssociatedTextButton:AssociatedTextButton(componentName, associatedParam)");
	}


	void setAssociatedParameterValue() override
	{
		DBG("Entered method: AssociatedTextButton:setAssociatedParameterValue()");
		associatedParameter->setValue(getValue());
		//associatedParameter->setValue( getToggleState() );
	}

	void setAssociatedParameterValueNotifyingHost() override
	{
		DBG("Entered method: AssociatedTextButton:setAssociatedParameterValueNotifyingHost()");
		DBG("CurrentValue to pass to parameter: " + String(getValue()));
		associatedParameter->setValueNotifyingHost(getValue());
	}

	float getAssociatedParameterValue() override
	{
		DBG("Entered method: AssociatedTextButton:getAssociatedParameterValue()");
		return associatedParameter->getValue();
	}

	float getValue()
	{
		DBG("Entered method: AssociatedTextButton:getValue() with getToggleState: " + String(getToggleState()));
		return (getToggleState() == true) ? 1.0 : 0.0;
	}

};
} // Namespace
#endif  // ZEN_ASSOCIATED_TEXT_BUTTON_H_INCLUDED