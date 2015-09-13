/* ==============================================================================
//  AssociatedSlider.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Contains a Slider component and its associated Parameter
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/


#ifndef ZEN_ASSOCIATED_SLIDER_H_INCLUDED
#define ZEN_ASSOCIATED_SLIDER_H_INCLUDED

#include "JuceHeader.h"
#include <sstream>
#include "../parameters/ZenParameter.h"
#include "AssociatedComponent.h"

namespace Zen
{

	class AssociatedSlider : virtual public Slider, public AssociatedComponent
	{
	public:

		AssociatedSlider(const String& componentName, ZenParameter* associatedParam, const String& inLabel = "")
			: Slider(componentName),
			  AssociatedComponent(associatedParam, inLabel)			 
		{
			DBG("Entered method: AssociatedSlider:AssociatedSlider(componentName, associatedParam)");
			setValue( associatedParam->getValueForGUIComponent() );			
		}

		virtual String getTextFromValue(double inValue) override
		{
			std::stringstream numberFormatter;
			numberFormatter.precision(getDisplayPrecision());
			numberFormatter << std::fixed << getValue() << unitLabel;
			String result = numberFormatter.str();			
		//	result.append(unitLabel, 20);			
			return result;			
		}

		virtual double getValueFromText(const String& text) override
		{
			//convert from decibel to skewed value here
			String t(text.trimStart());

			if (t.endsWith(getTextValueSuffix()))
				t = t.substring(0, t.length() - unitLabel.length());

			while (t.startsWithChar('+'))
				t = t.substring(1).trimStart();

			return t.initialSectionContainingOnly("0123456789.,-")
				.getDoubleValue();
		}

		void setAssociatedParameterValue() override
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
		}

		void setValueFromLinearNormalized(const float& inValue, NotificationType notification)
		{
			float rawMin = getMinimum();
			setValue((rawMin + (getMaximum() - rawMin) * inValue), notification);
		}
		float getLinearNormalizedValue()
		{
			float rawMin = getMinimum();
			float rawMax = getMaximum();

			return (getValue() - rawMin) / (rawMax - rawMin);
		}

	protected:
	private:
		
	
	};
}
#endif  // ZEN_ASSOCIATED_SLIDER_H_INCLUDED