/**
	MagmaBubble
	Looks dangerous

	@author 
*/

global func CastMagmaBubbles(int num, int level, int x, int y)
{
	return CastObjects(Fx_MagmaBubble, num, level, x, y);
}

protected func Initialize()
{
	DoCon(RandomX(5, 15));
	AddEffect("Move", this, 100, 2, this);
	Sound("ef_Bubble*");
	return;
}

public func FxMoveTimer(object target, effect, int time)
{
	if (!GBackLiquid(0, -3) && !GetEffect("Fade", this) || time > 108)
		AddEffect("Fade", target, 100, 1, target);

	// Bubbles burst into smaller bubles
	if (!Random(25) && target->GetCon() > 100)
	{
		for (var i = 0; i < 2; i++)
		{
			var bubble = CreateObjectAbove(Fx_MagmaBubble);
			bubble->SetCon(2 * target->GetCon() / 3);
			bubble->SetYDir(target->GetYDir());
		}
		RemoveObject();
		return -1;
	}

	// Jittery movement
	SetYDir(GetYDir() - 3 + Random(7));
	if (Inside(GetXDir(), -6, 6))
		SetXDir(GetXDir() + 2 * Random(2) - 1);
		
	var prey = FindObject(Find_Distance(GetCon()/15, 0, 0), Find_OCF(OCF_Alive));
	if(prey != nil)
			Explode(10);

	return 1;
}

public func FxFadeStart(object target, effect, int temporary)
{
	// Store alpha here
	if (temporary == 0)
		effect.alpha = 255;
	return 1;
}

public func FxFadeTimer(object target, effect)
{
	var alpha = effect.alpha;
	if (alpha <= 0)
	{
		RemoveEffect("Move", this);
		RemoveObject();
		return -1;
	}
	SetClrModulation(RGBa(255, 255, 255, alpha));
	effect.alpha = alpha - 5;
	return 1;
}

// No need to blow up scenario object list with bubble spam
func SaveScenarioObject() { return false; }
