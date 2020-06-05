#include "SKSELink.h"

namespace Undaunted
{
#ifndef FormRefListdef
#define FormRefListdef
	class FormRef {
	public:
		FormType type;
		UInt32 formId;
		NiPoint3 pos;
		NiPoint3 rot;
		int scale;
	};

	class FormRefList {
	public:
		FormRef* data;
		int length;
		FormRefList* AddItem(FormRef item);
	};
#endif
}