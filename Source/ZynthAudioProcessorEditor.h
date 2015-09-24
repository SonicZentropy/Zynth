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
#include "zen_utils/GUI/ZenDebugWindow.h"


using namespace Zen;
using jcf::ValueTreeEditor;

/// <summary> GUI Editor for zynth audio processor. </summary>
class ZynthAudioProcessorEditor  : public AudioProcessorEditor,
                                   public Timer,
	                               public DragAndDropContainer,
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

private:
	ZynthAudioProcessor* processor;
    
    //==============================================================================
    ScopedPointer<AssociatedTextButton> muteButton;
    ScopedPointer<AssociatedSlider> gainSlider;
    ScopedPointer<AssociatedTextButton> bypassButton;
	
	ScopedPointer<ZenDebugWindow> zWin;
	ScopedPointer<ValueTreeEditor> vtEditor;
	

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZynthAudioProcessorEditor)
		void createComponentsTree();
};

#endif   // __JUCE_HEADER_E0F368D2F71ED5FC__