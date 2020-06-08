#include "RiftManager.h"
#include <queue> 
#include <Undaunted\ConfigUtils.h>
#include "BoundingBoxs.h"

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
		for (int i = 0; i < Libary.length + 10; i++)
		{
			Libary.SwapItem(rand() % Libary.length, rand() % Libary.length);
		}
		BlockDeckPosition = 0;
		_MESSAGE("Shuffle BlockLib");
	}

	
	Block FindBlockWithJoin(const char* Type)
	{
		bool foundenterance = false;
		while (!foundenterance)
		{
			for (; BlockDeckPosition < Libary.length; BlockDeckPosition++)
			{
				_MESSAGE("Comparing %s and %s", Libary.data[BlockDeckPosition].enterancetile.exittype.c_str(), Type);
				if (Libary.data[BlockDeckPosition].enterancetile.exittype.compare(Type) == 0)
				{
					if (Libary.data[BlockDeckPosition].exitslist.length > 0)
					{
						foundenterance = true;
						Block rblock = Block();
						rblock.boundingbox = Libary.data[BlockDeckPosition].boundingbox;
						rblock.enterancetile = Libary.data[BlockDeckPosition].enterancetile;
						// Dereference
						for (int i = 0; i < Libary.data[BlockDeckPosition].exitslist.length; i++)
						{
							rblock.exitslist.AddItem(Libary.data[BlockDeckPosition].exitslist.data[i]);
						}
						for (int i = 0; i < Libary.data[BlockDeckPosition].navlist.length; i++)
						{
							rblock.navlist.AddItem(Libary.data[BlockDeckPosition].navlist.data[i]);
						}
						for (int i = 0; i < Libary.data[BlockDeckPosition].reflist.length; i++)
						{
							rblock.reflist.AddItem(Libary.data[BlockDeckPosition].reflist.data[i]);
						}
						BlockDeckPosition++;
						return rblock;
					}
				}
			}
			ShuffleDeck();
		}
	}

	Block FindDeadend(const char* Type)
	{
		bool foundenterance = false;
		while (!foundenterance)
		{
			for (; BlockDeckPosition < Libary.length; BlockDeckPosition++)
			{
				_MESSAGE("Comparing %s and %s", Libary.data[BlockDeckPosition].enterancetile.exittype.c_str(), Type);
				if (Libary.data[BlockDeckPosition].enterancetile.exittype.compare(Type) == 0)
				{
					if (Libary.data[BlockDeckPosition].exitslist.length == 0)
					{
						foundenterance = true;
						Block rblock = Block();
						rblock.boundingbox = Libary.data[BlockDeckPosition].boundingbox;
						rblock.enterancetile = Libary.data[BlockDeckPosition].enterancetile;
						// Dereference
						for (int i = 0; i < Libary.data[BlockDeckPosition].exitslist.length; i++)
						{
							rblock.exitslist.AddItem(Libary.data[BlockDeckPosition].exitslist.data[i]);
						}
						for (int i = 0; i < Libary.data[BlockDeckPosition].navlist.length; i++)
						{
							rblock.navlist.AddItem(Libary.data[BlockDeckPosition].navlist.data[i]);
						}
						for (int i = 0; i < Libary.data[BlockDeckPosition].reflist.length; i++)
						{
							rblock.reflist.AddItem(Libary.data[BlockDeckPosition].reflist.data[i]);
						}
						BlockDeckPosition++;
						return rblock;
					}
				}
			}
			ShuffleDeck();
		}
	}

	std::queue<Tile> RemoveOrAddExitFromQueue(std::queue<Tile> que, Tile exit)
	{
		std::queue <Tile> exits;
		bool foundexit = false;

		while (que.size() > 0)
		{
			Tile oldexit = que.front();
			que.pop();
			if (oldexit.x == exit.x &&
				oldexit.y == exit.y &&
				oldexit.z == exit.z)
			{
				//This exit already existed, this means we should join these up.
				foundexit = true;
			}
			else
			{
				exits.push(oldexit);
			}
		}
		if (!foundexit)
		{
			exits.push(exit);
		}
		return exits;
	}

	VMResultArray<float> RiftManagerRotations;
	RefList riftmanobjectrefs = RefList();
	BoundingBoxList boundingboxes = BoundingBoxList();

	void BuildRift(VMClassRegistry* registry, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		//Debug
		srand(time(NULL));
		NiPoint3 startingpoint = Target->pos;// +NiPoint3(rand() % 1000, rand() % 1000, rand() % 1000);
		std::queue <Tile> exits;
		std::queue <Tile> sideexits;

		FormRefList formlist = FormRefList();
		_MESSAGE("Place the enterance.");
		bool foundenterance = false;
		Block Enteranceblock = FindBlockWithJoin("Entrance");
		
		formlist.AddItem(Enteranceblock.reflist.data[0]);
		boundingboxes.AddItem(Enteranceblock.boundingbox);

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

			Block selectedblock;
			bool validbox = false;
			BoundingBox box;
			while (!validbox)
			{
				validbox = true;
				selectedblock = FindBlockWithJoin(exit.exittype.c_str());
				box = selectedblock.boundingbox;
				box.position.x += exit.x;
				box.position.y += exit.y;
				selectedblock.RotateAroundPivot(Vector3(0, 0, 0), exit.bearing);
				validbox = !boundingboxes.Intersects(box);
				_MESSAGE("block validbox: %i", validbox);
				if (validbox)
				{
					for (int i = 0; i < selectedblock.exitslist.length && validbox; i++)
					{
						validbox = !boundingboxes.Intersects(BoundingBox(Vector2(selectedblock.exitslist.data[i].x + exit.x - 128, selectedblock.exitslist.data[i].y + exit.y - 128), 256, 256));
						_MESSAGE("exitslist validbox: %i", validbox);

					}
				}
			}
			boundingboxes.AddItem(box);

			_MESSAGE("Place the block");
			for (int i = 0; i < selectedblock.reflist.length; i++)
			{
				FormRef ref = selectedblock.reflist.data[i];
				ref.pos.x += exit.x;
				ref.pos.y += exit.y;
				ref.pos.z += exit.z;
				formlist.AddItem(ref);
			}
			TileList newexits = TileList();
			for (int i = 0; i < selectedblock.exitslist.length; i++)
			{
				Tile newexit = selectedblock.exitslist.data[i];
				newexit.x += exit.x;
				newexit.y += exit.y;
				newexit.z += exit.z;
				newexit.bearing += exit.bearing;
				newexits.AddItem(newexit);
			}
			//Choose a main path
			int exitnumber = 0; //This code doesn't work yet. Basically I wanted to select a random exit but it leads to the chance of having an exit which can nver be furfilled.
			if (newexits.length > 1)
			{
				exitnumber = 0;//rand() % (newexits.length - 1);
				_MESSAGE("exitnumber: %i", exitnumber);
			}
			exits.push(newexits.data[exitnumber]);
			//Add the remainders to the side paths
			for (int i = 0; i < newexits.length; i++)
			{
				if (i != exitnumber)
				{
					sideexits.push(newexits.data[i]);
				}
			}
			placedtiles++;
			_MESSAGE("Update the navmesh");
			for (int i = 0; i < selectedblock.navlist.length; i++)
			{
				MarkTile(selectedblock.navlist.data[i].x + exit.x, selectedblock.navlist.data[i].y + exit.y, selectedblock.navlist.data[i].z + exit.z, selectedblock.navlist.data[i].quadsize);
			}			
		}
		//Place the final room


		//Close the remaining exits
		while (sideexits.size() > 0)
		{
			Tile exit = sideexits.front();
			sideexits.pop();
			Block selectedblock = FindDeadend(exit.exittype.c_str());
			selectedblock.RotateAroundPivot(Vector3(0, 0, 0), exit.bearing);
			_MESSAGE("Place the block");
			for (int i = 0; i < selectedblock.reflist.length; i++)
			{
				FormRef ref = selectedblock.reflist.data[i];
				ref.pos.x += exit.x;
				ref.pos.y += exit.y;
				ref.pos.z += exit.z;
				formlist.AddItem(ref);
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
