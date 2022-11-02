#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "exception.h"
#include "Sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "Iterators.h"
#include <vector>
#include "random.h"
#include <list>
#include <chrono>
#include "Command.h"



using namespace Traits;


template<typename T>
ArraySequence<Argument<T>>* _MakeArgumentList(string s, bool canBeExclusive) {
	ArraySequence<Argument<T>>* res = new ArraySequence<Argument<T>>;
	string restricted = "!-=+*&^%$#";
	size_t current = s.find('-', 0);
	while (current != -1) {
		if (current == -1) break;
		if (current + 1 >= s.size() || s[current + 2] != ' ') throw SetException(IncorrectInputFormat);
		char flag = s[current + 1];
		current += 2;
		while (current < s.size() && s[current] == ' ') {
			if (s[current] != ' ') throw SetException(IncorrectInputFormat);
			current++;
		}
		if (current >= s.size()) throw SetException(IncorrectInputFormat);
		string arg_value;
		while (current < s.size() && s[current] != ' ') {
			if (restricted.find(s[current]) != -1) throw SetException(IncorrectInputFormat);
			arg_value += s[current];
			current++;
		}
		current = s.find('-', current + 1);
		Argument<T> cur(flag, _TypeCast<T>::cast(arg_value));
		for (ArraySequence<Argument<string>>::iterator i = res->begin(); (i != res->end()) && !canBeExclusive; i++) {
			if ((*i).GetFlag() == cur.GetFlag()) {
				throw SetException(MutuallyExclusive);
			}
		}
		if (res->find(res->begin(), res->end(), cur) == res->end()) {
			res->Append(cur);
		}
	}
	return res;
}

_CmdType _GetCommand(string s) {
	string res;
	for (int i = 0; i < s.size() && i < s.find(' '); i++) {
		res += s[i];
	}
	if (res == "input") {
		return Input;
	}
	else if (res == "print") {
		return Output;
	}
	else if (res == "sort") {
		return Sort;
	}
	else if (res == "time") {
		return Time;
	}
	else if (res == "exit") {
		return Exit;
	}
	else if (res == "graphics") {
		return Graphics;
	}
	else if (res == "help") {
		return Help;
	}
	return None;
}

template<typename T, class _It>
ArraySequence<Argument<string>>* _CmdListener(Sequence<T, _It>* vec) {
	string s;
	cout << "\n[System] Enter command:\n>> ";
	getline(cin, s);
	try {
		_CmdType command_number = _GetCommand(s);
		ArraySequence<Argument<string>>* argList = _MakeArgumentList<string>(s, command_number == Time);
		while (command_number != Input && command_number != Exit) {
			vec = cmd<T, _It>(command_number, vec, argList);
			cout << "\n[System] Enter command:\n>> ";
			getline(cin, s);
			command_number = _GetCommand(s);
			//delete argList;
			argList = _MakeArgumentList<string>(s, command_number == Time);
		}
		if (command_number == Input) {
			return argList;
		}
		else {
			system("cls");
			cout << "\n\n\tGoodbye!\n\n";
			return nullptr;
		}
	}
	catch (SetException e) {
		cout << "[Error] " << e.message() << "\n";
		return _CmdListener<T, _It>(vec);
	}
}

bool _StartMenuPoint() {
	//сюда в любом случае попадаем из _CmdListener, но если надо создавать новую хуйню, то пересоздаём 
	//и заходим в бесконечный цикл работы с этой последовательностью, но если в нём ввели input,
	//то запускаем функцию заного c ввода.
	cout << "\n\n\tПривет! Для ознакомления со списком команд введи help.\n\n";

	ArraySequence<int>* nullvec = nullptr;
	ArraySequence<Argument<string>>* argList = nullptr;
	try {
		argList = _CmdListener<int>(nullvec);
	}
	catch (SetException e) {
		cout << "[Error] " << e.message() << "\n";
		argList = _CmdListener<int>(nullvec);
	}
	while (true) {
		if (argList == nullptr) {
			return false;
		}
		string containerType = "array";
		string type = "integer";
		size_t n = 0;
		bool n_checked = false;
		bool isRandom = false;
		for (int i = 0; i < argList->GetLength(); i++) {
			Argument<string> cur = (*argList)[i];
			if (cur.GetFlag() == 't') {
				type = cur.Getvalue();
			}
			else if (cur.GetFlag() == 'n') {
				n_checked = true;
				n = _TypeCast<int>::cast(cur.Getvalue());
			}
			else if (cur.GetFlag() == 's') {
				containerType = cur.Getvalue();
			}
			else if (cur.GetFlag() == 'r') {
				isRandom = true;
				n = _TypeCast<int>::cast(cur.Getvalue());
			}
			else {
				throw SetException(UnknownFlag);
			}
		}
		if (n_checked && isRandom) throw SetException(MutuallyExclusive);
		if (!n_checked && !isRandom) {
			throw SetException(NoRequiredArgument);
		}
		if (containerType == "array") {
			if (type == "integer") {
				ArraySequence<int>* vec = nullptr;
				if (!isRandom) {
					vec = input<int, ArrayIterators<int>>::execute(n);
				}
				else {
					vec = random<int, ArrayIterators<int>>::generate(n);
				}
				argList = _CmdListener<int, ArrayIterators<int>>(vec);
				delete vec;
			}
			else if (type == "float") {
				ArraySequence<double>* vec = nullptr;
				if (!isRandom) {
					vec = input<double, ArrayIterators<double>>::execute(n);
				}
				else {
					vec = random<double, ArrayIterators<double>>::generate(n);
				}
				argList = _CmdListener<double, ArrayIterators<double>>(vec);
				delete vec;
			}
			else if (type == "string") {
				ArraySequence<string>* vec = nullptr;
				if (!isRandom) {
					vec = input<string, ArrayIterators<string>>::execute(n);
				}
				else {
					vec = random<string, ArrayIterators<string>>::generate(n);
				}
				argList = _CmdListener<string, ArrayIterators<string>>(vec);
				delete vec;
			}
			else {
				throw SetException(IncorrectValue);
			}
		}
		else if (containerType == "list") {
			if (type == "integer") {
				ListSequence<int>* vec = nullptr;
				if (!isRandom) {
					vec = input<int, ListIterators<int>>::execute(n);
				}
				else {
					vec = random<int, ListIterators<int>>::generate(n);
				}
				argList = _CmdListener<int, ListIterators<int>>(vec);
				delete vec;
			}
			else if (type == "float") {
				ListSequence<double>* vec = nullptr;
				if (!isRandom) {
					vec = input<double, ListIterators<double>>::execute(n);
				}
				else {
					vec = random<double, ListIterators<double>>::generate(n);
				}
				argList = _CmdListener<double, ListIterators<double>>(vec);
				delete vec;
			}
			else if (type == "string") {
				ListSequence<string>* vec = nullptr;
				if (!isRandom) {
					vec = input<string, ListIterators<string>>::execute(n);
				}
				else {
					vec = random<string, ListIterators<string>>::generate(n);
				}
				argList = _CmdListener<string, ListIterators<string>>(vec);
				delete vec;
			}
			else {
				throw SetException(IncorrectValue);
			}
		}
		else {
			throw SetException(IncorrectValue);
		}
	}
}


int main() {
	srand(time(0));
	setlocale(LC_ALL, "Russian");
	bool check = true;
	while (check) {
		try {
			check = _StartMenuPoint();
		}
		catch (SetException e) {
			cout << "[Error] " << e.message() << "\n";
		}
	}
	return 0;
}