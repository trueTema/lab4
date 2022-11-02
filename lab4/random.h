#define ll long long

template<typename T>
struct random_unit {
	static T generate() {
		return T();
	}
};

template<>
struct random_unit<int> {
	static int generate() {
		return rand()%20000 - 10000;
	}
};

template<>
struct random_unit<long long> {
	static long long generate() {
		return rand() % 200000 - 100000;
	}
};

template<>
struct random_unit<char> {
	static char generate() {
		return char(rand()%90 + 32);
	}
};

template<>
struct random_unit<double> {
	static double generate() {
		return (double)random_unit<long long>::generate() / (double)(rand()%99 + 1);
	}
};

template<>
struct random_unit<bool> {
	static bool generate() {
		return rand()%2;
	}
};

template<>
struct random_unit<std::string> {
	static std::string generate() {
		std::string res;
		size_t len = rand() % 30;
		for (int i = 0; i < len; i++) {
			res += random_unit<char>::generate();
		}
		return res;
	}
};

template<class T, class _It>
struct random {
	static ListSequence<T>* generate(size_t size) {
		ListSequence<T>* res = new ListSequence<T>(size, T());
		for (typename ListIterators<T>::iterator i = res->begin(); i != res->end(); *i = random_unit<T>::generate(), i++);
		return res;
	}
};

template<class T>
struct random<T, ArrayIterators<T>> {
	static ArraySequence<T>* generate(size_t size) {
		ArraySequence<T>* res = new ArraySequence<T>(size, T());
		for (typename ArrayIterators<T>::iterator i = res->begin(); i != res->end(); *i = random_unit<T>::generate(), i++);
		return res;
	}
};

template<class T, class _It>
struct random_sorted {
	static ListSequence<T>* generate(size_t size) {
		ListSequence<T>* res = random<T, _It>::generate(size);
		return res;
	}
};

template<class _It>
struct random_sorted<int, _It> {
	static ListSequence<int>* generate(size_t size) {
		ListSequence<int>* res = new ListSequence<int>(size, int());
		*(res->begin()) = random_unit<int>::generate();
		for (typename ListIterators<int>::iterator i = res->begin() + 1; i != res->end(); *i = *(i - 1) + rand() % 100, i++);
		return res;
	}
};

template<class T>
struct random_sorted<T, ArrayIterators<T>> {
	static ArraySequence<T>* generate(size_t size) {
		ArraySequence<T>* res = random<T, ArrayIterators<T>>::generate(size);
		return res;
	}
};

template<>
struct random_sorted<int, ArrayIterators<int>> {
	static ArraySequence<int>* generate(size_t size) {
		ArraySequence<int>* res = new ArraySequence<int>(size, int());
		*(res->begin()) = random_unit<int>::generate();
		for (typename ArrayIterators<int>::iterator i = res->begin() + 1; i != res->end(); *i = *(i-1) + rand()%100, i++);
		return res;
	}
};
