#include <iostream>
#include<vector>
#include<set>
#include "AVLTree.h"
#include "SimpleBenchTool4cpp/runtime_assert.hpp"
#include "SimpleBenchTool4cpp/timer.hpp"

//测试规模
const int testNum = 1000000;

//验证内容是否一致
void validSequence(AVLTree& avl, std::set<int>& rb)
{
	auto avlIter = avl.begin();
	auto rbIter = rb.begin();
	while (avlIter != avl.end())
	{
		assert(*avlIter == *rbIter);
		++avlIter;
		++rbIter;
	}
	assert(rbIter == rb.end());
}

//产生随机序列
std::vector<int> genRandomNum()
{
	srand(time(NULL));
	std::vector<int> ret;
	for (int i = 0; i < testNum; i++)
	{
		ret.push_back(rand() % testNum);
	}
	return ret;
}

//测试插入操作
void testAVLTreeInsert(AVLTree& avl, std::set<int>& rb, std::vector<int>& src)
{
	{
		Timer t;
		for (auto n : src)
		{
			avl.insert(n);
		}
	}
	{
		Timer t;
		for (auto n : src)
		{
			rb.insert(n);
		}
	}
	validSequence(avl, rb);
}

//测试遍历操作
void testAVLTreeIter(AVLTree& avl, std::set<int>& rb)
{
	int n1, n2;
	{
		Timer t;
		for (int num : avl)
		{
			n1 = num;
		}
	}
	{
		Timer t;
		for (int num : rb)
		{
			n2 = num;
		}
	}
	assert(n1 == n2);
}

//测试查找操作
void testAVLTreeFind(AVLTree& avl, std::set<int>& rb, std::vector<int>& src)
{
	assert(avl.size() == rb.size());
	{
		Timer t;
		for (auto&& n : src)
		{
			assert(avl.find(n));
		}
	}
	{
		Timer t;
		for (auto&& n : src)
		{
			assert(rb.find(n) != rb.end());
		}
	}
}

//测试删除操作
void testAVLErase(AVLTree& avl, std::set<int>& rb, std::vector<int>& src)
{
	{
		Timer t;
		for (int n : src)
		{
			avl.remove(n);
		}
	}
	{
		Timer t;
		for (int n : src)
		{
			rb.erase(n);
		}
	}
}

int main()
{
	AVLTree avl;
	std::set<int> rb;
	auto randomTestNum = genRandomNum();
	std::cout << "-----------------insert--------------" << std::endl;
	testAVLTreeInsert(avl, rb, randomTestNum);
	std::cout << "-----------------iter--------------" << std::endl;
	testAVLTreeIter(avl, rb);
	std::cout << "-----------------find--------------" << std::endl;
	testAVLTreeFind(avl, rb, randomTestNum);
	std::cout << "-----------------erase--------------" << std::endl;
	testAVLErase(avl, rb, randomTestNum);
}
