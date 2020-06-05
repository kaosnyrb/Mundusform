#include "WorldCellList.h"
namespace Undaunted
{
	WorldCellList* Undaunted::WorldCellList::AddItem(WorldCell item)
	{
		WorldCellList* currentlist = this;
		WorldCellList newlist = WorldCellList();
		newlist.length = currentlist->length + 1;
		newlist.data = new WorldCell[newlist.length];
		for (int i = 0; i < currentlist->length; i++)
		{
			newlist.data[i] = currentlist->data[i];
		}
		newlist.data[currentlist->length] = item;
		currentlist->data = newlist.data;
		currentlist->length = newlist.length;
		return currentlist;
	}
}