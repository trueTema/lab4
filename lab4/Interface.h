#pragma once
#include <string>
#include "ISorter.h"
using namespace std;

typedef enum _CmdType { Input, Output, Sort, Exit, None, Time, Graphics, Help };



template<_CmdType Type, typename T, class _It>
struct command;

template<typename _ArgumentType>
struct Argument {
private:
	friend bool operator !=(const Argument<_ArgumentType>&, const Argument<_ArgumentType>&);
	friend bool operator ==(const Argument<_ArgumentType>&, const Argument<_ArgumentType>&);
	_ArgumentType value;
	char flag;
public:
	Argument() {
		this->flag = char();
		this->value = _ArgumentType();
	}
	Argument(char flag, _ArgumentType value) {
		this->flag = flag;
		this->value = value;
	}
	~Argument() {}
	void print() {
		std::cout << std::endl << '-' << this->flag << " " << this->value << std::endl;
	}
	char GetFlag() {
		return this->flag;
	}
	_ArgumentType Getvalue() {
		return this->value;
	}
	bool operator !=(const Argument<_ArgumentType>& other) {
		return !(this->flag == other.flag && this->value == other.value);
	}
	bool operator ==(const Argument<_ArgumentType>& other) {
		return (this->flag == other.flag && this->value == other.value);
	}
};

template<class _ArgumentType>
bool operator !=(const Argument<_ArgumentType>& cur, const Argument<_ArgumentType>& other) {
	return !(cur.flag == other.flag && cur.value == other.value);
}
template<class _ArgumentType>
bool operator ==(const Argument<_ArgumentType>& cur, const Argument<_ArgumentType>& other) {
	return (cur.flag == other.flag && cur.value == other.value);
}

namespace Traits {
	template<typename T>
	struct check_T {
		static char get_type() {
			return false;
		}
	};
	template<>
	struct check_T<int> {
		static bool get_type() {
			return true;
		}
	};
	template<>
	struct check_T<short int> {
		static bool get_type() {
			return true;
		}
	};
	template<>
	struct check_T<long long> {
		static bool get_type() {
			return true;
		}
	};

	template<typename T>
	struct _TypeCast {
		static T cast(std::string x) {
			try {
				return T(x);
			}
			catch (...) {
				throw SetException(IncorrectValue);
			}
		}
	};
	template<>
	struct _TypeCast<int> {
		static int cast(std::string x) {
			try {
				return std::stoi(x);
			}
			catch (...) {
				throw SetException(IncorrectValue);
			}
		}
	};
	template<>
	struct _TypeCast<double> {
		static double cast(std::string x) {
			try {
				return std::stof(x);
			}
			catch (...) {
				throw SetException(IncorrectValue);
			}
		}
	};
	template<>
	struct _TypeCast<string> {
		static string cast(std::string x) {
			return x;
		}
	};

	template<typename T>
	struct _Length {
		static size_t cast(T x) {
			try {
				return to_string(x).size();
			}
			catch (...) {
				throw SetException(IncorrectValue);
			}
		}
	};
	template<>
	struct _Length<string> {
		static size_t cast(string x) {
			return x.size();
		}
	};

	template<typename T>
	struct _ToString {
	public:
		static string cast(T x) {
			return to_string(x);
		}
	};
	template<>
	struct _ToString<SortType> {
	public:
		static string cast(SortType x) {
			switch (x) {
			case QuickSort:
				return "Quick Sort";
			case MergeSort:
				return "Merge Sort";
			case ShakerSort:
				return "Shaker Sort";
			case BitonicSort:
				return "Bitonic Sort";
			case BinaryInsertionSort:
				return "Binary Insertion Sort";
			default:
				throw SetException(IncorrectValue);
			}

		}
	};

	template<typename T, class _It>
	struct timing {
	public:
		static double get_time(size_t n, ISorter<T, ListIterators<T>>* srt) {
			double average = 0;
			for (int i = 0; i < 10; i++) {
				ListSequence<T> seq = random<T, ListIterators<T>>::generate(n);
				auto begin = std::chrono::system_clock::now();
				srt->sort(seq.begin(), seq.end());
				auto end = std::chrono::system_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
				average += elapsed_ms.count();
			}
			return average / 10;
		}
		static double get_time_sorted(size_t n, ISorter<T, ListIterators<T>>* srt) {
			double average = 0;
			for (int i = 0; i < 10; i++) {
				ListSequence<T> seq = random_sorted<T, ListIterators<T>>::generate(n);
				auto begin = std::chrono::system_clock::now();
				srt->sort(seq.begin(), seq.end());
				auto end = std::chrono::system_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
				average += elapsed_ms.count();
			}
			return average / 10;
		}
	};
	template<typename T>
	struct timing<T, ArrayIterators<T>> {
	public:
		static double get_time(size_t n, ISorter<T, ArrayIterators<T>>* srt) {
			double average = 0;
			for (int i = 0; i < 10; i++) {
				ArraySequence<T> seq = random<T, ArrayIterators<T>>::generate(n);
				auto begin = std::chrono::system_clock::now();
				srt->sort(seq.begin(), seq.end());
				auto end = std::chrono::system_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
				average += elapsed_ms.count();
			}
			return average / 10;
		}
		static double get_time_sorted(size_t n, ISorter<T, ArrayIterators<T>>* srt) {
			double average = 0;
			for (int i = 0; i < 10; i++) {
				ArraySequence<T> seq = random_sorted<T, ArrayIterators<T>>::generate(n);
				auto begin = std::chrono::system_clock::now();
				srt->sort(seq.begin(), seq.end());
				auto end = std::chrono::system_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
				average += elapsed_ms.count();
			}
			return average / 10;
		}
	};

	string remake_string(string x, size_t n) {
		while (x.size() < n) {
			x += ' ';
		}
		return x;
	}
}



template<typename T>
ArraySequence<Argument<T>>* _MakeArgumentList(string);
_CmdType _GetCommand(string);

template<typename T, class _It>
ArraySequence<Argument<string>>* _CmdListener(Sequence<T, _It>*);

