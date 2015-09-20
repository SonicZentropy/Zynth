/*==============================================================================
//  ZynthAudioProcessorEditor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/14
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Main GUI Component Controller
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/


#ifndef __JUCE_HEADER_E0F368D2F71ED5FC__
#define __JUCE_HEADER_E0F368D2F71ED5FC__

#include "JuceHeader.h"
#include "ZynthAudioProcessor.h"
#include "zen_utils/ZenHeader.h"
#include "zen_utils/utilities/ValueTreeWindow.h"
#include "zen_utils/GUI/ZenDebugWindow.h"


using namespace Zen;

/// <summary> GUI Editor for zynth audio processor. </summary>
class ZynthAudioProcessorEditor  : public AudioProcessorEditor,
                                   public Timer,
	                               public DragAndDropContainer,
	                               //public Component,
                                   public ButtonListener,
                                   public SliderListener
{
public:
    //==============================================================================
	explicit ZynthAudioProcessorEditor (ZynthAudioProcessor& ownerFilter);
    ~ZynthAudioProcessorEditor();

    
	void timerCallback() override;    

	//==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;


	void deleteSelectedItems();
	bool keyPressed(const KeyPress& key) override;

	static ValueTree createTree(const String& desc)
	{
		ValueTree t("Item");
		t.setProperty("name", desc, nullptr);
		return t;
	}

	static ValueTree createRandomTree(int& counter, int depth)
	{
		ValueTree t = createTree("Item " + String(counter++));

		if (depth < 3)
			for (int i = 1 + Random::getSystemRandom().nextInt(7); --i >= 0;)
				t.addChild(createRandomTree(counter, depth + 1), -1, nullptr);

		return t;
	}


	static ValueTree createRootValueTree()
	{
		ValueTree vt = createTree("This demo displays a ValueTree as a treeview.");
		vt.addChild(createTree("You can drag around the nodes to rearrange them"), -1, nullptr);
		vt.addChild(createTree("..and press 'delete' to delete them"), -1, nullptr);
		vt.addChild(createTree("Then, you can use the undo/redo buttons to undo these changes"), -1, nullptr);

		int n = 1;
		vt.addChild(createRandomTree(n, 0), -1, nullptr);

		return vt;
	}


private:
	ZynthAudioProcessor* processor;
    
    //==============================================================================
    ScopedPointer<AssociatedTextButton> muteButton;
    ScopedPointer<AssociatedSlider> gainSlider;
    ScopedPointer<AssociatedTextButton> bypassButton;

	ScopedPointer<Component> vTreeComponent;
	ScopedPointer<ValueTreeEditor> vtEditor;
	ScopedPointer<ValueTreesDemo> vTree;
	ZenDebugWindow* zWin;

	//ScopedPointer<ValueTree> testTree;
	TreeView tree;
	TextButton undoButton, redoButton;
	ScopedPointer<ValueTreeItem> rootItem;
	UndoManager undoManager;
	

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZynthAudioProcessorEditor)
};

#endif   // __JUCE_HEADER_E0F368D2F71ED5FC__