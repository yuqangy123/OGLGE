#pragma once
#include "base.h"
#include <vector>
#include <map>
#include <list>


NS_STRUCT_BEGIN

/*
一个简单的哈希表
*/
template<class T>
class hashtable
{
public:
	class iterator
	{
	friend hashtable;
	public:
		iterator()
		{
			table = nullptr;
			tblIndex = lstIndex = 0;
		}
		iterator(std::vector<std::list<T>>* tbl, uint tableIndex = 0, uint listIndex = 0)
		{
			table = tbl;
			tblIndex = tableIndex;
			lstIndex = listIndex;
		}
		iterator(const iterator& other)
		{
			table = other.table;
			tblIndex = other.tblIndex;
			lstIndex = other.lstIndex;
		}
		iterator& operator ++()
		{
			if (!valid())
				return *this;

			if(lstIndex < (*table)[tblIndex-1].size())
			{
				++lstIndex;
			}
			else
			{
				uint oldTbl = tblIndex;
				for(uint tbl = tblIndex+1; tbl <= table->size(); ++tbl)
				{
					if ((*table)[tbl-1].size() > 0)
					{
						tblIndex = tbl;
						lstIndex = 1;
						break;
					}
				}
				if(oldTbl == tblIndex)
				{
					tblIndex = lstIndex = 0;
				}
			}
			return *this;
		}
		iterator& operator --()
		{
			if (!valid())
				return *this;

			if (lstIndex > 1)
			{
				--lstIndex;
			}
			else
			{
				uint oldTbl = tblIndex;
				for (uint tbl = tblIndex-1; tbl > 0; --tbl)
				{
					if (*table[tbl - 1].size() > 0)
					{
						tblIndex = tbl;
						lstIndex = (*table)[tbl].size();
						break;
					}
				}
				if (oldTbl == tblIndex)
				{
					tblIndex = lstIndex = 0;
				}
			}
			return *this;
		}
		T operator *()
		{
			if (!valid())
				return T();

			if (tblIndex != 0 && lstIndex != 0)
			{
				std::list<T>::iterator listItr = (*table)[tblIndex-1].begin();
				uint n = 1;
				while (listItr != (*table)[tblIndex-1].end())
				{
					if (n == lstIndex)
					{
						return *listItr;
					}
					++n;
					++listItr;
				}
			}
			return T();
		}
		T* operator ->()
		{
			return &(operator*());
		}
		bool operator !=(const iterator& other)const
		{
			return (table != other.table || !valid() || !other.valid() || tblIndex != other.tblIndex || lstIndex != other.lstIndex);
		}
		
	protected:
		void clear()
		{
			tblIndex = 0;
			lstIndex = 0;
		}
		bool valid()const
		{
			return !(table == nullptr || tblIndex == 0 || lstIndex == 0);
		}
		
	protected:
		std::vector<std::list<T>>* table;
		uint tblIndex;
		uint lstIndex;
	};

	//default table size is 1024
	hashtable<T>() : m_nodeNum(0)
	{
		m_table.resize(1024);
	}

	~hashtable<T>()
	{

	}

	void push(const T& data, unsigned int index = 0)
	{
		/*
		use Fibonacci arithmetic
		index = (value * 2654435769) >> 28
		16 bit, mult 40503
		32 bit，2654435769
		64 bit，11400714819323198485
		*/
		//default 32bit
		//http://blog.csdn.net/jesseshen/article/details/6662617
		index = (index * 2654435769) >> 28;
		m_table[index].push_back(data);
		++m_nodeNum;
	}

	void remove(unsigned int index, const T& data)
	{
		index = (index * 2654435769) >> 28;
		for (auto itr = m_table[index].begin(); itr != m_table[index].end(); ++itr)
		{
			if (*itr == data)
			{
				m_table[index].remove(itr);
				--m_nodeNum;
				break;
			}
		}
	}

	uint size()
	{
		return m_nodeNum;
	}

	iterator begin()
	{
		iterator itr;
		itr.clear();
		itr.table = &m_table;

		uint vsize = m_table.size();
		for (int n = 0; vsize>n; ++n)
		{
			if (m_table[n].size() > 0)
			{
				itr.tblIndex = n + 1;
				itr.lstIndex = 1;
				break;
			}
		}

		return itr;
	}
	//優化，这里的查询速度可以优化
	iterator end()
	{
		iterator itr;
		itr.clear();
		itr.table = &m_table;
		uint vsize = m_table.size();
		for (int n = vsize - 1; n >= 0; --n)
		{
			if (m_table[n].size() > 0)
			{
				itr.tblIndex = n + 1;
				itr.lstIndex = m_table[n].size();
				break;
			}
		}

		return itr;
	}

	iterator erase(iterator& object)
	{
		if (!object.valid() || object.table != &m_table)
			return object;

		auto lstItr = m_table[object.tblIndex].begin();
		int n = 1;
		while (lstItr != m_table[object.tblIndex].end())
		{
			if (n == object.lstIndex)
			{
				m_table[object.tblIndex].erase(lstItr);

				iterator itr = object;
				if (m_table[object.tblIndex].size() == 0)
					++itr;
				return itr;
			}
			++lstItr;
		}
		return object;
	}


protected:
	std::vector<std::list<T>> m_table;
	uint m_nodeNum;
};
NS_STRUCT_END

