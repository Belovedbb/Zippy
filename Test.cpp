#include "gtest/gtest.h"
#include "ArrayList.h"
#include "PriorityList.h"
#include "Huffman_algo.h"
#include "Zippy.h"
#include <random>

using list::ArrayList;
using list::LinkedList;
using list::PriorityList;
using algo::huffman_algo;
using std::cout;
using std::endl;
using zippy::compressor;
using zippy::decompressor;
namespace fs = std::experimental::filesystem;


extern size_t file_size = -1;


void testArrayList() {
	//start int test
	{
		ArrayList<int> testInt;
		for (int i = 0; i < 1000; i++)
			testInt.add(i + 1);

		for (int i = 0; i < 1000; i++)
			EXPECT_EQ(testInt.get(i), i + 1);

		for (int i = 0; i < 1000; i++)
			testInt.remove(0);
		EXPECT_TRUE(testInt.is_empty() == true);
		EXPECT_EQ(testInt.size(), 0);
		for (int i = 0; i < 500; i++)
			testInt.add(i);
		for (int i = 0; i < 500; i++)
			EXPECT_EQ(testInt.get(i), i);
		testInt.clear();
		EXPECT_TRUE(testInt.is_empty());
	}//end int test

	//start string test
	{
		ArrayList<std::string> testString;
		//std::string str;
		for (int i = 0; i < 1000; i++)
			testString.add(std::to_string(i + 1));

		for (int i = 0; i < 1000; i++)
			EXPECT_EQ(testString.get(i), std::to_string(i + 1));

		for (int i = 0; i < 1000; i++)
			testString.remove(0);

		EXPECT_TRUE(testString.is_empty() == true);
		EXPECT_EQ(testString.size(), 0);
		for (int i = 0; i < 500; i++)
			testString.add(std::to_string(i));
		for (int i = 0; i < 500; i++)
			EXPECT_EQ(testString.get(i), std::to_string(i));
		testString.clear();
		EXPECT_TRUE(testString.is_empty());
	}//end string test


	//start struct test
	{
		struct Attributes {
			int len;
			bool condition;
		};
		ArrayList<Attributes> testStruct;
		Attributes attributes[1000];

		for (int i = 0; i < 1000; i++) {
			for (int j = 0; j < 2; j++) {
				attributes[i].len = i;
				attributes[i].condition = true;
			}
			testStruct.add(attributes[i]);
		}


		for (int i = 0; i < 1000; i++) {
			EXPECT_EQ(testStruct.get(i).len, i);
			EXPECT_EQ(testStruct.get(i).condition, true);
		}

		for (int i = 0; i < 1000; i++)
			testStruct.remove(0);

		EXPECT_TRUE(testStruct.is_empty() == true);
		EXPECT_EQ(testStruct.size(), 0);

		for (int i = 0; i < 500; i++)
			testStruct.add(attributes[i]);
		for (int i = 0; i < 500; i++) {
			EXPECT_EQ(testStruct.get(i).condition, true);
			EXPECT_EQ(testStruct.get(i).len, i);
		}

		testStruct.clear();

		EXPECT_TRUE(testStruct.is_empty());
	}//end struct test
}

