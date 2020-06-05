#include "WorldCellList.h"
#include <Undaunted\FormRefList.h>

namespace Undaunted
{
	TESObjectREFR* GetRefObjectInCurrentCell(UInt32 formID);
	//TESObjectREFR* GetRefObjectFromWorld(UInt32 formID);
	TESObjectREFR* GetRandomObjectInCell(WorldCell worldcell);
	void BuildWorldList();
	WorldCell GetRandomWorldCell();
	WorldCell GetNamedWorldCell(BSFixedString WorldspaceName);
	void MoveRefToWorldCell(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot);
	WorldCell GetWorldCellFromRef(TESObjectREFR* object);

	void CaptureArea();
	void AddRift(FormRefList reflist);
	FormRefList GetRandomRift();

	void InitBakedRiftStartMarkers();
	TESObjectREFR* GetRandomBakedRiftStartMarker();

}