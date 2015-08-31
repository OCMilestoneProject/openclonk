/**
	Meteor
	A burning rock falling from the sky, explodes on impact.
	
	@author Maikel	
*/

local spawnID; // Object that is spawned upon impact

/*-- Disaster Control --*/

public func SetChance(int chance, id spawnobject)
{
	if (this != AlienMeteor)
		return;
	var effect = GetEffect("IntAlienMeteorControl");
	if (!effect)
	 	effect = AddEffect("IntAlienMeteorControl", nil, 100, 20, nil, AlienMeteor);
	effect.chance = chance;
	effect.spawn = spawnobject;
	return;
}

public func GetChance()
{
	if (this != AlienMeteor)
		return;
	var effect = GetEffect("IntAlienMeteorControl");
	if (effect)
		return effect.chance;
	return;
}

protected func FxIntAlienMeteorControlTimer(object target, proplist effect, int time)
{
	if (Random(100) < effect.chance && !Random(10))
	{
		// Launch a meteor.
		var meteor = CreateObjectAbove(AlienMeteor);
		var x = Random(LandscapeWidth());
		var y = 0;
		var size = RandomX(90, 100);
		var xdir = RandomX(-22, 22);
		var ydir = RandomX(28, 36);
		meteor->Launch(x, y, size, xdir, ydir, effect.spawn);		
	}
	return FX_OK;
}

// Scenario saving
func FxIntAlienMeteorControlSaveScen(obj, fx, props)
{
	props->Add("AlienMeteor", "AlienMeteor->SetChance(%d)", fx.chance);
	return true;
}

global func LaunchAlienMeteor(int x, int y, int size, int xdir, int ydir, id spawn)
{
	var meteor = CreateObjectAbove(AlienMeteor);
	return meteor->Launch(x, y, size, xdir, ydir, spawn);
}

/*-- Meteor --*/

public func Launch(int x, int y, int size, int xdir, int ydir, id spawn)
{
	// Launch from indicated position.
	SetPosition(x, y);
	// Set the meteor's size.
	SetCon(BoundBy(size, 20, 100));
	// Set the initial velocity.
	SetXDir(xdir);
	SetYDir(ydir);
	// Safety check.
	if (!IsLaunchable())
		return false;
	// Set the object that should be created on impact
	spawnID = spawn;
	// Set right action.
	AddEffect("IntMeteor", this, 100, 1, this);
	// Emits light
	SetLightRange(400, 100);
	SetLightColor(RGB(100, 254, 255));
	
	return true;
}

private func IsLaunchable()
{
	if (GBackSemiSolid() || Stuck())
	{
		RemoveObject();
		return false;
	}
	return true;
}

protected func FxIntMeteorTimer()
{
	var size = GetCon();
	// Air drag.
	var ydir = GetYDir(100);
	ydir -= size * ydir ** 2 / 11552000; // Magic number.
	SetYDir(ydir, 100);
	// Fire trail.
	var fire = 
	{
		R = 255,
		B = 255,
		G = 255,
		Alpha = PV_Linear(255,0),
		Size = PV_Linear(30,150),
		Stretch = 1000,
		Phase = 0,
		Rotation = -GetR() + RandomX(-15,15),
		ForceX = -GetXDir()/2 + RandomX(-5,5),
		ForceY = -GetYDir()/2,
		DampingX = 1000,
		DampingY = 1000,
		BlitMode = GFX_BLIT_Additive,
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		Attach = nil
	};
	var sparkright = 
	{
		R = 255,
		B = 255,
		G = 255,
		Alpha = PV_KeyFrames(0, 0, 0, 500, 255, 1000, 0),
		Size = PV_Linear(20,100),
		Stretch = 1000,
		Phase = 0,
		Rotation = 60 + RandomX(-10,10),
		ForceX = 0,
		ForceY = 0,
		DampingX = 1000,
		DampingY = 1000,
		BlitMode = GFX_BLIT_Additive,
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		Attach = ATTACH_Back | ATTACH_MoveRelative
	};
	var sparkleft = 
	{
		R = 255,
		B = 255,
		G = 255,
		Alpha = PV_KeyFrames(0, 0, 0, 500, 255, 1000, 0),
		Size = PV_Linear(30,100),
		Stretch = 1000,
		Phase = 0,
		Rotation = -60 + RandomX(-10,10),
		ForceX = 0,
		ForceY = 0,
		DampingX = 1000,
		DampingY = 1000,
		BlitMode = GFX_BLIT_Additive,
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		Attach = ATTACH_Back | ATTACH_MoveRelative
	};
	var trail = 
	{
		R = 255,
		B = 255,
		G = 255,
		Alpha = PV_Linear(255,0),
		Size = GetCon()/3,
		Stretch = 1000,
		Phase = 0,
		Rotation = 0,
		ForceX = 0,
		ForceY = 0,
		DampingX = 1000,
		DampingY = 1000,
		BlitMode = GFX_BLIT_Additive,
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		Attach = ATTACH_Front | ATTACH_MoveRelative
	};
	
	CreateParticle("BlueFireTrail", RandomX(-1,1), -15, 0, -GetYDir(), 7, trail, 1);
	
	CreateParticle("BlueSpark", 10, 15, 100 + GetXDir(), -GetYDir(), 20, sparkright, 1);
	CreateParticle("BlueSpark", -10, 15, -100 + GetXDir(), -GetYDir(), 20, sparkleft, 1);
		
	for(var i = 0; i < 4; i++)
		{
		CreateParticle("BlueFire", RandomX(-8,8), RandomX(-15,-35), 0, 0, 30, fire, 1);
		}
	
	// Sound.

	return 1;
}

protected func Hit(int xdir, int ydir)
{
	CreateObjectAbove(spawnID, 0, 0, GetOwner());
	Explode(45);
	return;
}

// Scenario saving
func FxIntAlienMeteorSaveScen(obj, fx, props)
{
	props->AddCall("AlienMeteor", obj, "AddEffect", "\"IntMeteor\"", obj, 100, 1, obj);
	return true;
	}

/*-- Proplist --*/

local Name = "$Name$";
