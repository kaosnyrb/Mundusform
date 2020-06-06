#pragma once
#include <Undaunted\BlockLibary.h>
#include <Undaunted\FormRefList.h>
#include <Undaunted\LocationUtils.h>
#include <Undaunted\NavmeshTool.h>
#include <Undaunted\SKSELink.h>
#include "RefList.h"

namespace Undaunted {

	void SetBlockLibary(BlockLibary lib);

	void BuildRift(VMClassRegistry* registry, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace);
	VMResultArray<float> GetRiftManagerRotations();
	RefList GetRiftManagerRefs();
}