#include "RiftList.h"
namespace Undaunted
{
	RiftList* RiftList::AddItem(RiftRef item)
	{
		RiftList* currentlist = this;
		RiftList newlist = RiftList();
		newlist.length = currentlist->length + 1;
		newlist.data = new RiftRef[newlist.length];
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