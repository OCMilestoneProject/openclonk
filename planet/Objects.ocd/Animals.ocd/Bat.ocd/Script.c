/**
	Bat
	Flutters around at night

	@Win
*/

local Name = "$Name$";
local Description = "$Description$";

// time is in frames

local move_vectorX = 0;
local move_vectorY = 0;
local move_speed = 12; // bear in  mind this is also multiplied by a random factor
local move_random_dir = 3; // max random multiplier

local target_food = 0; // Try to reach this object

local ultra_sound_particle;

local daytime;

local reproduction_timer = 0;
local reproduction_interval = 500;

local dir = 1; // 1 = right; -1 = left
local z_rot = 0; // rotation around Z axis

local flip_timer = 0;
local flip_interval = 0;

local is_expat = 0;

local startled_timer = 0; // Doesn't sit while startled

//local shell_rotation;

func Construction()
{
	daytime = FindObject(Find_ID(Environment_Time));

	AddEffect("CoreBehaviour",this,1,1,this);
	
	Fly();
	
	// Some bats don't stick to the swarm and start their own.
	if(!Random(100))
		is_expat = 1;
	
	ultra_sound_particle = {
	R = 255,
	B = 255,
	G = 255,
		
	Alpha = PV_Linear(255,0),
	Size = PV_Linear(5,60),
	Stretch = 1000,
	DampingX = 1000,
	DampingY = 1000,
	BlitMode = 0,
	CollisionVertex = 0,
	OnCollision = PC_Stop(),
	};
}

func Place(int amount, proplist rectangle, proplist settings)
{
	var max_tries = 2 * amount;
	var loc_area = nil;
	if (rectangle) loc_area = Loc_InArea(rectangle);
	var f;
	
	while ((amount > 0) && (--max_tries > 0))
	{
		var spot = FindLocation(Loc_Material("Tunnel"), Loc_Space(20), loc_area);
		if (!spot) continue;
		
		f = CreateObjectAbove(this, spot.x, spot.y, NO_OWNER);
		if (!f) continue;
		
		if (f->Stuck())
		{
			f->RemoveObject();
			continue;
		}
		--amount;
	}
	return f; // return last created bat
}

protected func FxCoreBehaviourTimer(object target, effect, int time)
{
	if(daytime == nil)
		FlightRoutine();
	else if(daytime->IsNight() || GetAction() != "Sit")
			FlightRoutine();

	if(GetCon() < 100)
		if(!Random(10))
			DoCon(1);
}

protected func FlightRoutine()
{
	if(GetAction() == "Sit")
	{
		if(daytime == nil)
			if(!Random(150))
				Fly();
		if(daytime != nil || !GBackSolid(0, -12))
			Fly();
	}
	else
	{
		// Move
		MoveImpulse();
		// Make sound
		if(!Random(300))
			Sound("BatFlutter*");
		if(startled_timer > 0)
			startled_timer--;
		//if(!Random(300))
			//Sound("Bat*");
		// Calculate rotation based on direction
		var rot_inc = 20;
		if(dir == 1)
			if(z_rot > 0)
			{
				z_rot -= rot_inc;
			}
		if(dir == -1)
			if(z_rot < 180)
			{
				z_rot += rot_inc;
			}
		// Rotate based on flight direction
		this.MeshTransformation = Trans_Mul(Trans_Rotate(GetYDir() * 2 * dir, 0, 0, 1), Trans_Rotate(z_rot, 0, 1, 0));
		// Grow and reproduce
		if(GetCon() >= 100)
		{
			reproduction_timer++;
			if(reproduction_timer >= reproduction_interval)
			{	
				if(!Random(150))
				{
					var population = FindObjects(Find_ID(Bat), Find_OCF(OCF_Alive), Find_Distance(500), Find_Exclude(this));
					if(GetLength(population) <= 10)
					{
						var new_bat = CreateObjectAbove(Bat);
						new_bat->SetCon(60);
					}
					
				}
				reproduction_timer = 0;
			}
		}
		var predator = FindObject(Find_ID(Clonk), Find_Distance(50));
		// First priority: stay outta water
		if(GBackLiquid(0, 4))
		{
			move_vectorX = RandomX(-1, 1);
			move_vectorY = -1;
		}
		// Second priority: Stay away from Clonks D:
		else if(predator != nil)
		{
			move_vectorX = - Normalize(predator->GetX() - GetX());
			move_vectorY = - Normalize(predator->GetY() - GetY());
		}
		// Third: Explore
		else if(!Random(80) && !IsTurning())
			ChangeDirection();
		// Fourth: Fly towards food
		else if(target_food)
		{
			move_vectorX = Normalize(target_food->GetX() - GetX());
			move_vectorY = Normalize(target_food->GetY() - GetY());

			if(Distance(GetX(), GetY(), target_food->GetX(), target_food->GetY()) < 10)
				{
				target_food->RemoveObject();
				ChangeDirection();
				Sound("Munch1");
				}
		}
		// Fly towards other bats to loosely make a swarm
		else if(!Random(100) && !is_expat)
		{
			var fellow = FindObject(Find_ID(Bat), Find_OCF(OCF_Alive), Find_Distance(200), Find_Exclude(this));
			if(fellow != nil)
			{
				move_vectorX = Normalize(fellow->GetX() - GetX());
				move_vectorY = Normalize(fellow->GetY() - GetY());
			}
		}
		// When above ground don't fly too far up into the sky
		else if(GBackSky(0,0))
			if(!GBackSolid(0, 100) && !GBackLiquid(0, 100))
				if(PathFree(GetX(), GetY(), GetX(), GetY() + 100))
				{
					move_vectorX = RandomX(-1, 1);
					move_vectorY = 1;
				}
		if(!Random(450))
		{
			// Search for food
			target_food = FindObject(Find_Func("IsAnimalFood"), Find_Distance(100), Find_NoContainer());
			if(target_food != nil && !PathFree(GetX(), GetY(), target_food->GetX(), target_food->GetY()))
				target_food = nil;
			else
			{
				CreateParticle("Shockwave", 0, 0, 0, 0 , 30, ultra_sound_particle, 1);
				Sound("BatChirp");
			}
		}	
		
	}
}

