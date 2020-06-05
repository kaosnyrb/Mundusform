namespace Undaunted
{
#ifndef Intlistdef
#define Intlistdef
	class IntList {
	public:
		UInt32* data;
		int length;
		IntList* AddItem(UInt32 item);
	};

#endif
}