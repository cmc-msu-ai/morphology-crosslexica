#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <locale>
#include "morf analizer.h"
using namespace std;

bool FileExists(LPCTSTR fname) {
	return ::GetFileAttributes(fname) != DWORD(-1);
}

int main() {
	setlocale(LC_ALL, "");
	SetConsoleCP(1251);
	wcout.imbue(locale("rus_rus.866"));
    wcin.imbue(locale("rus_rus.866"));

	wcout << L"Вас приветствует программа морфологического анализа" << std::endl;
	wcout << L"Есть ли у вас ранее созданный бинарный файл словаря? (д/н) ";
	string buf;
	cin >> buf;
	ifstream binIn;
	if (buf[0] != 'д') {
		bool done = false;
		ifstream in;
		while (!done) {
			wcout << L"Введите полный путь к текстовому файлу словаря" << std::endl;
			cin >> buf;
			//buf="C:\\VOC.TXT";
			//if (FileExists(buf.c_str())) {
				in.open(buf.c_str());
				done = true;
			//} else {
			//	wcout << L"Указанного файла не существует. Попробуйте еще раз" << std::endl;
			//}
		}
		wcout << L"Введите полный путь к бинарному файлу словаря (если по заданному пути файла не существует, он будет создан" << std::endl;
		cin >> buf;
		//buf="С:\\BIN";
		ofstream out(buf.c_str(), std::ios_base::binary);
		wcout << L"Производится переработка словаря. Это может занять несколько минут" << std::endl;
		MorfAnalizer::translateDictionary(in, 1 << 17, out);
		in.close();
		out.close();
		binIn.open(buf.c_str(), std::ios_base::binary);
		wcout << L"Готово" << std::endl;
	} else {
		bool done = false;
		while (!done) {
			wcout << L"Введите полный путь к бинарному файлу словаря" << std::endl;
			cin >> buf;
			//if (FileExists(buf.c_str())) {
				binIn.open(buf.c_str(), std::ios_base::binary);
				done = true;
			//} else {
			//	wcout << L"Указанного файла не существует. Попробуйте еще раз" << std::endl;
			//}
		}
	}

	//binIn.open("F:\\newdict.aut", std::ios_base::binary);

	//wcout << L"Загрузка словаря" << std::endl;
	MorfAnalizer MA;
	MA.readFromBinaryStream(binIn);
	//binIn.close();
	//wcout << L"Готово" << std::endl;
	wcout << L"Далее для каждого введенного слова будут выводиться все возможные морфологические характеристики" << std::endl;
	wcout << L"Для выхода наберите \"quit\"" << std::endl;
	vector<MorfCharacteristics> res;
	cin >> buf;
	while (buf != string("quit")) {
		MA.analize(buf, &res);
		if (res.size() == 0) {
			wcout << L"Данного слова в словаре не найдено" << std::endl;
		} else {
			for (int i = 0; i < (int)res.size(); ++i) {
				wcout << res[i] << L"\n";
			}
		}
		wcout << L"\n";
		cin >> buf;
	}
	string word("он");
	MorfCharacteristics cur(MorfCharacteristics::PRONOUN);
	MorfCharacteristics need(MorfCharacteristics::PRONOUN);
//	need.setGender(2);
//	vector<vector<string> > res = MA.synthesize(word, cur, need);
	getchar();
	getchar();
	return 0;
}