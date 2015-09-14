#include "ZynthAudioProcessorEditor.h"

//==============================================================================
ZynthAudioProcessorEditor::ZynthAudioProcessorEditor (ZynthAudioProcessor& ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
	processor = &ownerFilter;

	// #TODO: Change Is_Synth in AppConfig.h for Bitwig
    setName("ZynthMainComponent");
    addAndMakeVisible (muteButton = new AssociatedTextButton("Mute Button", processor->muteParam));
    muteButton->setTooltip ("Mute all audio");
    muteButton->setButtonText ("MUTE");
    muteButton->addListener (this);

	
    addAndMakeVisible (gainSlider = new AssociatedSlider ("Gain Slider", processor->audioGainParam, "dBC"));
	//setValue from processor->audioGainParam->getValueInDecibels
	// #TODO: construct AssociatedParameter set from value internal
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

