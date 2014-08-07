#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <locale>
#include "morf analizer.h"
using namespace std;

void MainFunc() {
	setlocale(LC_ALL, "");
	SetConsoleCP(1251);
	wcout.imbue(locale("rus_rus.866"));
    wcin.imbue(locale("rus_rus.866"));

	ifstream input_file("input.txt");
	ofstream output_file("output.txt");
	string buf, str;

	ifstream binIn;
	binIn.open("dict.aut", std::ios_base::binary);
	MorfAnalizer MA;
	MA.readFromBinaryStream(binIn);
	binIn.close();
	
	vector<MorfCharacteristics> res;

	while (! input_file.eof()) {
		input_file >> buf;
		MA.analize(buf, &res);
		if (res.size() == 0) {
			//wcout << L"Данного слова в словаре не найдено" << std::endl;
		} else {
			for (int i = 0; i < (int)res.size(); ++i) {
				//output_file << res[i] << endl;
			}
		}
	}

}