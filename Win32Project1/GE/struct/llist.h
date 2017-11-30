#pragma once
#include "base.h"
#include <list>


NS_STRUCT_BEGIN

template<class T>
struct atom
{
	T value;
	atom* prev;
	atom* next;

	atom() :prev(nullptr), next(nullptr) {};
	atom(const atom<T>& other) {
		this->value = other.value;
		this->prev = other.prev;
		this->next = other.next;
	}
	void link(atom<T>* back) {
		if (back) {
			this->next = back;
			back->prev = this;
		}
	}
};

template<class T>
class miniList
{
public:
	class iterator
	{
		friend miniList;
	public:
		iterator() :node(nullptr) {}
		iterator(const iterator& other)
		{
			node = other.node;
		}
		iterator(atom<T>* nd) {
			node = nd;
		}
		iterator operator ++()
		{
			do
			{
				if (!valid())break;

				if (node->next)
				{
					node = node->next;
				}
			} while (false);
			return *this;
		}
		iterator& operator --()
		{
			do
			{
				if (!valid())break;

				if (node->prev)
				{
					node = node->prev;
				}
			} while (false);
			return *this;
		}
		T operator*()
		{
			if (!valid())
				return T();

			return node->value;
		}
		T* operator->()
		{
			if (!valid())
				return nullptr;

			return &(node->value);
		}
		bool operator !=(const iterator& other)const
		{
			return (!valid() || !other.valid() || node != other.node);
		}
		bool operator ==(const iterator& other)const
		{
			return !operator !=(other);
		}
		iterator operator +(int value)
		{
			iterator addItr = *this;
			for (int n = 0; n < value; ++n)
			{
				if (addItr.node->next && addItr.node->next->next)
					addItr.node = addItr.node->next;
				else
					break;
			}
			return addItr;
		}
		iterator operator -(int value)
		{
			iterator addItr = *this;
			for (int n = 0; n < value; ++n)
			{
				if (addItr.node->prev && addItr.node->prev->prev)
					addItr.node = addItr.node->prev;
				else
					break;
			}
			return addItr;
		}
		atom<T>* getAtom() { return node; }
		bool valid()const
		{
			return (node != nullptr);
		}

	protected:
		void clear()
		{
			node = nullptr;
		}
		

	protected:
		atom<T>* node;
	};

	miniList()
	{
		init();
	}
	T front()
	{
		if (nodeNum == 0)
		{
			return T();
		}
		return beginAtom->next->value;
	}
	T back()
	{
		if (nodeNum == 0)
		{
			return T();
		}
		return endAtom->prev->value;
	}
	void push_back(const T& value)
	{
		iterator endItr(endAtom);
		--endItr;
		insert(endItr, value);
	}
	void push_front(const T& value)
	{
		iterator beginItr(beginAtom);
		insert(beginItr, value);
	}
	void pop_back()
	{
		if (nodeNum > 0)
		{
			erase(iterator(endAtom->prev));
		}
	}
	void pop_front()
	{
		if (nodeNum > 0)
		{
			erase(iterator(beginAtom->next));
		}
	}
	unsigned int size()
	{
		return nodeNum;
	}

	//begin和end要主要，begin是第一个有效元素，end是end标识元素
	iterator begin()
	{
		if (nodeNum > 0)
			return iterator(beginAtom->next);
		return iterator();
	};
	iterator end()
	{
		return iterator(endAtom);
	}
	void insert(iterator& itr, const T& value)
	{
		atom<T>* newNode = new atom<T>();
		newNode->value = value;

		atom<T>* preNextItr = itr.node->next;
		itr.node->link(newNode);
		newNode->link(preNextItr);

		++nodeNum;
	}
	void erase(iterator& itr)
	{
		if (itr.node->prev)
			itr.node->prev->next = itr.node->next;
		if (itr.node->next)
			itr.node->next->prev = itr.node->prev;

		delete itr.node;
		itr.node = nullptr;
		--nodeNum;
	}

protected:
	void init()
	{
		beginAtom = new atom<T>();
		endAtom = new atom<T>();
		beginAtom->link(endAtom);
		nodeNum = 0;
	}

protected:
	atom<T>* beginAtom;
	atom<T>* endAtom;
	unsigned int nodeNum;
};

template<class T>
class llist
{
public:
	friend miniList<T>;

	typedef struct Node_
	{
		uint prior;
		miniList<T> list;
		Node_* prev;
		Node_* next;

		Node_() :prior(0), prev(nullptr), next(nullptr){};
		void link(Node_* back) {
			this->next = back;
			if (back) back->prev = this;
		}
	}Node;

