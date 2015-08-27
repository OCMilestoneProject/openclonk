/*-- Bushy Tree --*/

#include Library_Plant
#include Library_Tree

private func SeedChance() { return 500; }
private func SeedArea() { return 400; }
private func SeedAmount() { return 10; }

local lib_tree_burned = Tree_Bushy_Burned;

local Name = "$Name$";
local Touchable = 0;
local BlastIncinerate = 2;
local ContactIncinerate = 6;
local NoBurnDecay = 1;