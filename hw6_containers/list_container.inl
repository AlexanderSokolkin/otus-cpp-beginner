#include <stdexcept>



// --------- Node --------- //

template <typename T, typename Allocator>
bool ListContainer<T, Allocator>::Node::operator==(const typename ListContainer<T, Allocator>::Node& t_otherNode) const
{
	return	m_val == t_otherNode.m_val		&&
			m_next == t_otherNode.m_next	&&
			m_prev == t_otherNode.m_prev;
}


// --------- iterators --------- //

template <typename T, typename Allocator>
ListContainer<T, Allocator>::iterator::iterator(Node* t_elem) :
	m_elem(t_elem), m_empty(false)
{

}

template <typename T, typename Allocator>
ListContainer<T, Allocator>::iterator::iterator() :
	m_elem(nullptr), m_empty(true)
{

}

template <typename T, typename Allocator>
ListContainer<T, Allocator>::iterator::iterator(const typename ListContainer<T, Allocator>::iterator& t_otherIterator) :
	m_elem(t_otherIterator.m_elem), m_empty(t_otherIterator.m_empty)
{

}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator& ListContainer<T, Allocator>::iterator::operator=(const iterator& t_otherIterator)
{
	m_elem = t_otherIterator.m_elem;
	m_empty = t_otherIterator.m_empty;

	return *this;
}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator& ListContainer<T, Allocator>::iterator::operator++()
{
	m_elem = m_elem->m_next;
	return *this;
}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator ListContainer<T, Allocator>::iterator::operator++(int)
{
	typename ListContainer<T, Allocator>::iterator newIter(*this);
	operator++();
	return newIter;
}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator& ListContainer<T, Allocator>::iterator::operator--()
{
	m_elem = m_elem->m_prev;
	return *this;
}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator ListContainer<T, Allocator>::iterator::operator--(int)
{
	typename ListContainer<T, Allocator>::iterator newIter(*this);
	operator--();
	return newIter;
}

template <typename T, typename Allocator>
bool ListContainer<T, Allocator>::iterator::operator==(const typename ListContainer<T, Allocator>::iterator& t_otherIterator) const
{
	if (m_empty || t_otherIterator.m_empty) {
		return false;
	}
	if (!m_elem && !t_otherIterator.m_elem) {
		return true;
	}
	if (!m_elem || !t_otherIterator.m_elem) {
		return false;
	}

	return *m_elem == *(t_otherIterator.m_elem);
}

template <typename T, typename Allocator>
bool ListContainer<T, Allocator>::iterator::operator!=(const typename ListContainer<T, Allocator>::iterator& t_otherIterator) const
{
	return !(*this == t_otherIterator);
}

template <typename T, typename Allocator>
T& ListContainer<T, Allocator>::iterator::operator*() const
{
	return m_elem->m_val;
}



// --------- ListContainer --------- //


template <typename T, typename Allocator>
ListContainer<T, Allocator>::ListContainer() :
	m_head(nullptr), m_tail(nullptr), m_size(0)
{

}

template <typename T, typename Allocator>
ListContainer<T, Allocator>::ListContainer(size_t t_count, const T& t_val) :
	ListContainer()
{
	for (int i = 0; i < t_count; ++i) {
		push_back(t_val);
	}
}

template <typename T, typename Allocator>
ListContainer<T, Allocator>::ListContainer(std::initializer_list<T> t_init) :
	ListContainer()
{
	for (auto it = t_init.begin(); it != t_init.end(); ++it) {
		push_back(*it);
	}
}

template <typename T, typename Allocator>
ListContainer<T, Allocator>::ListContainer(const ListContainer& t_otherList)
{
	m_allocator = node_allocator_traits::select_on_container_copy_construction(t_otherList.m_allocator);
	Node* node = t_otherList.m_head;
	while (node) {
		push_back(node->m_val);
		node = node->m_next;
	}
	m_size = t_otherList.m_size;
}

template <typename T, typename Allocator>
ListContainer<T, Allocator>::ListContainer(ListContainer&& t_otherList)
{
	m_allocator = t_otherList.m_allocator; // ???
	m_head = t_otherList.m_head;
	m_tail = t_otherList.m_tail;
	m_size = t_otherList.m_size;

	t_otherList.m_head = nullptr;
	t_otherList.m_tail = nullptr;
	t_otherList.m_size = 0;
}

template <typename T, typename Allocator>
ListContainer<T, Allocator>& ListContainer<T, Allocator>::operator=(const ListContainer& t_otherList)
{
	clear();

	m_allocator = node_allocator_traits::select_on_container_copy_construction(t_otherList.m_allocator);
	Node* node = t_otherList.m_head;
	while (node) {
		push_back(node->m_val);
		node = node->m_next;
	}
	m_size = t_otherList.m_size;

	return *this;
}

template <typename T, typename Allocator>
ListContainer<T, Allocator>& ListContainer<T, Allocator>::operator=(ListContainer&& t_otherList)
{
	clear();

	m_allocator = t_otherList.m_allocator; // ???
	m_head = t_otherList.m_head;
	m_tail = t_otherList.m_tail;
	m_size = t_otherList.m_size;

	t_otherList.m_head = nullptr;
	t_otherList.m_tail = nullptr;
	t_otherList.m_size = 0;

	return *this;
}

template <typename T, typename Allocator>
ListContainer<T, Allocator>::~ListContainer()
{
	clear();
}


template <typename T, typename Allocator>
T& ListContainer<T, Allocator>::front()
{
	if (!m_head) {
		throw std::out_of_range("no items in the list");
	}
	return m_head->m_val;
}

template <typename T, typename Allocator>
const T& ListContainer<T, Allocator>::front() const
{
	return const_cast<ListContainer<T, Allocator>*>(this)->front();
}

