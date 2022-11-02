#pragma once
#include <algorithm>
#include "Interface.h"
#include "ISorter.h"
#include <string>
#include <vector>
#include <thread>
#include <future>
#include "graphics.h"
using namespace Traits;



template<_CmdType Type, typename T, class _It>
struct command {
public:
	static Sequence<T, _It>* execute(Sequence<T, _It>* vec, ArraySequence<Argument<string>>* argList) {
		throw SetException(UnknownCommand);
		return vec;
	}
};

template<typename T, class _It>
struct command<Help, T, _It> {
public:
	static void execute() {
		cout << "\n\n[System] Набор возможных команд:\n({} - обязательные параметры, [] - необязательные)\n\n";
		cout << "\tinput {-r} [-s] {-n}\t-отвечает за создание последовательности";
		cout << "\n\t\t-r сгенерировать случайную последовательность(число)";
		cout << "\n\t\t-s указать тип последовательности (array, list)";
		cout << "\n\t\t-n указать длину последовательности (число) - взаимоисклчающее с -r\n\n";
		cout << "\tprint\t- вывод последовательности, созданной ранее";
		cout << "\tsort {-a} [-s] [-f] [-p]\t-отвечает за сортировку последовательности";
		cout << "\n\t\t-a тип сортировки (quick_sort, merge_sort, binary_insertion_sort, bitonic_sort, shaker_sort)";
		cout << "\n\t\t-s указывает начало участка сортировки (число)";
		cout << "\n\t\t-f указывает конец участка сортировки (число)";
		cout << "\n\t\t-p параметр сортировки\n\n";
		cout << "\ttime {-a} [-s]\t-замер времени работы сортировок";
		cout << "\n\t\t-a тип сортировки (quick_sort, merge_sort, binary_insertion_sort, bitonic_sort, shaker_sort) (возможно несколько)";
		cout << "\n\t\t-s указать тип последовательности (array, list)\n\n";
		cout << "\tgraphics {-a} [-s]\t-построение графика времени работы сортировок";
		cout << "\n\t\t-a тип сортировки (quick_sort, merge_sort, binary_insertion_sort, bitonic_sort, shaker_sort)";
		cout << "\n\t\t-s указать тип последовательности (array, list)\n\n";
		cout << "\thelp\t-список всех команд.\n\n";
	}
};

template<typename T, class _It>
struct command<Graphics, T, _It> {
private:
	static void checktime(ISorter<T, _It>* srt) {
		size_t number_of_clocking = 0;
		for (size_t i = 0; i < 20; i++) {
			arr_x[i] = -1;
			arr_y[i] = -1;
		}
		for (size_t num = (srt->GetType() == BitonicSort ? 64 : 1000); num < (srt->GetType() == BitonicSort ? 35000 : 30000); num += (srt->GetType() == BitonicSort ? num : srt->GetType() == ShakerSort ? 3000 : 1500)) {
			double x = 0;
			x = timing<T, _It>::get_time(num, srt);
			//cout << num << " " << x << endl;
			//cout << "\n=======\n";
			arr_x[number_of_clocking] = num;
			arr_y[number_of_clocking] = x;
			number_of_clocking++;
		}

	}
public:
	static Sequence<T, _It>* execute( ArraySequence<Argument<string>>* argList) {
		ISorter<T, _It>* srt = nullptr;
		for (size_t i = 0; i < argList->GetLength(); i++) {
			Argument<string> cur = argList->Get(i);
			string val = cur.Getvalue();
			switch (cur.GetFlag())
			{
			case 'a':
				if (srt) {
					throw SetException(MutuallyExclusive);
				}
				if (val == "quick_sort") {
					srt = new Quick_Sort<T, _It>;
				}
				else if (val == "merge_sort") {
					srt = new Merge_Sort<T, _It>;
				}
				else if (val == "bitonic_sort") {
					srt = new Bitonic_Sort<T, _It>;
				}
				else if (val == "shaker_sort") {
					srt = new Shaker_Sort<T, _It>;
				}
				else if (val == "binary_insertion_sort") {
					srt = new Binary_Insertion_Sort<T, _It>;
				}
				else {
					throw SetException(IncorrectValue);
				}
				break;
			default:
				if (cur.GetFlag() != 's') {
					throw SetException(UnknownFlag);
				}
				break;
			}
		}
		if (!srt) throw SetException(NoRequiredArgument);
		checktime(srt);
		init();
	}
};


