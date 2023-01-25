#include "list_container.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <initializer_list>
#include <string>
#include <memory>


struct MockItem
{
	MOCK_METHOD(void, Die, ());
	~MockItem() {Die();}
};



TEST(ListContainer, CreateContainer)
{
	// Arrange

	// Act

	// Assert
	EXPECT_NO_THROW(ListContainer<int>());
	EXPECT_NO_THROW(ListContainer<int>(10, 2));
	EXPECT_NO_THROW(ListContainer<int>({1,2,3,4,5,6}));
}

TEST(ListContainer, CreateContainerWithDefaultValue)
{
	// Arrange
	ListContainer<int> list = ListContainer<int>(5, 123);
	size_t expectedSize = 5;
	int expectedValue = 123;

	// Act

	// Assert
	ASSERT_EQ(list.size(), expectedSize);
	int index = 0;
	for (auto elem : list) {
		EXPECT_EQ(elem, expectedValue) << "wrong value in position " << index;
		++index;
	}
}
TEST(ListContainer, CreateContainerWithInitList)
{
	// Arrange
	std::initializer_list<int> initList = {123, 45, 9, 770};
	ListContainer<int> list = ListContainer<int>(initList);
	size_t expectedSize = initList.size();

	// Act

	// Assert
	ASSERT_EQ(list.size(), expectedSize);
	int index = 0;
	for (auto elem : list) {
		EXPECT_EQ(elem, *(initList.begin() + index)) << "wrong value in position " << index;
		++index;
	}
}

TEST(ListContainer, FrontThrow)
{
	// Arrange
	ListContainer<int> list;

	// Act

	// Assert
	ASSERT_THROW(list.front(), std::out_of_range);
}

TEST(ListContainer, BackThrow)
{
	// Arrange
	ListContainer<int> list;

	// Act

	// Assert
	ASSERT_THROW(list.back(), std::out_of_range);
}

TEST(ListContainer, PushBack)
{
	// Arrange
	ListContainer<int> list = ListContainer<int>(3, 111);
	int expectedValue = 777;
	size_t expectedSize = 4;

	// Act
	ASSERT_NO_THROW(list.push_back(777));

	// Assert
	ASSERT_EQ(list.size(), expectedSize);
	ASSERT_EQ(list.back(), expectedValue);
}

TEST(ListContainer, PushFront)
{
	// Arrange
	ListContainer<int> list = ListContainer<int>(3, 111);
	int expectedValue = 777;
	size_t expectedSize = 4;

	// Act
	ASSERT_NO_THROW(list.push_front(777));

	// Assert
	ASSERT_EQ(list.size(), expectedSize);
	ASSERT_EQ(list.front(), expectedValue);
}

TEST(ListContainer, PopBack)
{
	// Arrange
	ListContainer<int> list = ListContainer<int>({23, 56, 89});
	int expectedValue = 56;
	size_t expectedSize = 2;

	// Act
	ASSERT_NO_THROW(list.pop_back());

	// Assert
	ASSERT_EQ(list.size(), expectedSize);
	ASSERT_EQ(list.back(), expectedValue);
}

TEST(ListContainer, PopFront)
{
	// Arrange
	ListContainer<int> list = ListContainer<int>({23, 56, 89});
	int expectedValue = 56;
	size_t expectedSize = 2;

	// Act
	ASSERT_NO_THROW(list.pop_front());

	// Assert
	ASSERT_EQ(list.size(), expectedSize);
	ASSERT_EQ(list.front(), expectedValue);
}

TEST(ListContainer, Insert)
{
	// Arrange
	ListContainer<int> list = ListContainer<int>(6, 2);
	int expectedValue = 56;
	size_t expectedSize = 7;
	int position = 3;

	// Act
	auto it = list.begin();
	for (int i = 0; i < position; ++i) {
		++it;
	}
	ASSERT_NO_THROW(it = list.insert(it, 56));
	auto resIt = list.begin();
	for (int i = 0; i < position; ++i) {
		++resIt;
	}

	// Assert
	ASSERT_EQ(list.size(), expectedSize);
	ASSERT_EQ(it, resIt);
	ASSERT_EQ(*resIt, expectedValue);
}

