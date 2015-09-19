/*==============================================================================
//  ValueTreeWindow.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/19
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Display of Value Tree for debugging, etc
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/
#ifndef VALUETREEWINDOW_H_INCLUDED
#define VALUETREEWINDOW_H_INCLUDED
#include "JuceHeader.h"
/** Display a separate desktop window for viewed and editing a value tree's
property fields.
*/
class ValueTreeEditor : public DocumentWindow
{
	/** Display properties for a tree. */
	class PropertyEditor : public PropertyPanel
	{
	public:
		PropertyEditor()
		{
			noEditValue = "not editable";
		}

		void setSource(ValueTree& tree)
		{
			clear();
			t = tree;
			const int maxChars = 200;
			Array<PropertyComponent *> pc;

			for (int i = 0; i < t.getNumProperties(); ++i)
			{
				const Identifier name = t.getPropertyName(i).toString();
				Value v = t.getPropertyAsValue(name, nullptr);
				TextPropertyComponent* tpc;

				if (v.getValue().isObject())
				{
					tpc = new TextPropertyComponent(noEditValue, name.toString(), maxChars, false);
					tpc->setEnabled(false);
				} else
				{
					tpc = new TextPropertyComponent(v, name.toString(), maxChars, false);
				}

				pc.add(tpc);
			}

			addProperties(pc);
		}

	private:
		Value noEditValue;
		ValueTree t;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyEditor);
	};

	class Item : public TreeViewItem,
		public ValueTree::Listener
	{
	public:
		Item(PropertyEditor* propertiesEditor, ValueTree tree)
			:propertiesEditor(propertiesEditor),
			t(tree)
		{
			t.addListener(this);
		}

		~Item()
		{
			clearSubItems();
		}

		bool mightContainSubItems() override
		{
			return t.getNumChildren() > 0;
		}

		void itemOpennessChanged(bool isNowOpen) override
		{
			clearSubItems();
			if (!isNowOpen) return;
			int children = t.getNumChildren();
			for (int i = 0; i < children; ++i)
			{
				addSubItem(new Item(propertiesEditor, t.getChild(i)));
			}
		}

		virtual void valueTreeChildRemoved(ValueTree& parentTree,
			ValueTree& childWhichHasBeenRemoved,
			int indexFromWhichChildWasRemoved) override
		{
			
		}

		virtual void valueTreeChildOrderChanged(ValueTree& parentTree,
			int oldIndex,
			int newIndex) override
		{

		}

		void paintItem(Graphics& g, int w, int h) override
		{
			Font font;
			Font smallFont(11.0);
			if (isSelected())
				g.fillAll(Colours::white);
			const float padding = 20.0f;
			String typeName = t.getType().toString();
			const float nameWidth = font.getStringWidthFloat(typeName);
			const float propertyX = padding + nameWidth;

			g.setColour(Colours::black);
			g.setFont(font);
			g.drawText(t.getType().toString(), 0, 0, w, h, Justification::left, false);
			g.setColour(Colours::blue);
			g.setFont(smallFont);
			String propertySummary;

			for (int i = 0; i < t.getNumProperties(); ++i)
			{
				const Identifier name = t.getPropertyName(i).toString();
				propertySummary += " [" + name.toString() + "] " + t.getProperty(name).toString();
			}
			g.drawText(propertySummary, propertyX, 0, w - propertyX, h, Justification::left, true);
		}

		void itemSelectionChanged(bool isNowSelected) override
		{
			if (isNowSelected)
			{
				t.removeListener(this);
				propertiesEditor->setSource(t);
				t.addListener(this);
			}
		}

		/* Enormous list of ValueTree::Listener options... */
		void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override
		{
			if (t != treeWhosePropertyHasChanged) return;
			t.removeListener(this);
			//            if (isSelected())
			//                propertiesEditor->setSource(t);
			repaintItem();
			t.addListener(this);
		}

		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override
		{
			treeHasChanged();
		}

		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved)
		{
			treeHasChanged();
		}

		void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved)
		{
			treeHasChanged();
		}

		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override
		{
			treeHasChanged();
		}

		void valueTreeRedirected(ValueTree& treeWhichHasBeenChanged) override
		{
			treeHasChanged();
		}

	private:
		PropertyEditor* propertiesEditor;
		ValueTree t;
		Array<Identifier> currentProperties;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Item);
	};

	/** Display a tree. */
	class Editor : public Component
	{
	public:
		Editor() : layoutResizer(&layout, 1, false)
		{
			layout.setItemLayout(0, -0.1, -0.9, -0.6);
			layout.setItemLayout(1, 5, 5, 5);
			layout.setItemLayout(2, -0.1, -0.9, -0.4);
			setSize(1000, 700);
			addAndMakeVisible(treeView);
			addAndMakeVisible(propertyEditor);
			addAndMakeVisible(layoutResizer);
		}

		~Editor()
		{
			treeView.setRootItem(nullptr);
		}

		void resized() override
		{
			Component* comps[] = {&treeView, &layoutResizer, &propertyEditor};
			layout.layOutComponents(comps, 3, 0, 0, getWidth(), getHeight(), true, true);
		}

		void setTree(ValueTree newTree)
		{
			if (newTree == ValueTree::invalid)
			{
				treeView.setRootItem(nullptr);
			} else if (tree != newTree)
			{
				tree = newTree;
				treeView.setRootItem(new Item(&propertyEditor, tree));
			}
		}
	public:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor);
		ValueTree tree;
		TreeView treeView;
		PropertyEditor propertyEditor;
		StretchableLayoutManager layout;
		StretchableLayoutResizerBar layoutResizer;
	};
public:
	ValueTreeEditor()
		: DocumentWindow("Value Tree Editor",
			Colours::lightgrey,
			DocumentWindow::allButtons)
	{
		editor = new Editor();
		setContentNonOwned(editor, true);
		setResizable(true, false);
		setUsingNativeTitleBar(true);
		centreWithSize(getWidth(), getHeight());
		setVisible(true);
	}

	~ValueTreeEditor()
	{
		editor->setTree(ValueTree::invalid);
	}

	void closeButtonPressed() override
	{
		setVisible(false);
	}

	void setSource(ValueTree& v)
	{
		editor->setTree(v);
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeEditor);
	ScopedPointer<Editor> editor;
};
#endif // VALUETREEWINDOW_H_INCLUDED
