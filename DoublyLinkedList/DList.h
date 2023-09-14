#include <iostream>

// Let it be known that I hate using regions,
// but this was for a course (the prof made me do this)
// and I wanted to make this easier to read soooo... yeah

template<typename Type>
class DList {

#pragma region PrivateMembers

private:

	struct Node {
		// NOTE: Values set to -1 for unit test purposes
		Type data;
		Node* next = reinterpret_cast<Node*>(-1);
		Node* prev = reinterpret_cast<Node*>(-1);

		Node(const Type& _data, Node* _next = nullptr, Node* _prev = nullptr) {
			this->data = _data;
			if (_next == nullptr)
			{
				this->next = NULL;
			}
			else
			{
				this->next = _next;
			}

			if (_prev == nullptr)
			{
				this->prev = NULL;
			}
			else
			{
				this->prev = _prev;
			}
		}
	};

	// Optional recursive helper method for use with Rule of 3
	//
	// In:	_curr		The current Node to copy
	void RecursiveCopy(const Node* _curr) {
		Node* prev = nullptr;

		if (_curr->next != NULL)
		{
			if (this->mHead == NULL) // list is empty
			{
				this->mHead = new Node(_curr->data);
				prev = this->mHead;
				++mSize;
			}
			else
			{
				prev = mTail;
			}

			mTail = new Node(_curr->next->data);
			mTail->prev = prev;
			prev->next = mTail;
			RecursiveCopy(_curr->next);
			++mSize;
		}
		return;
	}

	// Optional recursive helper method for use with Clear
	// 
	// In:	_curr		The current Node to clear
	void RecursiveClear(const Node* _curr) {
		if (_curr != NULL)
		{
			Node* next = _curr->next;
			RecursiveClear(next);
			delete _curr;
			_curr = NULL;
			--mSize;
		}
		return;
	}

#pragma endregion

public:

	class Iterator {
	public:

		Node* mCurr;

		// Pre-fix increment operator
		//
		// Return: Invoking object with curr pointing to next node
		// 
		// Example:
		//		I - Iterator's curr
		//		R - Return
		/*
			Before

			0<-[4]<->[5]<->[6]->0
					  I

			After

			0<-[4]<->[5]<->[6]->0
							I
							R
		*/
		Iterator& operator++() {
			mCurr = mCurr->next;
			return *this;
		}

		// Post-fix increment operator
		//
		// In:	(unused)		Post-fix operators take in an unused int, so that the compiler can differentiate
		//
		// Return:	An iterator that has its curr pointing to the "old" curr
		// NOTE:	Will need a temporary iterator 
		//
		// Example:
		//		I - Iterator's curr
		//		R - Return
		/*
			Before

			0<-[4]<->[5]<->[6]->0
					  I

			After

			0<-[4]<->[5]<->[6]->0
					  R		I

		*/
		Iterator operator++(int) {

			Iterator val{};
			val.mCurr = this->mCurr;
			this->mCurr = this->mCurr->next;
			return val;
		}

		// Pre-fix decrement operator
		//
		// Return: Invoking object with curr pointing to previous node
		//
		// Example:
		//		I - Iterator's curr
		//		R - Return
		/*
			Before

			0<-[4]<->[5]<->[6]->0
					  I

			After

			0<-[4]<->[5]<->[6]->0
				I
				R
		*/
		Iterator& operator--() {

			this->mCurr = this->mCurr->prev;
			return *this;
		}

		// Post-fix decrement operator
		//
		// In:	(unused)		Post-fix operators take in an unused int, so that the compiler can differentiate
		//
		// Return:	An iterator that has its curr pointing to the "old" curr
		// NOTE:	Will need a temporary iterator 
		//
		// Example:
		//		I - Iterator's curr
		//		R - Return
		/*
			Before

			0<-[4]<->[5]<->[6]->0
					  I

			After

			0<-[4]<->[5]<->[6]->0
				I	  R

		*/
		Iterator operator--(int) {
			Iterator val{};
			val.mCurr = this->mCurr;
			this->mCurr = this->mCurr->prev;
			return val;
		}

		// Dereference operator
		//
		// Return: The data the curr is pointing to
		Type& operator*() {
			return this->mCurr->data;
		}

		// Not-equal operator (used for testing)
		// 
		// In:	_iter		The iterator to compare against
		//
		// Return: True, if the iterators are not pointing to the same node
		bool operator != (const Iterator& _iter) const {
			return mCurr != _iter.mCurr;
		}
	};

	// Data members
	// NOTE: All values set to -1 for unit test purposes
	Node* mHead = reinterpret_cast<Node*>(-1);
	Node* mTail = reinterpret_cast<Node*>(-1);
	size_t mSize = -1;

	// Default constructor
	//		Creates a new empty linked list
	DList() {
		mHead = NULL;
		mTail = NULL;
		mSize = 0;
	}

