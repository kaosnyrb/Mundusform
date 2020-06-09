#include "BoundingBoxs.h"
#include <Undaunted\ConfigUtils.h>

namespace Undaunted {
	BoundingBoxList* BoundingBoxList::AddItem(BoundingBox item)
	{
		BoundingBoxList* currentlist = this;
		BoundingBoxList newlist = BoundingBoxList();
		newlist.length = currentlist->length + 1;
		newlist.data = new BoundingBox[newlist.length];
		for (int i = 0; i < currentlist->length; i++)
		{
			newlist.data[i] = currentlist->data[i];
		}
		newlist.data[currentlist->length] = item;
		currentlist->data = newlist.data;
		currentlist->length = newlist.length;
		return currentlist;
	}

	bool BoundingBoxList::Intersects(BoundingBox item)
	{
		BoundingBoxList* currentlist = this;
		if (item.height == 0 && item.width == 0) return false;
		int boundboxesEnabled = GetConfigValueInt("BoundingBoxCheck");
		if (boundboxesEnabled == 0) return false;

		bool intersects = false;
		for (int i = 0; i < currentlist->length; i++)
		{
			if (currentlist->data[i].position.x < item.position.x + item.width &&
				currentlist->data[i].position.x + currentlist->data[i].width > item.position.x &&
				currentlist->data[i].position.y < item.position.y + item.height &&
				currentlist->data[i].position.y + currentlist->data[i].height > item.position.y) {
				_MESSAGE("Collision!: %f,%f : %f,%f", currentlist->data[i].position.x, currentlist->data[i].position.y, item.position.x, item.position.y);
				intersects = true;
			}
			else
			{
				_MESSAGE("No Collision: %f,%f : %f,%f", currentlist->data[i].position.x, currentlist->data[i].position.y, item.position.x, item.position.y);
			}
		}

		return intersects;
	}

}
