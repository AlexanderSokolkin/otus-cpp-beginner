#pragma once

#include <initializer_list>
#include <iterator>
#include <memory>
#include <iostream>


template <typename T, typename Allocator = std::allocator<T>>
class SequenceContainer
{
public:
	using alloc_traits = std::allocator_traits<Allocator>;

// ------------------------------------------------------------
	class iterator
	{
		friend class SequenceContainer;
		iterator(T* t_elem);

	public:
		using difference_type		=	int;
		using value_type			=	T;
		using pointer				=	T*;
		using reference				=	T&;
		using iterator_category		=	std::random_access_iterator_tag;

		iterator();
		iterator(const iterator& t_otherIterator);

		iterator& operator++(); // prefix
		iterator operator++(int); // postfix
		iterator& operator--(); // prefix
		iterator operator--(int); // postfix
		iterator& operator+=(int t_n);
		iterator& operator-=(int t_n);
		iterator operator+(int t_n) const;
		iterator operator-(int t_n) const;
		difference_type operator-(const iterator& t_otherIterator) const;

		bool operator<(const iterator& t_otherIterator) const;
		bool operator>(const iterator& t_otherIterator) const;
		bool operator<=(const iterator& t_otherIterator) const;
		bool operator>=(const iterator& t_otherIterator) const;
		bool operator==(const iterator& t_otherIterator) const;
		bool operator!=(const iterator& t_otherIterator) const;

		T& operator*() const;

	private:
		T*	m_elem;
	};
// ------------------------------------------------------------


	SequenceContainer();
	explicit SequenceContainer(size_t t_cap);
	SequenceContainer(size_t t_size, const T& t_initVal);
	SequenceContainer(std::initializer_list<T> t_initList);
	SequenceContainer(const SequenceContainer<T, Allocator>& t_otherContainer);
	SequenceContainer(SequenceContainer<T, Allocator>&& t_otherContainer);
	SequenceContainer<T, Allocator>& operator=(const SequenceContainer<T, Allocator>& t_otherContainer);
	SequenceContainer<T, Allocator>& operator=(SequenceContainer<T, Allocator>&& t_otherContainer);

	~SequenceContainer();


	bool empty() const;
	size_t size() const;
	size_t capacity() const;
	void reserve(size_t t_newCap);
	void resize(size_t t_count, const T& t_initVal = T());
	void clear();
	void push_back(const T& t_val);
	void pop_back();
	T& at(int t_index);
	const T& at(int t_index) const;
	iterator insert(iterator t_pos, const T& t_val);
	iterator erase(iterator t_pos);
	iterator erase(iterator t_first, iterator t_second);
	void remove(int t_index);
	iterator begin();
	iterator end();
	bool operator==(const SequenceContainer<T, Allocator>& t_otherContainer) const;
	bool operator!=(const SequenceContainer<T, Allocator>& t_otherContainer) const;
	T& operator[](int t_index);
	const T& operator[](int t_index) const;

private:
	void deallocate(T* t_data, size_t t_size, size_t t_cap);

private:
	T*			m_data;
	size_t		m_size;
	size_t		m_capacity;
	Allocator	m_allocator;
};

#include "sequence_container.inl"