	// Destructor
	//		Cleans up all dynamically allocated memory
	~DList() {
		this->Clear();
	}

	// Copy constructor
	//		Used to initialize one object to another
	// In:	_copy			The object to copy from
	DList(const DList& _copy) {
		this->mHead = NULL;
		this->mTail = NULL;
		this->mSize = 0;
		*this = _copy;
	}

	// Assignment operator
	//		Used to assign one object to another
	// In:	_assign			The object to assign from
	//
	// Return: The invoking object (by reference)
	//		This allows us to daisy-chain
	DList& operator=(const DList& _assign) {
		if (this != &_assign)
		{
			if (this->mHead != NULL)
				this->Clear();
			RecursiveCopy(_assign.mHead);
		}

		return *this;
	}

	// Add a piece of data to the front of the list
	//
	// In:	_data			The object to add to the list
	void AddHead(const Type& _data) {
		Node* temp = new Node(_data);

		if (mHead == NULL)
		{
			mHead = temp;
			mTail = mHead;
			++mSize;
			return;
		}

		temp->data = _data;
		mHead->prev = temp;
		temp->next = mHead;
		temp->prev = NULL;
		mHead = temp;
		++mSize;
	}

	// Add a piece of data to the end of the list
	//
	// In:	_data			The object to add to the list
	void AddTail(const Type& _data) {
		Node* temp = new Node(_data);

		if (mTail == NULL)
		{
			mTail = temp;
			mHead = mTail;
			++mSize;
			return;
		}

		mTail->next = temp;
		temp->prev = mTail;
		temp->next = NULL;
		mTail = temp;
		++mSize;
	}

	// Clear the list of all dynamic memory
	//			Resets the list to its default state
	void Clear() {
		RecursiveClear(this->mHead);
		mHead = NULL;
		mTail = NULL;
	}

	void print()
	{
		auto temp = mHead;

		while (temp != NULL)
		{
			std::cout << temp->data << ' ';
			temp = temp->next;
		}
		std::cout << std::endl;
	}

#pragma region IteratorMethods

	// Insert a piece of data *before* the passed-in iterator
	//
	// In:	_iter		The iterator
	//		_data		The value to add
	//
	// Example:
	/*
		Before

			0<-[4]<->[5]<->[6]->0
					  I

		After

			0<-[4]<->[9]<->[5]<->[6]->0
					  I
	*/
	// Return:	The iterator
	// SPECIAL CASE:	Inserting at head or empty list
	// NOTE:	The iterator should now be pointing to the new node created
	Iterator Insert(Iterator& _iter, const Type& _data) {
		Node* temp = new Node(_data);
		Node* before = _iter.mCurr;

		if (this->mHead == NULL)
		{
			this->mHead = temp;
			mTail = this->mHead;
			this->mHead->next = NULL;
			_iter.mCurr = this->mHead;
		}

		else if (before == this->mHead)
		{
			temp->next = before;
			before->prev = temp;
			temp->prev = NULL;
			this->mHead = temp;
			_iter.mCurr = temp;
		}
		else
		{
			temp->next = before;
			temp->prev = before->prev;
			before->prev = temp;
			temp->prev->next = temp;
			_iter.mCurr = temp;
		}
		++mSize;
		return _iter;
	}

	// Erase a Node from the list
	//
	// In:	_iter		The iterator
	//
	// Example
	/*

		Before

			0<-[4]<->[5]<->[6]->0
					  I

		After

			0<-[4]<->[6]->0
					  I
	*/
	// Return:	The iterator
	// SPECIAL CASE:	Erasing head or tail
	// NOTE:	The iterator should now be pointing at the node after the one erased
	Iterator Erase(Iterator& _iter) {
		if (mSize == 0) { return _iter; }
		Node* temp = NULL;
		if (_iter.mCurr == this->mHead)
		{
			temp = mHead;
			mHead = mHead->next;
			delete temp;
			mHead->prev = NULL;
			_iter.mCurr = mHead;
		}
		else if (_iter.mCurr == this->mTail)
		{
			temp = mTail;
			mTail = mTail->prev;
			delete temp;
			mTail->next = NULL;
			_iter.mCurr = nullptr;
		}
		else
		{
			temp = _iter.mCurr;
			_iter.mCurr = temp->next;
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			delete temp;
		}
		--mSize;
		return _iter;
	}

	// Set an Iterator at the front of the list
	// 
	// Return: An iterator that has its curr pointing to the list's head
	Iterator Begin() const {
		Iterator val{};
		val.mCurr = this->mHead;
		return val;
	}

	// Set an Iterator pointing to the end of the list
	// 
	// Return: An iterator that has its curr pointing to a null pointer
	Iterator End() const {
		Iterator val{};
		val.mCurr = nullptr;
		return val;
	}
};
#pragma endregion