#pragma once
#include <utility>
#include <stdexcept>
template <typename T>
class Base_DNode
{
protected:
	Base_DNode<T>* next;
	Base_DNode<T>* prev;
public:
	Base_DNode() : next(this), prev(this) {};
	virtual T const& GetVal() const = 0;
	virtual T& GetVal() = 0;
};

template <typename T>
class T_DNode : public Base_DNode<T>
{
protected:
	T val;
public:
	T_DNode() : Base_DNode<T>(), val() {};
	T_DNode(T const& _val) : Base_DNode<T>(), val(_val) {};
	T const& GetVal() const noexcept override { return val; }
	T& GetVal() noexcept override { return val; }
};

template <typename T>
class T_DList
{
private:
	Base_DNode<T>* Head;
	mutable T_DNode<T>* pCur;
	size_t size;
public:
	T_DList()
	{
		Head = new Base_DNode<T>;
		size = 0;
		GoHead();
	}
	T_DList(T const& _val)
	{
		Head = new Base_DNode<T>;
		push_back(_val);
		size = 1;
		GoHead();
	}
	T_DList(T const* arr, size_t sz)
	{
		Head = new Base_DNode<T>;
		size = 0;
		for (size_t i = 0; i < sz; i++)
			push_back(arr[i]);
		GoHead();
	}
	T_DList(T_DList<T> const& _Lst)
	{
		Head = new Base_DNode<T>;
		size = 0;
		for (pCur = _Lst.Head->next; pCur != _Lst.Head; pCur = pCur->next)
			push_back(pCur->GetVal());
		GoHead();
	}
	T_DList(T_DList<T>&& _Lst)
	{
		Head = new Base_DNode<T>;
		swap(*this, _Lst);
		GoHead();
	}
	T_DList& operator = (T_DList<T> const& _Lst)
	{
		clear();
		for (pCur = _Lst.Head->next; pCur != _Lst.Head; pCur = pCur->next)
			push_back(pCur->GetVal());
		GoHead();
		return this*;
	}
	T_DList& operator = (T_DList<T>&& _Lst)
	{
		swap(*this, _Lst);
		GoHead();
		return *this;
	}
	T_DList(T_DList<T> const& _Lst, size_t count)	// Copy first count nodes, but not more than size of _Lst
	{
		if (count > _Lst.size)
			count = _Lst.size;
		Head = new Base_DNode<T>;
		size = 0;
		pCur = _Lst.Head->next;
		for (size_t i = 0; i < count; i++)
		{
			push_back(pCur->GetVal());
			GoNext();
		}
		GoHead();
	}
	T_DList(size_t count, T_DList<T> const& _Lst)	// Copy last count nodes, but not more than size of _Lst
	{
		if (count > _Lst.size)
			count = _Lst.size;
		Head = new Base_DNode<T>;
		size = 0;
		pCur = _Lst.Head->prev;
		for (size_t i = 0; i < count; i++)
		{
			push_front(pCur->GetVal());
			GoPrev();
		}
		GoHead();
	}
	T_DList(T_DList<T> const& _Lst, size_t pos, size_t count)	// Copy count nodes starting from pos
	{
		if (pos + count > size)
			throw std::overflow_error("Out of range");
		size_t i = 0;
		Head = new Base_DNode<T>;
		size = 0;
		pCur = _Lst.Head->next;
		for (; i < pos; i++)
			GoNext();
		for (i = 0; i < count; i++)
		{
			push_back(pCur->GetVal());
			GoNext();
		}
		GoFirst();
	}

	~T_DList()
	{
		T_DNode<T>* tmp;
		while (Head->next != Head)
		{
			tmp = Head->next;
			Unbind(tmp);
			delete tmp;
		}
		delete Head;
	}

	friend void swap(T_DList<T>& lhs, T_DList<T>& rhs)
	{
		std::swap(lhs.Head, rhs.Head);
		std::swap(lhs.size, rhs.size);
		std::swap(lhs.pCur, rhs.pCur);
		return;
	}

	size_t GetSize() const
	{
		return size;
	}

	static void Bind(T_DNode<T>* node, T_DNode<T>* _prev, T_DNode<T>* _next)
	{
		node->next = _next;
		node->prev = _prev;
		_next->prev = _prev->next = this;
		return;
	}
	static void Unbind(T_DNode<T>* node)
	{
		node->next->prev = node->prev;
		node->prev->next = node->next;
		node->next = node->prev = this;
		return;
	}