template<typename T, class _It>
struct command<Time, T, _It> {
	static void checktime(ISorter<T, _It>* srt, bool sorted) {
		for (size_t num = (srt->GetType() == BitonicSort ? 64 : 1000); num < (srt->GetType() == BitonicSort ? 35000 : 30000); num += (srt->GetType() == BitonicSort ? num : 3000)) {
			double x = 0;
			x = sorted ? timing<T, _It>::get_time_sorted(num, srt) : timing<T, _It>::get_time(num, srt);
			
			if (x > 1000) {
				cout.precision(1);
				cout << fixed << x / 1000 << "\t";
			}
			else {
				cout.precision(3);
				cout << fixed << x / 1000 << "\t";
			}
		}
	}
public:
	static void execute(ArraySequence<Argument<string>>* argList) {
		ArraySequence<ISorter<T, _It>*> sorts_list;
		for (int i = 0; i < argList->GetLength(); i++) {
			Argument<string> cur = argList->Get(i);
			if (cur.GetFlag() == 'a') {
				ISorter<T, _It>* srt = nullptr;
				string val = cur.Getvalue();
				if (val == "quick_sort") {
					srt = new Quick_Sort<T, _It>;
				}
				else if (val == "merge_sort") {
					srt = new Merge_Sort<T, _It>;
				}
				else if (val == "bitonic_sort") {
					srt = new Bitonic_Sort<T, _It>;
				}
				else if (val == "shaker_sort") {
					srt = new Shaker_Sort<T, _It>;
				}
				else if (val == "binary_insertion_sort") {
					srt = new Binary_Insertion_Sort<T, _It>;
				}
				else {
					throw SetException(IncorrectValue);
				}
				if (sorts_list.GetLength() > std::thread::hardware_concurrency()) {
					throw SetException(TooManySort);
				}
				sorts_list.Append(srt);
			}
			else if (cur.GetFlag() != 's') {
				throw SetException(UnknownFlag);
			}
		}
		if (sorts_list.GetLength() == 0) {
			throw SetException(NoRequiredArgument);
		}
		system("cls");
		cout << "\n\n" << remake_string("Size", 20) << "\t";
		for (size_t num = (sorts_list[0]->GetType() == BitonicSort ? 64 : 1000); num < (sorts_list[0]->GetType() == BitonicSort ? 35000 : 30000); num += (sorts_list[0]->GetType() == BitonicSort ? num : 3000))
			cout << remake_string(to_string(num), 6) << "\t";
		cout << "\n\n";
		for (int i = 0; i < sorts_list.GetLength(); i++) {
			if (i != 0 && sorts_list[i]->GetType() == BitonicSort) {
				cout << "\n\t=========================================\n";
				cout << "\n\n" << remake_string("Size", 20) << "\t";
				for (size_t num = 64; num < 35000; num += num)
					cout << remake_string(to_string(num), 6) << "\t";
				cout << "\n\n";

			}
			cout << remake_string(_ToString<SortType>::cast(sorts_list[i]->GetType()), 20) << "\t";
			checktime(sorts_list[i], false);
			cout << "\n\n<sorted>\t\t";
			checktime(sorts_list[i], true);

			cout << "\n\n";
		}
		cout << "\n\n";
		system("pause");
	}

};

template<typename T, class _It>
struct input {
public:
	static ListSequence<T>* execute(size_t n) {
		cout << "\nEnter sequence values:\n>> ";
		ListSequence<T>* vec = new ListSequence<T>(n, T());
		for (typename ListIterators<T>::iterator i = vec->begin(); i != vec->end(); i++) {
			T cur = T();
			cin >> cur;
			*i = cur;
		}
		cin.clear();
		while (cin.get() != '\n');
		//cin.get();
		return vec;
	}
};

ostream& operator <<(ostream& os, const string& x) {
	os << '\"';
	for (auto i : x) {
		os << i;
	}
	os << '\"';
	return os;
}

template<typename T>
struct input<T, ArrayIterators<T>> {
public:
	static ArraySequence<T>* execute(size_t n) {
		cout << "\nEnter sequence values:\n>> ";
		ArraySequence<T>* vec = new ArraySequence<T>(n, T());

		for (typename ArrayIterators<T>::iterator i = vec->begin(); i != vec->end(); i++) {
			T cur = T();
			cin >> cur;
			*i = cur;
		}
		cin.clear();
		while (cin.get() != '\n');
		//cin.get();
		return vec;
	}
};



template<typename T, class _It>
struct command<Output, T, _It> {
public:
	static Sequence<T, _It>* execute(Sequence<T, _It>* vec, ArraySequence<Argument<string>>* argList) {
		if (vec == nullptr) {
			throw SetException(EmptySequence);
		}
		cout << "\n{ ";
		size_t len = 0;
		bool printed = false;
		for (typename _It::iterator i = vec -> begin(); i != vec->end(); i++) {
			if (len < 40 || i == vec->end() - 1) {
				cout << *i;
				if (i != vec->end() - 1) cout << ',';
				cout << " ";
				len += _Length<T>::cast(*i);
			}
			if (len >= 40 && !printed) {
				cout << "..., ";
				printed = true;
			}
			
		}
		cout << "}\n";
		return vec;
	}
};

