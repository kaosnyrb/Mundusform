#include <Undaunted\FormRefList.h>
#include <Undaunted\NavmeshTool.h>
#include <Undaunted\BoundingBoxs.h>
namespace Undaunted
{
#ifndef BlockLibarydef
#define BlockLibarydef
	class Block {
	public:
		FormRefList reflist;
		TileList navlist;
		TileList exitslist;
		Tile enterancetile;
		BoundingBox boundingbox;
		std::string type;
		void RotateAroundPivot(Vector3 pivot, float angle);
	};

	class BlockLibary {
	public:
		Block* data;
		int length;
		BlockLibary* AddItem(Block item);
		BlockLibary* SwapItem(int first, int second);
	};

#endif
}