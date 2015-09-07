/**
	ControllerGoal
	Controlls the goal display.

	@author Maikel
*/


// HUD margin and size in tenths of em.
static const GUI_Controller_Goal_IconSize = 60;
static const GUI_Controller_Goal_IconMargin = 10;

// Local variables to keep track of the goal HUD menu.
local goal_gui_target;
local goal_gui_menu;
local goal_gui_id;

public func Construction()
{
	var plr = GetOwner();
	// Create a menu target.
	goal_gui_target = CreateObject(Dummy, AbsX(0), AbsY(0), plr);
	goal_gui_target.Visibility = VIS_Owner;
	// Create the goal HUD menu.
	var y_margin = GUI_Controller_Wealth_IconMargin;
	var y_end = y_margin + GUI_Controller_Wealth_IconSize;
	// Also take into account the margin and size of the wealth HUD.
	var x_margin = y_margin + GUI_Controller_Wealth_IconMargin + GUI_Controller_Wealth_IconSize;
	var x_end = y_end + GUI_Controller_Wealth_IconMargin + GUI_Controller_Wealth_IconSize;
	goal_gui_menu = 
	{
		Target = goal_gui_target,
		ID = 1,
		Style = GUI_Multiple,
		Left = Format("100%%%s", ToEmString(-x_end)),
		Right = Format("100%%%s", ToEmString(-x_margin)),
		Top = ToEmString(y_margin),
		Bottom = ToEmString(y_end),
		Priority = 1,
		// hover part of the menu is created by a goal update.
		text = 
		{
			Target = goal_gui_target,
			ID = 3,
			Style = GUI_TextHCenter | GUI_TextBottom,
			Text = nil,
			Priority = 3,
		},
	};
	goal_gui_id = GuiOpen(goal_gui_menu);
	return _inherited(...);
}

public func Destruction()
{
	// This also closes the goal HUD menu.
	if (goal_gui_target)
		goal_gui_target->RemoveObject();
	return _inherited(...);
}

// Callback from the goal library: display this goal.
public func OnGoalUpdate(object goal)
{
	// Also notify the open info menu.
	OnGoalWindowUpdate(goal);
	// If there is no goal then remove the hover interface and hide the menu.
	if (!goal)
	{
		goal_gui_target.Visibility = VIS_None;
		GuiClose(goal_gui_id, goal_gui_menu.hover.ID, goal_gui_menu.Target);
		return _inherited(goal, ...);
	}
	// Only update if something has changed.
	if (goal_gui_menu.Symbol != goal->GetID() || goal_gui_menu.GraphicsName != goal->GetGraphics() || goal_gui_menu.text.Text != goal->~GetShortDescription(GetOwner()))
	{
		goal_gui_target.Visibility = VIS_Owner;
		goal_gui_menu.text.Text = goal->~GetShortDescription(GetOwner());
		goal_gui_menu.Symbol = goal->GetID();
		goal_gui_menu.GraphicsName = goal->GetGraphics();
		// Also add an hover and mouse click element.
		goal_gui_menu.hover = 
		{
			Target = goal_gui_target,
			ID = 2,
			Symbol = { Std = nil, OnHover = GUI_Controller_Goal},
			OnClick = GuiAction_Call(this, "OnGoalClick", GetOwner()),
			OnMouseIn = GuiAction_SetTag("OnHover"),
			OnMouseOut = GuiAction_SetTag("Std"),
			Priority = 2,
		};		
		GuiUpdate(goal_gui_menu, goal_gui_id);
	}
	return _inherited(goal, ...);
}

public func OnGoalClick(int plr)
{
	if (goal_info_menu)
		return;
	// Open the goal menu if not already open.
	if (!goal_info_menu)
		OpenGoalWindow(plr);
	return;
}


/*-- Goal Info Menu --*/

local goal_info_target;
local goal_info_menu;
local goal_info_id;

