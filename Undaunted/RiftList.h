#include "SKSELink.h"
#include <Undaunted\FormRefList.h>

namespace Undaunted
{
#ifndef RiftListdef
#define RiftListdef
	class RiftRef {
	public:
		FormRefList reflist;
	};

	class RiftList {
	public:
		RiftRef* data;
		int length;
		RiftList* AddItem(RiftRef item);
	};
#endif
}