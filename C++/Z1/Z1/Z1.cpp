#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum TFile {
	FileIn,
	FileOut
};

enum TErrors {
	FailFileOpen = 0,
	FailFileCreateOrOpen,
	FailData,
	FailLimitOfData,
	FailStringEmpty,
	FailStringsLengths,
	NotTXTFile
};
const string ERRORS[7]{
	"Неудалось открыть файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Неудалось открыть или создать файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Некорректные данные данные. Попробуйте еще раз.",
	"Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз.",
	"Строка слишком короткая. Попробуйте еще раз.",
	"Подстрока не может быть длиннее строки. Попробуйте еще раз.",
	"Расширение файла должно быть TXT. Попробуйте еще раз."
};

const int MAX_LIMIT = 256;
const int MIN_LIMIT = 1;

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

bool checkStringToSym(string str) {
	for (char sym : str)
		if ((int)sym == (char)1)
			return false;
	return true;
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

void inputDataWithConsole(string &mainString, string &subString, int &indexOfIn) {
	cout << "Введите строку: " << endl;
	getline(cin, mainString);
	while (mainString.length() == 0 || !checkStringToSym(mainString)) {
		cout << ERRORS[FailStringEmpty] << endl;
		getline(cin, mainString);
	}

	cout << "Введите подстроку: " << endl;
	getline(cin, subString);
	while (subString.length() == 0 || subString.length() > mainString.length() || !checkStringToSym(mainString)) {
		cout << (subString.length() == 0 ? ERRORS[FailStringEmpty]
			: ERRORS[FailStringsLengths]) << endl;
		getline(cin, subString);
	}

	cout << "Введите номер вхождения подстроки в строку от "
		<< MAX_LIMIT << " до " << MAX_LIMIT << ": " << endl;
	consoleInputWithChecking(indexOfIn, MAX_LIMIT, MIN_LIMIT);
}

bool fileInputWithChecking(fstream &fin, int &num, const int MAX_LIMIT_NUM, const int MIN_LIMIT_NUM) {
	bool osFail = false;

	fin >> num;

	if (fin.fail())
		return true;
	if (num > MAX_LIMIT_NUM || num < MIN_LIMIT_NUM)
		return true;
	return false;
}

bool inputDataWithFile(string &mainString, string &subString, int &indexOfIn) {
	fstream fin;
	openFile(fin, FileIn);

	getline(fin, mainString);
	getline(fin, subString);

	bool inFailStatus = fileInputWithChecking(fin, indexOfIn, MAX_LIMIT, MIN_LIMIT);

	if (inFailStatus || mainString.length() == 0 || subString.length() == 0) {
		cout << ERRORS[FailData];
		fin.close();
		return true;
	}

	fin.close();
	return !(checkStringToSym(mainString) && checkStringToSym(subString));
}

void inData(string &mainString, string &subString, int &indexOfIn, const int inType) {
	if (inType == 1)
		inputDataWithConsole(mainString, subString, indexOfIn);
	else
		while (inputDataWithFile(mainString, subString, indexOfIn));
}

int *prefixFunction(const string &str) {
	int *prefixArray = new int[str.length()] {0};

	for (int i = 1; i < str.length(); i++) {
		int j = prefixArray[i - 1];

		while (j > 0 && str[i] != str[j]) {
			j = prefixArray[j - 1];
		}

		if (str[i] == str[j])
			prefixArray[i] = j + 1;
		else
			prefixArray[i] = j;
	}

	return prefixArray;
}

int findIndexOfIn(string mainString, string subString, int positionOfIn) {
	int i = 0;
	int *prefixArray = prefixFunction(subString + char(1) + mainString);

	int lengthOfString = subString.length() + 1 + mainString.length();
	for (i = 0; i < lengthOfString; i++)
		if (prefixArray[i] == subString.length() && --positionOfIn == 0)
			return i - 2 * subString.length() + 1;
	return -1;
}

void outInformation(int position, int outType) {
	fstream fout;

	if (outType == 2) {
		openFile(fout, FileOut);
		fout << endl << "[Result Task #1]" << endl;
	}

	if (position == -1) {
		cout << "Нет вхождения подстроки в строку под таким номером." << endl;
		if (outType == 2)
			fout << "There is no occurrence of a substring in a string with this number." << endl;
	}
	else {
		cout << "Позиция вхождения: " << position << endl;
		if (outType == 2)
			fout << "Position of occurrence: " << position << endl;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	cout << "Программа для нахождения m-ого вхождения подстроки в строку." << endl;

	string mainString;
	string subString;
	int indexOfIn;

	int inType = 0;
	cout << "Введите предпочитаемый тип ввода данных:" << endl;
	cout << "\t1 - из консоли,\n\t2 - из файла." << endl;
	consoleInputWithChecking(inType, 2, 1);

	inData(mainString, subString, indexOfIn, inType);

	int outType;
	cout << "Введите предпочитаемый тип вывода данных:" << endl;
	cout << "\t1 - только в консоли,\n\t2 - в консоль и в файл." << endl;
	consoleInputWithChecking(outType, 2, 1);

	outInformation(findIndexOfIn(mainString, subString, indexOfIn), outType);

	cout << "Для выхода из программы нажмите Enter...";
	cin.get();

	return 0;
}