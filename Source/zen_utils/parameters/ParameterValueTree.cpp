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
#include "..\utilities\ZenParamUtils.h"
namespace Zen
{
	//paramValueTree = new ParameterValueTree(name, value.getValueSource(), defaultValue, shouldBeSmoothed);
	
ParameterValueTree::ParameterValueTree()
{
	DBG("In ParameterValueTree::ParameterValueTree() - Should never be called");
	jassertfalse;
}

ParameterValueTree::ParameterValueTree(String inName, Value parameterValueReference, float inDefaultValue, bool inShouldBeSmoothed)
	: parameterName(inName),
	parameterValue(parameterValueReference),
	defaultValue(inDefaultValue),
	isSmoothed(inShouldBeSmoothed),
	parameterValueTree("Parameter")
{
	constructValueTree();
	parameterValue.addListener(this);
	defaultValue.addListener(this);
	isSmoothed.addListener(this);
	
}

ParameterValueTree::~ParameterValueTree()
{ }

void ParameterValueTree::constructValueTree()
{
	//parameter value, defaultvalue, isSmoothed
	
	// #TODO: CURRENT WORK SPOT, trying to fix valuetree for serialization
	parameterValueTree.setProperty("name", parameterName, nullptr);
	
	ValueTree valueChild("parameterValue");
	valueChild.setProperty("name", parameterName, nullptr);
	valueChild.setProperty("value", parameterValue, nullptr);
	parameterValueTree.addChild(valueChild, -1, nullptr);

	ValueTree defaultValueChild("defaultParameterValue");
	defaultValueChild.setProperty("name", "Default Value: " + defaultValue.getValue().toString(), nullptr);
	parameterValueTree.addChild(defaultValueChild, -1, nullptr);

	ValueTree parameterSmoothedChild("isSmoothed");
	parameterSmoothedChild.setProperty("name", "Is Smoothed: " + isSmoothed.getValue().toString() , nullptr);
	parameterValueTree.addChild(parameterSmoothedChild, -1, nullptr);

}

ValueTree ParameterValueTree::getValueTree()
{
	return parameterValueTree;
}

void ParameterValueTree::valueChanged(Value& value)
{
	if(value == parameterValue)
		parameterValueTree.getChildWithName("parameterValue").setProperty("name", "Value: " + parameterValue.getValue().toString(), nullptr);
	else if (value == defaultValue)
		parameterValueTree.getChildWithName("defaultParameterValue").setProperty("name", "Default Value: " + defaultValue.getValue().toString(), nullptr);
	else if (value == isSmoothed)
		parameterValueTree.getChildWithName("isSmoothed").setProperty("name", "Is Smoothed: " + isSmoothed.getValue().toString(), nullptr);
}

}  // Namespace Zen