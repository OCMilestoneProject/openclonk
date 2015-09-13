/**
	Rain
	
	@authors Randrian
*/

local width;
local material_name;
local strength;

func GetMaterialColor(szName)
{
	var color = GetAverageTextureColor(szName);
	return [GetRGBaValue(color, 1), GetRGBaValue(color, 2),GetRGBaValue(color, 3)];
}

protected func Initialize()
{
	SetPosition(LandscapeWidth()/2,0);
	width = 500;
	strength = 100;
	material_name = "Water";
	AddEffect("IntRain", this, 1, 1, this);
	Sound("StereoRain.ogg", true, strength, 0, 1);
	return 1;
}

public func SetStrength(value)
{
	strength = value;
	Sound("StereoRain.ogg", true, strength);
}

global func Particles_Rain(iSize, color)
{
	return
	{
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		ForceY = GetGravity()/10,//PV_Gravity(100),
		Size = iSize,
		R = color[0], G = color[1], B = color[2],
		Rotation = PV_Direction(),
		CollisionDensity = 25,
		Stretch = 3000,
	};
}
global func Particles_Snow(iSize, color)
{
	return
	{
		Phase = PV_Random(0, 16),
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		DampingY = 1000,//PV_Cos(PV_Linear(0,1800),5,990),
		ForceY = 0,//GetGravity()/100,//PV_Gravity(100),
		ForceX = PV_Sin(PV_Step(RandomX(5,10), Random(180)),RandomX(5,8),0),
		Size = iSize,
		R = color[0], G = color[1], B = color[2],
		Rotation = PV_Random(360),
		CollisionDensity = 25,
		Stretch = 1000,
	};
}
global func Particles_Rain2(iSize, color)
{
	return
	{
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		ForceY = PV_Gravity(60),
		Size = iSize,
		R = color[0], G = color[1], B = color[2],
		Rotation = PV_Direction(),
		CollisionDensity = 25,		
		Stretch = PV_KeyFrames(0, 0, 1000, 500, 1000, 1000, 0),
	};
}
global func Particles_Splash(iSize, color)
{
	return
	{
		Phase = PV_Linear(0, 4),
		Alpha = PV_KeyFrames(0, 0, 255, 500, 255, 1000, 0),
		Size = iSize,
		R = color[0], G = color[1], B = color[2],
		Rotation = PV_Random(-5,5),
		Strech = 500+Random(500),
	};
}
global func Particles_SplashWater(iSize, color)
{
	return
	{
		Phase = PV_Linear(0, 13),
		Alpha = PV_KeyFrames(0, 0, 255, 500, 255, 1000, 0),
		Size = iSize,
		R = color[0], G = color[1], B = color[2],
		Rotation = PV_Random(-5,5),
		Stretch = PV_Linear(3000, 3000),
		Attach = ATTACH_Back,
	};
}

public func DropHit(x, y)
{
	var color = GetMaterialColor(material_name);
	// Some material combinations cast smoke
	if(GBackLiquid(x,y) && (material_name == "Acid" || material_name == "Lava" || material_name == "DuroLava") && GetMaterial(x,y) == Material("Water"))
	{
		Smoke(x, y, 3, RGB(150,160,150));
	}
	// Liquid? liquid splash!
	else if(GBackLiquid(x,y))
	{
		if(!GBackLiquid(x-1,y) || !GBackLiquid(x+1,y)) y += 1;
		CreateParticle("RaindropSplashLiquid", x, y, 0, 0, 50, Particles_SplashWater(RandomX(2,5), color), 0);
	}
	// Solid? normal splash!
	else
	{
		if( (material_name == "Acid" && GetMaterial(x,y) == Material("Earth")) || material_name == "Lava" || material_name == "DuroLava")
			Smoke(x, y, 3, RGB(150,160,150));
		CreateParticle("RaindropSplash", x, y-1, 0, 0, 5, Particles_Splash(RandomX(5,10), color), 0);
		CreateParticle("RaindropSmall", x, y, RandomX(-4, 4), -Random(10), PV_Random(300, 300), Particles_Rain2(20, color), 0);
	}
}

private func FxIntRainTimer()
{
	var borderleft = -width/2;
	var borderright = +width/2;
	var color = GetMaterialColor(material_name);
	
	// Calculate count according to strength and area
	var count = width*strength/10000;
	if(count < 1 && Random(10000) < width*strength)
		count = 1;
	for(var cnt = 0; cnt < count; cnt++)
	{
		// Cast Rain
		var name = "Raindrop";
		if(material_name == "Lava" || material_name == "DuroLava")
			name = "RaindropLava";
		var x = RandomX(borderleft, borderright);
		var y = -50;
		var xdir = RandomX(GetWind(0,0,1)-5, GetWind(0,0,1)+5)/5;
		var ydir = 30;
		if(material_name == "Snow")
		{
			CreateParticle("RaindropSnow", x, y, xdir, 10, PV_Random(2000, 3000), Particles_Snow(RandomX(0,3), color), 0);
			if(!Random(10))
				CastPXS(material_name, 1, 0, x, 0, 0, 0);
			continue;
		}		
		CreateParticle(name, x, y, xdir, ydir, PV_Random(200, 300), Particles_Rain(RandomX(10,30), color), 0);
		// Sometimes "real" rain falls
		if(!Random(10))
			InsertMaterial(Material(material_name), x, 0, xdir, ydir);
		//	CastPXS(material_name, 1, 0, x, 100, 0, 0);
		// Simulate particle path
		x *= 100;
		y *= 100;
		xdir *= 10;
		ydir *= 10;
		var ticks = 0;
		while(!GBackSemiSolid(x/100,y/100)) { ticks += 1; y += ydir; ydir += GetGravity()/10; x += xdir; }// CreateParticle("Smoke", x/100, y/100, 0, 0, 100, Particles_Magic(), 5);}
		x /= 100;
		y /= 100;
		while(GBackSemiSolid(x,y)) { y -= 1; }
		y += 1;
		ScheduleCall(this, "DropHit", ticks, 0, x, y);
	}
	
	return 1;
}
