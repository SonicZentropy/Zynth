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
// Display a separate desktop window for viewed and editing a value tree's property fields.

//==============================================================================
class ValueTreeItem : public TreeViewItem,
	private ValueTree::Listener
{
public:
	ValueTreeItem(const ValueTree& v, UndoManager& um)
		: tree(v), undoManager(um)
	{
		tree.addListener(this);
	}

	String getUniqueName() const override
	{
		return tree["name"].toString();
	}

	bool mightContainSubItems() override
	{
		return tree.getNumChildren() > 0;
	}

	void paintItem(Graphics& g, int width, int height) override
	{
		g.setColour(Colours::black);
		g.setFont(15.0f);

		g.drawText(tree["name"].toString(),
			4, 0, width - 4, height,
			Justification::centredLeft, true);
	}

	void itemOpennessChanged(bool isNowOpen) override
	{
		if (isNowOpen && getNumSubItems() == 0)
			refreshSubItems();
		else
			clearSubItems();
	}

	var getDragSourceDescription() override
	{
		return "Drag Demo";
	}

	bool isInterestedInDragSource(const DragAndDropTarget::SourceDetails& dragSourceDetails) override
	{
		return dragSourceDetails.description == "Drag Demo";
	}

	void itemDropped(const DragAndDropTarget::SourceDetails&, int insertIndex) override
	{
		moveItems(*getOwnerView(),
			getSelectedTreeViewItems(*getOwnerView()),
			tree, insertIndex, undoManager);
	}

	static void moveItems(TreeView& treeView, const Array<ValueTree>& items,
		ValueTree newParent, int insertIndex, UndoManager& undoManager)
	{
		if (items.size() > 0)
		{
			ScopedPointer<XmlElement> oldOpenness(treeView.getOpennessState(false));

			for (int i = items.size(); --i >= 0;)
			{
				ValueTree& v = items.getReference(i);

				if (v.getParent().isValid() && newParent != v && !newParent.isAChildOf(v))
				{
					if (v.getParent() == newParent && newParent.indexOf(v) < insertIndex)
						--insertIndex;

					v.getParent().removeChild(v, &undoManager);
					newParent.addChild(v, insertIndex, &undoManager);
				}
			}

			if (oldOpenness != nullptr)
				treeView.restoreOpennessState(*oldOpenness, false);
		}
	}

	static Array<ValueTree> getSelectedTreeViewItems(TreeView& treeView)
	{
		Array<ValueTree> items;

		const int numSelected = treeView.getNumSelectedItems();

		for (int i = 0; i < numSelected; ++i)
			if (const ValueTreeItem* vti = dynamic_cast<ValueTreeItem*> (treeView.getSelectedItem(i)))
				items.add(vti->tree);

		return items;
	}

private:
	ValueTree tree;
	UndoManager& undoManager;

	void refreshSubItems()
	{
		clearSubItems();

		for (int i = 0; i < tree.getNumChildren(); ++i)
			addSubItem(new ValueTreeItem(tree.getChild(i), undoManager));
	}

	void valueTreePropertyChanged(ValueTree&, const Identifier&) override
	{
		repaintItem();
	}

	void valueTreeChildAdded(ValueTree& parentTree, ValueTree&) override { treeChildrenChanged(parentTree); }
	void valueTreeChildRemoved(ValueTree& parentTree, ValueTree&, int) override { treeChildrenChanged(parentTree); }
	void valueTreeChildOrderChanged(ValueTree& parentTree, int, int) override { treeChildrenChanged(parentTree); }
	void valueTreeParentChanged(ValueTree&) override {}

	void treeChildrenChanged(const ValueTree& parentTree)
	{
		if (parentTree == tree)
		{
			refreshSubItems();
			treeHasChanged();
			setOpen(true);
		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeItem)
};

//==============================================================================
class ValueTreesDemo : public Component,
	public DragAndDropContainer,
	private ButtonListener,
	private Timer
{
public:
	ValueTreesDemo()
		: undoButton("Undo"),
		redoButton("Redo")
	{
		addAndMakeVisible(tree);

		tree.setDefaultOpenness(true);
		tree.setMultiSelectEnabled(true);
		tree.setRootItem(rootItem = new ValueTreeItem(createRootValueTree(), undoManager));
		tree.setColour(TreeView::backgroundColourId, Colours::white);

		addAndMakeVisible(undoButton);
		addAndMakeVisible(redoButton);
		undoButton.addListener(this);
		redoButton.addListener(this);

		startTimer(500);
	}

	~ValueTreesDemo()
	{
		tree.setRootItem(nullptr);
	}

	void paint(Graphics& g) override
	{
		//fillTiledBackground(g);
	//	g.fillAll(Colour(0xff303030));
	}

	void resized() override
	{
		Rectangle<int> r(getLocalBounds().reduced(8));

		Rectangle<int> buttons(r.removeFromBottom(22));
		undoButton.setBounds(buttons.removeFromLeft(100));
		buttons.removeFromLeft(6);
		redoButton.setBounds(buttons.removeFromLeft(100));

		r.removeFromBottom(4);
		tree.setBounds(r);
	}

	static ValueTree createTree(const String& desc)
	{
		ValueTree t("Item");
		t.setProperty("name", desc, nullptr);
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

	static ValueTree createRandomTree(int& counter, int depth)
	{
		ValueTree t = createTree("Item " + String(counter++));

		if (depth < 3)
			for (int i = 1 + Random::getSystemRandom().nextInt(7); --i >= 0;)
				t.addChild(createRandomTree(counter, depth + 1), -1, nullptr);

		return t;
	}

	void deleteSelectedItems()
	{
		Array<ValueTree> selectedItems(ValueTreeItem::getSelectedTreeViewItems(tree));

		for (int i = selectedItems.size(); --i >= 0;)
		{
			ValueTree& v = selectedItems.getReference(i);

			if (v.getParent().isValid())
				v.getParent().removeChild(v, &undoManager);
		}
	}

	bool keyPressed(const KeyPress& key) override
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

	void buttonClicked(Button* b) override
	{
		if (b == &undoButton)
			undoManager.undo();
		else if (b == &redoButton)
			undoManager.redo();
	}

private:
	TreeView tree;
	TextButton undoButton, redoButton;
	ScopedPointer<ValueTreeItem> rootItem;
	UndoManager undoManager;

	void timerCallback() override
	{
		undoManager.beginNewTransaction();
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreesDemo);
};

//==============================================================================
//==============================================================================

class ValueTreeEditor : public DocumentWindow
{
//==============================================================================
	/** Display properties for a tree. */
	class PropertyEditor : public PropertyPanel
	{
	public:
		PropertyEditor()
		{
			noEditValue = "not editable";
		}
		virtual ~PropertyEditor()
		{
			DBG( "Property editor child component #: " + String(this->getNumChildComponents()) );
			
			//noEditValue.
			

			this->clear();
			
		}

		void setSource(ValueTree& tree)
		{
			DBG("SetSource");
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
				delete tpc;
				tpc = nullptr;
			}

			addProperties(pc);
			pc.clear();
			DBG("End SetSource");
		}

	///private:
		Value noEditValue;
		ValueTree t;
		JUCE_DECLARE_NON_COPYABLE(PropertyEditor);
	};
//==============================================================================
	class Item : public TreeViewItem,
		public ValueTree::Listener
	{
	public:
		Item(PropertyEditor* inPropertiesEditor, ValueTree tree)
			:t(tree)
		{
			propertiesEditor = inPropertiesEditor;
			t.addListener(this);
		}

		~Item()
		{
			clearSubItems();
			propertiesEditor = nullptr;
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

		virtual void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override
		{

		}

		virtual void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override
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
		ScopedPointer<PropertyEditor> propertiesEditor;
		ValueTree t;
		Array<Identifier> currentProperties;
		JUCE_DECLARE_NON_COPYABLE(Item);
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
			vtDemo = new ValueTreesDemo();
			addAndMakeVisible(vtDemo);
			/*addAndMakeVisible(treeView);
			addAndMakeVisible(propertyEditor);
			addAndMakeVisible(layoutResizer);*/
		}

		~Editor()
		{
			//treeView.deleteAllChildren();
			//treeView.getNumChildComponents();
			
			//HERE HERE
			//treeView.deleteRootItem();
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
				// HERE HERE
				treeView.setRootItem(new Item(&propertyEditor, tree));
			}
		}
	public:
		JUCE_DECLARE_NON_COPYABLE(Editor);
		ScopedPointer<ValueTreesDemo> vtDemo;
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
			DocumentWindow::allButtons),
		    editor()
	{
		//HERE HERE HERE
		setContentNonOwned(&editor, true);
		setResizable(true, false);
		setUsingNativeTitleBar(true);
		centreWithSize(getWidth(), getHeight());
		setVisible(true);
	}

	~ValueTreeEditor()
	{
		editor.setTree(ValueTree::invalid);
	}

	void closeButtonPressed() override
	{
		setVisible(false);
	}

	void setSource(ValueTree& v)
	{
		editor.setTree(v);
	}

private:
	JUCE_DECLARE_NON_COPYABLE(ValueTreeEditor);
	Editor editor;
};

#endif // VALUETREEWINDOW_H_INCLUDED