TEST(ListContainer, EraseOneElement)
{
	// Arrange
	std::initializer_list<int> initList = {1, 2, 3, 4, 5};
	std::initializer_list<int> expectedList = {1, 2, 4, 5};
	ListContainer<int> list = ListContainer<int>(initList);
	int position = 2;

	// Act
	auto it = list.begin();
	for (int i = 0; i < position; ++i) {
		++it;
	}
	ASSERT_NO_THROW(list.erase(it));

	// Assert
	ASSERT_EQ(list.size(), expectedList.size());
	int index = 0;
	for (auto elem : list) {
		EXPECT_EQ(elem, *(expectedList.begin() + index)) << "wrong value in position " << index;
		++index;
	}
}

TEST(ListContainer, EraseSeveralItems)
{
	// Arrange
	std::initializer_list<int> initList = {1, 2, 3, 4, 5};
	std::initializer_list<int> expectedList = {1, 5};
	ListContainer<int> list = ListContainer<int>(initList);

	// Act
	auto itBegin = list.begin();
	auto itEnd = list.begin();
	++itBegin;
	for (int i = 0; i < list.size() - 2; ++i) {
		++itEnd;
	}
	ASSERT_NO_THROW(list.erase(itBegin, itEnd));

	// Assert
	ASSERT_EQ(list.size(), expectedList.size());
	int index = 0;
	for (auto elem : list) {
		EXPECT_EQ(elem, *(expectedList.begin() + index)) << "wrong value in position " << index;
		++index;
	}
}

TEST(ListContainer, Clear)
{
	// Arrange
	ListContainer<std::string> list(5, "val");
	size_t expectedSize = 0;

	// Act
	ASSERT_NO_THROW(list.clear());

	// Assert
	EXPECT_EQ(list.size(), expectedSize);
}

TEST(ListContainer, Destructor)
{
	// Arrange
	ListContainer<std::shared_ptr<MockItem>> list;
	for (int i = 0; i < 10; ++i) {
		list.push_back(std::shared_ptr<MockItem>(new MockItem));
	}

	// Act
	for (auto it = list.begin(); it != list.end(); ++it) {
		EXPECT_CALL(**it, Die())
			.Times(1);
	}
	list.clear();

	// Assert
}

TEST(ListContainer, EquallyOperator)
{
	// Arrange
	std::initializer_list<int> initList = {1, 2, 3, 4};
	ListContainer<int> list1(initList);
	ListContainer<int> list2(initList);

	// Act

	// Assert
	ASSERT_EQ(list1, list2);
}

TEST(ListContainer, CopyConstructor)
{
	// Arrange
	ListContainer<int> list1(10, 123);
	ListContainer<int> list2(list1);

	// Act

	// Assert
	ASSERT_EQ(list1, list2);
}

TEST(ListContainer, CopyOperator)
{
	// Arrange
	ListContainer<int> list1(10, 123);
	ListContainer<int> list2;

	// Act
	ASSERT_NO_THROW(list2 = list1);

	// Assert
	ASSERT_EQ(list1, list2);
}

TEST(ListContainer, MoveConstructor)
{
	// Arrange
	ListContainer<int> list1(10, 123);
	ListContainer<int> list1Copy(list1);
	ListContainer<int> list2(std::move(list1));

	// Act

	// Assert
	ASSERT_EQ(list2, list1Copy);
}


TEST(ListContainer, MoveOperator)
{
	// Arrange
	ListContainer<int> list1(10, 123);
	ListContainer<int> list1Copy(list1);
	ListContainer<int> list2;

	// Act
	list2 = std::move(list1);

	// Assert
	ASSERT_EQ(list2, list1Copy);
}
