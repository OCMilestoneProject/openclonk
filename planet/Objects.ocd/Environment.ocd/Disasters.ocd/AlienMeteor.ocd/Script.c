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
	var effect = GetEffect("IntMeteorControl");
	if (!effect)
	 	effect = AddEffect("IntMeteorControl", nil, 100, 20, nil, AlienMeteor);
	effect.chance = chance;
	effect.spawn = spawnobject;
	return;
}

public func GetChance()
{
	if (this != AlienMeteor)
		return;
	var effect = GetEffect("IntMeteorControl");
	if (effect)
		return effect.chance;
	return;
}

protected func FxIntMeteorControlTimer(object target, proplist effect, int time)
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
func FxIntMeteorControlSaveScen(obj, fx, props)
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
	// Set random rotation.
	//SetR(Random(360));
	SetRDir(RandomX(-1, 1));
	// Safety check.
	if (!IsLaunchable())
		return false;
	// Set the object that should be created on impact
	spawnID = spawn;
	// Set right action.
	AddEffect("IntMeteor", this, 100, 1, this);
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
	// Smoke trail.
	CreateParticle("Smoke", PV_Random(-2, 2), PV_Random(-2, 2), PV_Random(-3, 3), PV_Random(-3, 3), 30 + Random(60), Particles_SmokeTrail(), 5);
	// Fire trail.
	var trail = 
	{
		R = 255,
		B = 255,
		G = 255,
		Alpha = 230,
		Size = size/3,
		Stretch = 1000,
		Phase = 0,
		Rotation = 0,
		ForceX = -GetXDir(),
		ForceY = -GetYDir(),
		DampingX = 1000,
		DampingY = 1000,
		BlitMode = GFX_BLIT_Additive,
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		Attach = ATTACH_Front | ATTACH_MoveRelative
	};
	var fire = 
	{
		R = 255,
		B = 255,
		G = 255,
		Alpha = 100,
		Size = 15,
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

	CreateParticle("BlueFireTrail", RandomX(-1,1), -15, 0, 0, 1000, trail, 1);
		
	for(var i = 0; i < 40; i++)
		{
		CreateParticle("BlueFire", RandomX(-10,10), RandomX(-20,-35), 0, 0, 70, fire, 1);
		}
	//CreateParticle("MagicSpark", 0, 0, PV_Random(-20, 20), PV_Random(-20, 20), 16, Particles_SparkFire(), 4);
	
	// Sound.

	// Burning and friction decrease size.
	if (size > 10 && !Random(5))
		DoCon(-1);

	return 1;
}

protected func Hit(int xdir, int ydir)
{
	var size = 10 + GetCon();
	var speed2 = 20 + (xdir ** 2 + ydir ** 2) / 10000;
	// Some fire sparks.
	var particles =
	{
		Prototype = Particles_Fire(),
		Attach = nil
	};
	//CreateParticle("Fire", PV_Random(-size / 4, size / 4), PV_Random(-size / 4, size / 4), PV_Random(-size/4, size/4), PV_Random(-size/4, size/4), 30, particles, 20 + size);
	// Explode meteor, explode size scales with the energy of the meteor.	
	var dam = size * speed2 / 500;
	dam = BoundBy(size/2, 5, 30);
	CreateObjectAbove(spawnID, 0, 0, GetOwner());
	Explode(dam);
	return;
}

// Scenario saving
func FxIntMeteorSaveScen(obj, fx, props)
{
	props->AddCall("AlienMeteor", obj, "AddEffect", "\"IntMeteor\"", obj, 100, 1, obj);
	return true;
	}

/*-- Proplist --*/

local Name = "$Name$";
