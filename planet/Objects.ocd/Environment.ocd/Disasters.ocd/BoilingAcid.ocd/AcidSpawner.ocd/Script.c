/**
	AcidSpawner
	Spawns Fx_AcidBubble

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local maxTime = 12;
local timer = 0;
local count = 7;

public func Initialize()
{
	AddEffect("Boil",this,1,1,this);
}

/*

Periodically spawns bubbles until count runs out

*/

func FxBoilTimer(object target, effect, int time)
{
	timer++;
	if(timer > maxTime)
	{
		timer = 0;
		
		var randAmount = RandomX(1, 2);
		
		count -= randAmount;
		
		Fx_AcidBubble->CastAcidBubbles(randAmount, RandomX(-10,-30), GetX(), GetY());
	}
	
	if(!GBackLiquid(0,0) || count <= 0)
		{
		RemoveObject();
		}
}
