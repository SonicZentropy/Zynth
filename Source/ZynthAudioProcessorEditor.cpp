#include "ZynthAudioProcessorEditor.h"

//==============================================================================
ZynthAudioProcessorEditor::ZynthAudioProcessorEditor(ZynthAudioProcessor& ownerFilter)
	: AudioProcessorEditor(ownerFilter),
	undoButton("Undo"),
	redoButton("Redo")
{
	processor = &ownerFilter;

	// #NOTES: Change Is_Synth in AppConfig.h for Bitwig
    setName("ZynthMainComponent");
    addAndMakeVisible (muteButton = new AssociatedTextButton("Mute Button", processor->muteParam));
    muteButton->setTooltip ("Mute all audio");
    muteButton->setButtonText ("MUTE");
	muteButton->setClickingTogglesState(true);
    muteButton->addListener (this);

	
    addAndMakeVisible (gainSlider = new AssociatedSlider ("Gain Slider", processor->audioGainParam, "dBC"));
    gainSlider->setTooltip ("Adjusts audio gain");
    gainSlider->setRange (-96, 12, 0.01);
    gainSlider->setSliderStyle (Slider::LinearHorizontal);
    gainSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	gainSlider->setTextValueSuffix("dB");
	gainSlider->setDoubleClickReturnValue(true, 0.0);
	gainSlider->addListener (this);
	
    addAndMakeVisible (bypassButton = new AssociatedTextButton ("Bypass Button", processor->bypassParam));
    bypassButton->setTooltip ("Bypass Plugin");
    bypassButton->setButtonText ("Bypass");
	bypassButton->setClickingTogglesState(true);
    bypassButton->addListener (this);


	vTreeComponent = new Component();
	vTreeComponent->setSize(800, 800);
	vTreeComponent->setVisible(true);
	
	vTree = new ValueTreesDemo();

	//vtEditor = new ValueTreeEditor();
	//vtEditor->setContentOwned(&tree, true);
	//vtEditor->addAndMakeVisible(tree);

	//tree is TreeView
	tree.setDefaultOpenness(true);
	tree.setMultiSelectEnabled(true);
	tree.setRootItem(rootItem = new ValueTreeItem(createRootValueTree(), undoManager));
	tree.setColour(TreeView::backgroundColourId, Colours::white);
	addAndMakeVisible(tree);

	//vTreeComponent->addAndMakeVisible(tree);
	//vTree->addAndMakeVisible(vTreeComponent);

	//vtEditor->setContentNonOwned(vTreeComponent, true);

	//addAndMakeVisible(vTree);
	//vTree->
	

/*

	Identifier testRoot("TestRoot");
	ValueTree testTree(testRoot);
	Identifier testIdent("TestID");
	Identifier testIdent2("TestID2");
	testTree.setProperty(testIdent, 123, nullptr);
	testTree.setProperty(testIdent2, 545, nullptr);*/

	//vtEditor = new ValueTreeEditor();
	//vtEditor->setSource(testTree);
//	vtEditor->addToDesktop();

    this->setSize (600, 600);
	startTimer(50); // Start timer poll with 50ms rate

}

ZynthAudioProcessorEditor::~ZynthAudioProcessorEditor()
{
    muteButton = nullptr;
    gainSlider = nullptr;
    bypassButton = nullptr;
	vtEditor = nullptr;
	vTree = nullptr;
	tree.setRootItem(nullptr);
	rootItem = nullptr;
//	testTree = nullptr;

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

	Rectangle<int> r(getLocalBounds().reduced(8));

	Rectangle<int> buttons(r.removeFromBottom(22));
	undoButton.setBounds(buttons.removeFromLeft(100));
	buttons.removeFromLeft(6);
	redoButton.setBounds(buttons.removeFromLeft(100));

	r.removeFromBottom(4);
	tree.setBounds(r);
}





void ZynthAudioProcessorEditor::deleteSelectedItems()
{
	Array<ValueTree> selectedItems(ValueTreeItem::getSelectedTreeViewItems(tree));

	for (int i = selectedItems.size(); --i >= 0;)
	{
		ValueTree& v = selectedItems.getReference(i);

		if (v.getParent().isValid())
			v.getParent().removeChild(v, &undoManager);
	}
}

void ZynthAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{	
	if (buttonThatWasClicked == &undoButton)
		undoManager.undo();
	else if (buttonThatWasClicked == &redoButton)
		undoManager.redo();
	else dynamic_cast<AssociatedButton*>(buttonThatWasClicked)->setAssociatedParameterValueNotifyingHost();
}

void ZynthAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{    		
	dynamic_cast<AssociatedSlider*>(sliderThatWasMoved)->setAssociatedParameterValueNotifyingHost();
}

bool ZynthAudioProcessorEditor::keyPressed(const KeyPress& key)
{
	if (key == KeyPress::deleteKey)
	{
		deleteSelectedItems();
		return true;
	}

	if (key == KeyPress('z', ModifierKeys::commandModifier, 0))
	{
		undoManager.undo();
		return true;
	}

	if (key == KeyPress('z', ModifierKeys::commandModifier | ModifierKeys::shiftModifier, 0))
	{
		undoManager.redo();
		return true;
	}

	return Component::keyPressed(key);
}

void ZynthAudioProcessorEditor::timerCallback()
{
	undoManager.beginNewTransaction();
	//exchange data between UI Elements and the Plugin (ourProcessor)
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