private func OpenGoalWindow(int plr)
{
	var goals = FindObjects(Find_Category(C4D_Goal));
	var nr_goals = GetLength(goals);
	var menu_width = BoundBy(nr_goals * 4, 20, 40); // in em
	
	// Create a menu target.
	goal_info_target = CreateObject(Dummy, AbsX(0), AbsY(0), plr);
	goal_info_target.Visibility = VIS_Owner;
	
	// Safety: there has to be at least one goal.
	if (nr_goals <= 0)
		return;
	
	// Main menu
	goal_info_menu =
	{
		Target = goal_info_target,
		Decoration = GUI_MenuDeco,
		Left = Format("50%%-%dem", menu_width),
		Right = Format("50%%+%dem", menu_width),
		Top = "50%-8em",
		Bottom = "50%+16em",
		BackgroundColor = {Std = 0},
		OnClose = GuiAction_Call(this, "OnGoalWindowClosed"),
	};
	
	// Close button
	GuiAddCloseButton(goal_info_menu, this, "OnCloseButtonClick");
	
	// Text submenu
	goal_info_menu.text =
	{
		Target = goal_info_target,
		ID = 1,
		Left = "0%",
		Right = "100%",
		Top = "0%+8em",
		Bottom = "100%",
		Text = "",
		BackgroundColor = {Std = 0},	
	};
	
	// Goal icons: maximum number of 10 goals for now
	var prop_goals = [];
	for (var i = 0; i < Min(10, nr_goals); i++)
	{
		var prop_goal = Format("goal%d", i);
		prop_goals[i] = GoalSubMenu(goals[i], i);	
		goal_info_menu[prop_goal] = prop_goals[i];
	}
	
	// Select first goal and its description.
	OnGoalGUIHover(goals[0]);

	goal_info_id = GuiOpen(goal_info_menu);
	return;
}

public func CloseGoalWindow()
{
	GuiClose(goal_info_id);
	return;
}

public func OnGoalWindowClosed()
{
	if (goal_info_target)
		goal_info_target->RemoveObject();
	goal_info_id = nil;
	goal_info_menu = nil;
	return;
}

private func GoalSubMenu(object goal, int nr, int size)
{
	if (size == nil)
		size = 8;
	
	// Create the goal submenu with id counting upwards from 2.
	var prop_goal = 
	{
		Target = goal_info_target,
		ID = nr + 2,
		Left = Format("0%%+%dem", nr * size),
		Right = Format("0%%+%dem", (nr + 1) * size),
		Top = "0%",
		Bottom = Format("0%%+%dem", size),
		Symbol = goal->GetID(),
		GraphicsName = goal->GetGraphics(),
		BackgroundColor = {Std = 0, Hover = 0x50ffffff},
		OnMouseIn = [GuiAction_SetTag("Hover"), GuiAction_Call(this, "OnGoalGUIHover", goal)],
		OnMouseOut = GuiAction_SetTag("Std"),
		goal_object = goal, 
	};
	// Indicate whether the goal is already fulfilled with a star.
	if (goal->~IsFulfilled())
	{
		prop_goal.star = 
		{
			Target = goal_info_target,
			Left = "100%-2em",
			Right = "100%",
			Top = "0%",
			Bottom = "0%+2em",
			Symbol = Icon_Ok,
			BackgroundColor = {Std = 0},	
		};
	}
	return prop_goal;
}

public func OnCloseButtonClick()
{
	CloseGoalWindow();
	return;
}

public func OnGoalGUIHover(object goal)
{
	// change text to the current goal.
	var prop_text = goal_info_menu.text;
	prop_text.Text = goal->~GetDescription(GetOwner());
	var id_text = prop_text.ID;
	GuiUpdate(prop_text, goal_info_id, id_text, goal_info_target);
	return;
}

private func OnGoalWindowUpdate(object goal)
{
	if (!goal_info_menu)
		return;
	
	var index = 0, goal_menu;
	while ((goal_menu = goal_info_menu[Format("goal%d", index)]))
	{
		if (goal_menu.goal_object == goal)
			break;
		index++;
	}
	if (!goal_menu)
		return;
	var prop_goal = Format("goal%d", index);
	goal_info_menu[prop_goal] = GoalSubMenu(goal, index);	
	GuiUpdate(goal_info_menu[prop_goal], goal_info_id, goal_info_menu[prop_goal].ID, goal_info_menu[prop_goal].Target);
	return;
}
