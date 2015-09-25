/*
	LavaCore
	The lava high life

	Moves rythmically, spews lava, acts as platform
	@Win
*/

local Name = "$Name$";
local Plane = 399;
local Description = "$Description$";
local CorrosionResist = 1;
local MaxEnergy = 100000;

// time is in frames

local shell;

local fossilized = 0;

local move_vectorX = 0;
local move_vectorY = 0;
local move_speed = 10; // bear in  mind this is also multiplied by a random factor
local move_random_dir = 3; // max random multiplier

local move_interval = 50;
local move_timer = 0;

local move_surface_interval = 500;
local move_surface_timer = 0;

local stay_surface_time = 300;
local stay_surface_timer = 0;
local surfaced = 0;

local shoot_interval = 4;
local shoot_timer = 0;

local out_of_lava_survive_time = 30;
local out_of_lava_survive_timer = 0;

local max_size = 40;

func Construction()
{
	DetermineMaxSize();
	
	shell = CreateObjectAbove(LavaCoreShell);
	shell->InitAttach(this);
	
	SetAction("Swim");
	SetComDir(COMD_None);
	
	AddEffect("CoreBehaviour",this,1,1,this);
	
}

protected func DetermineMaxSize()
{
	var temp_size = 0;
	var size_inc = 6;
	var distance_step = 100;
	
	for(var i = 1; i < 6; i++)
	{
		// Up
		if(PathFree(GetX(), GetY(), GetX(), GetY() - distance_step * i))
		{
			temp_size += size_inc;
		}
		// Down
		if(PathFree(GetX(), GetY(), GetX(), GetY() + distance_step * i))
		{
			temp_size += size_inc;
		}
		// Left
		if(PathFree(GetX(), GetY(), GetX() - distance_step * i, GetY()))
		{
			temp_size += size_inc;
		}
		// Right
		if(PathFree(GetX(), GetY(), GetX() + distance_step * i, GetY()))
		{
			temp_size += size_inc;
		}
	}
	
	max_size += temp_size;
}

func Place(int amount, proplist rectangle, proplist settings)
{
	var max_tries = 2 * amount;
	var loc_area = nil;
	if (rectangle) loc_area = Loc_InArea(rectangle);
	var f;
	
	while ((amount > 0) && (--max_tries > 0))
	{
		var spot = FindLocation(Loc_Material("Lava"), Loc_Space(20), loc_area);
		if(!Random(2))
			spot = FindLocation(Loc_Material("DuroLava"), Loc_Space(20), loc_area);
		if (!spot) continue;
		
		f = CreateObjectAbove(this, spot.x, spot.y, NO_OWNER);
		if (!f) continue;
		
		f->PlaceConstruction();
		
		if (f->Stuck())
		{
			f->Delete();
			continue;
		}
		--amount;
	}
	return f; // return last created lavacore
}

func PlaceConstruction()
{
	SetCon(max_size);
	shell->SetCon(max_size);
}

public func Delete()
{
	shell->RemoveObject();
	RemoveObject();
}

protected func FxCoreBehaviourTimer(object target, effect, int time)
{
	// when not surfacing normally, check if still swimming in lava. If not, turn to stone
	var mat = MaterialName(GetMaterial(0, -3));

	if(surfaced)
	{
		if(fossilized)
		{
		surfaced = 0;
		return 1;
		}
		var mat2 = MaterialName(GetMaterial(0, 5));
		
		if(mat2 != "Lava" && mat2 != "DuroLava")
		{
			surfaced = 0;
			move_surface_timer = 0;
			stay_surface_timer = 0;
			
			ContactTop();
			return 1;
		}
		// Search for Clonks and other animals to fry
		var prey = FindObject(Find_OCF(OCF_Alive), Find_Distance(55), Find_Not(Find_ID(LavaCore)));
		
		if(prey != nil && prey->GetID() != LavaCore && GetCon() >= max_size)
		{
			// Stop rotating the shell to cover the core from the top
			StopRotateShellToTop();
			// When prey is in front of the opening, open fire
			if(PathFree(GetX(), GetY(), prey->GetX(), prey->GetY()))
			{
				shoot_timer++;
				if(shoot_timer >= shoot_interval)
				{
					AttackTarget(prey);
				}
				StopRotateShell();
			}
			// Rotate the shell opening towards the prey
			else if(!GetEffect("ShellRotate", shell))
			{
				StartRotateShell(prey);
			}
		}
		else
			StartRotateShellToTop();
		
		// Stay still while surfaced
		SetYDir(0);
		SetXDir(0);
		
		// dive back down when time is up
		stay_surface_timer++;
		
		if(stay_surface_timer >= stay_surface_time)
		{
			surfaced = 0;
			move_surface_timer = 0;
			stay_surface_timer = 0;
			
			ContactTop();
		}
	}
	// When NOT surfaced and behaving normally
	else
	{	
		// When we're not in lava or durolava we are starting to dry up and eventually petrify
		if(!fossilized)
			if(mat != "Lava" && mat != "DuroLava")
			{
				out_of_lava_survive_timer++;
			
				if(out_of_lava_survive_timer >= out_of_lava_survive_time)
				{
					Fossilize();
				}
				else
					ContactTop();
		}
		// If we're in lava and petrified, then revive
		if(mat == "Lava" || mat == "DuroLava")
		{
			//out_of_lava_survive_timer = 0;
			
			if(fossilized)
				{
					Revive();
				}
		}
		if(fossilized)
			return 1;
		
		// Grow and reproduce
		if((Random(100) < 2 && !Random(15)) && GetCon() >= max_size)
			{
			var rival = FindObject(Find_ID(LavaCore), Find_Exclude(this), Find_Distance(200 + max_size));
			if(rival == nil)
				{
				var newcore = CreateObjectAbove(LavaCore);
				newcore->SetCon(30);
				newcore.shell->SetCon(30);
				}
			}
			
		if(GetCon() < max_size)
			if(!Random(45))
				{
				DoCon(1);
				shell->DoCon(1);
				}
		
		// Periodically surfacing
		move_surface_timer++;
		
		if(move_surface_timer >= move_surface_interval)
		{
			// Move up to reach the surface
			move_vectorX = 0;
			move_vectorY = -1;
			
			MoveImpulse();
			
			// When Sky or Tunnel is detected, we confirm that we are on the surface
			if(mat == "Tunnel" || mat == nil)
			{
				surfaced = 1;
			}
			else
				surfaced = 0;
		}
		// When not trying to reach the surface, splish splash around in intervals
		else
		{
			move_timer++;
			if(move_timer >= move_interval)
			{
				move_vectorX = RandomX(-1, 1);
				move_vectorY = RandomX(-1, 1);
			
				MoveImpulse();
			}
			
			// when detecting other Lavacores, swim in opposite directions. This keeps them spread evenly in a body of lava.
			var rival = FindObject(Find_ID(LavaCore), Find_Exclude(this), Find_Distance(10 + max_size));
			if(rival != nil && rival != this)
				if(PathFree(GetX(), GetY(), rival->GetX(), rival->GetY()))
				{
					if(rival->GetX() > GetX())
						move_vectorX = -1;
					else
						move_vectorX = 1;
					if(rival->GetY() > GetY())
						move_vectorY = 1;
					else
						move_vectorY = -1;
				
					MoveImpulse();
				}
			
			// dampen movement over time to make movements seem like impulses
			if(GetXDir() > 0)
				SetXDir(GetXDir() - 1);
			else
				SetXDir(GetXDir() + 1);
			if(GetYDir() > 0)
				SetYDir(GetYDir() - 1);
			else
				SetYDir(GetYDir() + 1);
		}
	}
}

