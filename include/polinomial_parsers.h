#pragma once
#include <stdexcept>
#include <sstream>
template <typename T> void ParseDegree(std::string& const s, uint8_t(&arr)[3], T& val)
{
	uint8_t _arr[3]{ 0, 0, 0 };
	size_t tmp = 0;;
	std::stringstream sval;
	size_t p1 = 0, p2 = 0, i =0, sz=s.size();
	i = s.find('*');
	if (i != std::string::npos)
	{
		if (!(s[i + 1] >= '0' && s[i + 1] <= '9'))
		{
			bool prev_deg = false;
			size_t k = i + 1;
			int j = 0, const r = 'x' - 'X';
			for (; i < sz; k++)
			{
				if (!prev_deg && s[k] >= 'x' && s[k] <= 'z' || s[k] >= 'X' && s[k] <= 'Z'])
					if ((s[k] - 'W') % r > j)
					{
						j = (s[k] - 'W') / r;
						prev_deg = true;
					}
					else
						throw std::exception("Incorrect order of degrees/Repeated degrees");
				else if (s[k] >= '0' && s[k] <= '9')
				{
					while (k < sz && s[k] >= '0' && s[k] <= '9' && tmp < 256)
					{
						tmp = tmp * 10 + (s[k] - '0');
						k++;
					}
					if (tmp < 256)
					{
						_arr[j-1] = tmp;
						tmp = 0;
					}
					else throw std::out_of_range("Too large degree");
					prev_deg = false;
				}
				else throw std::exception("Missing degree value/Incorrect symbol");
			}
		}
		else throw std::exception("Missing degree letter");
		sval << s.substr(0, i);
		sval >> val;
	}
	else
	{
		sval << s;
		sval >> val;
	}
	for (i=0;i<3;i++)
		arr[i] = _arr[i];
	return;
}