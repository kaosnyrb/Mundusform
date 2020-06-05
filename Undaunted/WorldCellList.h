#include "SKSELink.h"

namespace Undaunted
{
#ifndef WorldCellListdef
#define WorldCellListdef
	class WorldCell {
	public:
		TESWorldSpace* world;
		TESObjectCELL* cell;
	};

	class WorldCellList {
	public:
		WorldCell* data;
		int length;
		WorldCellList* AddItem(WorldCell item);
	};
#endif
}