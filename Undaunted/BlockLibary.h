#include <Undaunted\FormRefList.h>
#include <Undaunted\NavmeshTool.h>
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