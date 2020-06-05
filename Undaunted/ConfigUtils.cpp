#include "ConfigUtils.h"
#include <time.h>

namespace Undaunted
{
	std::string s_configPath;
	IntList BadRegionList;
	UnDictionary SettingsList;

	//Regions
	void AddBadRegionToConfig(UInt32 region)
	{
		_MESSAGE("Adding %08X to Bad Region List", region);
		BadRegionList.AddItem(region);
	}

	IntList GetBadRegions() {
		return BadRegionList;
	}

	void AddConfigValue(std::string key, std::string value)
	{
		//_MESSAGE("CONFIGLENGTH: %i", SettingsList.length);
		//check if it exists		
		for (int i = 0; i < SettingsList.length; i++)
		{
			if (SettingsList.data[i].key.compare(key) == 0)
			{
				SettingsList.data[i].value = value;
				//_MESSAGE("SET: %s : %s", key, value);
				return;
			}
		}
		//doesn't exist
		UnKeyValue setting = UnKeyValue();
		setting.key = key;
		setting.value = value;
		SettingsList.AddItem(setting);
		//_MESSAGE("ADD: %s : %s", key.c_str(), value.c_str());
	}

	UInt32 GetConfigValueInt(std::string key)
	{
		for (int i = 0; i < SettingsList.length; i++)
		{
			//_MESSAGE("Comparing %s : %s", key.c_str(), SettingsList.data[i].key.c_str());
			if (SettingsList.data[i].key.compare(key) == 0)
			{
				//_MESSAGE("Found Key %s : %s", key.c_str(), SettingsList.data[i].value.c_str());
				return atoi(SettingsList.data[i].value.c_str());
			}
		}
		//Not found.
		return 0;
	}

	UInt32 Playerlevel;
	void SetPlayerLevel(UInt32 level)
	{
		Playerlevel = level;
	}

	UInt32 GetPlayerLevel()
	{
		return Playerlevel;
	}

}