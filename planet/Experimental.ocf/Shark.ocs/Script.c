func Initialize()
{
	Shark->Place(2);
	Squid->Place(1);
	Seaweed->Place(71);
	Coral->Place(4);
}

private func InitializePlayer(iPlr)
{
	SetFoW(0, iPlr);
	var crew = GetCrew(iPlr);
	crew->SetPosition(10,300);
	crew->CreateContents(Shovel);
}

func RelaunchPlayer(int iPlr)
{
	var pclonk = CreateObject(Clonk, 10,300, iPlr);
	pclonk->MakeCrewMember(iPlr);
	SetCursor(iPlr, pclonk);
	pclonk->CreateContents(Shovel);
}
