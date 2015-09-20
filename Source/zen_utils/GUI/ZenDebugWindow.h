/*==============================================================================
//  ZenDebugWindow.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/20
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Debug Window For Trees
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENDEBUGWINDOW_H_INCLUDED
#define ZENDEBUGWINDOW_H_INCLUDED
#include "JuceHeader.h"

class ZenDebugWindow : public DocumentWindow
{
public:
	ZenDebugWindow();
	~ZenDebugWindow();

	void closeButtonPressed() override;
};

#endif // ZENDEBUGWINDOW_H_INCLUDED