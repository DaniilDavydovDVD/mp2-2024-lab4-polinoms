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
	mutable T_DNode<T>* pCur;
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
	T_DList(T_DList<T> const& _Lst, size_t count)	// Copy first count nodes, but not more than size of _Lst
	{
		if (count > _Lst.size)
			count = _Lst.size;
		Head = new Base_DNode;
		size = 0;
		pCur = _Lst.Head->next;
		for (size_t i = 0; i < count; i++)
		{
			push_back(pCur->val);
			GoNext();
		}
		pCur = Head->next;
	}
	T_DList(size_t count, T_DList<T> const& _Lst)	// Copy last count nodes, but not more than size of _Lst
	{
		if (count > _Lst.size)
			count = _Lst.size;
		Head = new Base_DNode;
		size = 0;
		pCur = _Lst.Head->prev;
		for (size_t i = 0; i < count; i++)
		{
			push_front(pCur->val);
			GoPrev();
		}
		pCur = Head->next;
	}
	T_DList(T_DList<T> const& _Lst, size_t pos, size_t count)	// Copy count nodes starting from pos
	{
		if (pos + count > size)
			throw std::overflow_error("Out of range");
		Head = new Base_DNode;
		size = 0;
		T_DNode* tmp = _Lst.pCur;
		_Lst.GoTo(pos);
		for (size_t i = 0; i < count; i++)
		{
			push_back(pCur->val);
			_Lst.GoNext();
		}
		_Lst.pCur = tmp;
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

	size_t GetSize() const
	{
		return size;
	}

	// Adding Nodes
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
	void insert(T const& _val, size_t pos) noexcept
	{
		GoTo(pos);
		new T_DNode<T>(pCur, pCur->next, _val);
		size++;
	}
	void insert_at(T const& _val, size_t pos)
	{
		GoTo(pos);
		new T_DNode<T>(pCur, pCur->next, _val);
		size++;
	}

	// Deleting Nodes
	void pop_back()
	{
		delete Head->prev;
		size--;
		return;
	}
	void pop_backs()
	{
		if (Head->prev != Head)
		{
			delete Head->prev;
			size--;
		}
		else throw std::logic_error("Can't delete Head");
		return;
	}
	void pop_front()
	{
		delete Head->next;
		size--;
		return;
	}
	void pop_fronts()
	{
		if (Head->next != Head)
		{
			delete Head->next;
			size--;
		}
		else throw std::logic_error("Can't delete Head");
		return;
	}
	void exclud_next()
	{
		delete pCur->next;
		size--;
		return;
	}
	void exclude_nexts()
	{
		if (pCur != Head)
		{
			delete pCur->next;
			size--;
		}
		else throw std::logic_error("Can't delete Head");
		return;
	}
	void exclud_prev()
	{
		delete pCur->prev;
		size--;
		return;
	}
	void exclude_prevs()
	{
		if (pCur != Head)
		{
			delete pCur->prev;
			size--;
		}
		else throw std::logic_error("Can't delete Head");
		return;
	}
	void exclude(size_t pos)
	{
		GoTo(pos);
		delete pCur;
		size--;
		GoFirst();
		return;
	}
	void exclude_at(size_t pos)
	{
		GoAt(pos);
		delete pCur;
		size--;
		GoFirst();
		return;
	}
	void clear()
	{
		while (Head->next != Head)
		{
			delete Head->next;
			size--;
		}
		return;
	}

	//Navigation
	void GoHead() const noexcept
	{
		pCur = Head;
		return;
	}
	void GoFirst() const noexcept
	{
		pCur = Head->next;
		return;
	}
	void GoLast() const noexcept
	{
		pCur = Head->prev;
		return;
	}
	void GoNext() const noexcept
	{
		pCur = pCur->next;
		return;
	}
	void GoPrev() const noexcept
	{
		pCur = pCur->prev;
		return;
	}
	void GoTo(size_t pos) const noexcept
	{
		if (pos <= size - pos)
		{
			GoHead();
			for (size_t i = 0; i < pos; i++)
				GoNext();
		}
		else
		{
			GoLast();
			pos = size - pos;
			for (size_t i = 0; i < pos; i++)
				GoPrev();
		}
		return;
	}
	void GoAt(size_t pos) const
	{
		if (pos >= size)
			throw std::overflow_error("Out of range");
		if (pos <= size - pos)
		{
			GoHead();
			for (size_t i = 0; i < pos; i++)
				GoNext();
		}
		else
		{
			GoLast();
			pos = size - pos;
			for (size_t i = 0; i < pos; i++)
				GoPrev();
		}
		return;
	}

	// Access to values
	T& GetCurVal() noexcept
	{
		return pCur->val;
	}
	T const& GetCurVal() const noexcept
	{
		return pCur->val;
	}
	T& AtCurVal()
	{
		if (pCur == Head)
			throw std::underflow_error("Can't access value of List Head");
		return pCur->val;
	}
	T const& AtCurVal() const
	{
		if (pCur == Head)
			throw std::underflow_error("Can't access value of List Head");
		return pCur->val;
	}

	// Indexed access
	T& operator[] (size_t pos) noexcept
	{
		GoTo(pos);
		return  GetCurVal();
	}
	T const& operator[] (size_t pos) const noexcept
	{
		GoTo(pos);
		return  GetCurVal();
	}
	T& at(size_t pos)
	{
		GoAt(pos);
		return  GetCurVal();
	}
	T const& at(size_t pos) const
	{
		GoAt(pos);
		return  GetCurVal();
	}

	static T_DList sorted_mrege(T_DList& const lhs, T_DList const& rhs)
	{
		T_DList Res;
		T_DNode<T> P1 = lhs.Head->next, P2 = rhs.Head->next;
		Base_DNode const H1 = lhs.Head, H2 = rhs.Head;
		while (P1 != H1 && P2 != H2)
		{
			if (P1->val <= P2->val)
			{
				Res.push_back(P1->val);
				P1 = P1->next;
			}
			else
			{
				Res.push_back(P2->val);
				P2 = P2->next;
			}
		}
		while (P1 != H1)
		{
			Res.push_back(P1->val);
			P1 = P1->next;
		}
		while (P2 != H2)
		{
			Res.push_back(P2->val);
			P2 = P2->next;
		}
		return Res;
	}

	static T_DList sorted_merge(T_DList&& lhs, T_DList&& rhs)
	{
		if (lhs.size < rhs.size)
			std::swap(lhs, rhs);
		T_DList Res(lhs);
		T_DNode<T> P1 = Res.Head->next, P2 = rhs.Head->next;
		Base_DNode const H1 = Res.Head, H2 = rhs.Head;
		while (P1 != H1 && P2 != H2)
		{
			if (P1->val <= P2->val)
			{
				P1 = P1->next;
			}
			else
			{
				P1->prev->next = P2;
				P2->prev = P1->prev;
				P1->prev = P2;
				P2 = P2->next;
				P2->prev->next = P1;
			}

		}
		while (P2 != H2)
		{
			H1->prev->next = P2;
			P2->prev = H1->prev;
			H1->prev = P2;
			P2 = P2->next;
			P2->prev->next = H1;
		}
		rhs.Head->prev = rhs.Head->next = rhs.Head;
		return Res;
	}
};