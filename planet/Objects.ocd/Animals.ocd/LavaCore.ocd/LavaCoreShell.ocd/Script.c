/**
	LavaCoreShell
	Protects the lava core

	@author 
*/

local Name = "$Name$";
local Plane = 500;
local Description = "$Description$";

local rota_interval = 2;
local rota_timer = 0;
local rota_increment = 2;

local rota_top_upper_freedom = 54;
local rota_top_lower_freedom = 46;

func Initialize()
{
	
}

public func InitAttach(object parent)
{
	SetAction("Attach", parent);
}

// Slowly rotates the opening towards an enemy

public func StartRotation()
{
	StopAll();
	AddEffect("ShellRotate",this,1,1,this);
}

public func StopRotation()
{
	RemoveEffect("ShellRotate", this);
}

// slowly rotates so that it covers the top of the core when surfacing

public func StartRotationToTop()
{
	StopAll();
	AddEffect("ShellRotateToTop",this,1,1,this);
}

public func StopRotationToTop()
{
	RemoveEffect("ShellRotateToTop", this);
}

// quickly rotates when moving normally

public func StartQuickRotation()
{
	StopAll();
	AddEffect("ShellQuickRotate",this,1,1,this);
}

public func StopQuickRotation()
{
	RemoveEffect("ShellQuickRotate", this);
}

protected func StopAll()
{
	RemoveEffect("ShellQuickRotate", this);
	RemoveEffect("ShellRotateToTop", this);
	RemoveEffect("ShellRotate", this);
}

// Rotate to enemy
protected func FxShellRotateTimer(object target, effect, int time)
{
	rota_timer++;
	
	if(rota_timer >= rota_interval)
	{
		rota_timer = 0;
		SetR(GetR() + rota_increment * 2);
	}
}

protected func FxShellRotateToTopTimer(object target, effect, int time)
{
	rota_timer++;
	
	if(!(GetR() < rota_top_upper_freedom && GetR() > rota_top_lower_freedom))
		if(rota_timer >= rota_interval)
		{
			rota_timer = 0;
			SetR(GetR() + rota_increment * 2);
		}
}

protected func FxShellQuickRotateTimer(object target, effect, int time)
{
	if(time >= 20)
		{
		StopQuickRotation();
		return -1;
		}
		
	rota_timer++;
	
	if(rota_timer >= rota_interval)
	{
		rota_timer = 0;
		SetR(GetR() + rota_increment);
	}
}

local ActMap = {

Attach = {
	Prototype = Action,
	Name = "Attach",
	Procedure = DFA_ATTACH,
	Speed = 100,
	Accel = 16,
	Decel = 16,
	Length = 1,
	Delay = 0,
	FacetBase=1,
	NextAction = "Attach",
}
};