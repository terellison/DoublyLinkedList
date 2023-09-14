#include "DList.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	DList<int>* list = new DList<int>();

	list->AddTail(1);
	list->AddTail(2);
	list->AddTail(3);

	auto copyLst = new DList<int>(*list);

	list->print();
	copyLst->print();

	delete list, copyLst;

	_CrtDumpMemoryLeaks();

	return 0;
}