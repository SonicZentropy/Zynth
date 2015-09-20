#include "ZenDebugWindow.h"

ZenDebugWindow::ZenDebugWindow ()
:DocumentWindow("Zen Debug Window", Colours::lightgrey, DocumentWindow::allButtons)
{
	setResizable(true, false);
	setUsingNativeTitleBar(true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);
}

ZenDebugWindow::~ZenDebugWindow()
{
	
}

void ZenDebugWindow::closeButtonPressed() 
{
	setVisible(false);
}