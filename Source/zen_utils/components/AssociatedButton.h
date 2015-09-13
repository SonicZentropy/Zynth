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

	AssociatedButton(const String& componentName, ZenParameter* associatedParam)
		: Button(componentName), AssociatedComponent(associatedParam)
	{
		DBG("Entered method: AssociatedButton:AssociatedButton(componentName, associatedParam)");
		// #TODO: add setToggleState conversion in associated button constructor
		setToggleState(associatedParam->getBoolFromValue(), sendNotification);
	}

	virtual void setAssociatedParameterValue() override = 0;
	virtual void setAssociatedParameterValueNotifyingHost() override = 0;
	virtual float getAssociatedParameterValue() override = 0;

};
}
#endif  // ZEN_ASSOCIATED_BUTTON_H_INCLUDED