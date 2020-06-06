#include "RiftManager.h"
#include <queue> 
#include <Undaunted\ConfigUtils.h>


namespace Undaunted {
	BlockLibary Libary;
	void SetBlockLibary(BlockLibary lib)
	{
		Libary = lib;
		_MESSAGE("BlockLibary is set with %i entries", Libary.length);
		//Shuffle the Libary
		srand(time(NULL));
		for (int i = 0; i < Libary.length + 10; i++)
		{
			Libary.SwapItem(rand() % Libary.length, rand() % Libary.length);
		}
	}
	int BlockDeckPosition = 0;

	void ShuffleDeck()
	{
		srand(time(NULL));
		for (int i = 0; i < Libary.length + 10; i++)
		{
			Libary.SwapItem(rand() % Libary.length, rand() % Libary.length);
		}
		BlockDeckPosition = 0;
	}

	
	Block FindBlockWithEnterance(const char* Type)
	{
		bool foundenterance = false;
		while (!foundenterance)
		{
			for (; BlockDeckPosition < Libary.length; BlockDeckPosition++)
			{
				_MESSAGE("Comparing %s and %s", Libary.data[BlockDeckPosition].enterancetile.exittype.c_str(), Type);
				if (Libary.data[BlockDeckPosition].enterancetile.exittype.compare(Type) == 0)
				{
					foundenterance = true;
					return Libary.data[BlockDeckPosition];
				}
			}
			ShuffleDeck();
		}
	}

	VMResultArray<float> RiftManagerRotations;
	RefList riftmanobjectrefs = RefList();


	void BuildRift(VMClassRegistry* registry, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		//Debug
		srand(time(NULL));
		NiPoint3 startingpoint = Target->pos;// +NiPoint3(rand() % 1000, rand() % 1000, rand() % 1000);
		std::queue <Tile> exits;
		FormRefList formlist = FormRefList();

		_MESSAGE("Place the enterance.");
		bool foundenterance = false;
		Block Enteranceblock = FindBlockWithEnterance("Entrance");

		formlist.AddItem(Enteranceblock.reflist.data[0]);
		_MESSAGE("Update the navmesh");
		for (int i = 0; i < Enteranceblock.navlist.length; i++)
		{
			MarkTile(Enteranceblock.navlist.data[i].x, Enteranceblock.navlist.data[i].y, Enteranceblock.navlist.data[i].z, Enteranceblock.navlist.data[i].quadsize);
		}
		for (int i = 0; i < Enteranceblock.exitslist.length; i++)
		{
			exits.push(Enteranceblock.exitslist.data[i]);
		}

		_MESSAGE("While we have exits open");
		int maxtiles = GetConfigValueInt("RiftGenerationMaxBlocks");
		int placedtiles = 0;
		//While there are still exits and we haven't reached the cap

		while (exits.size() > 0 && placedtiles < maxtiles - exits.size())
		{
			Tile exit = exits.front();
			exits.pop();
			_MESSAGE("Find a unplaced the exit.");

			_MESSAGE("Select a block that enterance matches the exit");

			Block selectedblock = FindBlockWithEnterance(exit.exittype.c_str());
			_MESSAGE("Place the block");
			for (int i = 0; i < selectedblock.reflist.length; i++)
			{
				FormRef ref = selectedblock.reflist.data[i];
				ref.pos.x += exit.x;
				ref.pos.y += exit.y;
				ref.pos.z += exit.z;
				formlist.AddItem(ref);
			}
			for (int i = 0; i < selectedblock.exitslist.length; i++)
			{
				Tile newexit = selectedblock.exitslist.data[i];
				newexit.x += exit.x;
				newexit.y += exit.y;
				newexit.z += exit.z;
				exits.push(newexit);
			}
			placedtiles++;
			_MESSAGE("Update the navmesh");
			for (int i = 0; i < selectedblock.navlist.length; i++)
			{

				MarkTile(selectedblock.navlist.data[i].x + exit.x, selectedblock.navlist.data[i].y + exit.y, selectedblock.navlist.data[i].z + exit.z, selectedblock.navlist.data[i].quadsize);
			}			
		}
		

		RiftManagerRotations = VMResultArray<float>();
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
			rotation.x = rotation.x;// *(180.0 / 3.141592653589793238463);
			rotation.y = rotation.y;// *(180.0 / 3.141592653589793238463);
			rotation.z = rotation.z;// *(180.0 / 3.141592653589793238463);

			TESObjectREFR* spawned = PlaceAtMe(registry, 1, Target, spawnForm, 1, true, false);
			spawned->unk90 = formlist.data[i].scale;
			spawned->pos = position;
			spawned->rot = rotation;
			//			MoveRefToWorldCell(spawned, cell, worldspace, position, rotation);


			_MESSAGE("Spawn details: %f, %f, %f, %f, %f, %f", position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);
			Ref newref = Ref();
			newref.objectRef = spawned;
			riftmanobjectrefs.AddItem(newref);
			RiftManagerRotations.push_back(rotation.x);
			RiftManagerRotations.push_back(rotation.y);
			RiftManagerRotations.push_back(rotation.z);
		}
	}

	VMResultArray<float> GetRiftManagerRotations()
	{
		return RiftManagerRotations;
	}

	RefList GetRiftManagerRefs()
	{
		return riftmanobjectrefs;
	}
}
