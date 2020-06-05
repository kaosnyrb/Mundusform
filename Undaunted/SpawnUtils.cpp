#include "SpawnUtils.h"
#include "ConfigUtils.h"

namespace Undaunted
{
	TESObjectREFR* SpawnMonsterAtRef(VMClassRegistry* registry,UInt32 Type, TESObjectREFR* ref, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		NiPoint3 startingpoint = ref->pos;
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
		{
			_MESSAGE("Failed to Spawn. Form Invalid: %08X", Type);
			return NULL;
		}
		int spawnradius = GetConfigValueInt("BountyEnemyInteriorSpawnRadius");
		NiPoint3 offset = NiPoint3(rand() & spawnradius, rand() & spawnradius, 0);
		MoveRefToWorldCell(ref, cell, worldspace, ref->pos + offset, NiPoint3(0, 0, 0));
		TESObjectREFR* spawned = PlaceAtMe(registry, 1, ref, spawnForm, 1, true, false);
		MoveRefToWorldCell(ref, cell, worldspace, startingpoint, NiPoint3(0, 0, 0));
		return spawned;
	}

	TESObjectREFR* SpawnMonsterInCell(VMClassRegistry* registry, UInt32 Type, WorldCell wcell)
	{
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
		{
			_MESSAGE("Failed to Spawn. Form Invalid: %08X", Type);
			return NULL;
		}
		TESObjectREFR* target = GetRandomObjectInCell(wcell);
		TESObjectREFR* spawned = PlaceAtMe(registry, 1, target, spawnForm, 1, false, false);
		return spawned;
	}


	VMResultArray<float> RiftRotations;
	RefList riftobjectrefs = RefList();

	RefList SpawnRift(VMClassRegistry* registry, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		//Debug
		srand(time(NULL));
		NiPoint3 startingpoint = Target->pos;// +NiPoint3(rand() % 1000, rand() % 1000, rand() % 1000);


		riftobjectrefs = RefList();
		FormRefList formlist = GetRandomRift();
		RiftRotations = VMResultArray<float>();
		for (int i = 0; i < formlist.length; i++)
		{
			TESForm* spawnForm = LookupFormByID(formlist.data[i].formId);
			if (spawnForm == NULL)
			{
				_MESSAGE("Spawnform is null");
				continue;
			}
			NiPoint3 position = startingpoint + formlist.data[i].pos;
			NiPoint3 rotation = formlist.data[i].rot;
			rotation.x = rotation.x* (180.0 / 3.141592653589793238463);
			rotation.y = rotation.y* (180.0 / 3.141592653589793238463);
			rotation.z = rotation.z* (180.0 / 3.141592653589793238463);

			TESObjectREFR* spawned = PlaceAtMe(registry, 1, Target, spawnForm, 1, true, false);
			spawned->unk90 = formlist.data[i].scale;
			spawned->pos = position;
			spawned->rot = rotation;
//			MoveRefToWorldCell(spawned, cell, worldspace, position, rotation);


			_MESSAGE("Spawn details: %f, %f, %f, %f, %f, %f", position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);
			Ref newref = Ref();
			newref.objectRef = spawned;
			riftobjectrefs.AddItem(newref);
			RiftRotations.push_back(rotation.x);
			RiftRotations.push_back(rotation.y);
			RiftRotations.push_back(rotation.z);		
		}
		for (int i = 0; i < 20; i++)
		{
			SpawnMonsterAtRef(registry, 0x00039CFC, riftobjectrefs.data[i].objectRef, cell, worldspace);
		}
		return riftobjectrefs;
	}

	VMResultArray<float> GetRiftRotations()
	{
		return RiftRotations;
	}
	RefList GetCurrentRiftRefs()
	{
		return riftobjectrefs;
	}
}