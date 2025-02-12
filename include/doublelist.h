#pragma once
#include <utility>
#include <stdexcept>
struct Base_DNode
{
	Base_DNode* next;
	Base_DNode* prev;
	Base_DNode() : next(this), prev(this) {};
	Base_DNode(Base_DNode* _prev, Base_DNode* _next) : next(_next), prev(_prev)
	{
		next->prev = prev->next = this;
	}
	~Base_DNode()
	{
		next->prev = prev;
		prev->next = next;
	}
};

template <typename T>
struct T_DNode : public Base_DNode
{
	T val;
	T_DNode() : Base_DNode(), val() {};
	T_DNode(Base_DNode* _prev, Base_DNode* _next, T const& _val) : Base_DNode(_prev, _next), val(_val) {};
};

template <typename T>
class T_DList
{
private:
	Base_DNode* Head;
	T_DNode<T>* pCur;
	size_t size;
public:
	T_DList()
	{
		Head = new Base_DNode;
		size = 0;
	}
	T_DList(T const& _val)
	{
		Head = new Base_DNode;
		push_back(_val);
		size = 1;
	}
	T_DList(T const* arr, size_t sz)
	{
		Head = new Base_DNode;
		size = 0;
		for (size_t i = 0; i < sz; i++)
			push_back(arr[i]);
	}
	T_DList(T_DList<T> const& _Lst)
	{
		Head = new Base_DNode;
		size = 0;
		for (pCur = _Lst.Head->next; pCur != _Lst.Head; pCur = pCur->next)
			push_back(pCur->val);
		pCur = Head->next;
	}
	T_DList(T_DList<T>&& _Lst)
	{
		Head = new Base_DNode;
		swap(*this, _Lst);
	}
	~T_DList()
	{
		while (Head->next != Head)
			delete Head->next;
	}

	friend void swap(T_DList<T>& lhs, T_DList<T>& rhs)
	{
		std::swap(lhs.Head, rhs.Head);
		return;
	}

	size_t GetSize()
	{
		return size;
	}
	
	void push_back(T const& _val)
	{
		new T_DNode<T>(Head->prev, Head, _val);
		size++;
		return;
	}

	void push_front(T const& _val)
	{
		new T_DNode<T>(Head, Head->next, _val);
		size++;
		return;
	}

	void GoFirst() noexcept
	{
		pCur = Head->next;
		return;
	}
	void GoLast() noexcept
	{
		pCur = Head->prev;
		return;
	}
	void GoNext() noexcept
	{
		pCur = pCur->next;
		return;
	}
	void GoPrev() noexcept
	{
		pCur = pCur->prev;
		return;
	}
	T& GetCurVal() noexcept
	{
		return pCur->val;
	}
	T& AtCurVal()
	{
		if (pCur == Head)
			throw std::underflow_error("Can't access value of List Head");
		return pCur->val;
	}

	T& operator[] (size_t pos) noexcept
	{
		GoFirst();
		for (size_t i = 0; i < pos; i++)
			GoNext();
		return  GetCurVal();
	}
	T& at (size_t pos)
	{
		GoFirst();
		for (size_t i = 0; i < pos; i++)
			GoNext();
		return  AtCurVal();
	}

};