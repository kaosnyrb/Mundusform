#include "LocationUtils.h"
#include <Undaunted\ConfigUtils.h>
#include "WorldCellList.h"
#include "FormRefList.h"
#include <Undaunted\RiftList.h>
#include <Undaunted\RefList.h>
#include "NavmeshTool.h"

namespace Undaunted {
	WorldCellList worldCellList;
	bool worldCellListBuilt = false;



	TESObjectREFR* GetRefObjectInCurrentCell(UInt32 formID)
	{
		TESObjectCELL* parentCell = GetPlayer()->parentCell;
		int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
		_MESSAGE("GetObjectInCurrentCell Num Ref: %i", numberofRefs);
		for (int i = 0; i < numberofRefs; i++)
		{
			TESObjectREFR* ref = papyrusCell::GetNthRef(parentCell, i, 0);
			if (ref != NULL)
			{
				if (ref->formID != NULL)
				{
					if (ref->formID == formID)
					{
						_MESSAGE("ref->formID == formID");
						return ref;
					}
				}
			}
		}
		return NULL;
	}

	//Use Sparingly.
	/*
	TESObjectREFR* GetRefObjectFromWorld(UInt32 formID)
	{
		DataHandler* handler = GetDataHandler();
		_MESSAGE("RegionList Count: %08X", handler->regionList->Count());
		UInt32 regioncount = handler->regionList->Count();
		for (UInt32 i = 0; i < regioncount; i++)
		{
			//Some regions are dodgy
			if (i != 0x00000033 && i != 0x00000036 && i != 0x0000009B && i != 0x00000110)
			{
				_MESSAGE("processing worldSpace %08X", i);
				TESRegion* test = (TESRegion*)handler->regionList->GetNthItem(i);
				if (test != NULL)
				{
					if (test->worldSpace == NULL)
					{
						_MESSAGE("worldSpace %08X is null", i);
					}
					else
					{
						TESObjectCELL* cell = test->worldSpace->unk088;
						if (cell != NULL)
						{
							_MESSAGE("Regioncell form id %08X", cell->formID);
							if (cell->formID == 0x00000D74)
							{
								int numberofRefs = papyrusCell::GetNumRefs(cell, 0);
								if (numberofRefs > 0)
								{
									for (int i = 0; i < numberofRefs; i++)
									{
										TESObjectREFR* ref = papyrusCell::GetNthRef(cell, i, 0);
										if (ref != NULL)
										{
											if (ref->formID != NULL)
											{
												if (ref->formID == formID)
												{
													return ref;
												}
											}
										}
									}
								}
							}
						}
						else
						{
							_MESSAGE("unk088 is null", i);
						}
					}

				}
				else
				{
					_MESSAGE("RegionList %08X is null", i);
				}
			}
		}
		return NULL;
	}*/

	TESObjectREFR* GetRandomObjectInCell(WorldCell worldcell)
	{
		int numberofRefs = papyrusCell::GetNumRefs(worldcell.cell, 0);
		//_MESSAGE("GetRandomObjectInCell Num Ref: %i", numberofRefs);		
		if (numberofRefs == 0)return NULL;
		while (true)
		{
			int Nth = rand() % numberofRefs;
			TESObjectREFR* ref = papyrusCell::GetNthRef(worldcell.cell, Nth, 0);
			bool valid = true;
			if (ref != NULL && valid)
			{
				return ref;
			}
		}
		return NULL;
	}

