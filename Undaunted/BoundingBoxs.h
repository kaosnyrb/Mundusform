#include <skse64\GameForms.h>
namespace Undaunted
{
#ifndef BoundingBoxesdef
#define BoundingBoxesdef
	class BoundingBox {
	public:
		Vector2 position;
		float width;
		float height;
		BoundingBox(Vector2 _pos,  float _width, float _height)
		{
			position = _pos;
			width = _width;
			height = _height;
		}
		BoundingBox(){}
	};

	class BoundingBoxList {
	public:
		BoundingBox* data;
		int length;
		BoundingBoxList* AddItem(BoundingBox item);
		bool Intersects(BoundingBox item);
	};

#endif
}