#include "sequence_container.h"

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



TEST(SequenceContainer, CreateContainer)
{
	// Arrange

	// Act

	// Assert
	EXPECT_NO_THROW(SequenceContainer<int>());
	EXPECT_NO_THROW(SequenceContainer<int>(10));
	EXPECT_NO_THROW(SequenceContainer<int>(10, 2));
	EXPECT_NO_THROW(SequenceContainer<int>({1,2,3,4,5,6}));
}

TEST(SequenceContainer, CreateContainerWithMemoryAllocation)
{
	// Arrange
	SequenceContainer<int> vector = SequenceContainer<int>(10);
	size_t expectedCapacity = 10;

	// Act

	// Assert
	ASSERT_EQ(vector.capacity(), expectedCapacity);
}

TEST(SequenceContainer, CreateContainerWithDefaultValue)
{
	// Arrange
	SequenceContainer<int> vector = SequenceContainer<int>(5, 123);
	size_t expectedSize = 5;
	int expectedValue = 123;

	// Act

	// Assert
	ASSERT_EQ(vector.size(), expectedSize);
	for (size_t i = 0; i < vector.size(); ++i) {
		EXPECT_EQ(vector[i], expectedValue) << "wrong value in position " << i;
	}
}

TEST(SequenceContainer, CreateContainerWithInitList)
{
	// Arrange
	std::initializer_list<int> initList = {123, 45, 9, 770};
	SequenceContainer<int> vector = SequenceContainer<int>(initList);
	size_t expectedSize = initList.size();

	// Act

	// Assert
	ASSERT_EQ(vector.size(), expectedSize);
	for (size_t i = 0; i < initList.size(); ++i) {
		EXPECT_EQ(vector[i], *(initList.begin() + i)) << "wrong value in position " << i;
	}
}

TEST(SequenceContainer, PushBack)
{
	// Arrange
	SequenceContainer<int> vector = SequenceContainer<int>(3, 111);
	int expectedValue = 777;
	size_t expectedSize = 4;

	// Act
	ASSERT_NO_THROW(vector.push_back(777));

	// Assert
	ASSERT_EQ(vector.size(), expectedSize);
	ASSERT_EQ(vector[vector.size() - 1U], expectedValue);
}

TEST(SequenceContainer, PopBack)
{
	// Arrange
	SequenceContainer<int> vector = SequenceContainer<int>({23, 56, 89});
	int expectedValue = 56;
	size_t expectedSize = 2;

	// Act
	ASSERT_NO_THROW(vector.pop_back());

	// Assert
	ASSERT_EQ(vector.size(), expectedSize);
	ASSERT_EQ(vector[vector.size() - 1], expectedValue);
}

TEST(SequenceContainer, Insert)
{
	// Arrange
	SequenceContainer<int> vector = SequenceContainer<int>(6, 2);
	int expectedValue = 56;
	size_t expectedSize = 7;
	int position = 3;

	// Act
	ASSERT_NO_THROW(vector.insert(vector.begin() + position, 56));

	// Assert
	ASSERT_EQ(vector.size(), expectedSize);
	ASSERT_EQ(vector[position], expectedValue);
}

TEST(SequenceContainer, Erase)
{
	// Arrange
	std::initializer_list<int> initList = {1, 2, 3, 4, 5};
	std::initializer_list<int> expectedList = {1, 2, 4, 5};
	SequenceContainer<int> vector = SequenceContainer<int>(initList);
	int position = 2;

	// Act
	ASSERT_NO_THROW(vector.erase(vector.begin() + position));

	// Assert
	ASSERT_EQ(vector.size(), expectedList.size());
	for (size_t i = 0; i < expectedList.size(); ++i) {
		EXPECT_EQ(vector[i], *(expectedList.begin() + i)) << "wrong value in position " << i;
	}
}

TEST(SequenceContainer, Remove)
{
	// Arrange
	std::initializer_list<int> initList = {1, 2, 3, 4, 5};
	std::initializer_list<int> expectedList = {1, 2, 4, 5};
	SequenceContainer<int> vector = SequenceContainer<int>(initList);
	int position = 2;

	// Act
	ASSERT_NO_THROW(vector.remove(position));

	// Assert
	ASSERT_EQ(vector.size(), expectedList.size());
	for (int i = 0; i < expectedList.size(); ++i) {
		EXPECT_EQ(vector[i], *(expectedList.begin() + i)) << "wrong value in position " << i;
	}
}

TEST(SequenceContainer, Resize)
{
	// Arrange
	SequenceContainer<int> vector(10, 123);
	size_t expectedSize = 20;

	// Act
	ASSERT_NO_THROW(vector.resize(20));


	// Assert
	ASSERT_EQ(vector.size(), expectedSize);
}

TEST(SequenceContainer, Reserve)
{
	// Arrange
	SequenceContainer<int> vector;
	size_t expectedCapacity = 7;

	// Act
	ASSERT_NO_THROW(vector.reserve(7));

	// Assert
	ASSERT_EQ(vector.capacity(), expectedCapacity);
}

TEST(SequenceContainer, Clear)
{
	// Arrange
	SequenceContainer<std::string> vector(5, "val");
	size_t expectedSize = 0;

	// Act
	ASSERT_NO_THROW(vector.clear());

	// Assert
	EXPECT_EQ(vector.size(), expectedSize);
}

TEST(SequenceContainer, Destructor)
{
	// Arrange
	SequenceContainer<std::shared_ptr<MockItem>> vector;
	for (int i = 0; i < 10; ++i) {
		vector.push_back(std::shared_ptr<MockItem>(new MockItem));
	}

	// Act
	for (int i = 0; i < vector.size(); ++i) {
		EXPECT_CALL(*vector[i], Die())
			.Times(1);
	}
	vector.clear();

	// Assert
}



TEST(SequenceContainer, EquallyOperator)
{
	// Arrange
	std::initializer_list<int> initList = {1, 2, 3, 4};
	SequenceContainer<int> vector1(initList);
	SequenceContainer<int> vector2(initList);

	// Act

	// Assert
	ASSERT_TRUE(vector1 == vector2);
}

TEST(SequenceContainer, CopyConstructor)
{
	// Arrange
	SequenceContainer<int> vector1(10, 123);
	SequenceContainer<int> vector2(vector1);

	// Act

	// Assert
	ASSERT_EQ(vector1, vector2);
}

TEST(SequenceContainer, CopyOperator)
{
	// Arrange
	SequenceContainer<int> vector1(10, 123);
	SequenceContainer<int> vector2;

	// Act
	ASSERT_NO_THROW(vector2 = vector1);

	// Assert
	ASSERT_EQ(vector1, vector2);
}

TEST(SequenceContainer, MoveConstructor)
{
	// Arrange
	SequenceContainer<int> vector1(10, 123);
	SequenceContainer<int> vector1Copy(vector1);
	SequenceContainer<int> vector2(std::move(vector1));

	// Act

	// Assert
	ASSERT_EQ(vector2, vector1Copy);
}

TEST(SequenceContainer, MoveOperator)
{
	// Arrange
	SequenceContainer<int> vector1(10, 123);
	SequenceContainer<int> vector1Copy(vector1);
	SequenceContainer<int> vector2;

	// Act
	vector2 = std::move(vector1);

	// Assert
	ASSERT_EQ(vector2, vector1Copy);
}

TEST(SequenceContainer, AtOutOfRange)
{
	// Arrange
	SequenceContainer<int> vector(11, 0);

	// Act


	// Assert
	ASSERT_THROW(vector.at(20), std::out_of_range);
}
