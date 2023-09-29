
#ifndef _PNODE
#define _PNODE
template < typename T>
class PNode
{
private:
	T item; // A data item
	PNode<T>* next; // Pointer to next node
	int priority;
public:
	PNode();
	PNode(const T& r_Item, const int& prio);
	PNode(const T& r_Item, PNode<T>* nextNodePtr, const int& prio);
	void setItem(const T& r_Item);
	void setNext(PNode<T>* nextNodePtr);
	void setPriority(const int& prio);
	T getItem() const;
	PNode<T>* getNext() const;
	int getPriority() const;
	void Print() const;
}; // end Node
#endif

template < typename T>
PNode<T>::PNode() : priority(0)
{
	next = nullptr;
}

template < typename T>
PNode<T>::PNode(const T& r_Item, const int& prio)
{
	item = r_Item;
	next = nullptr;
	priority = prio;
}

template < typename T>
PNode<T>::PNode(const T& r_Item, PNode<T>* nextNodePtr, const int& prio)
{
	item = r_Item;
	next = nextNodePtr;
	priority = prio;
}
template < typename T>
void PNode<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void PNode<T>::setNext(PNode<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
void PNode<T>::setPriority(const int& prio)
{
	priority = prio;
}

template < typename T>
T PNode<T>::getItem() const
{
	return item;
}

template < typename T>
PNode<T>* PNode<T>::getNext() const
{
	return next;
}

template < typename T>
int PNode<T>::getPriority() const
{
	return priority;
}
