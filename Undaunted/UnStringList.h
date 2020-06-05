
namespace Undaunted
{
#ifndef UnDictionaryListdef
#define UnDictionaryListdef
	class UnKeyValue {
	public:
		std::string key;
		std::string value;
	};

	class UnDictionary {
	public:
		UnKeyValue* data;
		int length;
		UnDictionary* AddItem(UnKeyValue item);
	};

	class UnStringlist {
	public:
		std::string* data;
		int length;
		UnStringlist* AddItem(std::string item);
	};

#endif
}