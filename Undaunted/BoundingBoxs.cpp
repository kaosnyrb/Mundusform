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
			BoundingBox box1 = item;
			BoundingBox box2 = currentlist->data[i];
			// collision x-axis?
			bool collisionX = box1.position.x + box1.width > box2.position.x &&
				box2.position.x + box2.width > box1.position.x;
			// collision y-axis?
			bool collisionY = box1.position.y + box1.height > box2.position.y &&
				box2.position.y + box2.height > box1.position.y;

			if (collisionX && collisionY)
			{
				_MESSAGE("Collision!: %f,%f,%f,%f : %f,%f,%f,%f", currentlist->data[i].position.x, currentlist->data[i].position.y, currentlist->data[i].width, currentlist->data[i].height, item.position.x, item.position.y, item.width, item.height);
				intersects = true;
			}
			else
			{
				_MESSAGE("No Collision: %f,%f,%f,%f : %f,%f,%f,%f", currentlist->data[i].position.x, currentlist->data[i].position.y, currentlist->data[i].width, currentlist->data[i].height, item.position.x, item.position.y, item.width, item.height);
			}
		}

		return intersects;
	}

}
