#include "pch.h"
#include "CppUnitTest.h"
#include "..\lab4\ArraySequence.h"
#include "..\lab4\Sequence.h"
#include "..\lab4\ListSequence.h"
#include "..\lab4\Iterators.h"
#include "..\lab4\ISorter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(DynamicArrayTest)
		{
			int src1[10] = { 1,4,9,16,25,36,49,64,81,100 };
			int src2[3] = { 777,666,555 };
			DynamicArray<int> test1(src1, 10); DynamicArray<int> test1_1(src2, 3);
			double src3[7] = { 0.5, 0.66, 1.56, 12773.9865, -17.5, 0.22222, 9.78 };

			//test1
			test1 = *(test1.Concat(&test1_1));
			DynamicArray<int> expected1 = { 1,4,9,16,25,36,49,64,81,100,777,666,555 };
			Assert::IsTrue(expected1 == test1, L"DynamicArray: wrong answer, test 1");

			//test2
			test1_1 = *(test1.SplitArray([](int a) {return a > 15; }));
			DynamicArray<int> expected2_1 = { 16,25,36,49,64,81,100,777,666,555 };
			DynamicArray<int> expected2_2 = { 1,4,9 };
			Assert::IsTrue(expected2_1 == test1_1 && expected2_2 == test1, L"DynamicArray: wrong answer, test 2");

			//test3
			DynamicArray<double> test3(src3, 7);
			DynamicArray<double> test3_1 = *(test3.GetSubArray(test3.begin() + 1, test3.begin() + 6));
			DynamicArray<double> expected3 = { 0.66, 1.56, 12773.9865, -17.5, 0.22222 };
			Assert::IsTrue(expected3 == test3_1, L"DynamicArray: wrong answer, test 3");

			//test4
			DynamicArray<int> test4_1 = { 1,2,6,13,12,14,15,0,7,18,29 };
			DynamicArray<int>::iterator it1 = test4_1.find(test4_1.begin(), test4_1.end() - 2, 13);
			DynamicArray<int>::iterator it2 = test4_1.find(test4_1.begin(), test4_1.end() - 2, 4);
			Assert::IsTrue(it1 != test4_1.end() && it2 == test4_1.end(), L"DynamicArray: wrong answer, test 4");
			test4_1.InsertAt(28, test4_1.begin());
			test4_1.InsertAt(28, test4_1.begin() + 2);
			test4_1.del_item(test4_1.begin() + 1);
			DynamicArray<int> expected4 = { 28, 28, 2,6,13,12,14,15,0,7,18,29 };
			Assert::IsTrue(expected4 == test4_1, L"DynamicArray: wrong answer, test 5");

		}

		TEST_METHOD(LinkedListTest)
		{
			int src1[10] = { 1,4,9,16,25,36,49,64,81,100 };
			int src2[3] = { 777,666,555 };
			LinkedList<int> test1(src1, 10); LinkedList<int> test1_1(src2, 3);
			double src3[7] = { 0.5, 0.66, 1.56, 12773.9865, -17.5, 0.22222, 9.78 };

			//test1
			test1 = *(test1.Concat(&test1_1));
			LinkedList<int> expected1 = { 1,4,9,16,25,36,49,64,81,100,777,666,555 };
			Assert::IsTrue(expected1 == test1, L"DynamicArray: wrong answer, test 1");

			//test2
			test1_1 = *(test1.SplitList([](int a) {return a > 15; }));
			LinkedList<int> expected2_1 = { 16,25,36,49,64,81,100,777,666,555 };
			LinkedList<int> expected2_2 = { 1,4,9 };
			Assert::IsTrue(expected2_1 == test1_1 && expected2_2 == test1, L"DynamicArray: wrong answer, test 2");

			//test3
			LinkedList<double> test3(src3, 7);
			LinkedList<double> test3_1 = *(test3.GetSubList(test3.begin() + 1, test3.begin() + 6));
			LinkedList<double> expected3 = { 0.66, 1.56, 12773.9865, -17.5, 0.22222 };
			Assert::IsTrue(expected3 == test3_1, L"DynamicArray: wrong answer, test 3");

			//test4
			LinkedList<int> test4_1 = { 1,2,6,13,12,14,15,0,7,18,29 };
			LinkedList<int>::iterator it1 = test4_1.find(test4_1.begin(), test4_1.end() - 2, 13);
			LinkedList<int>::iterator it2 = test4_1.find(test4_1.begin(), test4_1.end() - 2, 4);
			Assert::IsTrue(it1 != test4_1.end() && it2 == test4_1.end(), L"DynamicArray: wrong answer, test 4");
			test4_1.InsertAt(28, test4_1.begin());
			test4_1.InsertAt(28, test4_1.begin() + 2);
			test4_1.del_item(test4_1.begin() + 1);
			LinkedList<int> expected4 = { 28, 28, 2,6,13,12,14,15,0,7,18,29 };
			Assert::IsTrue(expected4 == test4_1, L"DynamicArray: wrong answer, test 5");
		}
		TEST_METHOD(ShakerSortTest)
		{
			ArraySequence<int> seq = { 8,4,19,22,0,4,4,5,5,5,6,71,28 };
			ISorter<int, ArrayIterators<int>>* srt = nullptr;
			//test1
			srt = new Shaker_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test1(&seq);
			srt->sort(test1.begin(), test1.end(), cmp__up(int)); 
			ArraySequence<int> expected1 = { 0,4,4,4,5,5,5,6,8,19,22,28,71};
			Assert::IsTrue(expected1 == test1, L"ShakerSort: wrong answer, test 1");

			//test2
			srt = new Shaker_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test2(&seq);
			srt->sort(test2.begin(), test2.end(), cmp__down(int));
			ArraySequence<int> expected2 = { 71,28,22,19,8,6,5,5,5,4,4,4,0 };
			Assert::IsTrue(expected2 == test2, L"ShakerSort: wrong answer, test 2");

			//test3
			srt = new Shaker_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test3(&seq);
			srt->sort(test3.begin(), test3.end(), cmp__odd_even_up(int));
			ArraySequence<int> expected3 = { 0,4,4,4,6,8,22,28, 5,5,5,19,71 };
			Assert::IsTrue(expected3 == test3, L"ShakerSort: wrong answer, test 3");

			//test4
			srt = new Shaker_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test4(&seq);
			srt->sort(test4.begin(), test4.end(), cmp__digits_count_up(int));
			ArraySequence<int> expected4 = { 0, 8, 4, 4, 4, 5, 5, 5, 6, 19, 22, 71, 28 };
			Assert::IsTrue(expected4 == test4, L"ShakerSort: wrong answer, test 4");
		}
		TEST_METHOD(QuickSortTest)
		{
			ArraySequence<int> seq = { 8,4,19,22,0,4,4,5,5,5,6,71,28 };
			ISorter<int, ArrayIterators<int>>* srt = nullptr;
			//test1
			srt = new Quick_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test1(&seq);
			srt->sort(test1.begin(), test1.end(), cmp__up(int));
			ArraySequence<int> expected1 = { 0,4,4,4,5,5,5,6,8,19,22,28,71 };
			Assert::IsTrue(expected1 == test1, L"Quick_Sort: wrong answer, test 1");

			//test2
			srt = new Quick_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test2(&seq);
			srt->sort(test2.begin(), test2.end(), cmp__down(int));
			ArraySequence<int> expected2 = { 71,28,22,19,8,6,5,5,5,4,4,4,0 };
			Assert::IsTrue(expected2 == test2, L"Quick_Sort: wrong answer, test 2");

			//test3
			srt = new Quick_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test3(&seq);
			srt->sort(test3.begin(), test3.end(), cmp__odd_even_up(int));
			ArraySequence<int> expected3 = { 0, 4, 4, 4, 6, 8, 22, 28, 5, 5, 5, 19, 71 };
			Assert::IsTrue(expected3 == test3, L"Quick_Sort: wrong answer, test 3");

			//test4
			srt = new Quick_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test4(&seq);
			srt->sort(test4.begin(), test4.end(), cmp__digits_count_up(int));
			ArraySequence<int> expected4 = { 0, 4, 4, 8, 5, 4, 5, 5, 6, 19, 22, 71, 28 };
			Assert::IsTrue(expected4 == test4, L"Quick_Sort: wrong answer, test 4");

		}

		TEST_METHOD(MergeSortTest)
		{
			ArraySequence<int> seq = { 8,4,19,22,0,4,4,5,5,5,6,71,28 };
			ISorter<int, ArrayIterators<int>>* srt = nullptr;
			//test1
			srt = new Merge_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test1(&seq);
			srt->sort(test1.begin(), test1.end(), cmp__up(int));
			ArraySequence<int> expected1 = { 0,4,4,4,5,5,5,6,8,19,22,28,71 };
			Assert::IsTrue(expected1 == test1, L"MergeSort: wrong answer, test 1");

			//test2
			srt = new Merge_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test2(&seq);
			srt->sort(test2.begin(), test2.end(), cmp__down(int));
			ArraySequence<int> expected2 = { 71,28,22,19,8,6,5,5,5,4,4,4,0 };
			Assert::IsTrue(expected2 == test2, L"MergeSort: wrong answer, test 2");

			//test3
			srt = new Merge_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test3(&seq);
			srt->sort(test3.begin(), test3.end(), cmp__odd_even_up(int));
			ArraySequence<int> expected3 = { 0,4,4,4,6,8,22,28, 5,5,5,19,71 };
			Assert::IsTrue(expected3 == test3, L"MergeSort: wrong answer, test 3");

			//test4
			srt = new Merge_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test4(&seq);
			srt->sort(test4.begin(), test4.end(), cmp__digits_count_up(int));
			ArraySequence<int> expected4 = { 0, 8, 4, 4, 4, 5, 5, 5, 6, 19, 22, 71, 28 };
			Assert::IsTrue(expected4 == test4, L"MergeSort: wrong answer, test 4");
		}

		TEST_METHOD(BinaryInsertionSortTest)
		{
			ArraySequence<int> seq = { 8,4,19,22,0,4,4,5,5,5,6,71,28 };
			ISorter<int, ArrayIterators<int>>* srt = nullptr;
			//test1
			srt = new Binary_Insertion_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test1(&seq);
			srt->sort(test1.begin(), test1.end(), cmp__up(int));
			ArraySequence<int> expected1 = { 0,4,4,4,5,5,5,6,8,19,22,28,71 };
			Assert::IsTrue(expected1 == test1, L"BinaryInsertionSort: wrong answer, test 1");

			//test2
			srt = new Binary_Insertion_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test2(&seq);
			srt->sort(test2.begin(), test2.end(), cmp__down(int));
			ArraySequence<int> expected2 = { 71,28,22,19,8,6,5,5,5,4,4,4,0 };
			Assert::IsTrue(expected2 == test2, L"BinaryInsertionSort: wrong answer, test 2");

			//test3
			srt = new Binary_Insertion_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test3(&seq);
			srt->sort(test3.begin(), test3.end(), cmp__odd_even_up(int));
			ArraySequence<int> expected3 = { 0,4,4,4,6,8,22,28, 5,5,5,19,71 };
			Assert::IsTrue(expected3 == test3, L"BinaryInsertionSort: wrong answer, test 3");

			//test4
			srt = new Binary_Insertion_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test4(&seq);
			srt->sort(test4.begin(), test4.end(), cmp__digits_count_up(int));
			ArraySequence<int> expected4 = { 0, 8, 4, 4, 4, 5, 5, 5, 6, 19, 22, 71, 28 };
			Assert::IsTrue(expected4 == test4, L"BinaryInsertionSort: wrong answer, test 4");
		}
		TEST_METHOD(BitonicSortTest)
		{
			ArraySequence<int> seq = { 8,4,19,22,0,4,4,5,5,5,6,71,28,16,32,14 };
			ISorter<int, ArrayIterators<int>>* srt = nullptr;
			//test1
			srt = new Bitonic_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test1(&seq);
			srt->sort(test1.begin(), test1.end(), cmp__up(int));
			ArraySequence<int> expected1 = { 0,4,4,4,5,5,5,6,8,14,16,19,22,28,32,71 };
			Assert::IsTrue(expected1 == test1, L"BitonicSort: wrong answer, test 1");

			//test2
			srt = new Bitonic_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test2(&seq);
			srt->sort(test2.begin(), test2.end(), cmp__down(int));
			ArraySequence<int> expected2 = { 71,32,28,22,19,16,14,8,6,5,5,5,4,4,4,0 };
			Assert::IsTrue(expected2 == test2, L"BitonicSort: wrong answer, test 2");

			//test3
			srt = new Bitonic_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test3(&seq);
			srt->sort(test3.begin(), test3.end(), cmp__odd_even_up(int));
			ArraySequence<int> expected3 = { 0,4,4,4,6,8,14,16,22,28,32, 5,5,5,19,71 };
			Assert::IsTrue(expected3 == test3, L"BitonicSort: wrong answer, test 3");

			//test4
			srt = new Bitonic_Sort<int, ArrayIterators<int>>;
			ArraySequence<int> test4(&seq);
			srt->sort(test4.begin(), test4.end(), cmp__digits_count_up(int));
			ArraySequence<int> expected4 = { 0, 8, 4, 4, 4, 5, 5, 5, 6, 28, 14, 32, 71, 16, 22, 19 };
			Assert::IsTrue(expected4 == test4, L"BitonicSort: wrong answer, test 4");
		}
	};
}
