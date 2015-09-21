/*==============================================================================
//  ParameterValueTree.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/20
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Constructs and maintains a ValueTree representation of a parameter and its associated data
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ParameterValueTree.h"
namespace Zen
{
	//paramValueTree = new ParameterValueTree(name, value.getValueSource(), defaultValue, shouldBeSmoothed);
	
	ParameterValueTree::ParameterValueTree(String inName, Value parameterValueReference, float inDefaultValue, bool inShouldBeSmoothed)
		: parameterName(inName), 
		parameterValue(parameterValueReference), 
		defaultValue(inDefaultValue), 
		isSmoothed(inShouldBeSmoothed),
		parameterValueTree("Parameter")
	{
	

	}

	ParameterValueTree::~ParameterValueTree()
	{
	
	}

	void ParameterValueTree::constructValueTree()
	{
	   parameterValueTree.setProperty("name", )
	}

}