void testLinkedList() {

	//start int test
	{
		LinkedList<int> testInt;
		testInt.add(5);
		testInt.add(6);
		testInt.add(2);
		testInt.clear();
		EXPECT_TRUE(testInt.is_empty());
		std::pair<int, int> pair1{ 1,2 };
		std::pair<int, int> pair2{ 2,3 };
		std::pair<int, int> pair3{ 4,5 };
		testInt.add(pair1);
		testInt.add(pair2);
		testInt.add(pair3);
		std::pair<int, int> pair_result = testInt.get_pair(2);
		EXPECT_EQ(pair_result.first, testInt.get_pair(2).first);
		testInt.clear();
		EXPECT_TRUE(testInt.is_empty());
		testInt.add(5);
		testInt.add(6);
		testInt.add(1);
		testInt.add(8);

		EXPECT_EQ(testInt.pop(), 8);
		EXPECT_EQ(testInt.pop(), 1);
		EXPECT_EQ(testInt.pop(), 6);
		EXPECT_EQ(testInt.size(), 1);

		testInt.remove(testInt.size() - 1);

		EXPECT_TRUE(testInt.is_empty());
		testInt.add(5);
		testInt.add(9);
		testInt.add(7);
		testInt.remove(1);

		EXPECT_EQ(testInt.size(), 2);
		EXPECT_EQ(testInt.get(1), 7);

		testInt.clear();
	}//end int test

	{
		LinkedList<std::string> list_string;
		for (size_t i = 0; i < 10000; i++)
			list_string.add(std::to_string(i));
		for (int i = 0; i < 10000; i++)
			EXPECT_EQ(list_string.get(i), std::to_string(i));
		list_string.clear();
		EXPECT_EQ(list_string.size(), 0);
		std::pair<std::string, int> pair{ "1",0 };
		std::pair<std::string, int> pair1{ "2", 1 };
		std::pair<std::string, int> pair2{ "3", 2 };
		list_string.add(pair);
		list_string.add(pair1);
		list_string.add(pair2);
		EXPECT_EQ(list_string.get_pair(0).first, "1");
		EXPECT_EQ(list_string.get_pair(2).first, "3");

		EXPECT_EQ(list_string.pop_pair().first, "3");
		EXPECT_EQ(list_string.pop_pair().second, 1);
		list_string.clear();
		EXPECT_EQ(list_string.size(), 0);
		std::pair<std::string, int> pair3{ "4",3 };
		std::pair<std::string, int> pair4{ "5", 4 };
		std::pair<std::string, int> pair5{ "6", 5 };
		list_string.add(pair3);
		list_string.add(pair4);
		list_string.add(pair5);
		list_string.remove(1);
		EXPECT_EQ(list_string.get_pair(0).first, "4");
		EXPECT_EQ(list_string.get_pair(1).first, "6");
		EXPECT_EQ(list_string.size(), 2);
	}

}

void testPriorityList() {
	//generate random priorities
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> urd(1, 1000);
	//start int test
	{
		PriorityList<int> list_int(list::PRIORITY::LOWEST);
		for (int i = 0; i < 1000; i++)
			list_int.queue_list(static_cast<int>(urd(mt)), i);
		int p_value = 0;
		for (int i = 0; i < 1000; i++) {
			list_int.get(p_value);
			list_int.deque_list();
			EXPECT_EQ(p_value, i);
		}
		EXPECT_EQ(list_int.size(), 0);

		PriorityList<int> list_int2(list::PRIORITY::LOWEST);
		for (int i = 1000; i >= 0; i--)
			list_int2.queue_list(static_cast<int>(urd(mt)), i);
		int j = 0;
		for (int i = 1000; i >= 0; i--) {
			list_int2.get(p_value);
			list_int2.deque_list();
			EXPECT_EQ(p_value, j);
			j++;
		}
		EXPECT_EQ(list_int.size(), 0);
	}//end int test

	//start string test
	{
		PriorityList<std::string> list_string{ list::PRIORITY::LOWEST };
		for (int i = 0; i < 1000; i++)
			list_string.queue_list(std::to_string(i), i);
		int priority;
		for (int i = 0; i < 1000; i++) {
			EXPECT_EQ(list_string.get(priority), std::to_string(i));
			EXPECT_EQ(priority, i);
			list_string.deque_list();
		}
		for (int i = 0; i < 1000; i++)
			list_string.queue_list(std::to_string(i), i);
	}//end string test
}

void testHuffmanAlgo() {
	huffman_algo huffman("this is why i love kung fu and i dont want anything to do with you");
}

TEST(TESTARRAYLIST, ARRAYLIST) {
	testArrayList();

}

TEST(TESTLINKEDLIST, LINKEDLIST) {
	testLinkedList();
}

TEST(TESTPRIORITYLIST, PRIORITYLIST) {
	//testPriorityList();
}
TEST(TESTZIPPY, HUFFMAN_ALGO) {
	testHuffmanAlgo();
}
TEST(TESTZIPPY, ZIPPY) {
	//const char * str = "C:/Users/Beloved/Documents/cool.txt";
	//const char * str2 = "C:/Users/Beloved/Documents/f.zippy";
	char* filefile = new char[strlen(__argv[1])];
	strcpy_s(filefile, sizeof __argv[1], __argv[1]);
	delete[] filefile;
	if (__argc != 2) {
		std::cerr << "panic: unable to process input";
	} 
	else if((file_size  = static_cast<size_t>(fs::file_size(__argv[1]))) < 2000) {
		
		compressor compressor_object = compressor(__argv[1] );
		compressor_object.compress();
	}
	else {
		std::cerr << "panic: content cant be handled";
	}

	
}
