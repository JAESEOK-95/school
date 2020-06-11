#ifndef _CLINKEDLIST_H
#define _CLINKEDLIST_H


template <typename T>
class CNode
{
	T	mData;
	CNode<T>*	mNext;

public:

	CNode()
	{
		memset(&mData, 0, sizeof(mData));
		mNext = nullptr;
	}

	CNode(T _data) : mData(_data), mNext(nullptr)
	{
	}
	~CNode()
	{
	}

	T GetmData()
	{
		return mData;
	}

	CNode<T>*  GetmNext()
	{
		return mNext;
	}

	void SetmData(T _data)
	{
		mData=_data;
	}
	void SetmNext(CNode<T>* _next)
	{
		mNext=_next;
	}
	
};


template<typename T>
class CLinkedList
{
	CNode<T>*	mHead;
	CNode<T>*	mTail;

public:
	CLinkedList();
	~CLinkedList();

	CNode<T>*  GetmHead()
	{
		return mHead; 
	}
	CNode<T>*  GetmTail()
	{ 
		return mTail; 
	}

	bool	Insert(T  _data);
	bool	Delete(T  _data);
};

template <typename T>
CLinkedList<T>::CLinkedList()
{
	mHead = new CNode<T>();
	mTail = mHead;
}


template <typename T>
CLinkedList<T>::~CLinkedList()
{
	CNode<T>* ptr = mHead->GetmNext();
	delete mHead;

	while (ptr != nullptr)
	{
		mHead = ptr;
		ptr = ptr->GetmNext();
		delete mHead;
	}
}

template <typename T>
bool CLinkedList<T>::Insert(T _data)
{
	CNode<T>* ptr = new CNode<T>(_data);
	mTail->SetmNext(ptr);
	mTail = ptr;
	return true;
}

template <typename T>
bool CLinkedList<T>::Delete(T _data)
{
	CNode<T>* pre = mHead;
	CNode<T>* ptr = pre->GetmNext();
	bool flag = false;

	while (ptr != nullptr)
	{
		if (ptr->GetmData() == _data)
		{
			flag = true;
			pre->SetmNext(ptr->GetmNext());
			if (ptr->GetmNext() == nullptr)
			{
				mTail = pre;
			}
			delete ptr;
			break;
		}

		pre = ptr;
		ptr = ptr->GetmNext();

	}

	if (flag)
	{
		return true;
	}

	return false;

}







#endif