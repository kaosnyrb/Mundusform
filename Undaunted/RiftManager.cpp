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

	
	Block FindBlockWithJoin(const char* ConectorType, const char* blocktype)
	{
		bool foundenterance = false;
		while (!foundenterance)
		{
			for (; BlockDeckPosition < Libary.length; BlockDeckPosition++)
			{
				_MESSAGE("Comparing %s and %s", Libary.data[BlockDeckPosition].enterancetile.exittype.c_str(), ConectorType);
				if (Libary.data[BlockDeckPosition].enterancetile.exittype.compare(ConectorType) == 0 &&
					Libary.data[BlockDeckPosition].type.compare(blocktype) == 0 )
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

	Block FindDeadend(const char* Type, const char* blocktype)
	{
		bool foundenterance = false;
		while (!foundenterance)
		{
			for (; BlockDeckPosition < Libary.length; BlockDeckPosition++)
			{
				_MESSAGE("Comparing %s and %s", Libary.data[BlockDeckPosition].enterancetile.exittype.c_str(), Type);
				if (Libary.data[BlockDeckPosition].enterancetile.exittype.compare(Type) == 0 &&
					Libary.data[BlockDeckPosition].type.compare(blocktype) == 0)
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
	FormRefList formlist = FormRefList();

	void RenderBoundingBox(BoundingBox box)
	{
		FormRef ref = FormRef();
		ref.formId = 352102;//
		ref.pos.x = box.position.x;
		ref.pos.y = box.position.y;
		ref.pos.z = 0;
		ref.scale = 400;
		formlist.AddItem(ref);

		ref.pos.x = box.position.x + box.width;
		ref.pos.y = box.position.y;
		ref.pos.z = 0;
		ref.scale = 400;
		formlist.AddItem(ref);

		ref.pos.x = box.position.x;
		ref.pos.y = box.position.y + box.height;
		ref.pos.z = 0;
		ref.scale = 400;
		formlist.AddItem(ref);

		ref.pos.x = box.position.x + box.width;
		ref.pos.y = box.position.y + box.height;
		ref.pos.z = 0;
		ref.scale = 400;
		formlist.AddItem(ref);
	}

	int Work()
	{
		//Debug
		srand(time(NULL));
		std::queue <Tile> exits;
		std::queue <Tile> sideexits;
		int roomcount = GetConfigValueInt("RiftGenerationRooms");
		int hallcount = GetConfigValueInt("RiftGenerationHallLength");

		int roomBreaker = GetConfigValueInt("RiftGenerationRoomAttempts");
		int finalBreaker = GetConfigValueInt("RiftGenerationBreaker");

		int showboundingbox = GetConfigValueInt("ShowBoundingBox");


		_MESSAGE("Place the enterance.");
		bool foundenterance = false;
		Block Enteranceblock = FindBlockWithJoin("Entrance", "Entrance");

		for (int i = 0; i < Enteranceblock.reflist.length; i++)
		{
			FormRef ref = Enteranceblock.reflist.data[i];
			formlist.AddItem(ref);
		}
		boundingboxes.AddItem(Enteranceblock.boundingbox);
		if (showboundingbox == 1)
		{
			RenderBoundingBox(Enteranceblock.boundingbox);
		}

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


		int placedRooms = 0;
		int currentHallCount = hallcount;

		//While there are still exits and we haven't reached the cap
		while (exits.size() > 0 && placedRooms < roomcount)
		{
			Tile exit = exits.front();
			exits.pop();
			_MESSAGE("Find a unplaced the exit.");

			_MESSAGE("Select a block that enterance matches the exit");

			Block selectedblock;
			bool validbox = false;
			BoundingBox box;
			int Breaker = 0;
			int currentturncount = 0;
			int currentbearing = 0;

			bool isHall = false;
			while (!validbox)
			{
				Breaker++;
				validbox = true;
				if (currentHallCount > 0)
				{
					selectedblock = FindBlockWithJoin(exit.exittype.c_str(), "hall");
					isHall = true;
				}
				else
				{
					selectedblock = FindBlockWithJoin(exit.exittype.c_str(), "room");
				}
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
				if (Breaker > roomBreaker && !validbox)
				{
					_MESSAGE("Breaker 1 Activated. Trying to place hall.");
					//Can't place a room? Try a hall.
					currentHallCount = 1;
				}
				if (Breaker > finalBreaker && !validbox)
				{
					_MESSAGE("Breaker 2 Activated. Stopping generation.");
					return 0;
				}
			}
			if (isHall)
			{
				currentHallCount--;
			}
			else
			{
				placedRooms++;
				currentHallCount = hallcount;
			}
			boundingboxes.AddItem(box);
			if (showboundingbox == 1)
			{
				RenderBoundingBox(box);
			}

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
				exitnumber = rand() % (newexits.length - 1); //0
				_MESSAGE("exitnumber: %i", exitnumber);
			}
			if (currentbearing != newexits.data[exitnumber].bearing)
			{
				currentturncount++;
				currentbearing = newexits.data[exitnumber].bearing;
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
			_MESSAGE("Update the navmesh");
			for (int i = 0; i < selectedblock.navlist.length; i++)
			{
				MarkTile(selectedblock.navlist.data[i].x + exit.x, selectedblock.navlist.data[i].y + exit.y, selectedblock.navlist.data[i].z + exit.z, selectedblock.navlist.data[i].quadsize);
			}
		}
		//Place the final room
		_MESSAGE("Place the final room");

		Tile exit = exits.front();
		exits.pop();
		Block Exitblock = FindDeadend(exit.exittype.c_str(), "exit");
		Exitblock.RotateAroundPivot(Vector3(0, 0, 0), exit.bearing);
		for (int i = 0; i < Exitblock.reflist.length; i++)
		{
			FormRef ref = Exitblock.reflist.data[i];
			ref.pos.x += exit.x;
			ref.pos.y += exit.y;
			ref.pos.z += exit.z;
			formlist.AddItem(ref);
		}
		boundingboxes.AddItem(Exitblock.boundingbox);

		for (int i = 0; i < Exitblock.navlist.length; i++)
		{
			MarkTile(Exitblock.navlist.data[i].x + exit.x, Exitblock.navlist.data[i].y + exit.y, Exitblock.navlist.data[i].z + exit.z, Exitblock.navlist.data[i].quadsize);
		}

		//Close the remaining exits
		while (sideexits.size() > 0)
		{
			bool validbox = false;
			Tile exit = sideexits.front();
			Block selectedblock = FindDeadend(exit.exittype.c_str(), "end");
			selectedblock.RotateAroundPivot(Vector3(0, 0, 0), exit.bearing);
			BoundingBox box;
			box = selectedblock.boundingbox;
			box.position.x += exit.x;
			box.position.y += exit.y;
			validbox = !boundingboxes.Intersects(box);
			if (validbox)
			{
				sideexits.pop();
				_MESSAGE("Place the block");
				for (int i = 0; i < selectedblock.reflist.length; i++)
				{
					FormRef ref = selectedblock.reflist.data[i];
					ref.pos.x += exit.x;
					ref.pos.y += exit.y;
					ref.pos.z += exit.z;
					formlist.AddItem(ref);
				}
				_MESSAGE("Update the navmesh");
				for (int i = 0; i < selectedblock.navlist.length; i++)
				{
					MarkTile(selectedblock.navlist.data[i].x + exit.x, selectedblock.navlist.data[i].y + exit.y, selectedblock.navlist.data[i].z + exit.z, selectedblock.navlist.data[i].quadsize);
				}
				boundingboxes.AddItem(box);
				if (showboundingbox == 1)
				{
					RenderBoundingBox(box);
				}
			}
		}
		return 1;
	}

	void BuildRift(VMClassRegistry* registry, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		riftmanobjectrefs = RefList();
		boundingboxes = BoundingBoxList();
		formlist = FormRefList();

		int attemptcount = GetConfigValueInt("RiftGenerationTotalAttempts");
		int currentattempts = 0;

		int buildstatus = Work();
		while (buildstatus == 0 && currentattempts < attemptcount)
		{
			riftmanobjectrefs = RefList();
			boundingboxes = BoundingBoxList();
			formlist = FormRefList();
			currentattempts++;
			buildstatus = Work();
		}

		NiPoint3 startingpoint = Target->pos;
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
			rotation.x = rotation.x;
			rotation.y = rotation.y;
			rotation.z = rotation.z;

			TESObjectREFR* spawned = PlaceAtMe(registry, 1, Target, spawnForm, 1, true, false);
			spawned->unk90 = formlist.data[i].scale;
			spawned->pos = position;
			spawned->rot = rotation;

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