template <typename T, typename Allocator>
T& ListContainer<T, Allocator>::back()
{
	if (!m_tail) {
		throw std::out_of_range("no items in the list");
	}
	return m_tail->m_val;
}

template <typename T, typename Allocator>
const T& ListContainer<T, Allocator>::back() const
{
	return const_cast<ListContainer<T, Allocator>*>(this)->back();
}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator ListContainer<T, Allocator>::begin()
{
	return iterator(m_head);
}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator ListContainer<T, Allocator>::end()
{
	return iterator(m_tail->m_next);
}

template <typename T, typename Allocator>
bool ListContainer<T, Allocator>::empty() const
{
	return m_size == 0;
}

template <typename T, typename Allocator>
size_t ListContainer<T, Allocator>::size() const
{
	return m_size;
}

template <typename T, typename Allocator>
void ListContainer<T, Allocator>::clear()
{
	while (m_head) {
		Node* node = m_head->m_next;
		node_allocator_traits::destroy(m_allocator, m_head);
		node_allocator_traits::deallocate(m_allocator, m_head, 1);
		m_head = node;
	}
	m_tail = nullptr;
	m_size = 0;
}

template <typename T, typename Allocator>
void ListContainer<T, Allocator>::push_back(const T& t_val)
{
	Node* node = node_allocator_traits::allocate(m_allocator, 1);
	node_allocator_traits::construct(m_allocator, node, t_val, nullptr, nullptr);
	++m_size;
	if (!m_head) {
		m_head = m_tail = node;
		return;
	}
	m_tail->m_next = node;
	node->m_prev = m_tail;
	m_tail = node;
}

template <typename T, typename Allocator>
void ListContainer<T, Allocator>::pop_back()
{
	if (!m_tail) {
		return;
	}
	Node* node = m_tail->m_prev;
	node_allocator_traits::destroy(m_allocator, m_tail);
	node_allocator_traits::deallocate(m_allocator, m_tail, 1);
	--m_size;
	if (!node) {
		m_head = m_tail = nullptr;
		return;
	}
	m_tail = node;
	m_tail->m_next = nullptr;
}

template <typename T, typename Allocator>
void ListContainer<T, Allocator>::push_front(const T& t_val)
{
	Node* node = node_allocator_traits::allocate(m_allocator, 1);
	node_allocator_traits::construct(m_allocator, node, t_val, nullptr, nullptr);
	++m_size;
	if (!m_head) {
		m_head = m_tail = node;
		return;
	}
	node->m_next = m_head;
	m_head->m_prev = node;
	m_head = node;
}

template <typename T, typename Allocator>
void ListContainer<T, Allocator>::pop_front()
{
	if (!m_head) {
		return;
	}
	Node* node = m_head->m_next;
	node_allocator_traits::destroy(m_allocator, m_head);
	node_allocator_traits::deallocate(m_allocator, m_head, 1);
	--m_size;
	if (!node) {
		m_head = m_tail = nullptr;
		return;
	}
	m_head = node;
	m_head->m_prev = nullptr;
}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator ListContainer<T, Allocator>::insert(typename ListContainer<T, Allocator>::iterator t_pos, const T& t_val)
{
	if (t_pos.m_empty) {
		throw std::invalid_argument("passed iterator is not valid");
	}
	if (t_pos == end()) {
		push_back(t_val);
		return begin();
	}
	if (t_pos == begin()) {
		push_front(t_val);
		return begin();
	}

	Node* node = node_allocator_traits::allocate(m_allocator, 1);
	node_allocator_traits::construct(m_allocator, node, t_val, nullptr, nullptr);
	++m_size;
	node->m_prev = t_pos.m_elem->m_prev;
	node->m_next = t_pos.m_elem;
	t_pos.m_elem->m_prev->m_next = node;
	t_pos.m_elem->m_prev = node;

	return iterator(node);
}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator ListContainer<T, Allocator>::erase(typename ListContainer<T, Allocator>::iterator t_pos)
{
	if (t_pos.m_empty) {
		throw std::invalid_argument("passed iterator is not valid");
	}
	if (t_pos == end()) {
		return end();
	}

	if (t_pos == begin()) {
		pop_front();
		return begin();
	}
	if (t_pos.m_elem == m_tail) {
		pop_back();
		return iterator(m_tail);
	}

	Node* node = t_pos.m_elem;
	node->m_prev->m_next = node->m_next;
	node->m_next->m_prev = node->m_prev;
	iterator res = iterator(node->m_next);
	node_allocator_traits::destroy(m_allocator, node);
	node_allocator_traits::deallocate(m_allocator, node, 1);
	--m_size;

	return res;
}

template <typename T, typename Allocator>
typename ListContainer<T, Allocator>::iterator ListContainer<T, Allocator>::erase(typename ListContainer<T, Allocator>::iterator t_first, typename ListContainer<T, Allocator>::iterator t_second)
{
	while (t_first != t_second) {
		t_first = erase(t_first);
	}
	return erase(t_first);
//	for (auto it = t_second; it != t_first; --it) {
//		res = erase(it);
//	}

//	return res;
}

template <typename T, typename Allocator>
bool ListContainer<T, Allocator>::operator==(const ListContainer<T, Allocator>& t_otherList)
{
	if (m_size != t_otherList.m_size) {
		return false;
	}

	auto it1 = begin();
	auto it2 = t_otherList.begin();
	for (; it1 != end(); ++it1, ++it2) {
		if (it1.m_elem != it2.m_elem) {
			return false;
		}
	}

	return true;
}

template <typename T, typename Allocator>
bool ListContainer<T, Allocator>::operator!=(const ListContainer<T, Allocator>& t_otherList)
{
	return !(*this == t_otherList);
}
