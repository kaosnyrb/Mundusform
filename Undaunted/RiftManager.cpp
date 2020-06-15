#include "RiftManager.h"
#include <queue> 
#include <Undaunted\ConfigUtils.h>
#include "BoundingBoxs.h"

namespace Undaunted {
	BlockLibary Libary;
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

	void SetBlockLibary(BlockLibary lib)
	{
		Libary = lib;
		_MESSAGE("BlockLibary is set with %i entries", Libary.length);
		ShuffleDeck();
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

	bool BlockFitsExit(Block block, Tile exit)
	{
		bool validbox = !boundingboxes.Intersects(block.boundingbox);
		_MESSAGE("block validbox: %i", validbox);
		if (validbox)
		{
			for (int i = 0; i < block.exitslist.length && validbox; i++)
			{
				BoundingBox exitbox = BoundingBox(Vector2(block.exitslist.data[i].x, block.exitslist.data[i].y), 64, 64);
				validbox = !boundingboxes.Intersects(exitbox);
				_MESSAGE("exitslist validbox: %i", validbox);
			}
		}
		return validbox;
	}

	Block TranslateBlock(Block block, Tile exit)
	{
		block.boundingbox.position.x += exit.x;
		block.boundingbox.position.y += exit.y;
		for (int i = 0; i < block.reflist.length; i++)
		{
			block.reflist.data[i].pos.x += exit.x;
			block.reflist.data[i].pos.y += exit.y;
			block.reflist.data[i].pos.z += exit.z;
		}
		for (int i = 0; i < block.exitslist.length; i++)
		{
			block.exitslist.data[i].x += exit.x;
			block.exitslist.data[i].y += exit.y;
			block.exitslist.data[i].z += exit.z;
			block.exitslist.data[i].bearing += exit.bearing;
		}
		for (int i = 0; i < block.navlist.length; i++)
		{
			block.navlist.data[i].x += exit.x;
			block.navlist.data[i].y += exit.y;
			block.navlist.data[i].z += exit.z;
		}
		return block;
	}

	TileList PlaceBlock(Block block)
	{
		_MESSAGE("Place the block");
		boundingboxes.AddItem(block.boundingbox);
		for (int i = 0; i < block.reflist.length; i++)
		{
			formlist.AddItem(block.reflist.data[i]);
		}
		_MESSAGE("Update the navmesh");
		for (int i = 0; i < block.navlist.length; i++)
		{
			MarkTile(block.navlist.data[i].x, block.navlist.data[i].y, block.navlist.data[i].z, block.navlist.data[i].quadsize);
		}
		TileList newexits = TileList();
		for (int i = 0; i < block.exitslist.length; i++)
		{
			newexits.AddItem(block.exitslist.data[i]);
		}
		return newexits;
	}

	std::queue <Tile> exits;
	std::queue <Tile> sideexits;
	int roomcount;
	int hallcount;
	int roomBreaker;
	int finalBreaker;
	int showboundingbox;

	int Work()
	{
		//srand(1337);//Can set a fixed seed using this.
		srand(time(NULL));
		ShuffleDeck();

		_MESSAGE("Place the enterance.");
		Block Enteranceblock = FindBlockWithJoin("Entrance", "Entrance");		
		PlaceBlock(Enteranceblock);// We don't need to move the enterance, it's at 0,0,0
		for (int i = 0; i < Enteranceblock.exitslist.length; i++)
		{
			exits.push(Enteranceblock.exitslist.data[i]);
		}
		if (showboundingbox == 1) RenderBoundingBox(Enteranceblock.boundingbox);
		_MESSAGE("While we have exits open");
		int placedRooms = 0;
		int currentHallCount = hallcount;
		while (exits.size() > 0 && (placedRooms <= roomcount || currentHallCount > 0))
		{
			_MESSAGE("Get the next unplaced exit on the main branch.");
			Tile exit = exits.front();
			exits.pop();
			_MESSAGE("Select a block that entrance matches the exit");
			Block selectedblock;
			bool validbox = false;
			int Breaker = 0; // It's possible that there is no valid blocks for this generation attempt. This prevents an infinite loop.
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
				selectedblock.RotateAroundPivot(Vector3(0, 0, 0), exit.bearing);
				selectedblock = TranslateBlock(selectedblock, exit);
				validbox = BlockFitsExit(selectedblock, exit);
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
			//We have found a block that fits.
			if (isHall)
			{
				currentHallCount--;
			}
			else
			{
				placedRooms++;
				currentHallCount = hallcount;
			}
			if (showboundingbox == 1) RenderBoundingBox(selectedblock.boundingbox);			
			TileList newexits = PlaceBlock(selectedblock);
			//Choose a main path
			int exitnumber = 0;
			if (newexits.length > 1)
			{
				exitnumber = rand() % (newexits.length);
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
		}
		//Place the final room
		_MESSAGE("Place the final room");
		Tile exit = exits.front();
		exits.pop();
		Block Exitblock = FindDeadend(exit.exittype.c_str(), "exit");
		Exitblock.RotateAroundPivot(Vector3(0, 0, 0), exit.bearing);
		Exitblock = TranslateBlock(Exitblock, exit);
		TileList newexits = PlaceBlock(Exitblock);

		//Close the remaining exits
		int Breaker = 0;
		while (sideexits.size() > 0)
		{
			Breaker++;
			bool validbox = false;
			Tile exit = sideexits.front();
			Block selectedblock = FindDeadend(exit.exittype.c_str(), "end");
			selectedblock.RotateAroundPivot(Vector3(0, 0, 0), exit.bearing);
			selectedblock = TranslateBlock(selectedblock, exit);
			validbox = BlockFitsExit(selectedblock, exit);
			if (validbox)
			{
				sideexits.pop();
				TileList newexits = PlaceBlock(selectedblock);
				if (showboundingbox == 1) RenderBoundingBox(selectedblock.boundingbox);
			}
			if (Breaker > finalBreaker && !validbox)
			{
				_MESSAGE("Breaker 2 Activated. Stopping generation.");
				return 0;
			}
		}
		return 1;
	}

	void BuildRift(VMClassRegistry* registry, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		riftmanobjectrefs = RefList();
		boundingboxes = BoundingBoxList();
		formlist = FormRefList();
		exits = std::queue <Tile>();
		sideexits = std::queue <Tile>();
		InitNavmesh();

		int attemptcount = GetConfigValueInt("RiftGenerationTotalAttempts");
		roomcount = GetConfigValueInt("RiftGenerationRooms");
		hallcount = GetConfigValueInt("RiftGenerationHallLength");
		roomBreaker = GetConfigValueInt("RiftGenerationRoomAttempts");
		finalBreaker = GetConfigValueInt("RiftGenerationBreaker");
		showboundingbox = GetConfigValueInt("ShowBoundingBox");

		int currentattempts = 0;

		int buildstatus = Work();
		while (buildstatus == 0 && currentattempts < attemptcount)
		{
			riftmanobjectrefs = RefList();
			boundingboxes = BoundingBoxList();
			formlist = FormRefList();
			exits = std::queue <Tile>();
			sideexits = std::queue <Tile>();
			InitNavmesh();
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
