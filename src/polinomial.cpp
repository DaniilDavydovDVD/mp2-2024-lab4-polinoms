#include "polinomial.h"

Degree::Degree(uint32_t const& i = 0) : d(i) {};
Degree::Degree(Degree const& D) : d(D.d) {};
Degree& Degree::operator = (uint32_t const& i) noexcept
{
	d = i;
	return *this;
}
Degree Degree::operator +(Degree const& D) const noexcept
{
	Degree Res = d + D.d;
	return Res;
}
Degree& Degree::operator +=(Degree const& D) noexcept
{
	d += D.d;
	return *this;
}
bool Degree::operator ==(Degree const& D) const noexcept
{
	return (d == D.d);
}
bool Degree::operator != (Degree const& D) const noexcept
{
	return (d != D.d);
}
bool Degree::operator > (Degree const& D) const noexcept
{
	return (d > D.d);

}
bool Degree::operator < (Degree const& D) const noexcept
{
	return (d < D.d);

}
bool Degree::operator >= (Degree const& D) const noexcept
{
	return (d >= D.d);

}
bool Degree::operator <= (Degree const& D) const noexcept
{
	return (d <= D.d);

}