	void BuildWorldList()
	{
		if (worldCellListBuilt)
		{
			return;
		}
		DataHandler* handler = GetDataHandler();
		_MESSAGE("RegionList Count: %08X", handler->regionList->Count());

		IntList badregions = GetBadRegions();

		UInt32 regioncount = handler->regionList->Count();
		for (UInt32 i = 0; i < regioncount; i++)
		{
			//Check for badregion
			bool badRegion = false;
			for (UInt32 j = 0; j < badregions.length; j++)
			{
				if (badregions.data[j] == i)
				{
					badRegion = true;
				}
			}
			//Some regions are dodgy
			if (!badRegion)
			{
				_MESSAGE("processing worldSpace %08X", i);
				TESRegion* test = (TESRegion*)handler->regionList->GetNthItem(i);
				if (test != NULL)
				{
					if (test->worldSpace == NULL)
					{
						_MESSAGE("worldSpace %08X is null", i);
					}
					else
					{
						_MESSAGE("worldSpace %08X is not null", i);
						_MESSAGE("processing worldSpace %p", test->worldSpace);
						//Yeah. So some regions are really low in the memory stack?
						//They blow up if you try and use them so we filter out worldspace pointers below 000000000002433E
						if ((uintptr_t)test->worldSpace <= 148286)
						{
							_MESSAGE("Low Level Region, Ignoring.");
						}
						else
						{
							TESObjectCELL* cell = test->worldSpace->unk088;
							if (cell != NULL)
							{
								_MESSAGE("unk088 is not null for worldspace %08x", i);
								int numberofRefs = papyrusCell::GetNumRefs(cell, 0);
								if (numberofRefs > 0)
								{
									WorldCell wcell = WorldCell();
									wcell.cell = cell;
									wcell.world = test->worldSpace;

									//Check if we know about this cell
									bool badcell = false;
									for (int i = 0; i < worldCellList.length && !badcell; i++)
									{
										if (worldCellList.data[i].cell->formID == wcell.cell->formID)
										{
											badcell = true;
										}
									}

									if (!badcell)
									{
										worldCellList.AddItem(wcell);
									}
								}
							}
							else
							{
								_MESSAGE("unk088 is null for worldspace %08x", i);
							}
						}
					}
				}
				else
				{
					_MESSAGE("RegionList %08X is null", i);
				}
			}
		}
		_MESSAGE("worldCellList built. %i Entries", worldCellList.length);
		worldCellListBuilt = true;
		for (int i = 0; i < worldCellList.length; i++)
		{
			_MESSAGE("WorldName: %s", worldCellList.data[i].world->editorId.Get());
		}
	}

	WorldCell GetRandomWorldCell()
	{
		srand(time(NULL));
		int worldcellid = rand() % worldCellList.length;
		return worldCellList.data[worldcellid];
	}

	WorldCell GetNamedWorldCell(BSFixedString WorldspaceName)
	{
		for (int i = 0; i < worldCellList.length; i++)
		{
			if (strcmp(worldCellList.data[i].world->editorId.Get(), WorldspaceName.c_str()) == 0)
				return worldCellList.data[i];
		}

		_MESSAGE("Named World Cell not found: %s", WorldspaceName.Get());
		return WorldCell();
	}

	void MoveRefToWorldCell(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot)
	{
		if (object != NULL)
		{
			_MESSAGE("Moving %08X to %08X in %s", object->formID, cell->formID, worldspace->editorId.Get());
			NiPoint3 finalPos(pos);
			MoveRef(object, cell, worldspace, finalPos, rot);
		}
	}

	//Expensive...
	WorldCell GetWorldCellFromRef(TESObjectREFR* object)
	{
		NiPoint3 distance;
		TESObjectREFR* ref;
		int numberofRefs;
		for (int i = 0; i < worldCellList.length; i++)
		{
			numberofRefs = papyrusCell::GetNumRefs(worldCellList.data[i].cell, 0);
			for (int j = 0; j < numberofRefs; j++)
			{
				ref = papyrusCell::GetNthRef(worldCellList.data[i].cell, j, 0);
				if (ref != NULL)
				{
					distance = object->pos - ref->pos;
					Vector3 distvector = Vector3(distance.x, distance.y, distance.z);
					if (distvector.Magnitude() < 200)
					{
						return worldCellList.data[i];
					}
				}
			}
		}
		return WorldCell();
	}

