/*-- Tree Trunk 2 Front --*/

#include Library_Cover

local back;

private func Initialize()
{
	this.cover_area = Shape->Rectangle(-118, -24, 236, 57);
	_inherited();
}

public func Set(object trunk)
{
	back = trunk;
	AddTimer("CheckPosition", 5);
}

private func CheckPosition()
{
	if (!back) return RemoveObject();
	if (GetX() != back->GetX() || GetY() != back->GetY())
		SetPosition(back->GetX()+1, back->GetY()+2);
}

func EditCursorMoved()
{
	// Move main trunk along with front in editor mode
	if (back) back->SetPosition(GetX()+1, GetY()+2);
	return true;
}

func SaveScenarioObject() { return false; }

local Plane = 505;