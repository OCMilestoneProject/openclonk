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
local move_speed = 5; // bear in  mind this is also multiplied by a random factor
local move_random_dir = 3; // max random multiplier

local target_food = 0; // Try to reach this object

local ultra_sound_particle;

local daytime;

local reproduction_timer = 0;
local reproduction_interval = 500;

//local shell_rotation;

func Construction()
{
	daytime = FindObject(Find_ID(Environment_Time));
	SetAction("Flight");
	SetComDir(COMD_None);
	AddEffect("CoreBehaviour",this,1,1,this);
	ChangeDirection();
	
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
	Attach = nil
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
	else if(daytime->IsNight())
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
			if(!Random(30))
				Fly();
		else
			Fly();
	}
	else
	{
		// Rotate based on flight direction
		this.MeshTransformation = Trans_Rotate(GetYDir() * 2, 0, 0, 1);
		// Move
		MoveImpulse();
		// Grow and reproduce
		if(GetCon() >= 100)
		{
			reproduction_timer++;
			if(reproduction_timer >= reproduction_interval)
			{	
				if(!Random(100))
				{
					var new_bat = CreateObjectAbove(Bat);
					new_bat->SetCon(60);
				}
				reproduction_timer = 0;
			}
		}
		// Fly towards food
		if(target_food)
		{
			move_vectorX = target_food->GetX() - GetX();
			move_vectorY = target_food->GetY() - GetY();

			if(Distance(GetX(), GetY(), target_food->GetX(), target_food->GetY()) < 10)
				{
				target_food->RemoveObject();
				ChangeDirection();
				}
		}
		// When not hunting for food, fly around randomly
		else 
		{
			if(GBackLiquid(0, 4))
			{
				move_vectorX = RandomX(-1, 1);
				move_vectorY = -1;
			}
			else if(!Random(30))
				ChangeDirection();
			// When above ground don't fly too far up into the sky
			if(GBackSky(0,0))
				if(!GBackSolid(0, 100) && !GBackLiquid(0, 100))
					if(PathFree(GetX(), GetY(), GetX(), GetY() + 100))
					{
						move_vectorX = RandomX(-1, 1);
						move_vectorY = 1;
					}
			if(!Random(250))
			{
				// Search for food
				target_food = FindObject(Find_Func("IsAnimalFood"), Find_Distance(100));
				if(target_food != nil && !PathFree(GetX(), GetY(), target_food->GetX(), target_food->GetY()))
					target_food = nil;
				else
				{
					CreateParticle("Shockwave", 0, 0, 0, 0 , 30, ultra_sound_particle, 1);
				}
			}	
		}
	}
}

/*
	Causes the object to move 
*/
protected func MoveImpulse()
{
	SetXDir(move_vectorX * move_speed * RandomX(1, move_random_dir));
	SetYDir(move_vectorY * move_speed * RandomX(1, move_random_dir));
}

protected func ChangeDirection()
{
	move_vectorX = RandomX(-1,1);
	move_vectorY = RandomX(-1,1);
	
	MoveImpulse();
}

protected func Fly()
{
	Log("got up");
	SetAction("Flight");
	SetComDir(COMD_None);
			
	target_food = nil;
			
	ChangeDirection();
}

protected func Sit()
{
	Log("sat");
	move_vectorX = 0;
	move_vectorY = 0;
		
	MoveImpulse();
	
	SetAction("Sit");
	SetComDir(COMD_None);
}

protected func ContactBottom()
{
	move_vectorX = RandomX(-1, 1);
	move_vectorY = -1;
}

protected func ContactTop()
{
	if(daytime == nil)
	{
		if(!Random(10))
			{
			Sit();
			return 1;
			}
	}
	else if(daytime->IsDay())
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
	RemoveEffect("CoreBehaviour", this);
	SetAction("Dead");
}

local ActMap = {

Sit = {
	Prototype = Action,
	Name = "Sit",
	Procedure = DFA_FLIGHT,
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