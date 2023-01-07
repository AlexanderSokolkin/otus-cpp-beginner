#include "sequence_container.h"
#include "list_container.h"



template <typename Container>
void printContainer(Container& cont)
{
	for (auto it = cont.begin(); it != cont.end(); ++it) {
		if (it != cont.begin()) {
			std::cout << ", ";
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}

void testSequenceContainer()
{
	std::cout << "Testing SequenceContainer" << std::endl << std::endl;
	SequenceContainer<int> vector(10);
	std::cout << "size = " << vector.size() << ", " << "capacity = " << vector.capacity() << std::endl;

	for (int i = 0; i < 10; ++i) {
		vector.push_back(i);
	}
	std::cout << "size = " << vector.size() << ", " << "capacity = " << vector.capacity() << std::endl;
	printContainer(vector);

	vector.remove(6);
	vector.erase(vector.begin() + 4);
	vector.remove(2);
	printContainer(vector);

	vector.insert(vector.begin(), 10);
	printContainer(vector);

	vector.insert(vector.begin() + static_cast<int>(vector.size() / 2), 20);
	printContainer(vector);

	vector.push_back(30);
	printContainer(vector);

	vector.reserve(20);
	std::cout << "size = " << vector.size() << ", " << "capacity = " << vector.capacity() << std::endl;

	vector.resize(6);
	std::cout << "size = " << vector.size() << ", " << "capacity = " << vector.capacity() << std::endl;
	printContainer(vector);

	vector.pop_back();
	printContainer(vector);

	vector.erase(vector.begin() + 1, vector.begin() + 3);
	printContainer(vector);

	std::cout << std::endl;
}

void testListContainer()
{
	std::cout << "Testing ListContainer" << std::endl << std::endl;
	ListContainer<int> list;
	for (int i = 0; i < 10; ++i) {
		list.push_back(i);
	}
	printContainer(list);
	std::cout << list.size() << std::endl;

	auto it = list.begin();
	for (int i = 0; i < 6; ++i) {
		++it;
	}
	list.erase(it);
	printContainer(list);

	it = list.begin();
	for (int i = 0; i < list.size() / 2; ++i) {
		++it;
	}
	list.insert(it, 20);
	list.push_front(10);
	list.push_back(30);
	printContainer(list);

	auto first = list.begin();
	++first;
	auto second = list.begin();
	for (int i = 0; i < list.size() - 2; ++i) {
		++second;
	}
	list.erase(first, second);
	printContainer(list);

	std::cout << std::endl;
}


int main()
{
	testSequenceContainer();
	testListContainer();

	return 0;
}