/*
	Starts and stops rotating the shell towards an enemy
*/
protected func StartRotateShell(object prey)
{
	shell->StartRotation(prey);
}

protected func StopRotateShell()
{
	shell->StopRotation();
}

/*
	Starts and stops rotating the shell over the top of the core when surfacing
*/

protected func StartRotateShellToTop()
{
	shell->StartRotationToTop();
}

protected func StopRotateShellToTop()
{
	shell->StopRotationToTop();
}

/*
	Causes the object to move 
*/
protected func MoveImpulse()
{
	if(fossilized)
		return -1;
	move_timer = 0;
	// If we are being obstructed by the landscape while rising to the surface we have to wiggle a little
	if(IsSurfacing() && GetContact(-1) & CNAT_Left)
		{
		move_vectorX = RandomX(-1, 1);
		move_vectorY = 1;
		}
	if(IsSurfacing() && GetContact(-1) & CNAT_Right)
		{
		move_vectorX = -1;
		move_vectorY = RandomX(-1, 1);
		}
	// Contact at top? We probably are in a cave or something, cancel the surfacing
	if(IsSurfacing() && GetContact(-1) & CNAT_Top)
		{
		move_surface_timer = 0;
		ContactTop();
		return -1;
		}
	
	
	SetXDir(move_vectorX * move_speed * RandomX(1, move_random_dir));
	SetYDir(move_vectorY * move_speed * RandomX(1, move_random_dir));
	
	shell->StartQuickRotation();
}

public func IsSurfacing()
{
	return move_surface_timer >= move_surface_interval;
}

/*
	Shoots lava bubbles at the victim ):
*/

protected func AttackTarget(object prey)
{
	var bubble = CreateObject(BoilingLava_Bubble);
	bubble->SetXDir(prey->GetX() - GetX());
	bubble->SetYDir(prey->GetY() - GetY());
	bubble->DoCon(35);
	shoot_timer = 0;
}

protected func ContactBottom()
{
	move_vectorX = RandomX(-1, 1);
	move_vectorY = -1;
	
	MoveImpulse();
}

protected func ContactTop()
{
	move_vectorX = RandomX(-1, 1);
	move_vectorY = 1;
	
	MoveImpulse();
}

protected func ContactLeft()
{
	move_vectorX = RandomX(-1, 1);
	move_vectorY = 1;
	
	MoveImpulse();
}

protected func ContactRight()
{
	move_vectorX = -1;
	move_vectorY = RandomX(-1, 1);
	
	MoveImpulse();
}

protected func Death()
{
	shell->RemoveObject();
	Explode(BoundBy(max_size/2, 20, 90));
}

/*
	Called when the object was out of its lava pool for too long
*/
protected func Fossilize()
{
	out_of_lava_survive_timer = 0;
	fossilized = 1;
	
	SetAction("Flight");
	SetComDir(COMD_None);
	
	SetMeshMaterial("LavaCoreStoneMat");
	
	shell->SetMeshMaterial("LavaShellStoneMat");
	shell->StopAll();
	
}

protected func Revive()
{
	fossilized = 0;
	
	SetAction("Swim");
	SetComDir(COMD_None);
	
	SetMeshMaterial("LavaCoreMat");
	
	shell->SetMeshMaterial("LavaShellMat");
}

local ActMap = {

Swim = {
	Prototype = Action,
	Name = "Swim",
	Procedure = DFA_SWIM,
	Speed = 100,
	Accel = 16,
	Decel = 16,
	Length = 1,
	Delay = 0,
	FacetBase=1,
	NextAction = "Swim",
	StartCall = "UpdateSwim"
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