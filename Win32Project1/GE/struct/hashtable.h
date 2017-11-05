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
	}

protected:
	std::vector<std::list<T>> m_table;
};
NS_STRUCT_END