	// Adding Nodes
	void push_back(T const& _val)
	{
		Bind(new T_DNode<T>(_val), Head->prev, Head);
		size++;
		return;
	}
	void push_front(T const& _val)
	{
		Bind(new T_DNode<T>(_val), Head, Head->next);
		size++;
		return;
	}
	void insert(T const& _val, size_t pos) noexcept
	{
		GoTo(pos);
		Bind(new T_DNode<T>(_val), pCur->prev, pCur);
		size++;
		return;
	}
	void insert_at(T const& _val, size_t pos)
	{
		GoAt(pos);
		Bind(new T_DNode<T>(_val), pCur->prev, pCur);
		size++;
		return;
	}

	// Deleting Nodes
	void pop_back() noexcept
	{
		T_DNode<T>* tmp = Head->prev;
		Unbind(tmp);
		delete tmp;
		GoFirst();
		size--;
		return;
	}
	void pop_backs()
	{
		if (Head->prev != Head)
		{
			T_DNode<T>* tmp = Head->prev;
			Unbind(tmp);
			delete tmp;
			GoFirst();
			size--;
		}
		else throw std::logic_error("Can't delete Head");
		return;
	}
	void pop_front() noexcept
	{
		T_DNode<T>* tmp = Head->next;
		Unbind(tmp);
		delete tmp;
		GoFirst();
		size--;
		return;
	}
	void pop_fronts()
	{
		if (Head->next != Head)
		{
			T_DNode<T>* tmp = Head->next;
			Unbind(tmp);
			delete tmp;
			GoFirst();
			size--;
		}
		else throw std::logic_error("Can't delete Head");
		return;
	}
	void exclude_next() noexcept
	{
		T_DNode<T>* tmp = pCur->next;
		Unbind(tmp);
		delete tmp;
		size--;
		return;
	}
	void exclude_nexts()
	{
		if (pCur->next != Head)
		{
			T_DNode<T>* tmp = pCur->next;
			Unbind(tmp);
			delete tmp;
			size--;
		}
		else throw std::logic_error("Can't delete Head");
		return;
	}
	void exclude_prev() noexcept
	{
		T_DNode<T>* tmp = pCur->prev;
		Unbind(tmp);
		delete tmp;
		size--;
		return;
	}
	void exclude_prevs()
	{
		if (pCur->prev != Head)
		{
			T_DNode<T>* tmp = pCur->prev;
			Unbind(tmp);
			delete tmp;
			size--;
		}
		else throw std::logic_error("Can't delete Head");
		return;
	}
	void exclude(size_t pos)
	{
		GoTo(pos);
		T_DNode<T>* tmp = pCur;
		Unbind(tmp);
		delete tmp;
		size--;
		GoFirst();
		return;
	}
	void exclude_at(size_t pos)
	{
		GoAt(pos);
		T_DNode<T>* tmp = pCur;
		Unbind(tmp);
		delete tmp;
		size--;
		GoFirst();
		return;
	}
	void clear()
	{
		T_DNode<T>* tmp;
		while (Head->next != Head)
		{
			tmp = Head->next;
			Unbind(tmp);
			delete tmp;
		}
		size = 0;
		GoHead();
		return;
	}

	//Navigation
	inline void GoHead() const noexcept
	{
		pCur = Head;
		return;
	}
	inline void GoFirst() const noexcept
	{
		pCur = Head->next;
		return;
	}
	inline void GoLast() const noexcept
	{
		pCur = Head->prev;
		return;
	}
	inline void GoNext() const noexcept
	{
		pCur = pCur->next;
		return;
	}
	inline void GoPrev() const noexcept
	{
		pCur = pCur->prev;
		return;
	}
	void GoShift(long long int a)
	{
		if (a >= 0)
			for (size_t i = 0; i < a; i++)
				GoNext();
		else
			for (size_t i = 0; i < -a; i++)
				GoPrev();
		return;
	}
	void GoTo(size_t pos) const noexcept
	{
		if (pos < size / 2)
		{
			GoFirst();
			for (size_t i=0;i < pos;i++)
				GoNext();
		}
		else
		{
			GoLast();
			for (size_t i = size-1; i > pos; i--)
				GoPrev();
		}
		return;
	}
	void GoAt(size_t pos) const
	{
		if (pos >= size)
			throw std::overflow_error("Out of range");
		GoTo(pos);
		return;
	}

