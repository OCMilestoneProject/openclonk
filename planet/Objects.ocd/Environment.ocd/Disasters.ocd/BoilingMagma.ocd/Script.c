/**
	BoilingMagma
	Causes Lava on the map to boil

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local intensity;

protected func Initialize()
{
	SetPosition(0,0);
	intensity = (LandscapeWidth() * LandscapeHeight())/10000;
	AddEffect("Boiling",this,1,1,this);
}

func FxBoilingTimer(object target, effect, int time)
{
	for(var i = 0; i < intensity; i++)
	{
	var xRand = Random(LandscapeWidth());	
	var yRand = Random(LandscapeHeight());
	var mat = MaterialName(GetMaterial(xRand, yRand));
	var aboveMat = MaterialName(GetMaterial(xRand, yRand-1));
	var depthCheckMat = MaterialName(GetMaterial(xRand, yRand+30));

	if(mat == "DuroLava" || mat == "Lava")
		if(aboveMat == nil || aboveMat == "Tunnel")
			if(depthCheckMat == "DuroLava" || depthCheckMat == "Lava")
				if(PathFree(xRand, yRand, xRand, yRand+30))
			{	
				var spawner = CreateObject(MagmaSpawner, xRand, yRand);
				spawner->Init(mat);
			}
	}
}
