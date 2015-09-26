#include "ZynthAudioProcessorEditor.h"

//==============================================================================
ZynthAudioProcessorEditor::ZynthAudioProcessorEditor(ZynthAudioProcessor& ownerFilter)
	: AudioProcessorEditor(ownerFilter)
{
////	DBGM("In ZynthAudioProcessorEditor::ZynthAudioProcessorEditor() ");
	processor = &ownerFilter;

	// #NOTES: Change Is_Synth in AppConfig.h for Bitwig
	// #TODO: CURRENT WORK SPOT - figure out why gain value in gui gets reset to 10db sometimes
    setName("ZynthMainComponent");
    addAndMakeVisible (muteButton = new AssociatedTextButton("Mute Button", processor->muteParam));
    muteButton->setTooltip ("Mute all audio");
    muteButton->setButtonText ("MUTE");
	muteButton->setClickingTogglesState(true);
    muteButton->addListener (this);

	
    addAndMakeVisible (gainSlider = new AssociatedSlider ("Gain Slider", processor->audioGainParam, "dB"));
	// #TODO: SET VALUE HERE
    gainSlider->setTooltip ("Adjusts audio gain");
    gainSlider->setRange (-96, 12, 0.01);
    gainSlider->setSliderStyle (Slider::LinearHorizontal);
    gainSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	gainSlider->setTextValueSuffix("dB");
	gainSlider->setDoubleClickReturnValue(true, 0.0);
	//gainSlider->setValue(processor->)
	gainSlider->addListener (this);
	
    addAndMakeVisible (bypassButton = new AssociatedTextButton ("Bypass Button", processor->bypassParam));
    bypassButton->setTooltip ("Bypass Plugin");
    bypassButton->setButtonText ("Bypass");
	bypassButton->setClickingTogglesState(true);
    bypassButton->addListener (this);

    this->setSize (400, 400);

//	compDebugger = new ComponentDebugger(this);

	startTimer(50); // Start timer poll with 50ms rate
}

ZynthAudioProcessorEditor::~ZynthAudioProcessorEditor()
{
////	DBGM("In ZynthAudioProcessorEditor::~ZynthAudioProcessorEditor() ");
    muteButton = nullptr;
    gainSlider = nullptr;
    bypassButton = nullptr;
		
	//compDebugger = nullptr;
}

//==============================================================================
void ZynthAudioProcessorEditor::paint (Graphics& g)
{

    g.fillAll (Colour (0xff303030));
}

void ZynthAudioProcessorEditor::resized()
{
////	DBGM("In ZynthAudioProcessorEditor::resized() ");
    muteButton->setBounds (10, 6, 74, 24);
    gainSlider->setBounds (158, 8, 150, 24);
    bypassButton->setBounds (10, 38, 74, 24);
}


void ZynthAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{	
////	 DBGM("In ZynthAudioProcessorEditor::buttonClicked() ");
	 dynamic_cast<AssociatedButton*>(buttonThatWasClicked)->setAssociatedParameterValueNotifyingHost();
}

void ZynthAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{    		
////	DBGM("In ZynthAudioProcessorEditor::sliderValueChanged() with: " + tempSlider->getName());
	dynamic_cast<AssociatedSlider*>(sliderThatWasMoved)->setAssociatedParameterValueNotifyingHost();
}


void ZynthAudioProcessorEditor::timerCallback()
{
	//undoManager.beginNewTransaction();
	AssociatedSlider* currentSlider;
	AssociatedButton* currentButton;
	AssociatedComponent* currentComponent;
	ZenParameter* currentParameter;

	int numComponents = this->getNumChildComponents();
	// #ENHANCE:  Eventually change GUI updates to messages rather than polling
	//This looks ugly, but it's a whole lot better than a massive if chain checking EVERY param individually
	//This should handle every component's GUI updates automatically
	for (int i = 0; i < numComponents; i++)
	{
		currentComponent = dynamic_cast<AssociatedComponent*>(this->getChildComponent(i));
		if (nullptr == currentComponent) continue;

		currentParameter = currentComponent->getAssociatedParameter();
		if (!currentParameter->needsUIUpdate()) //Don't need to keep going if the Component doesn't need to be updated
		{
			continue;
		}

		currentSlider = dynamic_cast<AssociatedSlider*>(currentComponent);
		if(currentSlider != nullptr)
		{
			currentSlider->setValue(currentParameter->getValueForGUIComponent(), dontSendNotification);
 		} 
		else 
		{
			currentButton = dynamic_cast<AssociatedButton*>(this->getChildComponent(i));
			if (currentButton != nullptr)
			{
				currentButton->setToggleState(currentParameter->getBoolFromValue(), dontSendNotification);
 			}
			else
			{
				DBG("In ZynthAudioProcessorEditor::timerCallback() ");
				jassertfalse;
			}
		}
		currentParameter->resetUIUpdate(); //Finished Updating UI
	}
}

