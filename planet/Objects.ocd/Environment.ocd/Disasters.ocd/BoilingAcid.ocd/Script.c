/**
	BoilingMagma
	Causes Acid on the map to bubble

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local intensity;
//global maxAcidSpawnerCount;
//global acidSpawnerCount;

protected func Initialize()
{
	SetPosition(0,0);
	intensity = (LandscapeWidth() * LandscapeHeight())/50000;
	//BoilingAcid.maxAcidSpawnerCount = (LandscapeWidth() * LandscapeHeight())/50000;
	AddEffect("SearchAcid",this,1,1,this);
}

func FxSearchAcidTimer(object target, effect, int time)
{
	for(var i = 0; i < intensity; i++)
	//if(acidSpawnerCount < maxAcidSpawnerCount)
	{
	var xRand = Random(LandscapeWidth());	
	var yRand = Random(LandscapeHeight());
	var mat = MaterialName(GetMaterial(xRand, yRand));
	var randDepth = RandomX(30, 100);
	var depthCheckMat = MaterialName(GetMaterial(xRand, yRand+randDepth));

	if(mat == "Acid")
		if(depthCheckMat == "Acid")
			if(PathFree(xRand, yRand, xRand, yRand+randDepth))
			{	
				var nearbySpawner = FindObject(Find_Distance(RandomX(80,100), xRand, yRand), Find_ID(AcidSpawner));
				
				if(nearbySpawner == nil)
				{
					var spawner = CreateObject(AcidSpawner, xRand, yRand+randDepth);
					spawner->Init(mat);
					//acidSpawnerCount++;
				}
			}
	}
}