	void CaptureArea()
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\UndauntedRift.pas");
		FormRefList list = FormRefList();
		TESObjectCELL* parentCell = GetPlayer()->parentCell;
		int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
		//Right so instead of faffing about creating a sse edit script that can read in files we'll just generate an sse script.
		_MESSAGE("unit userscript;uses SkyrimUtils;uses mteFunctions;");
		_MESSAGE("function NewArrayElement(rec: IInterface; path: String): IInterface; var a: IInterface; begin a := ElementByPath(rec, path); if Assigned(a) then begin Result := ElementAssign(a, HighInteger, nil, false); end else begin a := Add(rec, path, true);Result := ElementByIndex(a, 0);end;end;");
		_MESSAGE("function Process(e: IInterface): integer;");
		_MESSAGE("var cell: IInterface; ref: IInterface; navm: IInterface; nvnm: IInterface; verts: IInterface;");
		_MESSAGE("begin Result := 0; if not (Signature(e) = 'CELL') then begin Exit; end; cell := createRecord(GetFile(e), 'CELL');");
		_MESSAGE("SetElementEditValues(cell, 'EDID', '%s');","Cell Name");
		_MESSAGE("SetElementEditValues(cell, 'LTMP', '%s');", "0006AB01");//Lighting template
		for (int i = 0; i < numberofRefs; i++)
		{
			TESObjectREFR* ref = papyrusCell::GetNthRef(parentCell, i, 0);
			if (ref != NULL)
			{
				if (ref->formID != NULL)
				{
					FormRef saveref = FormRef();
					saveref.formId = ref->baseForm->formID;
					saveref.pos = ref->pos;// -GetPlayer()->pos;
					saveref.rot = ref->rot;
					saveref.scale = ref->unk90;
					saveref.type = static_cast<FormType>(ref->baseForm->formType);
					if ((saveref.type == kFormType_Static
						//|| saveref.type == kFormType_LeveledCharacter
						|| saveref.type == kFormType_Activator
						|| saveref.type == kFormType_Sound
						|| saveref.type == kFormType_Light
						|| saveref.type == kFormType_Container
						|| saveref.type == kFormType_MovableStatic
						|| saveref.type == kFormType_Furniture
						|| saveref.type == kFormType_Reference
						|| saveref.type == kFormType_LeveledItem
						//|| saveref.type == kFormType_NPC
						|| saveref.type == kFormType_Hazard
						|| saveref.type == kFormType_Door
						|| saveref.type == kFormType_Flora
						|| saveref.type == kFormType_Tree
						|| saveref.type == kFormType_Grass
						|| saveref.type == kFormType_NAVM
						|| saveref.type == kFormType_NAVI) 
						&& saveref.formId < 4278190000)
					{
						_MESSAGE("ref: = Add(cell, 'REFR', true);");
						_MESSAGE("SetElementEditValues(ref, 'EDID', GetElementEditValues(getRecordByFormID('%08X'), 'EDID'));", saveref.formId);
						_MESSAGE("SetElementEditValues(ref, 'NAME', '%08X');", saveref.formId);
						_MESSAGE("SetElementEditValues(ref, 'XSCL', '%f');", ((float)saveref.scale)/100);
						_MESSAGE("seev(ref, 'DATA\\[0]\\[0]', %f);", saveref.pos.x);
						_MESSAGE("seev(ref, 'DATA\\[0]\\[1]', %f);", saveref.pos.y);
						_MESSAGE("seev(ref, 'DATA\\[0]\\[2]', %f);", saveref.pos.z);
						_MESSAGE("seev(ref, 'DATA\\[1]\\[0]', %f);", saveref.rot.x * (180.0 / 3.141592653589793238463));
						_MESSAGE("seev(ref, 'DATA\\[1]\\[1]', %f);", saveref.rot.y * (180.0 / 3.141592653589793238463));
						_MESSAGE("seev(ref, 'DATA\\[1]\\[2]', %f);", saveref.rot.z * (180.0 / 3.141592653589793238463));
					}
				}
			}
		}

