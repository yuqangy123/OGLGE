#pragma once
#include "base.h"
#include <list>


NS_STRUCT_BEGIN

template<class T>
class llist
{
public:
	typedef struct atom
	{
		T value;
		atom* prev;
		atom* next;

		atom():prev(nullptr), next(nullptr) {};
		atom(const atom& other) {
			this->value = other.value;
			this->prev = other.prev;
			this->next = other.next;
		}
		void link(atom* back) {
			if (back) {
				this->next = back;
				back->prev = this;
			}
		}
		
	};

	class miniList
	{
	public :
		class iterator
		{
		friend miniList;
		public:
			iterator() :node(nullptr){}
			iterator(const iterator& other)
			{
				node = other.node;
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

					if (listNode->prev)
					{
						listNode = listNode->prev;
					}
					else
					{
						if (tableNode->prev)
						{
							tableNode = tableNode->prev;
							listNode = tableNode->list->end;
						}
					}
				} while (false);
				return *this;
			}
			T operator *()
			{
				if (!valid())
					return T();

				return listNode->value;
			}
			T* operator ->()
			{
				if (!valid())
					return nullptr;

				return &listNode->value;
			}
			bool operator !=(const iterator& other)const
			{
				return (!valid() || !other.valid() || listNode != other.listNode || tableNode != other.tableNode);
			}

		protected:
			void clear()
			{
				inListAtom = nullptr;
				outListAtom = nullptr;
			}
			bool valid()const
			{
				return !(node == nullptr);
			}

		protected:
			atom* node;
		};

		miniList() :beginNode(nullptr), endNode(nullptr), iterator(nullptr), nodeNum(0) {};
		T front() {
			if (nodeNum == 0)
			{
				return T();
			}
			return begin->value;
		}
		T back() {
			if (nodeNum == 0)
			{
				return T();
			}
			return end->value;
		}
		void push_back(const T& value) {
			if (nodeNum == 0)
			{
				begin = new atom();
				end = new atom();
				begin->value = end->value = value;
			}
			else
			{
				atom* nd = new atom();
				nd->value = value;

				if (nodeNum == 1){
					begin->link(nd);
				}
				else{
					end->link(nd);
				}
				end = nd;
			}
			++nodeNum;
		}
		void push_front(const T& value) {
			if (nodeNum == 0)
			{
				begin = new atom();
				end = new atom();
				begin->value = end->value = value;
			}
			else
			{
				atom* nd = new atom();
				nd->value = value;

				if (nodeNum == 1) {
					nd->link(end);
				}
				else {
					nd->link(begin);
				}
				begin = nd;
			}
			++nodeNum;
		}
		void pop_back() {
			if (nodeNum > 0) {
				if (nodeNum == 1) {
					delete begin;
					delete end;
					begin = end = nullptr;
				}
				else if (nodeNum == 2) {
					end = end->prev;
					delete end->next;
					end->next = nullptr;
					begin->next = nullptr;
				}
				else
				{
					end = end->prev;
					delete end->next;
					end->next = nullptr;
				}
				--nodeNum;
			}
		}
		void pop_front() {
			if (nodeNum > 0) {
				if (nodeNum == 1) {
					delete begin;
					delete end;
					begin = end = nullptr;
				}
				else if (nodeNum == 2) {
					begin = begin->next;
					delete begin->prev;
					begin->prev = nullptr;
					end->prev = nullptr;
				}
				else
				{
					begin = begin->next;
					delete begin->prev;
					begin->prev = nullptr;
				}
				--nodeNum;
			}
		}
		void begin() { iterator = begin; };
		bool end() { return nullptr == iterator || iterator == end; }
		void operator ++() {
			if (iterator && iterator->next) {
				iterator = iterator->next;
			}
		}
		T operator ->() {
			if (iterator) {
				return iterator->value;
			}
			return T();
		}
		unsigned int size() { return nodeNum; }


	protected:
		atom* begin;
		atom* end;
		unsigned int nodeNum;
	};

	typedef struct Node
	{
		uint prior;
		miniList list;
		Node* prev;
		Node* next;

		Node() :prior(0), prev(nullptr), next(nullptr){};
		link(Node* back) {
			this->next = back;
			back->prev = this;
		}
	};
	class iterator
	{
	friend llist;
	friend miniList;
	public:
		iterator():listNode(nullptr), tableNode(nullptr) {}
		
		iterator(const iterator& other)
		{
			listNode = other.listNode;
			tableNode = other.tableNode;
		}
		iterator operator ++()
		{
			do
			{
				if (!valid())break;

				if (listNode->next)
				{
					listNode = listNode->next;
				}
				else
				{
					if (tableNode->next)
					{
						tableNode = tableNode->next;
						listNode = tableNode->list->begin;
					}
				}
			} while (false);
			return *this;
		}
		iterator& operator --()
		{
			do
			{
				if (!valid())break;

				if (listNode->prev)
				{
					listNode = listNode->prev;
				}
				else
				{
					if (tableNode->prev)
					{
						tableNode = tableNode->prev;
						listNode = tableNode->list->end;
					}
				}
			} while (false);
			return *this;
		}
		T operator *()
		{
			if (!valid())
				return T();

			return listNode->value;
		}
		T* operator ->()
		{
			if (!valid())
				return nullptr;

			return &listNode->value;
		}
		bool operator !=(const iterator& other)const
		{
			return (!valid() || !other.valid() || listNode != other.listNode || tableNode != other.tableNode);
		}
		
	protected:
		void clear()
		{
			inListAtom = nullptr;
			outListAtom = nullptr;
		}
		bool valid()const
		{
			return !(listNode == nullptr || tableNode == nullptr);
		}
		
	protected:
		atom* listNode;
		Node* tableNode;
	};

	//default table size is 1024
	llist<T>() : m_nodeNum(0)
	{
	}

	~llist<T>()
	{

	}

	void push(const T& data, unsigned int prior = 0)
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
					m_list.insert(itr + index, 1, nd);
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
			if (m_list.front().prior == prior)
			{
				std::list<T>& lst = m_list.front().list;
				if (auto itr = lst.begin(); itr != lst.end(); ++itr)
				{
					if (*itr == data)
					{
						lst.erase(itr);
						bBreak = true;
						break;
					}
				}
			}
			if (bBreak)break;

			if (m_list.back().prior == prior)
			{
				std::list<T>& lst = m_list.back().list;
				if (auto itr = lst.begin(); itr != lst.end(); ++itr)
				{
					if (*itr == data)
					{
						lst.erase(itr);
						bBreak = true;
						break;
					}
				}
			}
			if (bBreak)break;

			for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
			{
				if (prior == itr->prior)
				{
					std::list<T>& lst = itr->list;
					for (auto itr2 = lst.begin(); itr2 != lst.end(); ++itr2)
					{
						if (data == *itr2)
						{
							lst.erase(itr2);
							bBreak = true;
							break;
						}
					}
					break;
				}
			}
		} while (false);
		if(bBreak)--m_nodeNum;
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
	miniList<Node> m_list;
	uint m_nodeNum;
};
NS_STRUCT_END

