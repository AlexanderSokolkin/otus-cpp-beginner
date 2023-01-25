#pragma once

#include <initializer_list>
#include <iterator>
#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class ListContainer
{
	struct Node
	{
		Node(const T& t_val, Node* t_prev, Node* t_next) :
			m_val(t_val), m_prev(t_prev), m_next(t_next) {}

		T		m_val;
		Node*	m_prev;
		Node*	m_next;

		bool operator==(const Node& t_otherNode) const;
	};

public:
	using allocator_traits = std::allocator_traits<Allocator>;
	using node_allocator = typename allocator_traits::template rebind_alloc<Node>;
	using node_allocator_traits = std::allocator_traits<node_allocator>;

// ------------------------------------------------------------

	class iterator
	{
		friend class ListContainer;
		iterator(Node* t_elem);

	public:
		using difference_type		=	int;
		using value_type			=	T;
		using pointer				=	T*;
		using reference				=	T&;
		using iterator_category		=	std::forward_iterator_tag;

		iterator();
		iterator(const iterator& t_otherIterator);
		iterator& operator=(const iterator& t_otherIterator);

		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);

		bool operator==(const iterator& t_otherIterator) const;
		bool operator!=(const iterator& t_otherIterator) const;

		T& operator*() const;

	private:
		Node*	m_elem = nullptr;
		bool	m_empty;
	};
	friend class iterator;

// ------------------------------------------------------------

	ListContainer();
	ListContainer(size_t t_count, const T& t_val);
	ListContainer(std::initializer_list<T> t_init);
	ListContainer(const ListContainer& t_otherList);
	ListContainer(ListContainer&& t_otherList);
	ListContainer<T, Allocator>& operator=(const ListContainer& t_otherList);
	ListContainer<T, Allocator>& operator=(ListContainer&& t_otherList);

	~ListContainer();

	T& front();
	const T& front() const;
	T& back();
	const T& back() const;
	iterator begin();
	iterator end();
	bool empty() const;
	size_t size() const;
	void clear();
	void push_back(const T& t_val);
	void pop_back();
	void push_front(const T& t_val);
	void pop_front();
	iterator insert(iterator t_pos, const T& t_val);
	iterator erase(iterator t_pos);
	iterator erase(iterator t_first, iterator t_second);
	bool operator==(const ListContainer<T, Allocator>& t_otherList) const;
	bool operator!=(const ListContainer<T, Allocator>& t_otherList) const;


private:
	node_allocator	m_allocator;
	Node*			m_head = nullptr;
	Node*			m_tail = nullptr;
	size_t			m_size = 0;
};


#include "list_container.inl"
