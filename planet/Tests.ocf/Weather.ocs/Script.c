func Initialize()
{
	RemoveAll(Find_ID(WindBag));
	RemoveAll(Find_ID(Shovel));
	RemoveAll(Find_ID(GrappleBow));
	RemoveAll(Find_ID(Dynamite));
	CreateRain(LandscapeWidth()/4, 500, "Acid");

	SetSkyAdjust(RGB(50,50,50));	
}

func InitializePlayer(int plr)
{
	Log("init plr");
	var clonk = GetCrew(plr);
	if (clonk)
	{
		clonk->CreateContents(Shovel);
		clonk->CreateContents(Dynamite);
		clonk->CreateContents(Dynamite);
		clonk->CreateContents(Dynamite);
		clonk->CreateContents(Torch);
		clonk->SetPosition(150,100);
		//SetFoW(false, 0);
		//SetPlayerZoom(NO_OWNER, 1);
	}
}
