#pragma once
#include "base.h"
#include <vector>
#include <map>
#include <list>


NS_STRUCT_BEGIN

template<class T>
class hashtable
{
public:
	hashtable<T>()
	{
		m_nodeNum = 0;
	}

	~hashtable<T>()
	{

	}

	void push(unsigned int index, const T& data)
	{
		/*
		use Fibonacci arithmetic
		index = (value * 2654435769) >> 28
		16 bit, mult 40503
		32 bit£¬2654435769
		64 bit£¬11400714819323198485
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
		for(auto itr = m_table[index].begin(); itr != m_table[index].end(); ++itr)
		{
			if(*itr == data)
			{
				m_table[index].remove(itr);
				--m_nodeNum;
				break;
			}
		}
	}
	
	void size()
	{
		return m_nodeNum;
	}
	
	iterator& begin()
	{
		iterator itr;
		itr.clear();
		itr.table = table;
		uint vsize = m_table.size();
		for(int n=0; vsize>n;++n)
		{
			if (vsize[n].size() > 0)
			{
				itr.tblIndex = n+1;
				itr.lstIndex = 1;
				break;
			}
		}
		
		return itr;
	}
	iterator& end()
	{
		iterator itr;
		itr.clear();
		itr.table = table;
		uint vsize = m_table.size();
		for(int n=vsize-1; n>=0;--n)
		{
			if (vsize[n].size() > 0)
			{
				itr.tblIndex = n+1;
				itr.lstIndex = vsize[n].size();
				break;
			}
		}
		
		return itr;
	}
	
	
	class iterator
	{
	friend(hashtable);
	public:
		iterator(std::vector<std::list<T>>& tbl, uint tableIndex=0, uint listIndex=0)
		{
			table = tbl;
			tblIndex = tableIndex;
			lstIndex = listIndex;
		}
		
		iterator(const iterator& other)
		{
			value = other.value;
			tblIndex = other.tblIndex;
			lstIndex = other.lstIndex;
		}
		
		iterator& operator ++()
		{
			if(lstIndex < table[tblIndex].size())
			{
				++lstIndex;
			}
			else
			{
				uint oldTbl = tblIndex;
				for(uint tbl = tblIndex+1; tbl <= table.size(); ++tbl)
				{
					if (table[tbl-1].size() > 0)
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
			if(tblIndex == 0 && lstIndex == 0)
			else
			{
				if (lstIndex > 1)
				{
					--lstIndex;
				}
				else
				{
					uint oldTbl = tblIndex;
					for(uint tbl = tblIndex-1; tbl > 0; ++tbl)
					{
						if (table[tbl-1].size() > 0)
						{
							tblIndex = tbl;
							lstIndex = table[tbl].size();
							break;
						}
					}
					if(oldTbl == tblIndex)
					{
						tblIndex = lstIndex = 0;
					}	
				}
			}
			return *this;
		}
		
	protected:
		clear()
		{
			tblIndex = 0;
			lstIndex = 0;
		}
		
	protected:
		 std::vector<std::list<T>>& table;
		uint tblIndex;
		uint lstIndex;
	}

protected:
	std::vector<std::list<T>> m_table;
	uint m_nodeNum;
};
NS_STRUCT_END

