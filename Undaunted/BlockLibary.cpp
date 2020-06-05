#include "BlockLibary.h"
namespace Undaunted
{
	BlockLibary* Undaunted::BlockLibary::AddItem(Block item)
	{
		BlockLibary* currentlist = this;
		BlockLibary newlist = BlockLibary();
		newlist.length = currentlist->length + 1;
		newlist.data = new Block[newlist.length];
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