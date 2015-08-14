/**
	AcidBubble
	Looks dangerous

	@author 
*/

local isExplosive = 0;

local growTime = 2;
local growTimer = 0;

local init = 0;

global func CastAcidBubbles(int num, int level, int x, int y)
{
	return CastObjects(Fx_AcidBubble, num, level, x, y);
}

protected func Initialize()
{
	if(init == 1)
		return -1;
	init = 1;
	DoCon(RandomX(-70, -60));
	AddEffect("Move", this, 100, 2, this);
	
		
	if(!Random(10))
		{
		isExplosive = 1;
		SetGraphics("2");
		}
	return;
}

public func FxMoveTimer(object target, effect, int time)
{
	if (!GBackLiquid(0, -3) && !GetEffect("Fade", this) || time > 200)
		{
		AddEffect("Fade", target, 100, 1, target);
		if(!Random(20))
			Sound("ef_Bubble*");
		}
	
	growTimer++;
	
	if(growTimer >= growTime)
		{
		DoCon(2);
		growTimer = 0;
		}
	
	var nearbyBubble = FindObject(Find_Distance(GetCon()/15, 0, 0), Find_ID(Fx_AcidBubble));
	if(nearbyBubble != nil)
	{
		SetXDir(-(nearbyBubble->GetX() - GetX()) / 2);
		SetYDir(-(nearbyBubble->GetY() - GetY()) / 2);
	}
	
	if(isExplosive)
	{
		var prey = FindObject(Find_Distance(GetCon()/15, 0, 0), Find_OCF(OCF_Alive));
		if(prey != nil)
			Explode(10);
	}
	
	// Jittery movement
	//SetYDir(GetYDir() - 3 + Random(7));
	var speedUp = -3;
	if(GetEffect("Fade", this))	
		{
		speedUp = 0;
		if (Inside(GetXDir(), -6, 6))
			SetXDir(GetXDir() + RandomX(-6,6) );
		}
	
	
	SetYDir(GetYDir() - RandomX(3,4) + speedUp - (GetCon()/50));
		
	return 1;
}

public func FxFadeStart(object target, effect, int temporary)
{
	// Store alpha here
	if (temporary == 0)
		effect.alpha = 255;
	if (!GBackLiquid(0, -3) && !isExplosive)
		SetGraphics("3");
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
