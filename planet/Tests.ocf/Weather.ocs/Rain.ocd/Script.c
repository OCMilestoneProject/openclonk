/**
	Rain
	
	@authors Randrian
*/

local width;

protected func Initialize()
{
	SetPosition(LandscapeWidth()/2,0);
	width = 500;
	AddEffect("IntRain", this, 1, 1, this);
	return 1;
}

global func Particles_Rain(iSize)
{
	return
	{
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		ForceY = PV_Gravity(60),
		Size = iSize,
		R = 51, G = 66, B = 97,
		Rotation = PV_Direction(),
		CollisionDensity = 25,
		Stretch = 3000,
	};
}
global func Particles_Rain2(iSize)
{
	return
	{
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		ForceY = PV_Gravity(60),
		Size = iSize,
		R = 51, G = 66, B = 97,
		Rotation = PV_Direction(),
		Stretch = PV_KeyFrames(0, 0, 1000, 500, 1000, 1000, 0),
	};
}
global func Particles_Splash(iSize)
{
	return
	{
		Phase = PV_Linear(0, 4),
		Alpha = PV_KeyFrames(0, 0, 255, 500, 255, 1000, 0),
		Size = iSize,
		R = 51, G = 66, B = 97,
		Rotation = PV_Random(-5,5),
		Strech = 500+Random(500),
	};
}
global func Particles_SplashWater(iSize)
{
	return
	{
			Phase = PV_Linear(0, 13),
			Alpha = PV_KeyFrames(0, 0, 255, 500, 255, 1000, 0),
			Size = iSize,
			R = 51, G = 66, B = 97,
			Rotation = PV_Random(-5,5),
			Stretch = PV_Linear(3000, 3000),
			Attach = ATTACH_Back,
	};
}

private func FxIntRainTimer()
{
	var borderleft = -width/2;
	var borderright = +width/2;
	
	for(var cnt = 0; cnt < width/25; cnt++)
		CreateParticle("Raindrop", RandomX(borderleft, borderright), -50, RandomX(GetWind(0,0,1)-50, GetWind(0,0,1)+50)/8, 0, PV_Random(200, 300), Particles_Rain(RandomX(10,30)), 0);
	for(var cnt = 0; cnt < width/50; cnt++)
	{
		var x = RandomX(borderleft, borderright);
		var y = 0;
		while(!GBackSemiSolid(x,y)) y+=1;
		if(GBackLiquid(x,y))
		{
			if(!GBackLiquid(x-1,y) || !GBackLiquid(x+1,y)) y += 1;
			CreateParticle("RaindropSplashLiquid", x, y, 0, 0, 50, Particles_SplashWater(RandomX(2,5)), 0);
			continue;
		}
		CreateParticle("RaindropSplash", x, y-1, 0, 0, 5, Particles_Splash(RandomX(5,10)), 0);
		CreateParticle("RaindropSmall", x, y, RandomX(-4, 4), -Random(10), PV_Random(300, 300), Particles_Rain2(20), 0);
	}
	CastPXS("Water", 1, 0, RandomX(borderleft, borderright), 0, 0, 0);
	return 1;
}
