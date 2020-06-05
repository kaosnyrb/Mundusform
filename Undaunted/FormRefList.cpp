#include "FormRefList.h"
namespace Undaunted
{
	FormRefList* FormRefList::AddItem(FormRef item)
	{
		FormRefList* currentlist = this;
		FormRefList newlist = FormRefList();
		newlist.length = currentlist->length + 1;
		newlist.data = new FormRef[newlist.length];
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