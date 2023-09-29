#pragma once

#include "../Header Files/Cargo.h"
#include "LinkedList.h"

class LinkedNormal: public LinkedList<Cargo*>
{
public:
	int search(int key, Cargo*& item)
	{
		int i = 1;
		Node<Cargo*>* p = headPtr;
		while (p)
		{
			if (p->getItem()->getID() == key)
			{
				item = p->getItem();
				return i;
			}
			i++;
			p = p->getNext();
		}
		return -1;
	}
};