	// Access to values
	T& GetCurVal() noexcept
	{
		return pCur->GetVal();
	}
	T const& GetCurVal() const noexcept
	{
		return pCur->GetVal();
	}
	T& AtCurVal()
	{
		if (pCur == Head)
			throw std::underflow_error("Can't access value of List Head");
		return pCur->GetVal();
	}
	T const& AtCurVal() const
	{
		if (pCur == Head)
			throw std::underflow_error("Can't access value of List Head");
		return pCur->GetVal();
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

	void GetSorted()
	{
		if (size > 1)
		{
			T_DNode<T>* lhs, * rhs, * tmp;
			size_t width = 1, i, j, lk, rk, pos = 0;
			while (width < size)
			{
				GoFirst();
				for (; pos + 2 * width <= size;)
				{
					lhs = pCur;
					GoShift(width);
					pos += width;
					rhs = pCur;
					lk = rk = 0;
					GoShift(width);
					pos += width;
					while (lk < width && rk < width)
					{
						if (lhs->val > rhs->val)
						{
							tmp = rhs->next;
							Unbind(rhs);
							Bind(rhs, lhs->prev, lhs);
							rhs = tmp;
							rk++;
						}
						else
						{
							lhs = lhs->next;
							lk++;
						}
					}
				}
				if (size - pos > width)
				{
					size_t tail = size - pos - width;
					lhs = pCur;
					GoShift(width);
					rhs = pCur;
					lk = rk = 0;
					while (rk < tail && lk < width)
					{
						if (lhs->val > rhs->val)
						{
							tmp = rhs->next;
							Unbind(rhs);
							Insert(rhs, lhs->prev, lhs);
							rhs = tmp;
							rk++;
						}
						else
						{
							lhs = lhs->next;
							lk++;
						}
					}
				}
				width *= 2;
			}
			/*
			Vec<T_DNode<T>*> Node;
			for (GoFirst(); Node.GetSize() >= size; pCur = pCur->next->next)
				Node.push_back(pCur);
			for (; width <= size; width *= 2)
			{
				for (i = 0; i + 2 * width <= size; i += 2 * width)
				{
					lhs = Node[i];
					rhs = Node[i + width];
					lk = rk = 0;
					while(lk < width && rk < width)
					{
						if (lhs->val > rhs->val)
						{
							tmp = rhs->next;
							rhs->Unbind();
							rhs->Insert(lhs->prev, lhs);
							rhs = tmp;
							rk++;
						}
						else 
						{
							lhs = lhs->next;
							lk++;
						}
					}
				}
				if (size - i > width)
				{
					size_t tail = size - i - width;
					lhs = Node[i];
					rhs = Node[i + width];
					lk = rk = 0;
					while (rk < tail && lk < width)
					{
						if (lhs->val > rhs->val)
						{
							tmp = rhs->next;
							rhs->Unbind();
							rhs->Insert(lhs->prev, lhs);
							rhs = tmp;
							rk++;
						}
						else
						{
							lhs = lhs->next;
							lk++;
						}
					}
				}
			}*/
		}
		return;
	}

	static T_DList<T>& sorted_merge(T_DList<T>& const lhs, T_DList<T> const& rhs)
	{
		T_DList Res;
		T_DNode<T> P1 = lhs.Head->next, P2 = rhs.Head->next;
		Base_DNode<T> const H1 = lhs.Head, H2 = rhs.Head;
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

	static T_DList<T> sorted_merge(T_DList<T>&& lhs, T_DList<T>&& rhs)
	{
		if (lhs.size < rhs.size)
			std::swap(lhs, rhs);
		T_DList Res(lhs);
		T_DNode<T> P1 = Res.Head->next, P2 = rhs.Head->next, tmp;
		Base_DNode<T> const H1 = Res.Head, H2 = rhs.Head;
		while (P1 != H1 && P2 != H2)
		{
			if (P1->val <= P2->val)
			{
				P1 = P1->next;
			}
			else
			{
				tmp = P2->next;
				Res.Insert(P2, P1->prev, P1);
				P2 = tmp;
			}

		}
		while (P2 != H2)
		{
			tmp = P2->next;;
			Res.Insert(P2, P1->prev, P1);
			P2 = tmp;
		}
		H2->prev = H2->next = H2;
		return std::move(Res);
	}
};