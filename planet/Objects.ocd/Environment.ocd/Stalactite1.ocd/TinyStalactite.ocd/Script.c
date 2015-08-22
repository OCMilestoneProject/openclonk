/**
	TinyStalactite
	Covers up the base of stalactites

	@author Armin
*/

private func Initialize()
{
}

private func SetParent(object target)
{
	parent = target;
	AddEffect("Parent",this,1,1,this);
}

private func FxParentTimer(object target, effect, int time)
{
	//SetPosition(parent->GetX(), parent->GetY());
}

private func DrawWaterSource()
{
	var x = GetX();
	var y = GetY();
	for (var i = 0; i < RandomX(30,140); i++)
	{
		DrawMaterialQuad("Water", x,y, x+1,y, x+1,y+1, x,y+1);
		y--;
		x = x + RandomX(-1, 1);
	}
}

local Name = "$Name$";
local Description = "$Description$";
local parent;
local Plane = 500;