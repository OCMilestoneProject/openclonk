/**
	Stalactite1
	Hangs from the ceiling

	@author Armin
*/

private func Initialize()
{
	SetGraphics(Format("%d",Random(5)));
	
	SetProperty("MeshTransformation", Trans_Mul(Trans_Scale(600+Random(900)), Trans_Rotate(-25+Random(50),0,1,0)));
	
	/*var tinys = CreateObject(TinyStalactite, 0, 0);
	tinys->SetProperty("MeshTransformation", Trans_Mul(Trans_Scale(1000), Trans_Rotate(180,0,0,1)));
	tinys->SetParent(this);*/
}

private func Hit()
{
	//Sound();
	RemoveObject();
	return true;
}

private func SetStalagmite()
{
	SetProperty("MeshTransformation", Trans_Mul(Trans_Scale(1000), Trans_Rotate(180,0,0,1)));
}

public func Place(int amount, proplist rectangle, proplist settings)
{
	// Only allow definition call.
	if (this != Stalactite1) 
		return;
	// Default parameters.
	if (!settings) 
		settings = { size = [100, 100] };
	if (!settings.size) 
		settings.size = [100, 100];
	var loc_area = nil;
	if (rectangle) 
		loc_area = Loc_InRect(rectangle);
	var loc_background = Loc_Tunnel();
	if (settings.underground)
		loc_background = Loc_Tunnel();
		
	var stalactites = [];	
	for (var i = 0; i < amount; i++)
	{
		var size = RandomX(settings.size[0], settings.size[1]);
		var loc = FindLocation(loc_background, Loc_Not(Loc_Liquid()), Loc_Wall(CNAT_Right), Loc_Wall(CNAT_Top), loc_area);
		if (!loc)
			continue;

		var stalactite = CreateObject(Stalactite1);
		stalactite->SetPosition(loc.x, loc.y + 25);
		var tinys = CreateObject(TinyStalactite, 0, 0);
		tinys->SetPosition(loc.x, loc.y+4);
		tinys->SetParent(stalactite);
		stalactite->SetCon(size);
		// todo stalactite->AdjustPosition();
		var mat = MaterialName(stalactite->GetMaterial(0, -stalactite->GetObjHeight()/2));
		//stalactite->Message("Material: %s|Minus: %d", mat, stalactite->GetObjHeight()/2);
		if (mat == "Ice")
		{
			stalactite->SetClrModulation(RGBa(157,202,243,160));
			tinys->SetClrModulation(RGBa(157,202,243,160));
		}
		else
		{
			stalactite->SetClrModulation(GetAverageTextureColor(mat));
			tinys->SetClrModulation(GetAverageTextureColor(mat));
			tinys->DrawWaterSource();
		}
		if (!Random(2))
		{
			var xy;
			if (xy = FindConstructionSite(Stalactite1, loc.x, loc.y+60));
			{
				var stalactite2 = CreateObject(Stalactite1, loc.x, xy[1]-28);
				stalactite2->SetR(180);
				if (mat == "Ice")
				{
					stalactite2->SetClrModulation(RGBa(157,202,243,160));
				}
				else
				{
					stalactite2->SetClrModulation(GetAverageTextureColor(mat));
				}
			}
		}
		PushBack(stalactites, stalactite);	
	}
	return stalactites;
}

local Name = "$Name$";
local Description = "$Description$";