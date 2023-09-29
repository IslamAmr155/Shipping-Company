/** ADT list: Link-based implementation. Note: First Position = 1; ie index is 1, 2, 3, ....
@file LinkedList.h */
#pragma once
#ifndef _LINKED_LIST

#include "ListInterface.h"
#include "Node.h"
#include <iostream> //for Print function

using namespace std;
template < class ItemType>
class LinkedList : public ListInterface<ItemType>
{
protected:
	Node<ItemType>* headPtr; // Pointer to first node in the chain
	// (contains the first entry in the list)
	int itemCount; // Current count of list items
	// Locates a specified node in a linked list.
	// @pre position is the number of the desired node;
	// position >= 1 and position <= itemCount.
	// @post The node is found and a pointer to it is returned.
	// @param position The number of the node to locate.
	// @return A pointer to the node at the given position.
	Node<ItemType>* getNodeAt(int position) const;
public:
	LinkedList();
	LinkedList(const LinkedList<ItemType>& aList); //Copy constructor
	virtual ~LinkedList();
	bool isEmpty() const;
	int getLength() const;
	bool insert(int newPosition, const ItemType& newEntry);
	bool remove(int position);
	bool search(int key, ItemType& obj);
	void clear();
	/** return false if position < 1 or
	position > getLength(). */
	bool getEntry(int position, ItemType& Item) const;
	void Print() const;
}; // end LinkedList
#endif

template < class ItemType>
LinkedList<ItemType>::LinkedList() : headPtr(nullptr), itemCount(0)
{
}
// end default constructor

template < class ItemType>
LinkedList<ItemType>::LinkedList(const LinkedList<ItemType>& aList)
{
	itemCount = aList.getLength();
	Node<ItemType>* origChainPtr = aList.getNodeAt(1);
	if (origChainPtr == nullptr)
		headPtr = nullptr; // Original list is empty; so is copy
	else
	{
		// Copy first node
		headPtr = new Node<ItemType>();
		headPtr->setItem(origChainPtr->getItem());
		// Copy remaining nodes
		Node<ItemType>* newChainPtr = headPtr;
		// Last-node pointer
		while (origChainPtr->getNext() != nullptr)
		{
			origChainPtr = origChainPtr->getNext(); // Advance pointer
			// Get next item from original chain
			ItemType nextItem = origChainPtr->getItem();
			// Create a new node containing the next item
			Node<ItemType>* newNodePtr = new Node<ItemType>(nextItem);
			// Link new node to end of new chain
			newChainPtr->setNext(newNodePtr);
			// Advance pointer to new last node
			newChainPtr = newChainPtr->getNext();
		} // end while
		newChainPtr->setNext(nullptr); // Flag end of new chain
	} // end if
}
// end copy constructor

template < class ItemType>
bool LinkedList<ItemType>::getEntry(int position, ItemType& Item) const
{
	// Enforce precondition
	bool ableToGet = (position >= 1) && (position <= itemCount);
	if (ableToGet)
	{
		Node<ItemType>* nodePtr = getNodeAt(position);
		Item = nodePtr->getItem();
		return true;
	}
	else
	{
		return false;
	} // end if
}
// end getEntry

template < class ItemType>
Node<ItemType>* LinkedList<ItemType>::getNodeAt(int position) const
{
	// Debugging check of precondition
	if ((position >= 1) && (position <= itemCount))
	{
		// Count from the beginning of the chain
		Node<ItemType>* curPtr = headPtr;
		for (int skip = 1; skip < position; skip++)
			curPtr = curPtr->getNext();
		return curPtr;
	}
	else
		return nullptr;
}
// end getNodeAt

template < class ItemType>
bool LinkedList<ItemType>::insert(int newPosition, const ItemType& newEntry)
{
	bool ableToInsert = (newPosition >= 1) && (newPosition <= itemCount + 1);
	if (ableToInsert)
	{
		// Create a new node containing the new entry
		Node<ItemType>* newNodePtr = new Node<ItemType>(newEntry);
		if (!newNodePtr)
			//Heap overflow
			return false;
		// Attach new node to chain
		if (newPosition == 1)
		{
			// Insert new node at beginning of chain
			newNodePtr->setNext(headPtr);
			headPtr = newNodePtr;
		}
		else
		{
			// Find node that will be before new node
			Node<ItemType>* prevPtr = getNodeAt(newPosition - 1);
			// Insert new node after node to which prevPtr points
			newNodePtr->setNext(prevPtr->getNext());
			prevPtr->setNext(newNodePtr);
		} // end if
		itemCount++; // Increase count of entries
	} // end if
	return ableToInsert;
} // end insert

template < class ItemType>
bool LinkedList<ItemType>::remove(int position)
{
	bool ableToRemove = (position >= 1) && (position <= itemCount);
	if (ableToRemove)
	{
		Node<ItemType>* curPtr = nullptr;
		if (position == 1)
		{
			// Remove the first node in the chain
			curPtr = headPtr; // Save pointer to node
			headPtr = headPtr->getNext();
		}
		else
		{
			// Find node that is before the one to delete
			Node<ItemType>* prevPtr = getNodeAt(position - 1);
			// Point to node to delete
			curPtr = prevPtr->getNext();
			// Disconnect indicated node from chain by connecting the
			// prior node with the one after
			prevPtr->setNext(curPtr->getNext());
		} // end if
		// Return node to system
		curPtr->setNext(nullptr);
		delete curPtr;
		curPtr = nullptr;
		itemCount--; // Decrease count of entries
	} // end if
	return ableToRemove;
}
// end remove

template < class ItemType>
void LinkedList<ItemType>::clear()
{
	while (!isEmpty())
		remove(1);
}
// end clear

template < class ItemType>
LinkedList<ItemType>::~LinkedList()
{
	clear();
}
// end destructor

template < class ItemType>
bool LinkedList<ItemType>::isEmpty() const
{
	if (!itemCount)
		return true;
	return false;
}

template < class ItemType>
int LinkedList<ItemType>::getLength() const
{
	return itemCount;
}

template<class ItemType>
void LinkedList<ItemType>::Print() const
{
	ItemType Item;
	Node<ItemType>* ptr = headPtr;
	while (ptr)
	{
		Item = ptr->getItem();
		cout << Item;
		if (ptr->getNext())
		{
			cout << ", ";
		}
		ptr = ptr->getNext();
	}
}