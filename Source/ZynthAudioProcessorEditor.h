/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_E0F368D2F71ED5FC__
#define __JUCE_HEADER_E0F368D2F71ED5FC__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "ZynthAudioProcessor.h"
#include "zen_utils/ZenHeader.h"
using namespace Zen;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
Main GUI Handler
                                                                    //[/Comments]
*/
class ZynthAudioProcessorEditor  : public AudioProcessorEditor,
                                   public Timer,
                                   public ButtonListener,
                                   public SliderListener
{
public:
    //==============================================================================
	explicit ZynthAudioProcessorEditor (ZynthAudioProcessor& ownerFilter);
    ~ZynthAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
		void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	ZynthAudioProcessor* processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<AssociatedTextButton> muteButton;
    ScopedPointer<AssociatedSlider> gainSlider;
    ScopedPointer<AssociatedTextButton> bypassButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZynthAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...

//[/EndFile]

#endif   // __JUCE_HEADER_E0F368D2F71ED5FC__
