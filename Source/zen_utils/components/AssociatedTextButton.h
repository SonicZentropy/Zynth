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
		: AssociatedButton(componentName, associatedParam), TextButton(componentName), Button(componentName), Component(componentName)
	{
		DBG("Entered method: AssociatedTextButton:AssociatedTextButton(componentName, associatedParam)");
	}
	/*void setAssociatedParameterValue() override
	{
		DBG("Entered method: AssociatedSlider:setAssociatedParameterValue()");
		associatedParameter->setValue(getValue());

	}

	void setAssociatedParameterValueNotifyingHost() override
	{
		DBG("Entered method: AssociatedSlider:setAssociatedParameterValueNotifyingHost() with value: " + String(getValue()));
		associatedParameter->setValueNotifyingHost(getValue());
	}

	float getAssociatedParameterValue() override
	{
		DBG("Entered method: AssociatedTextButton:getAssociatedParameterValue()");
		return associatedParameter->getValue();
	}*/

};
} // Namespace
#endif  // ZEN_ASSOCIATED_TEXT_BUTTON_H_INCLUDED