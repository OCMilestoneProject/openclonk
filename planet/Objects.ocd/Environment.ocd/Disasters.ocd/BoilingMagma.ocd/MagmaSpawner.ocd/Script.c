/**
	BoilingMagma
	Causes Lava on the map to boil

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local maxTime = 2;
local timer = 0;
local count = 10;

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
		
		Fx_MagmaBubble->CastMagmaBubbles(randAmount, RandomX(-10,-30), GetX() + RandomX(-30, 30), GetY());
		
		if(count <= 0)
			RemoveObject();
	}
}
