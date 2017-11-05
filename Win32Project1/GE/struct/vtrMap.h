#pragma once
#include "base.h"
#include <vector>
#include <map>
#include <list>


NS_STRUCT_BEGIN

template<class T>
class thash
{
public:
	thash<T>()
	{

	}

	~thash<T>()
	{

	}

	void push(const T& data, int lv = 0)
	{
		{
			m_vector.push_back(lv);

			if (m_map.find(lv) == m_map.end())
			{
				std::vector<T> vtr;
				m_map[lv] = vtr;
			}
			m_map[lv].push_back(data);

		}
	}

protected:
	std::vector<int> m_vector;
	std::map<int, std::list<T>> m_hash;

};
NS_STRUCT_END

