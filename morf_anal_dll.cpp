// morf anal dll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "morf analizer.h"
#include "morf characteristics.h"
#include <fstream>
#include <string>
#include <vector>

static char* c_string(const std::string& str) {
	char* res = new char[str.size() + 1];
	strcpy_s(res, str.size(), str.c_str());
	return res;
}

static wchar_t* c_string(const std::wstring& str) {
	wchar_t* res = new wchar_t[str.size() + 1];
	wcsncpy_s(res, str.size(), str.c_str(), str.size());
	return res;
}

extern "C++" __declspec(dllexport) void* init() {
	return reinterpret_cast<void*>(new MorfAnalizer);
}

extern "C++" __declspec(dllexport) void close(void* MA) {
	delete reinterpret_cast<MorfAnalizer*>(MA);
}

extern "C++" __declspec(dllexport) void readDict(void* MA, char* path) {
	ifstream in(path, std::ios_base::binary);
	reinterpret_cast<MorfAnalizer*>(MA)->readFromBinaryStream(in);
	in.close();
}

extern "C++" __declspec(dllexport) void analize(void* MA, char* word, std::vector<void*>* res) {
	reinterpret_cast<MorfAnalizer*>(MA)->analize(string(word), res);
}

extern "C++" __declspec(dllexport) char* synthesizeWithFullInformation(void* MA, char* word, void* current, void* needed) {
	return c_string(reinterpret_cast<MorfAnalizer*>(MA)->synthesizeWithFullInformation(string(word), *reinterpret_cast<MorfCharacteristics*>(current), *reinterpret_cast<MorfCharacteristics*>(needed)));
}

extern "C++" __declspec(dllexport) char*** synthesize(void* MA, char* word, void* current, void* needed, int& first, int* second) {
	std::vector<std::vector<std::string>> buff;
	reinterpret_cast<MorfAnalizer*>(MA)->synthesize(string(word), *reinterpret_cast<MorfCharacteristics*>(current), *reinterpret_cast<MorfCharacteristics*>(needed), &buff);
	first = buff.size();
	second = new int[first];
	char*** res = new char**[first];
	for (int i = 0; i < first; ++i) {
		second[i] = buff[i].size();
		res[i] = new char*[second[i]];
		for (int j = 0; j < second[i]; ++j) {
			res[i][j] = c_string(buff[i][j]);
		}
	}
	return res;
}

extern "C++" __declspec(dllexport) void* createMorfCharacterictics(int partOfSpeech = 0) {
	return reinterpret_cast<void*>(new MorfCharacteristics(MorfCharacteristics::SpeechPart(partOfSpeech)));
}

extern "C++" __declspec(dllexport) void* copyMorfCharacterictics(void* mc) {
	return reinterpret_cast<void*>(new MorfCharacteristics(*reinterpret_cast<MorfCharacteristics*>(mc)));
}

extern "C++" __declspec(dllexport) void deleteMorfCharacterictics(void* mc) {
	delete reinterpret_cast<MorfCharacteristics*>(mc);
}

extern "C++" __declspec(dllexport) void setPartOfSpeech(void* mc, int part) {
	reinterpret_cast<MorfCharacteristics*>(mc)->setPartOfSpeech(MorfCharacteristics::SpeechPart(part));
}

extern "C++" __declspec(dllexport) unsigned getPartOfSpeech(void* mc) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->getPartOfSpeech();
}

extern "C++" __declspec(dllexport) void setMorfoclass(void* mc, unsigned morf) {
	reinterpret_cast<MorfCharacteristics*>(mc)->setMorfoclass(morf);
}

extern "C++" __declspec(dllexport) unsigned getMorfoclass(void* mc) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->getMorfoclass();
}

extern "C++" __declspec(dllexport) void setNumberMorfoclass(void* mc, unsigned morf) {
	reinterpret_cast<MorfCharacteristics*>(mc)->setNumberMorfoclass(morf);
}

extern "C++" __declspec(dllexport) unsigned getNumberMorfoclass(void* mc) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->getNumberMorfoclass();
}

extern "C++" __declspec(dllexport) void setNumber(void* mc, int num) {
	reinterpret_cast<MorfCharacteristics*>(mc)->setNumber(num);
}

extern "C++" __declspec(dllexport) int getNumber(void* mc) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->getNumber();
}

extern "C++" __declspec(dllexport) void setGender(void* mc, int gen) {
	reinterpret_cast<MorfCharacteristics*>(mc)->setGender(gen);
}

extern "C++" __declspec(dllexport) int getGender(void* mc) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->getGender();
}

extern "C++" __declspec(dllexport) void setCase(void* mc, int cs) {
	reinterpret_cast<MorfCharacteristics*>(mc)->setCase(cs);
}

extern "C++" __declspec(dllexport) int getCase(void* mc) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->getCase();
}

extern "C++" __declspec(dllexport) void setVerbForm(void* mc, int form) {
	reinterpret_cast<MorfCharacteristics*>(mc)->setVerbForm(form);
}

extern "C++" __declspec(dllexport) int getVerbForm(void* mc) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->getVerbForm();
}

extern "C++" __declspec(dllexport) void setAdjShortForm(void* mc, int form) {
	reinterpret_cast<MorfCharacteristics*>(mc)->setAdjShortForm(form);
}

extern "C++" __declspec(dllexport) int getAdjShortForm(void* mc) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->getAdjShortForm();
}

extern "C++" __declspec(dllexport) void setAdjShort(void* mc, bool form) {
	reinterpret_cast<MorfCharacteristics*>(mc)->setAdjShort(form);
}

extern "C++" __declspec(dllexport) bool isAdjShort(void* mc) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->isAdjShort();
}

extern "C++" __declspec(dllexport) bool satisfiesMask(void* mc, void* mask) {
	return reinterpret_cast<MorfCharacteristics*>(mc)->satisfiesMask(*reinterpret_cast<MorfCharacteristics*>(mask));
}

extern "C++" __declspec(dllexport) void copyConstantCharacteristics(void* to, void* from) {
	reinterpret_cast<MorfCharacteristics*>(to)->copyConstantCharacteristics(*reinterpret_cast<MorfCharacteristics*>(from));
}

extern "C++" __declspec(dllexport) void** possibleCharacteristicsSatisfiingMask(void* to, int& resSize) {
	vector<MorfCharacteristics> buff = reinterpret_cast<MorfCharacteristics*>(to)->possibleCharacteristicsSatisfiingMask();
	resSize = buff.size();
	void** res = new void*[resSize];
	for (int i = 0; i < resSize; ++i) {
		res[i] = reinterpret_cast<void*>(new MorfCharacteristics(buff[i]));
	}
	return res;
}

extern "C++" __declspec(dllexport) wchar_t* toString(void* mc) {
	return c_string(reinterpret_cast<MorfCharacteristics*>(mc)->toString().c_str());
}