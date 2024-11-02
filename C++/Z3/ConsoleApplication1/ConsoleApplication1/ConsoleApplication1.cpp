#include <iostream>
#include <fstream>
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
	NotTXTFile
};
const string ERRORS[5]{
	"Неудалось открыть файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Неудалось открыть или создать файл. Попробуйте еще раз, проверив путь и имя файла.",
	"Некорректные данные данные. Попробуйте еще раз.",
	"Ваше значение не соответствует числовым ограничениям. Попробуйте еще раз.",
	"Расширение файла должно быть TXT. Попробуйте еще раз."
};
const int MIN_LIMIT_SIZE = 0;
const int MAX_LIMIT_SIZE = 100;
const int MIN_LIMIT = -100;
const int MAX_LIMIT = 100;

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
	else
		return ((pathToFile[stringLength - 1] == 't') &&
		(pathToFile[stringLength - 2] == 'x') &&
		(pathToFile[stringLength - 3] == 't') &&
		(pathToFile[stringLength - 4] == '.'));
}

void inputWithCheckingFromConsole(int &value, const int &MAX_LIMIT, const int &MIN_LIMIT) {
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

bool inputWithCheckingFromFile(fstream &fin, int &num, const int MAX_LIMIT_NUM, const int MIN_LIMIT_NUM) {
	bool isFail = false;

	fin >> num;

	if (fin.fail()) {
		cout << ERRORS[FailData] << endl;
		isFail = true;
	}
	else if (!isFail && (num > MAX_LIMIT_NUM || num < MIN_LIMIT_NUM)) {
		cout << ERRORS[FailLimitOfData] << endl;
		isFail = true;
	}
	return isFail;
}

void openFile(fstream &file, TFile fileType) {
	bool isFail;

	do {
		isFail = false;

		cout << "\nВведите путь к файлу .txt для " << (fileType == FileIn ? "ввода" : "вывода") << " данных : " << endl;
		string filename;
		cin >> filename;

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

void inputDataWithConsole(int &n, int *&arr) {
	int i;

	cout << "Введите размерность массива: " << endl;
	inputWithCheckingFromConsole(n, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE);

	arr = new int[n];
	for (i = 0; i < n; i++) {
		cout << "Введите элемент массива #" << i + 1 << ":" << endl;
		inputWithCheckingFromConsole(arr[i], MAX_LIMIT, MIN_LIMIT);
	}
}

bool inputDataWithFile(int &n, int *&arr) {
	fstream fin;
	int i;
	bool isFail = false;

	openFile(fin, FileIn);

	if (inputWithCheckingFromFile(fin, n, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE))
		isFail = true;

	arr = new int[n];
	for (i = 0; i < n; i++)
		if (!isFail && inputWithCheckingFromFile(fin, arr[i], MAX_LIMIT, MIN_LIMIT))
			isFail = true;

	fin.close();
	return isFail;
}

void inData(int &n, int *&arr, int inType) {
	if (inType == 1)
		inputDataWithConsole(n, arr);
	else
		while (inputDataWithFile(n, arr));
}

void outArray(int n, int *&arr, int outType) {
	int i;
	fstream fout;

	cout << endl;
	if (outType == 2)
		openFile(fout, FileOut);

	for (i = 0; i < n; i++) {
		cout << arr[i] << ' ';
		if (outType == 2)
			fout << arr[i] << ' ';
	}

	cout << endl;
	if (outType == 2)
		fout << endl;

	cout << endl;

	fout.close();
}

void sortArray(const int n, int *&arr, const int typeSort) {
	int i, k;

	for (int k = 1; k < n; k++) {
		i = k;
		if (typeSort == 2)
			while (i > 0 && arr[i - 1] < arr[i]) {
				swap(arr[i], arr[i - 1]);
				i--;
			}
		else
			while (i > 0 && arr[i - 1] > arr[i]) {
				swap(arr[i], arr[i - 1]);
				i--;
			}
	}
}

void exitProgram(int *&arr) {
	delete[] arr;

	cout << "Для выхода из программы нажмите Enter...";
	cin.get();
}

int main() {
	setlocale(LC_ALL, "Russian");

	cout << "Программа для сортировки массива простыми вставками." << endl;

	cout << "Все значения массива должны быть от " << MIN_LIMIT << " до " << MAX_LIMIT << '.' << endl;
	cout << "Размерность массива должна быть от " << MIN_LIMIT_SIZE << " до " << MAX_LIMIT_SIZE << '.' << endl << endl;

	int inType = 0;
	int n;
	int *arr;

	cout << "Введите предпочитаемый тип ввода данных:" << endl;
	cout << "\t1 - из консоли,\n\t2 - из файла." << endl;
	inputWithCheckingFromConsole(inType, 2, 1);

	inData(n, arr, inType);

	outArray(n, arr, 1);

	int typeSort;
	cout << "Введите тип сортировки:" << endl;
	cout << "\t1 - по возрастанию,\n\t2 - по убыванию." << endl;
	inputWithCheckingFromConsole(typeSort, 2, 1);
	
	sortArray(n, arr, typeSort);

	int outType;
	cout << "Введите предпочитаемый тип вывода данных:" << endl;
	cout << "\t1 - только в консоли,\n\t2 - в консоль и в файл." << endl;
	inputWithCheckingFromConsole(outType, 2, 1);

	outArray(n, arr, outType);

	exitProgram(arr);
	return 0;
}
