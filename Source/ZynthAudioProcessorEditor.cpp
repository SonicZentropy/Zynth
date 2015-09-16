#include "ZynthAudioProcessorEditor.h"

//==============================================================================
ZynthAudioProcessorEditor::ZynthAudioProcessorEditor (ZynthAudioProcessor& ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
	processor = &ownerFilter;

	// #NOTES: Change Is_Synth in AppConfig.h for Bitwig
    setName("ZynthMainComponent");
    addAndMakeVisible (muteButton = new AssociatedTextButton("Mute Button", processor->muteParam));
    muteButton->setTooltip ("Mute all audio");
    muteButton->setButtonText ("MUTE");
    muteButton->addListener (this);

	
    addAndMakeVisible (gainSlider = new AssociatedSlider ("Gain Slider", processor->audioGainParam, "dBC"));
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
	dynamic_cast<AssociatedButton*>(buttonThatWasClicked)->setAssociatedParameterValueNotifyingHost();
}

void ZynthAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{    		
	dynamic_cast<AssociatedSlider*>(sliderThatWasMoved)->setAssociatedParameterValueNotifyingHost();
}

void ZynthAudioProcessorEditor::timerCallback()
{
	//exchange data between UI Elements and the Plugin (ourProcessor)
	
	AssociatedSlider* currentSlider;
	AssociatedButton* currentButton;
	AssociatedComponent* currentComponent;
	ZenParameter* currentParameter;

	int numComponents = this->getNumChildComponents();
	// #TODO: Eventually change GUI updates to messages rather than polling
	//This looks ugly, but it's a whole lot better than a massive if chain checking EVERY param individually
	for (int i = 0; i < numComponents; i++)
	{
		currentComponent = dynamic_cast<AssociatedComponent*>(this->getChildComponent(i));
		currentParameter = currentComponent->getAssociatedParameter();
		if (!currentParameter->needsUIUpdate()) //Don't need to keep going if the Component doesn't need to be updated
		{
			continue;
		}

		currentSlider = dynamic_cast<AssociatedSlider*>(currentComponent);
		if(currentSlider != nullptr)
		{
			currentSlider->setValue(currentParameter->getValueForGUIComponent(), dontSendNotification);
			DBG("Setting current slider " + String(currentParameter->getName()) + " in GUI Update");
		} 
		else 
		{
			currentButton = dynamic_cast<AssociatedButton*>(this->getChildComponent(i));
			if (currentButton != nullptr)
			{
				currentButton->setToggleState(currentParameter->getBoolFromValue(), dontSendNotification);
				DBG("Setting current button " + String(currentParameter->getName()) + " in GUI Update");
			}
			else
			{
				throw std::logic_error("Somehow a component is of no detected type!");
			}
			
		}
		currentParameter->resetUIUpdate();
	}
}