	/*
	因为list本身就是模板，在其模板参数未确定之前，也就是Property<N,V> 的具体类型没有确定之前，引用其class内部定义的type，这个type也是未知的，加上typename就是告诉编译器先不管具体类型，等模板实例化的时候再确定吧
	*/
	typedef typename miniList<Node>::iterator NodeIterator;
	typedef typename miniList<T>::iterator TIterator;

	class iterator
	{
	friend llist;
	public:
		iterator()
		{
		}		
		iterator(const iterator& other)
		{
			tableIterator = other.tableIterator;
			valueIterator = other.valueIterator;
		}
		iterator(const iterator& tableItr, const iterator& valueItr)
		{
			tableIterator = tableItr;
			valueIterator = valueItr;
		}
		iterator operator ++()
		{
			auto addItr = valueIterator;
			++addItr;
			if (addItr != valueIterator && addItr.getAtom()->next)
			{
				valueIterator = addItr;
			}
			else
			{
				auto addTableItr = tableIterator;
				auto lastItr = (tableIterator + 1);
				do
				{
					++addTableItr;
					if (addTableItr->list.size() > 0 || addTableItr == lastItr)
					{
						break;
					}
					lastItr = addTableItr;
				} while (true);

				if (addTableItr != lastItr) valueIterator = addTableItr->list.begin();
				tableIterator = addTableItr;
				
			}
			return *this;
		}
		iterator& operator --()
		{
			auto subItr = valueIterator;
			--subItr;
			if (subItr != valueIterator && subItr.getAtom()->prev)
			{
				valueIterator = subItr;
			}
			else
			{
				auto subTableItr = tableIterator;
				auto lastItr = (tableIterator-1);
				do
				{
					--subTableItr;
					if (subTableItr->list.size() > 0 || subTableItr == lastItr)
					{
						break;
					}
					lastItr = subTableItr;
				} while (true);

				if (subTableItr != lastItr)
				{
					valueIterator = --(subTableItr->list.end());
					tableIterator = subTableItr;
				}
				else
				{
					tableIterator = ++subTableItr;
				}
			}
			return *this;
		}
		T operator *()
		{
			return valueIterator.operator*();
		}
		T* operator ->()
		{
			return valueIterator.operator->();
		}
		bool operator !=(const iterator& other)const
		{
			return (!valid() || !other.valid() || tableIterator != other.tableIterator || valueIterator != other.valueIterator);
		}
		
	protected:
		bool valid()const
		{
			return (tableIterator.valid() && valueIterator.valid());
		}
		
	protected:		
		NodeIterator	tableIterator;
		TIterator		valueIterator;
	};

	//default table size is 1024
	llist<T>() : m_nodeNum(0)
	{
	}

	~llist<T>()
	{

	}

	void add(const T& data, unsigned int prior = 0)
	{
		do{
			if (m_list.size() == 0 || prior > m_list.back().prior)
			{
				Node nd;
				nd.prior = prior;
				nd.list.push_back(data);
				m_list.push_back(nd);
				break;
			}

			uint index = 0;
			for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
			{
				if (prior > itr->prior)
				{
					Node nd;
					nd.prior = prior;
					nd.list.push_back(data);
					m_list.insert(m_list.begin() + index, nd);
					break;
				}
				if (itr->prior == prior)
				{
					itr->list.push_back(data);
					break;
				}
				++index;
			}
		}while (false);
		++m_nodeNum;
	}

	void remove(const T& data, unsigned int prior)
	{
		bool bBreak = false;
		do {
			//optimize find arith
			for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
			{
				if (prior == itr->prior)
				{
					std::list<T>& lst = itr->list;
					for (auto itr2 = lst.begin(); itr2 != lst.end(); ++itr2)
					{
						if (itr2->value == data)
						{
							lst.erase(itr2);
							bBreak = true;
							break;
						}
					}
					break;
				}
				if (bBreak)break;
			}
		} while (false);
		if(bBreak)--m_nodeNum;
	}

	void erase(iterator& itr)
	{
		itr.tableIterator->list.erase(itr.valueIterator);
	}

	uint size()
	{
		return m_nodeNum;
	}

	iterator begin()
	{
		iterator itr;
		if (m_nodeNum > 0)
		{
			itr.tableIterator = m_list.begin();
			itr.valueIterator = itr.tableIterator->list.begin();
		}		
		return itr;
	}
	
	iterator end()
	{
		iterator itr;
		if (m_nodeNum > 0)
		{
			itr.tableIterator = m_list.end();
			itr.valueIterator = (itr.tableIterator-1)->list.end();
			--itr.valueIterator;
		}
		return itr;
	}


protected:
	miniList<Node> m_list;
	uint m_nodeNum;
};
NS_STRUCT_END

