#include <stdexcept>
#include <string>
#include <iostream>


// --------- iterators --------- //

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::iterator::iterator(T* t_elem) :
	m_elem(t_elem)
{

}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::iterator::iterator() :
	m_elem(nullptr)
{

}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::iterator::iterator(const typename SequenceContainer<T, Allocator>::iterator& t_otherIterator) :
	m_elem(t_otherIterator.m_elem)
{

}



template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator& SequenceContainer<T, Allocator>::iterator::operator++()
{
	++m_elem;
	return *this;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator SequenceContainer<T, Allocator>::iterator::operator++(int)
{
	SequenceContainer<T, Allocator>::iterator oldIter(*this);
	operator++();
	return *oldIter;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator& SequenceContainer<T, Allocator>::iterator::operator--()
{
	--m_elem;
	return *this;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator SequenceContainer<T, Allocator>::iterator::operator--(int)
{
	SequenceContainer<T, Allocator>::iterator oldIter(*this);
	operator--();
	return *oldIter;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator& SequenceContainer<T, Allocator>::iterator::operator+=(int t_n)
{
	m_elem = m_elem + t_n;
	return *this;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator& SequenceContainer<T, Allocator>::iterator::operator-=(int t_n)
{
	return operator+=(-t_n);
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator SequenceContainer<T, Allocator>::iterator::operator+(int t_n) const
{
	SequenceContainer<T, Allocator>::iterator result(*this);
	return result += t_n;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator SequenceContainer<T, Allocator>::iterator::operator-(int t_n) const
{
	SequenceContainer<T, Allocator>::iterator result(*this);
	return result -= t_n;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator::difference_type SequenceContainer<T, Allocator>::iterator::operator-(const typename SequenceContainer<T, Allocator>::iterator& t_otherIterator) const
{
	if (!this->m_elem || !t_otherIterator.m_elem) {
		return -1;
	}
	return static_cast<typename SequenceContainer<T, Allocator>::iterator::difference_type>(this->m_elem - t_otherIterator.m_elem);
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator operator+(int t_n, const typename SequenceContainer<T, Allocator>::iterator& t_iter)
{
	typename SequenceContainer<T, Allocator>::iterator result(t_iter);
	return result += t_n;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator operator-(int t_n, const typename SequenceContainer<T, Allocator>::iterator& t_iter)
{
	typename SequenceContainer<T, Allocator>::iterator result(t_iter);
	return result -= t_n;
}



template <typename T, typename Allocator>
bool SequenceContainer<T, Allocator>::iterator::operator<(const typename SequenceContainer<T, Allocator>::iterator& t_otherIterator) const
{
	return m_elem < t_otherIterator.m_elem;
}

template <typename T, typename Allocator>
bool SequenceContainer<T, Allocator>::iterator::operator>(const typename SequenceContainer<T, Allocator>::iterator& t_otherIterator) const
{
	return t_otherIterator < *this;
}

template <typename T, typename Allocator>
bool SequenceContainer<T, Allocator>::iterator::operator<=(const typename SequenceContainer<T, Allocator>::iterator& t_otherIterator) const
{
	return !(*this > t_otherIterator);
}

template <typename T, typename Allocator>
bool SequenceContainer<T, Allocator>::iterator::operator>=(const typename SequenceContainer<T, Allocator>::iterator& t_otherIterator) const
{
	return !(*this < t_otherIterator);
}

template <typename T, typename Allocator>
bool SequenceContainer<T, Allocator>::iterator::operator==(const typename SequenceContainer<T, Allocator>::iterator& t_otherIterator) const
{
	return !(*this < t_otherIterator || *this > t_otherIterator);
}

template <typename T, typename Allocator>
bool SequenceContainer<T, Allocator>::iterator::operator!=(const typename SequenceContainer<T, Allocator>::iterator& t_otherIterator) const
{
	return !(*this == t_otherIterator);
}



template <typename T, typename Allocator>
T& SequenceContainer<T, Allocator>::iterator::operator*() const
{
	return *m_elem;
}




// --------- SequenceContainer --------- //

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::SequenceContainer() :
	m_data(nullptr),
	m_size(0),
	m_capacity(0)
{

}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::SequenceContainer(size_t t_cap) :
	SequenceContainer()
{
	m_data = alloc_traits::allocate(m_allocator, t_cap);
	m_capacity = t_cap;
}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::SequenceContainer(size_t t_cap, const T& t_initVal) :
	SequenceContainer(t_cap)
{
	for (int i = 0; i < m_capacity; ++i) {
		alloc_traits::construct(m_allocator, m_data + i, t_initVal);
	}
	m_size = m_capacity;
}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::SequenceContainer(std::initializer_list<T> t_initList) :
	SequenceContainer(t_initList.size())
{
	int index = 0;
	for (auto it = t_initList.begin(); it != t_initList.end(); ++it) {
		alloc_traits::construct(m_allocator, m_data + index, *it);
		index++;
	}
	m_size = m_capacity;
}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::SequenceContainer(const SequenceContainer<T, Allocator>& t_otherContainer)
{
	m_capacity = t_otherContainer.m_capacity;
	m_size = t_otherContainer.m_size;
	m_allocator = alloc_traits::select_on_container_copy_construction(t_otherContainer.m_allocator);
	m_data = alloc_traits::allocate(m_allocator, m_capacity);
	for (int i = 0; i < m_capacity; ++i) {
		alloc_traits::construct(m_allocator, m_data + i, t_otherContainer.m_data[i]);
	}
}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::SequenceContainer(SequenceContainer<T, Allocator>&& t_otherContainer)
{
	m_capacity = t_otherContainer.m_capacity;
	m_size = t_otherContainer.m_size;
	m_allocator = t_otherContainer.m_allocator; // ???
	m_data = t_otherContainer.m_data;

	m_capacity = 0;
	m_size = 0;
	m_data = nullptr;
}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>& SequenceContainer<T, Allocator>::operator=(const SequenceContainer<T, Allocator>& t_otherContainer)
{
	deallocate(m_data, m_size, m_capacity);

	m_capacity = t_otherContainer.m_capacity;
	m_size = t_otherContainer.m_size;
	m_allocator = alloc_traits::select_on_container_copy_construction(t_otherContainer.m_allocator);
	m_data = alloc_traits::allocate(m_allocator, m_capacity);
	for (int i = 0; i < m_size; ++i) {
		alloc_traits::construct(m_allocator, m_data + i, t_otherContainer.m_data[i]);
	}

	return *this;
}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>& SequenceContainer<T, Allocator>::operator=(SequenceContainer<T, Allocator>&& t_otherContainer)
{
	deallocate(m_data, m_size, m_capacity);

	m_capacity = t_otherContainer.m_capacity;
	m_size = t_otherContainer.m_size;
	m_allocator = t_otherContainer.m_allocator; // ???
	m_data = t_otherContainer.m_data;

	t_otherContainer.m_capacity = 0;
	t_otherContainer.m_size = 0;
	t_otherContainer.m_data = nullptr;

	return *this;
}

template <typename T, typename Allocator>
SequenceContainer<T, Allocator>::~SequenceContainer()
{
	deallocate(m_data, m_size, m_capacity);
}

template <typename T, typename Allocator>
bool SequenceContainer<T, Allocator>::empty() const
{
	return m_size == 0;
}

template <typename T, typename Allocator>
size_t SequenceContainer<T, Allocator>::size() const
{
	return m_size;
}

template <typename T, typename Allocator>
size_t SequenceContainer<T, Allocator>::capacity() const
{
	return m_capacity;
}

template <typename T, typename Allocator>
void SequenceContainer<T, Allocator>::reserve(size_t t_newCap)
{
	if (m_capacity >= t_newCap) {
		return;
	}

	T* newData = alloc_traits::allocate(m_allocator, t_newCap);
	for (int i = 0; i < m_size; ++i) {
		alloc_traits::construct(m_allocator, newData + i, m_data[i]);
	}
	deallocate(m_data, m_size, m_capacity);
	m_data = newData;
	m_capacity = t_newCap;
}

template <typename T, typename Allocator>
void SequenceContainer<T, Allocator>::resize(size_t t_count, const T& t_initVal)
{
	if (t_count <= m_size) {
		for (int i = t_count; i < m_size; ++i) {
			alloc_traits::destroy(m_allocator, m_data + i);
		}
		m_size = t_count;
		return;
	}

	if (t_count > m_capacity) {
		reserve(t_count);
	}
	for (; m_size < t_count; ++m_size) {
		alloc_traits::construct(m_allocator, m_data + m_size, t_initVal);
	}
}

template <typename T, typename Allocator>
void SequenceContainer<T, Allocator>::clear()
{
	for (int i = 0; i < m_size; ++i) {
		alloc_traits::destroy(m_allocator, m_data + i);
	}
	m_size = 0;
}

template <typename T, typename Allocator>
void SequenceContainer<T, Allocator>::push_back(const T& t_val)
{
	if (m_size == m_capacity) {
		reserve(m_capacity * 2);
	}

	if (m_size < m_capacity) {
		alloc_traits::construct(m_allocator, m_data + m_size, t_val);
		++m_size;
		return;
	}
}

template <typename T, typename Allocator>
void SequenceContainer<T, Allocator>::pop_back()
{
	if (empty()) {
		return;
	}

	alloc_traits::destroy(m_allocator, m_data + m_size - 1);
	--m_size;
}

template <typename T, typename Allocator>
T& SequenceContainer<T, Allocator>::at(int t_index)
{
	if (t_index >= m_size) {
		std::string err = "Requested pos = ";
		err += std::to_string(t_index);
		err += ", but vector size = ";
		err += std::to_string(m_size);
		throw std::out_of_range(err);
	}

	return m_data[t_index];
}

template <typename T, typename Allocator>
const T& SequenceContainer<T, Allocator>::at(int t_index) const
{
	return const_cast<SequenceContainer*>(this)->at(t_index);
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator SequenceContainer<T, Allocator>::insert(iterator t_pos, const T& t_val)
{
	if (t_pos == end()) {
		push_back(t_val);
		return end() -= 1;
	}

	int insertIndex = t_pos.m_elem - m_data;
	if (m_size == m_capacity) {
		m_capacity *= 2;
		T* newData = alloc_traits::allocate(m_allocator, m_capacity);
		int newIndex = 0;
		for (int i = 0; i < m_size; ++i) {
			if (i == insertIndex) {
				alloc_traits::construct(m_allocator, newData + newIndex, t_val);
				++newIndex;
			}
			alloc_traits::construct(m_allocator, newData + newIndex, *(m_data + i));
			++newIndex;
		}
		m_data = newData;
		++m_size;
		return begin() += insertIndex;
	}
	else {
		for (int i = m_size; i > insertIndex; --i) {
			alloc_traits::construct(m_allocator, m_data + i, *(m_data + i - 1));
			alloc_traits::destroy(m_allocator, m_data + i - 1);
		}
		alloc_traits::construct(m_allocator, m_data + insertIndex, t_val);
		++m_size;
		return t_pos;
	}
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator SequenceContainer<T, Allocator>::erase(iterator t_pos)
{
	alloc_traits::destroy(m_allocator, t_pos.m_elem);
	int index = t_pos.m_elem - m_data;
	for (int i = index; i < m_size - 1; ++i) {
		alloc_traits::construct(m_allocator, m_data + i, *(m_data + i + 1));
		alloc_traits::destroy(m_allocator, m_data + i + 1);
	}
	--m_size;

	return begin() += index;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator SequenceContainer<T, Allocator>::erase(iterator t_first, iterator t_second)
{
	int count = 0;
	for (auto it = t_first; it <= t_second; ++it) {
		alloc_traits::destroy(m_allocator, it.m_elem);
		++count;
	}

	for (int i = 1; i <= count; ++i) {
		alloc_traits::construct(m_allocator, t_first.m_elem + i, *(t_second.m_elem + i));
	}
	m_size -= count;
	return t_first;
}

template <typename T, typename Allocator>
void SequenceContainer<T, Allocator>::remove(int t_index)
{
	alloc_traits::destroy(m_allocator, m_data + t_index);
	for (int i = t_index; i < m_size - 1; ++i) {
		alloc_traits::construct(m_allocator, m_data + i, *(m_data + i + 1));
		alloc_traits::destroy(m_allocator, m_data + i + 1);
	}
	--m_size;
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator SequenceContainer<T, Allocator>::begin()
{
	return typename SequenceContainer<T, Allocator>::iterator(m_data);
}

template <typename T, typename Allocator>
typename SequenceContainer<T, Allocator>::iterator SequenceContainer<T, Allocator>::end()
{
	return typename SequenceContainer<T, Allocator>::iterator(m_data + m_size);
}

template <typename T, typename Allocator>
bool SequenceContainer<T, Allocator>::operator==(const SequenceContainer<T, Allocator>& t_otherContainer) const
{
	if (m_size != t_otherContainer.m_size) {
		return false;
	}
	for (int i = 0; i < m_size; ++i) {
		if (m_data[i] != t_otherContainer[i]) {
			return false;
		}
	}
	return true;
}

template <typename T, typename Allocator>
bool SequenceContainer<T, Allocator>::operator!=(const SequenceContainer<T, Allocator>& t_otherContainer) const
{
	return !(*this == t_otherContainer);
}

template <typename T, typename Allocator>
T& SequenceContainer<T, Allocator>::operator[](int t_index)
{
	return m_data[t_index];
}

template <typename T, typename Allocator>
const T& SequenceContainer<T, Allocator>::operator[](int t_index) const
{
	return const_cast<SequenceContainer<T, Allocator>*>(this)->operator[](t_index);
}



template <typename T, typename Allocator>
void SequenceContainer<T, Allocator>::deallocate(T* t_data, size_t t_size, size_t t_cap)
{
	if (!t_data) {
		return;
	}

	for (int i = 0; i < t_size; ++i) {
		alloc_traits::destroy(m_allocator, t_data + i);
	}
	alloc_traits::deallocate(m_allocator, t_data, t_cap);
}
