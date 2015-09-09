/*
	Firefly
	Author: Randrian, Nachtschatten (Clinfinity)

	A small glowing being, often encountered in groups.
*/
static const Firefly_MaxSpawnDistance = 15;
static const Firefly_MaxDistance = 40;
static const Firefly_ShynessDistance = 40;

local attractedTo;
local timer;

public func SpawnSwarm(object attractedTo, int size)
{
	if(!size) size = 10;
	var x = attractedTo->GetX();
	var y = attractedTo->GetY();
	for(var i = 0; i < size; i++)
	{
		var firefly = CreateObject(Firefly, RandomX(x - Firefly_MaxSpawnDistance, x + Firefly_MaxSpawnDistance), RandomX(y - Firefly_MaxSpawnDistance, y + Firefly_MaxSpawnDistance), NO_OWNER);
		firefly.attractedTo = attractedTo;
	}
}


private func Flying() {
	var xdir, ydir;

	timer += Random(2);
	var angle = timer*180/10;
	SetObjDrawTransform(900+Sin(angle,100), 0, 0, 0, 900+Sin(angle, 100), 0, 1);

	var awayFrom = FindObject(Find_Distance(Firefly_ShynessDistance), Find_Category(C4D_Object), Find_OCF(OCF_HitSpeed1), Find_NoContainer());
	if(awayFrom)
	{
		xdir = BoundBy(GetX() - awayFrom->GetX(), -1, 1);
		ydir = BoundBy(GetY() - awayFrom->GetY(), -1, 1);
		if(xdir == 0) xdir = Random(2) * 2 - 1;
		if(ydir == 0) ydir = Random(2) * 2 - 1;
		xdir = RandomX(5 * xdir, 10 * xdir);
		ydir = RandomX(5 * ydir, 10 * ydir);
		// No check for liquids here, you can scare fireflies into those ;)
		SetSpeed(xdir, ydir);
	}
	else
	{
		if(Random(4)) return;
		
		if(attractedTo != 0 && ObjectDistance(attractedTo) > Firefly_MaxDistance)
		{
			xdir = BoundBy(attractedTo->GetX() - GetX(), -1, 1);
			ydir = BoundBy(attractedTo->GetY() - GetY(), -1, 1);
			xdir = RandomX(xdir, 6 * xdir);
			ydir = RandomX(ydir, 6 * ydir);
		}
		else
		{
			xdir = Random(120) - 60;
			ydir = Random(80) - 40;
		}

		if(GBackLiquid(xdir, ydir))
		{
			SetSpeed(0, 0);
		}
		else
		{
			xdir *= 10;
			ydir *= 10;
			while(GBackSemiSolid(0, ydir)) ydir-=1;
			SetCommand("MoveTo", 0, GetX()+xdir, GetY()+ydir);
			if(Random(10)==0 && attractedTo)
				SetCommand("MoveTo", attractedTo);
		}
	}
}

protected func Check()
{
	// Buried or in water: Instant death
	if(GBackSemiSolid())
	{
		Death();
	}
}

protected func Initialize()
{
	SetAction("Fly");
	
	SetGraphics("", GetID(), 1, 1, 0, GFX_BLIT_Additive);
	SetClrModulation(RGBa(200,255,100,50),1);
	SetGraphics("", GetID(), 2, 1, 0, GFX_BLIT_Additive);
	SetClrModulation(RGBa(200,255,0,255),2);

	SetObjDrawTransform(300, 0, 0, 0, 300, 0, 2);
}

public func CatchBlow()	{ RemoveObject(); }
public func Damage()	{ RemoveObject(); }
protected func Death()	{ RemoveObject(); }

local ActMap = {
	Fly = {
		Prototype = Action,
		Name = "Fly",
		Procedure = DFA_FLOAT,
		Speed = 30,
		Accel = 5,
		Decel = 5,
		Directions = 2,
		Length = 1,
		Delay = 1,
		NextAction = "Fly",
		PhaseCall = "Flying",
		EndCall = "Check",
	},
};

local Name = "$Name$";
local MaxEnergy = 40000;
local MaxBreath = 125;
local Placement = 2;
local NoBurnDecay = 1;
