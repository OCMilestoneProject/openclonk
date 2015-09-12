/* Sky race */

func Initialize()
{
	CreateObject(RainCloud, 100, 100);

	SetSkyAdjust(RGB(50,50,50));
}

func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
	JoinPlayer(iPlr);
	return;
}

 func RelaunchPlayer(int iPlr)
{
	var clonk = CreateObjectAbove(Clonk, 0, 0, iPlr);
	clonk->MakeCrewMember(iPlr);
  	SetCursor(iPlr,clonk);
	JoinPlayer(iPlr);
	return;
}

 func JoinPlayer(int iPlr)
{
	var clonk = GetCrew(iPlr);
	clonk->SetPosition(425, 498);
	clonk->DoEnergy(100000);
	clonk->CreateContents(Musket);
	clonk->CreateContents(LeadShot);
	clonk->CreateContents(GrappleBow);
	clonk->CreateContents(Bow);
	clonk->Collect(CreateObjectAbove(Arrow));
	
	return;
}