template<typename T, class _It>
struct command<Sort, T, _It> {
private:
	static const size_t arg_num = 3;
public:
	static Sequence<T, _It>* execute(Sequence<T, _It>* vec, ArraySequence<Argument<string>>* argList) {
		if (vec == nullptr) {
			throw SetException(EmptySequence);
		}
		int st = 1;
		int fin = vec->GetLength();
		bool (*sort_type)(T, T) = cmp__up(T);
		ISorter<T, _It>* srt = nullptr;
		for (int i = 0; i < argList->GetLength(); i++) {
			Argument<string> cur = (*argList)[i];
			string val = cur.Getvalue();
			switch (cur.GetFlag()) {
			case 'a':
				if (val == "quick_sort") {
					srt = new Quick_Sort<T, _It>;
				}
				else if (val == "merge_sort") {
					srt = new Merge_Sort<T, _It>;
				}
				else if (val == "bitonic_sort") {
					srt = new Bitonic_Sort<T, _It>;
				}
				else if (val == "shaker_sort") {
					srt = new Shaker_Sort<T, _It>;
				}
				else if (val == "binary_insertion_sort") {
					srt = new Binary_Insertion_Sort<T, _It>;
				}
				else {
					throw SetException(IncorrectValue);
				}
				break;
			case 's':
				st = _TypeCast<int>::cast(val);
				break;
			case 'f':
				fin = _TypeCast<int>::cast(val);
				break;

			case 'p':

				if (val == "inc") {
					sort_type = cmp__up(T);
				}
				else if (val == "r_inc") {
					sort_type = cmp__down(T);
				}
				else if (val == "len") {
					sort_type = cmp__digits_count_up(T);
				}
				else if (val == "r_len") {
					sort_type = cmp__digits_count_down(T);
				}
				else if (val == "sum") {
					sort_type = cmp__digits_sum_up(T);
				}
				else if (val == "r_sum") {
					sort_type = cmp__digits_sum_down(T);
				}
				else if (val == "odd") {
					sort_type = cmp__odd_even_up(T);
				}
				else if (val == "r_odd") {
					sort_type = cmp__odd_even_down(T);
				}
				else if (val == "even") {
					sort_type = cmp__even_odd_up(T);
				}
				else if (val == "r_even") {
					sort_type = cmp__even_odd_down(T);
				}
				else {
					throw SetException(IncorrectValue);
				}
				break;
			default:
				throw SetException(UnknownFlag);
			}
		}
		if (!srt) throw SetException(NoRequiredArgument);
		srt->sort(vec->begin() + st - 1, vec->begin() + fin, *sort_type);
		cout << "\n\n[System] Succsessfully sorted.\n";
		return vec;
	}
};



template<typename T, class _Iterators>
Sequence<T, _Iterators>* cmd(_CmdType cmd, Sequence<T, _Iterators>* vec, ArraySequence<Argument<string>>* argList) {
	SequenceType type = ArrayType;
	switch (cmd) {
	case Output:
		return command<Output, T, _Iterators>::execute(vec, argList);
		break;
	case Sort:
		return command<Sort, T, _Iterators>::execute(vec, argList);
		break;
	case Exit:
		return command<Exit, T, _Iterators>::execute(vec, argList);
		break;
	case Time:
		for (int i = 0; i < argList->GetLength(); i++) {
			Argument<string> cur = argList->Get(i);
			if (cur.GetFlag() == 's') {
				if (cur.Getvalue() == "array") {
					type = ArrayType;
				}
				else if (cur.Getvalue() == "list") {
					type = ListType;
				}
				else {
					throw SetException(IncorrectValue);
				}
			}
		}
		if (type == ArrayType) {
			command<Time, T, ArrayIterators<T>>::execute(argList);
			return vec;
		} 
		command<Time, T, ListIterators<T>>::execute(argList);
		return vec;
		break;
	case Graphics:
		for (int i = 0; i < argList->GetLength(); i++) {
			Argument<string> cur = argList->Get(i);
			if (cur.GetFlag() == 's') {
				if (cur.Getvalue() == "array") {
					type = ArrayType;
				}
				else if (cur.Getvalue() == "list") {
					type = ListType;
				}
				else {
					throw SetException(IncorrectValue);
				}
			}
		}
		if (type == ArrayType) {
			command<Graphics, T, ArrayIterators<T>>::execute(argList);
			return vec;
		}
		command<Graphics, T, ListIterators<T>>::execute(argList);
		return vec;
		break;
	case Help:
		command<Help, T, ListIterators<T>>::execute();
		return vec;
		break;
	default:
		return command<None, T, _Iterators>::execute(vec, argList);
		break;
	}
}

