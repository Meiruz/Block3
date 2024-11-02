#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;

enum TFile {
	FileIn,
	FileOut
};

enum TErrors {
	FailFileOpen = 0,
	FailData,
	FailLimitOfData,
	FailStringEmpty,
	NotTXTFile
};
const string ERRORS[7]{
	"Неудалось открыть файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Некорректные данные данные. Попробуйте еще раз.",
	"Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз.",
	"Строка слишком короткая. Попробуйте еще раз.",
	"Расширение файла должно быть TXT. Попробуйте еще раз."
};

set<char> mathStringSet = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '=', '-',  '*', '/' };

void cinBufClean() {
	cin.clear();
	while (cin.get() != '\n');
}

void strToLow(string &str) {
	for (char &sym : str) {
		int asciiValue = (int)sym;
		if (asciiValue > 64 && asciiValue < 91)
			sym = (char)(asciiValue + 32);
	}
}

bool checkFileNotTxt(string pathToFile) {
	strToLow(pathToFile);

	int stringLength = pathToFile.length();

	if (stringLength < 4)
		return true;
	return !((pathToFile[stringLength - 1] == 't') &&
		(pathToFile[stringLength - 2] == 'x') &&
		(pathToFile[stringLength - 3] == 't') &&
		(pathToFile[stringLength - 4] == '.'));
}

void openFile(fstream &file, TFile fileType) {
	bool isFail;

	do {
		isFail = false;

		cout << "\nВведите путь к файлу .txt для " <<
			(fileType == FileIn ? "ввода" : "вывода") << " данных : " << endl;
		string filename;
		getline(cin, filename);

		if (checkFileNotTxt(filename)) {
			cout << ERRORS[NotTXTFile];
			isFail = true;
		}
		else {
			file.open(filename, fileType == FileIn ? ios::in : ios::app);
			if (!file.is_open()) {
				cout << ERRORS[FailFileOpen];
				file.close();
				isFail = true;
			}
		}
	} while (isFail);
}

void consoleInputWithChecking(int &value, const int MAX_LIMIT, const int MIN_LIMIT) {
	bool isFail;

	do {
		isFail = false;
		cin >> value;

		if (cin.fail() || cin.get() != '\n') {
			cout << ERRORS[FailData] << endl;
			cinBufClean();
			isFail = true;
		}
		else if (value > MAX_LIMIT || value < MIN_LIMIT) {
			cout << ERRORS[FailLimitOfData] << endl;
			isFail = true;
		}
	} while (isFail);
}

void inputDataWithConsole(string &mainString) {
	cout << "Введите строку: " << endl;
	getline(cin, mainString);

	while (mainString.length() == 0) {
		cout << ERRORS[FailStringEmpty] << endl;
		getline(cin, mainString);
	}
}

bool inputDataWithFile(string &mainString) {
	fstream fin;
	openFile(fin, FileIn);

	getline(fin, mainString);

	fin.close();
	if (mainString.length() == 0)
		cout << ERRORS[FailStringEmpty] << endl;

	return (mainString.length() == 0);
}

void inData(string &mainString, const int inType) {
	if (inType == 1)
		inputDataWithConsole(mainString);
	else
		while (inputDataWithFile(mainString));
}

string getClearString(string mainString) {
	string newString;

	for (char sym : mainString) {
		if (mathStringSet.count(sym))
			newString += sym;
	}

	return newString;
}

void outInformation(string answer, int outType) {
	fstream fout;

	if (outType == 2) {
		openFile(fout, FileOut);
		fout << endl << "[Result Task #2]" << endl;
	}

	if (answer.length() == 0) {
		cout << "Строка пуста." << endl;
		if (outType == 2)
			fout << "String is empty." << endl;
	}
	else {
		cout << "Результат: " << answer << endl;
		if (outType == 2)
			fout << "Result: " << answer << endl;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	cout << "Программа для получения множеста элементов строки, являющимися числами или знаками мат. операций" << endl; \

		string mainString;

	int inType = 0;
	cout << "Введите предпочитаемый тип ввода данных:" << endl;
	cout << "\t1 - из консоли,\n\t2 - из файла." << endl;
	consoleInputWithChecking(inType, 2, 1);

	inData(mainString, inType);

	int outType;
	cout << "Введите предпочитаемый тип вывода данных:" << endl;
	cout << "\t1 - только в консоли,\n\t2 - в консоль и в файл." << endl;
	consoleInputWithChecking(outType, 2, 1);

	outInformation(getClearString(mainString), outType);

	cout << "Для выхода из программы нажмите Enter...";
	cin.get();

	return 0;
}