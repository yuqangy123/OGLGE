#pragma once
#include "base.h"
#include <list>


NS_STRUCT_BEGIN

template<class T>
class llist
{
public:
	typedef struct Node
	{
		uint prior;
		std::list<T> list;
		Node() :prior(0){};
	};
	class iterator
	{
	friend llist;
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
	llist<T>() : m_nodeNum(0)
	{
		m_list.resize(8);
	}

	~llist<T>()
	{

	}

	void push(const T& data, uint index = 0)
	{
		do{
			if (m_list.size() == 0 || index > m_list.back().prior)
			{
				Node nd;
				nd.prior = index;
				nd.list.push_back(data);
				m_list.push_back(nd);
				break;
			}

			uint index = 0;
			for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
			{
				if (index > itr->prior)
				{
					Node nd;
					nd.prior = index;
					nd.list.push_back(data);
					m_list.insert(itr + index, 1, nd);
					break;
				}
				if (itr->prior == index)
				{
					itr->list.push_back(data);
					++m_nodeNum;
					break;
				}
				++index;
			}
		}while (false);
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
	std::list<Node> m_list;
	uint m_nodeNum;
};
NS_STRUCT_END