		ExportNavmeshChunk();

		_MESSAGE("end;function Finalize: integer;begin Result := 0;  FinalizeUtils();end;end.");
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Undaunted.log");
	}

	//Raw Rifts

	RiftList riftList = RiftList();
	void AddRift(FormRefList reflist)
	{
		RiftRef newref = RiftRef();
		newref.reflist = reflist;
		riftList.AddItem(newref);
	}

	FormRefList GetRandomRift()
	{
		return riftList.data[rand() % riftList.length].reflist;
	}

	// Baked Rifts
	RefList RiftStartMarkers = RefList();
	void ShuffleBakedRifts()
	{
		srand(time(NULL));
		for (int i = 0; i < RiftStartMarkers.length + 10; i++)
		{
			RiftStartMarkers.SwapItem(rand() % RiftStartMarkers.length, rand() % RiftStartMarkers.length);
		}
	}

	void InitBakedRiftStartMarkers()
	{
		_MESSAGE("Finding all Rift Start Markers");
		RiftStartMarkers = RefList();
		DataHandler* dataHandler = GetDataHandler();
		const ModInfo* modInfo = dataHandler->LookupModByName("Undaunted.esp");
		if (modInfo == NULL)
		{
			_MESSAGE("Can't find Undaunted.esp. What the hell?");
			return;
		}
		UInt32 FormId = (modInfo->modIndex << 24) + 915120; //040DF6B0 - 01_Undaunted_RiftEnteranceMarker
		_MESSAGE("FormId: %08X", FormId);
		UInt32 cellcount = GetDataHandler()->cellList.m_size;
		_MESSAGE("cellcount: %08X", cellcount);
		for (int i = 0; i < cellcount; i++)
		{
			TESObjectCELL* parentCell = GetDataHandler()->cellList.m_data[i];
			int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
			for (int j = 0; j < numberofRefs; j++)
			{
				TESObjectREFR* ref = papyrusCell::GetNthRef(parentCell, j, 0);
				if (ref != NULL)
				{
					if (ref->formID != NULL)
					{
						if (ref->baseForm->formID == FormId)
						{
							Ref formref = Ref();
							formref.objectRef = ref;
							RiftStartMarkers.AddItem(formref);
						}
					}
				}
			}
		}
		ShuffleBakedRifts();
	}



	int currentRiftTarget = 0;
	TESObjectREFR* GetRandomBakedRiftStartMarker()
	{
		srand(time(NULL));
		currentRiftTarget++;
		if (currentRiftTarget >= RiftStartMarkers.length)
		{
			//Reshuffle deck
			_MESSAGE("Reshuffle deck");
			ShuffleBakedRifts();
			currentRiftTarget = 0;
		}
		_MESSAGE("currentRiftTarget: %i / %i", currentRiftTarget, RiftStartMarkers.length);

		Ref target = RiftStartMarkers.data[currentRiftTarget];

		return target.objectRef;
	}

	//Interiors
/*
if (SpawnLocref == NULL)
{
	TESObjectCELL* here = GetPlayer()->parentCell;
	_MESSAGE("Here form id %08X", here->formID);
	_MESSAGE("Cell list Size %08X", GetDataHandler()->cellList.m_size);
	UInt32 cellcount = GetDataHandler()->cellList.m_size;
	for (int i = 0; i < cellcount; i++)
	{
		TESObjectCELL* parentCell = GetDataHandler()->cellList.m_data[i];
		_MESSAGE("Cell form id %08X", parentCell->formID);
		_MESSAGE("Cell form id %08X", parentCell->formID);
		int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
		_MESSAGE("Num Ref: %i", numberofRefs);
		SpawnMonstersInCell(1, 0x06001DFC, parentCell);
	}
}
*/

}