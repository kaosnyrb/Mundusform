#include <skse64\GameForms.h>
namespace Undaunted
{
#ifndef RefListdef
#define RefListdef
	class Ref {
	public:
		TESObjectREFR* objectRef;
	};

	class RefList {
	public:
		Ref* data;
		int length;
		RefList* AddItem(Ref item);
		RefList* SwapItem(int first, int second);
	};

#endif
}