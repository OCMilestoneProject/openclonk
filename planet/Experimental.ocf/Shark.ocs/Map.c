#include Library_Map

func InitializeMap(proplist map)
{
	var map_size;
	map_size = [50, 40]; 
	map->Resize(map_size[0], map_size[1]);
	Draw("Earth", nil, [0, map_size[1]/2, map_size[0], map_size[1]/2]);
	Draw("Water", nil, [1, map_size[1]/2, map_size[0]-2, map_size[1]/2-1]);
	return true;
}
