/**
	Lava Bubble
	Looks dangerous

	@author Win
*/

local Name = "Lava Bubble";
local Plane = 500;
local jumped = 0;

public func Construction()
{
	if (GBackSolid(0, 0))
	{
		RemoveObject();	
		return;
	}
	FadeIn(5, true);
	AddEffect("Move", this, 1, 2, this);
	Sound("Liquids::Bubble*");
	return;
}

private func FxMoveTimer(object target, effect, int time)
{
	
	// Fade out bubble if outside liquid or time is up.
	if (time > 200 && !GetEffect("ObjFade", this))
		FadeOut(70, true);

	if(!GBackLiquid(0, -3) && !jumped)
			{
			SetPosition(GetX(),GetY()-1);
			SetYDir(GetYDir() + RandomX(-10,-20));
			Sound("Bubble*");
			jumped = 1;
			}
	if(!jumped)
	{
		// Causes bubbles to repel each other.
		var nearby_bubble = FindObject(Find_Distance(GetCon()/15, 0, 0), Find_ID(GetID()));
		if (nearby_bubble)
		{
			SetXDir(GetXDir() -(nearby_bubble->GetX() - GetX()) / 2);
			SetYDir(GetYDir() -(nearby_bubble->GetY() - GetY()) / 2);
		}
	}
	// Grow bubble over time.
	if ((time % 6) == 0)
		DoCon(2);
	
	// Explode near living things
	var prey = FindObject(Find_Distance(5, 0, 0), Find_OCF(OCF_Alive));
	if (prey)
	{
		Explode(10);
		return FX_OK;	
	}
	
	if (Inside(GetXDir(), -2, 2))
		SetXDir(GetXDir() + RandomX(-2, 2));
	// Delete when jumped out of lava and dove back in
	if(GBackLiquid(0, 0))
		if(jumped && !GetEffect("ObjFade", this))
		{
			FadeOut(8, true);
		}
	return FX_OK;
}

// No need to blow up scenario object list with bubble spam.
func SaveScenarioObject() { return false; }