protected func CatchBlow()
{
	if (GetAction() == "Dead") return;
	Hurt();
}
	
protected func Hurt()
{
	if(GetAction() == "Sit")
		Fly();
	Sound("Bat*");
	// When hurt, alarm nearby bats
	var swarm = FindObjects(Find_ID(Bat), Find_OCF(OCF_Alive), Find_Distance(200), Find_Exclude(this));
	for(var i = 0; i < GetLength(swarm); i++)
	{
		swarm[i]->Startle();
	}
}

protected func Startle()
{
	if(GetAction() == "Sit")
		{
		Fly();
		Log("wut");
		Sound("BatNoise*");
		startled_timer = 150;
		}
}

/*
Returns true if the bat is still turning
*/
protected func IsTurning()
{
	if(dir == 1)
		if(z_rot > 0)
		{
			return true;
		}
	if(dir == -1)
		if(z_rot < 180)
		{
			return true;
		}
	return false;
}

/*
	Causes the object to move 
*/
protected func MoveImpulse()
{
	if(move_vectorX > 0)
		dir = 1;
	if(move_vectorX < 0)
		dir = -1;
	
	SetXDir(( GetXDir() + move_vectorX * move_speed ) / 2);
	SetYDir(( GetYDir() +  move_vectorY * move_speed ) / 2);
}

protected func ChangeDirection()
{
	move_vectorX = RandomX(-1,1);
	move_vectorY = RandomX(-1,1);
	
	MoveImpulse();
}

protected func Fly()
{
	StopAnimation(1);
	SetAction("Flight");
	SetComDir(COMD_None);
	
	PlayAnimation("Fly", 1, Anim_Linear(0,0,GetAnimationLength("Fly"),10,ANIM_Loop), Anim_Const(1000));
			
	target_food = nil;
			
	ChangeDirection();
}

protected func Sit()
{
	StopAnimation(1);
	SetAction("Sit");
	SetComDir(COMD_None);
	
	PlayAnimation("Hang", 1, Anim_Linear(0,0,GetAnimationLength("Hang"),25,ANIM_Loop), Anim_Const(1000));
	
	SetXDir(0);
	SetYDir(0);
}

public func Normalize(int value)
{
	var min = -1;
	var max = 1;
	
	if (value <= min) 
		return -1;
    if (value >= max) 
    	return 1;
}

protected func ContactBottom()
{
	move_vectorX = RandomX(-1, 1);
	move_vectorY = -1;
}

protected func ContactTop()
{
	if(daytime == nil && !startled_timer)
	{
		if(!Random(10))
			{
			Sit();
			return 1;
			}
	}
	if(daytime != nil && !startled_timer) 
		if(daytime->IsDay())
		{
			Sit();
			return 1;
		}
		
	move_vectorX = RandomX(-1, 1);
	move_vectorY = 1;
}

protected func ContactLeft()
{
	move_vectorX = RandomX(-1, 1);
	move_vectorY = 1;
}

protected func ContactRight()
{
	move_vectorX = -1;
	move_vectorY = RandomX(-1, 1);
}

protected func Death()
{
	StopAnimation(1);
	PlayAnimation("Dead", 1, Anim_Linear(0,0,GetAnimationLength("Dead"),1,ANIM_Hold), Anim_Const(1000));
	RemoveEffect("CoreBehaviour", this);
	SetAction("Dead");
}

local ActMap = {

Sit = {
	Prototype = Action,
	Name = "Sit",
	Procedure = DFA_FLOAT,
	Speed = 100,
	Accel = 16,
	Decel = 16,
	Length = 1,
	Delay = 0,
	FacetBase=1,
	NextAction = "Sit",
},
Flight = {
	Prototype = Action,
	Name = "Flight",
	Procedure = DFA_FLIGHT,
	Speed = 100,
	Accel = 16,
	Decel = 16,
	Length = 1,
	Delay = 0,
	FacetBase=1,
	NextAction = "Flight",
},
Dead = {
	Prototype = Action,
	Name = "Dead",
	Procedure = DFA_NONE,
	Speed = 10,
	Length = 1,
	Delay = 0,
	FacetBase=1,
	Directions = 2,
	FlipDir = 1,
	NextAction = "Hold",
	NoOtherAction = 1,
	ObjectDisabled = 1,
}
};