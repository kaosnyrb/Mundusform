#include "IntList.h"
#include "UnStringList.h"

namespace Undaunted
{
	//Regions/Worldspaces
	void AddBadRegionToConfig(UInt32 region);
	IntList GetBadRegions();

	//General
	void AddConfigValue(std::string key, std::string value);
	UInt32 GetConfigValueInt(std::string key);
	void SetPlayerLevel(UInt32 level);
	UInt32 GetPlayerLevel();
}