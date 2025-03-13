#pragma once

#include "doublelist.h"
#include "polinomial_parsers.h"
class Degree
{
private:
	uint32_t d;
public:
	Degree(uint32_t const& i = 0);
	Degree(Degree const& D);
	Degree& operator = (uint32_t const& i) noexcept;
	Degree operator +(Degree const& D) const noexcept;
	Degree& operator +=(Degree const& D) noexcept;
	bool operator ==(Degree const& D) const noexcept;
	bool operator != (Degree const& D) const noexcept;
	bool operator > (Degree const& D) const noexcept;
	bool operator < (Degree const& D) const noexcept;
	bool operator >= (Degree const& D) const noexcept;
	bool operator <= (Degree const& D) const noexcept;
};

template <typename T>
class Monomial
{
private:
	Degree deg;
	T val;
	inline static const unsigned char Lit[3] = { 'z', 'y', 'x' };
public:
	Monomial(Degree const& _deg = 0, T const& _val = {}) : deg(_deg), val(_val) {};
	Monomial(std::string const& s) : deg()
	{
		unsigned char tmpc;
		size_t sz = s.size(), tmp = 0, foo, pos, a[3](0);
		pos = s.find('*');
		tmpc = 'x';
		for (i = pos + 1; i < sz; i++)
		{
			if ('x' <= s[i] && s[i] <= 'z')
				if (s[i] < tmpc || a[s[i] - 'x'] != 0)
					throw std::exception("Incorrect order of variables");
		}
	}
	/*Monomial(Monomial const& _M) : deg(_M.deg), val(_M.val) {};
	Monomial()*/
	uint16_t GetDeg(uint16_t i) const noexcept
	{
		uint32_t res = deg.d;
		switch (i)
		{
		case 2:
			res /= 256;
		case 1:
			res /= 256;
		}
		res %= 256;
		return res;
	}
	uint32_t GetDeg()
	{
		return deg.d;
	}
	bool operator == (Monomial<T> const& _M) const noexcept
	{
		return deg == _M.deg;
	}
	bool operator != (Monomial<T> const& _M) const noexcept
	{
		return deg != _M.deg;
	}
	bool operator > (Monomial<T> const& _M) const noexcept
	{
		return deg > _M.deg;
	}
	bool operator >= (Monomial<T> const& _M) const noexcept
	{
		return deg >= _M.deg;
	}
	bool operator < (Monomial<T> const& _M) const noexcept
	{
		return deg < _M.deg;
	}
	bool operator <= (Monomial<T> const& _M) const noexcept
	{
		return deg <= _M.deg;
	}
	friend std::istream& operator >> (std::istream& is, Monomial<T>& M)
	{
		std::string s1, s2;
		size_t sz, pos;
		is >> s1;
		sz = s1.size();
		pos = s1.rfind('x');
		s2 = s1.substr(pos, s1.size() - pos);
		s1.erase(pos, sz);
		if (s1.empty())
			throw std::exception("No arguement before variables");
		try
		{
			std::stringstream ss;
			ss << s1;
			ss >> M.val;
		}
		catch(std::exception& e)
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, Monomial<T> const& M)
	{
		os << M.val;
		size_t tmp = M.GetDeg(), tmp1;
		for (size_t i = 0; i < 4; i++)
		{
			tmp1 = tmp % N;
			if (tmp1 != 0)
				os << Monomial<T>::Lit[3 - i] << tmp1;
		}
		return os;
	}
};

template <typename T>
class Polinomial
{
private:
	T_DList<Monomial<T>> P;
public:
	Polinomial(Monomial<T>& const M) : P(M) {};
	Polinomial(Polinomial<T> const& Pol) : P(Pol.P) {};
	Polinomial(Polinomial<T>&& Pol) : P(std::move(Pol.P)) {};
	Polinomial<T>& operator = (Polinomial<T> const& Pol)
	{
		P = Pol.P;
		return *this;
	}
	Polinomial<T>& operator = (Polinomial<T>&& Pol)
	{
		P = std::move(Pol.P);
		return *this;
	}
};