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

	BlockLibary* BlockLibary::SwapItem(int first, int second)
	{
		Block First = this->data[first];
		Block Second = this->data[second];

		this->data[first] = Second;
		this->data[second] = First;
		return this;
	}

	NiPoint3 Rotate(Vector3 pivot, NiPoint3 p, float angle)
	{
		//_MESSAGE("Degree angle: %f", angle);
		angle = angle * (3.141592653589793238463 / 180.0);
		float s = sin(angle);
		float c = cos(angle);

		//_MESSAGE("Rad angle: %f", angle);
		//_MESSAGE("before p x/y: %f %f", p.x, p.y);

		// translate point back to origin:
		p.x -= pivot.x;
		p.y -= pivot.y;

		// rotate point
		float xnew = p.x * c - p.y * s;
		float ynew = p.x * s + p.y * c;

		// translate point back:
		p.x = xnew + pivot.x;
		p.y = ynew + pivot.y;

		//_MESSAGE("after p x/y: %f %f", p.x, p.y);

		return p;
	}

	void Block::RotateAroundPivot(Vector3 pivot, float angle)
	{
		for (int i = 0; i < reflist.length; i++)
		{
			reflist.data[i].pos = Rotate(pivot, reflist.data[i].pos, -angle);
			reflist.data[i].rot.z += angle;
		}
		for (int i = 0; i < exitslist.length; i++)
		{
			NiPoint3 exitpos = NiPoint3(exitslist.data[i].x, exitslist.data[i].y, exitslist.data[i].z);
			exitpos = Rotate(pivot, exitpos, -angle);///hmmmmm
			exitslist.data[i].x = exitpos.x;
			exitslist.data[i].y = exitpos.y;
			exitslist.data[i].z = exitpos.z;
		}
		
		NiPoint3 bbp = Rotate(pivot, NiPoint3(boundingbox.position.x, boundingbox.position.y,0),-angle);
		boundingbox.position.x = bbp.x;
		boundingbox.position.y = bbp.y;

		NiPoint3 bbwh = Rotate(pivot, NiPoint3(boundingbox.width, boundingbox.height, 0),-angle);
		boundingbox.width = bbwh.x;
		boundingbox.height = bbwh.y;

		for (int i = 0; i < navlist.length; i++)
		{
			NiPoint3 exitpos = NiPoint3(navlist.data[i].x, navlist.data[i].y, navlist.data[i].z);
			exitpos = Rotate(pivot, exitpos, -angle);
			navlist.data[i].x = exitpos.x;
			navlist.data[i].y = exitpos.y;
			navlist.data[i].z = exitpos.z;
		}
	}
}