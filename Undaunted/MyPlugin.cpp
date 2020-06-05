#include "MyPlugin.h"
#include <Undaunted\StartupManager.h>
#include <algorithm>
#include <string>
#include "UnStringList.h"
#include <Undaunted\NavmeshTool.h>
#include <Undaunted\LocationUtils.h>
#include <Undaunted\ConfigUtils.h>
#include <Undaunted\SpawnUtils.h>

namespace Undaunted {

	void hook_CaptureArea(StaticFunctionTag* base) {
		//Create a SSE Edit script to recreate the current cell.
		CaptureArea();
	}

	// Fill out the WorldList, this checks the loaded world cells and finds the persistant reference cells.
	// This takes a while so we only do this once at the start
	bool hook_InitSystem(StaticFunctionTag* base, UInt32 playerLevel)
	{
		DataHandler* dataHandler = GetDataHandler();
		_MESSAGE("Mod Count: %08X", dataHandler->modList.loadedMods.count);
		for (int i = 0; i < dataHandler->modList.loadedMods.count; i++)
		{
			ModInfo* mod;
			dataHandler->modList.loadedMods.GetNthItem(i, mod);
			_MESSAGE("Listing Mods: %s ", mod->name);
		}
		InitNavmesh();
		LoadSettings();
		LoadRifts();
		LoadBlocks();
		BuildWorldList();
		SetPlayerLevel(playerLevel);
		return true;
	}

	// Given a mod name and a FormId - load order, return the actualy form id
	UInt32 hook_GetModForm(StaticFunctionTag* base, BSFixedString ModName, UInt32 FormId){
		DataHandler* dataHandler = GetDataHandler();
		const ModInfo* modInfo = dataHandler->LookupModByName(ModName.c_str());
		if (modInfo != NULL)
		{
			FormId = (modInfo->modIndex << 24) + FormId;
			if (modInfo->IsFormInMod(FormId))
			{
				return FormId;
			}
			else
			{
				_MESSAGE("FormId  %08X Not Found in %s", FormId, ModName.Get());
				return UInt32();
			}
		}
		_MESSAGE("Mod Not Found: %s", ModName.Get());
		return UInt32();
	}

	// Pass in a config value
	void hook_SetConfigValue(StaticFunctionTag* base, BSFixedString key, BSFixedString value)
	{
		AddConfigValue(key.Get(), value.Get());
	}
	// Returns an int that is in the config
	UInt32 hook_GetConfigValueInt(StaticFunctionTag* base, BSFixedString key)
	{
		return GetConfigValueInt(key.Get());
	}
	
	BSFixedString hook_GetPlayerWorldSpaceName(StaticFunctionTag* base)
	{
		_MESSAGE("hook_GetPlayerWorldSpaceName");
		return GetCurrentWorldspaceName().Get();
	}


	bool hook_isPlayerInWorldSpace(StaticFunctionTag* base, BSFixedString worldspacename)
	{
		_MESSAGE("hook_isPlayerInWorldSpace");
		return _stricmp(GetCurrentWorldspaceName().Get(), worldspacename.Get()) == 0;
	}


	VMResultArray<TESObjectREFR*> hook_SpawnRift(StaticFunctionTag* base, UInt32 BountyId, TESObjectREFR* Startpoint)
	{
		_MESSAGE("hook_SpawnRift");
		return VMResultArray<TESObjectREFR*>();
	}

	VMResultArray<float> hook_GetRiftRotations(StaticFunctionTag* base)
	{
		_MESSAGE("hook_GetRiftRotations");
		VMResultArray<float> resultsarray = VMResultArray<float>();
		return GetRiftRotations();
	}

	
	VMResultArray<TESObjectREFR*> hook_GetRiftReferences(StaticFunctionTag* base)
	{
		_MESSAGE("hook_GetRiftReferences");
		RefList results = GetCurrentRiftRefs();
		VMResultArray<TESObjectREFR*> resultsarray = VMResultArray<TESObjectREFR*>();
		for (int i = 0; i < results.length; i++)
		{
			resultsarray.push_back(results.data[i].objectRef);
		}
		return resultsarray;
	}

	void hook_CaptureNavTile(StaticFunctionTag* base)
	{
		_MESSAGE("hook_CaptureNavTile");
		MarkTile(GetPlayer()->pos.x, GetPlayer()->pos.y, GetPlayer()->pos.z);
	}

	void hook_ExportNavMesh(StaticFunctionTag* base)
	{
		_MESSAGE("hook_ExportNavMesh");
		ExportNavmesh();
	}

	bool RegisterFuncs(VMClassRegistry* registry) {

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, UInt32>("InitSystem", "Mundusform_SystemScript", Undaunted::hook_InitSystem, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, BSFixedString>("GetPlayerWorldSpaceName", "Mundusform_SystemScript", Undaunted::hook_GetPlayerWorldSpaceName, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, BSFixedString>("isPlayerInWorldSpace", "Mundusform_SystemScript", Undaunted::hook_isPlayerInWorldSpace, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, BSFixedString, BSFixedString>("SetConfigValue", "Mundusform_SystemScript", Undaunted::hook_SetConfigValue, registry));
		
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, UInt32, BSFixedString>("GetConfigValueInt", "Mundusform_SystemScript", Undaunted::hook_GetConfigValueInt, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, UInt32, BSFixedString, UInt32>("GetModForm", "Mundusform_SystemScript", Undaunted::hook_GetModForm, registry));


		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, VMResultArray<TESObjectREFR*>, UInt32, TESObjectREFR*>("SpawnRift", "Mundusform_SystemScript", Undaunted::hook_SpawnRift, registry));


		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, VMResultArray<float>>("GetRiftRotations", "Mundusform_SystemScript", Undaunted::hook_GetRiftRotations, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, VMResultArray<TESObjectREFR*>>("GetRiftReferences", "Mundusform_SystemScript", Undaunted::hook_GetRiftReferences, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, void>("CaptureArea", "Mundusform_SystemScript", Undaunted::hook_CaptureArea, registry));

		//Navmesh
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, void>("CaptureNavTile", "Mundusform_SystemScript", Undaunted::hook_CaptureNavTile, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, void>("ExportNavMesh", "Mundusform_SystemScript", Undaunted::hook_ExportNavMesh, registry));


		return true;
	}
}
