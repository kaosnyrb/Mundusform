#include "UnStringList.h"
namespace Undaunted
{
	UnDictionary* Undaunted::UnDictionary::AddItem(UnKeyValue item)
	{
		UnDictionary* currentlist = this;
		UnDictionary newlist = UnDictionary();
		newlist.length = currentlist->length + 1;
		newlist.data = new UnKeyValue[newlist.length];
		for (int i = 0; i < currentlist->length; i++)
		{
			newlist.data[i] = currentlist->data[i];
		}
		newlist.data[currentlist->length] = item;
		currentlist->data = newlist.data;
		currentlist->length = newlist.length;
		return currentlist;
	}

	UnStringlist* UnStringlist::AddItem(std::string item)
	{
		UnStringlist* currentlist = this;
		UnStringlist newlist = UnStringlist();
		newlist.length = currentlist->length + 1;
		newlist.data = new std::string[newlist.length];
		for (int i = 0; i < currentlist->length; i++)
		{
			newlist.data[i] = currentlist->data[i];
		}
		newlist.data[currentlist->length] = item;
		currentlist->data = newlist.data;
		currentlist->length = newlist.length;
		return currentlist;
	}
}