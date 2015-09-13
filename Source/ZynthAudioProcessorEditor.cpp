/*==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

==============================================================================*/
#include "ZynthAudioProcessorEditor.h"

//==============================================================================
ZynthAudioProcessorEditor::ZynthAudioProcessorEditor (ZynthAudioProcessor& ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
	processor = &ownerFilter;

	// #TODO: Change Is_Synth in AppConfig.h for Bitwig
    setName("ZynthMainComponent");
	DBG("mutebutton:");
    addAndMakeVisible (muteButton = new AssociatedTextButton("Mute Button", processor->muteParam));
    muteButton->setTooltip ("Mute all audio");
    muteButton->setButtonText ("MUTE");
    muteButton->addListener (this);

	
    addAndMakeVisible (gainSlider = new AssociatedSlider ("Gain Slider", processor->audioGainParam, "dB"));
	//setValue from processor->audioGainParam->getValueInDecibels
    gainSlider->setTooltip ("Adjusts audio gain");
    gainSlider->setRange (-96, 12, 0.01);
    gainSlider->setSliderStyle (Slider::LinearHorizontal);
    gainSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    gainSlider->addListener (this);
	
    addAndMakeVisible (bypassButton = new AssociatedTextButton ("Bypass Button", processor->bypassParam));
    bypassButton->setTooltip ("Bypass Plugin");
    bypassButton->setButtonText ("Bypass");
    bypassButton->addListener (this);

    setSize (600, 400);

	gainSlider->setTextValueSuffix("dB");
	gainSlider->setDoubleClickReturnValue(true, 0.0);

	muteButton->setClickingTogglesState(true);
	bypassButton->setClickingTogglesState(true);

    //processor->RequestUIUpdate(); //UI Update must be performed every time a new editor is constructed - fix this?
	startTimer(20); // Start timer poll with 20ms rate

}

ZynthAudioProcessorEditor::~ZynthAudioProcessorEditor()
{
    muteButton = nullptr;
    gainSlider = nullptr;
    bypassButton = nullptr;
}

//==============================================================================
void ZynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colour (0xff303030));
}

void ZynthAudioProcessorEditor::resized()
{
    muteButton->setBounds (10, 6, 74, 24);
    gainSlider->setBounds (158, 8, 150, 24);
    bypassButton->setBounds (10, 38, 74, 24);
}

void ZynthAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{	
	AssociatedButton* clickedBtn = dynamic_cast<AssociatedButton*>(buttonThatWasClicked);

    if (buttonThatWasClicked == muteButton)
    {		
		//clickedBtn->setAssociatedParameterValueNotifyingHost();
		//processor->muteParam->setValueNotifyingHost(static_cast<AssociatedTextButton*>(buttonThatWasClicked)->);
	    //muteButton->getAssociatedParameter()->setValueNotifyingHost(muteButton->getValue());
		clickedBtn->setAssociatedParameterValueNotifyingHost();
    }
    else if (buttonThatWasClicked == bypassButton)
    {
		clickedBtn->setAssociatedParameterValueNotifyingHost();   
    }
}

void ZynthAudioProcessorEditor::sliderValueChanged (Slider* inSliderThatWasMoved)
{
	AssociatedSlider* sliderThatWasMoved = dynamic_cast<AssociatedSlider*>(inSliderThatWasMoved);

    if (sliderThatWasMoved == gainSlider)
    {		
		sliderThatWasMoved->setAssociatedParameterValueNotifyingHost();		
    }

}

void ZynthAudioProcessorEditor::timerCallback()
{
	// #TODO: FIX AUTOMATION and parameters changing without notification

	//exchange data between UI Elements and the Plugin (ourProcessor)

//	if (processor->muteParam->needsUIUpdate())
//		muteButton->setToggleState(0.0f != processor->muteParam->getValue(), dontSendNotification);

	if (processor->audioGainParam->needsUIUpdate())
	{
		gainSlider->setValue(processor->audioGainParam->getValueForGUIComponent(), dontSendNotification);
		processor->audioGainParam->resetUIUpdate();
	}
}

//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ZynthAudioProcessorEditor"
                 componentName="ZynthMainComponent" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="ZynthAudioProcessor&amp; ownerFilter" variableInitialisers="AudioProcessorEditor(ownerFilter)"
                 snapPixels="2" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff303030"/>
  <TEXTBUTTON name="Mute Button" id="bf3b325d1c639702" memberName="muteButton"
              virtualName="AssociatedTextButton" explicitFocusOrder="0" pos="10 6 74 24"
              tooltip="Mute all audio" buttonText="MUTE" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="Gain Slider" id="d2c6297df481a920" memberName="gainSlider"
          virtualName="AssociatedSlider" explicitFocusOrder="0" pos="158 8 150 24"
          tooltip="Adjusts audio gain" min="0" max="1" int="0.010000000000000000208"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="Bypass Button" id="2b30a4c7b3254496" memberName="bypassButton"
              virtualName="AssociatedTextButton" explicitFocusOrder="0" pos="14 38 74 24"
              tooltip="Bypass Plugin" buttonText="Bypass" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif