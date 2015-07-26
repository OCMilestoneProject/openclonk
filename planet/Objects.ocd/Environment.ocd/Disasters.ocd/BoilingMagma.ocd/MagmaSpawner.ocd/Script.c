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
local mat;

public func Init(string material)
{
	mat = material;
	AddEffect("Boil",this,1,1,this);
	Sound("BoilingMagma");
}

func FxBoilTimer(object target, effect, int time)
{
	timer++;
	if(timer > maxTime)
	{
		timer = 0;
		
		var randAmount = RandomX(1, 2);
		
		count -= randAmount;
		
		Fx_MagmaBubble->CastMagmaBubbles(randAmount, RandomX(-10,-30), GetX(), GetY());
		//Fx_MagmaBubble->CastMagmaBubbles(randAmount, RandomX(-10,-30), RandomX(-3,3), RandomX(-10,-30));
		//CastPXS(mat, randAmount, RandomX(-10,-30), 0, -5, -180, 50);
		
		if(count <= 0)
			RemoveObject();
	}
}
