/**
    Heal.c
    Function to heal livings over time.

    Author: Armin
*/

global func Heal(int amount)
{
	// Add effect.
	var effect = this->AddEffect("HealingOverTime", this, 1, 36);
	effect.healing_amount = amount;
	effect.done;
	return effect;
}

global func FxHealingOverTimeTimer(object target, proplist effect)
{
	// Stop healing the Clonk if he reached full health.
	if (target->GetEnergy() >= target.MaxEnergy/1000  || effect.done >= effect.healing_amount )
		return -1;
	target->DoEnergy(1);
	effect.done++;
	return true;
}
