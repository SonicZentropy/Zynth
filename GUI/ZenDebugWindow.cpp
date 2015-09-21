#include "ZenDebugWindow.h"

ZenDebugWindow::ZenDebugWindow ()
:DocumentWindow("Zen Debug Window", Colours::lightgrey, DocumentWindow::allButtons)
{
	setResizable(true, false);
	setUsingNativeTitleBar(true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);

	valueTreeDebugComponent = new ValueTreeDebugComponent();
	setContentNonOwned(valueTreeDebugComponent, true);
	setSize(800, 800);
}

/*
ZenDebugWindow::ZenDebugWindow(ValueTreeDebugComponent* inComponent)
	:DocumentWindow("Zen Debug Window", Colours::lightgrey, DocumentWindow::allButtons)
{
	setResizable(true, false);
	setUsingNativeTitleBar(true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);

	valueTreeDebugComponent = inComponent;
	setContentNonOwned(valueTreeDebugComponent, true);
	setSize(800, 800);
}*/


ZenDebugWindow::ZenDebugWindow(ValueTree* inValueTree)
	:DocumentWindow("Zen Debug Window", Colours::lightgrey, DocumentWindow::allButtons)
{
	setResizable(true, false);
	setUsingNativeTitleBar(true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);

	valueTreeDebugComponent = new ValueTreeDebugComponent(inValueTree);
	setContentNonOwned(valueTreeDebugComponent, true);
	setSize(800, 800);
}


ZenDebugWindow::~ZenDebugWindow()
{
	valueTreeDebugComponent = nullptr;
	
}

void ZenDebugWindow::closeButtonPressed() 
{
	setVisible(false);
}

void ZenDebugWindow::addValueTree(ValueTree inValueTree)
{
	
}