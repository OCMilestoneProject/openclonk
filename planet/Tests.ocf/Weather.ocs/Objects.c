/* Automatically created objects file */

func InitializeObjects()
{
	CreateObjectAbove(Grass, 313, 343);
	CreateObjectAbove(Grass, 322, 349);
	CreateObjectAbove(Grass, 335, 351);
	CreateObjectAbove(Grass, 327, 349);
	CreateObjectAbove(Grass, 341, 350);
	CreateObjectAbove(Grass, 147, 333);
	CreateObjectAbove(Grass, 191, 295);
	CreateObjectAbove(Grass, 134, 341);
	CreateObjectAbove(Grass, 141, 333);
	CreateObjectAbove(Grass, 164, 320);
	CreateObjectAbove(Grass, 116, 341);
	CreateObjectAbove(Grass, 184, 302);
	CreateObjectAbove(Grass, 419, 355);
	CreateObject(Grass, 461, 351);
	CreateObjectAbove(Tree_Coniferous2, 149, 341);
	CreateObjectAbove(Tree_Coniferous2, 135, 344);
	CreateObjectAbove(Tree_Coniferous2, 978, 361);
	CreateObjectAbove(Tree_Coniferous2, 168, 328);
	CreateObjectAbove(Tree_Coniferous2, 113, 350);
	CreateObjectAbove(Tree_Coniferous2, 0, 400);
	CreateObjectAbove(Tree_Coniferous2, 997, 358);

	var Branch001 = CreateObject(Branch, 906, 346);
	Branch001->SetR(2);

	CreateObjectAbove(Fern, 28, 364);
	CreateObjectAbove(Fern, 107, 344);
	CreateObjectAbove(Fern, 185, 304);
	CreateObjectAbove(Fern, 160, 329);
	CreateObjectAbove(Fern, 126, 344);
	CreateObjectAbove(Fern, 90, 344);
	CreateObjectAbove(Fern, 49, 358);

	CreateObjectAbove(Tree_Coniferous2, 113, 350);
	CreateObjectAbove(Tree_Coniferous2, 43, 365);
	CreateObjectAbove(Tree_Coniferous2, 114, 344);
	CreateObjectAbove(Tree_Coniferous2, 101, 352);

	CreateWaterfall(207,302,2,"Water");


	CreateLiquidDrain(226,406,6);;

	CreateObjectAbove(Tree_Coniferous3, 834, 364);


	CreateObjectAbove(Fern, 70, 346);

	CreateObjectAbove(Tree_Coniferous2, 72, 353);
	CreateObjectAbove(Tree_Coniferous2, 97, 348);
	
	CreateObjectAbove(Tree_Coniferous3, 201, 313);

	CreateObjectAbove(Tree_Coniferous3, 819, 371);
	CreateObjectAbove(Tree_Coniferous3, 887, 350);

	var Basement001 = CreateObject(Basement, 121, 346);
	Basement001->SetCategory(C4D_StaticBack);
	var Basement002 = CreateObject(Basement, 74, 351);
	Basement002->SetCategory(C4D_StaticBack);

	CreateObjectAbove(Windmill, 834, 367);

	CreateObjectAbove(ChemicalLab, 122, 342);

	CreateObjectAbove(Chest, 600, 353);

	CreateObjectAbove(Sawmill, 75, 345);

	var Clonk001;
	CreateObject(Torch, 241, 317);
	CreateObjectAbove(Torch, 271, 342);
	CreateObject(Torch, 241, 317);
	CreateObject(Torch, 241, 317, 0);

	CreateObject(Shovel, 241, 317, 0);

	CreateObject(Dynamite, 241, 317, 0);
	CreateObject(Dynamite, 241, 317, 0);
	CreateObject(Dynamite, 241, 317, 0);
	return true;
}
