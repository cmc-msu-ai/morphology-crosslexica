#pragma once
#include "automaton.h"
#include "const automaton.h"
#include "automaton builder.h"
#include "morf characteristics.h"
#include <fstream>

// A class, that implements all functions for morfological analysis.
class MorfAnalizer {
	ConstAutomaton aut;
	// A symbol which can never be found in dictionary.
	char sentinel;
public:
	MorfAnalizer() { }
	// Translates a .txt dictionary into a binary .aut one. Does not use aut field.
	static void translateDictionary(ifstream& in, int size, ofstream& out, char sentinell = '\0');
private:
	static void addSmallWords(AutomatonBuilder& autBuilder, char sentinel);
public:
	void analize(const string& word, vector<MorfCharacteristics>* result) const;
	void analize(const string& word, vector<void*>* result) const;
	void writeToBinaryStream(ofstream& out) const;
	void readFromBinaryStream(ifstream& in);
	vector<int> degreeStatistics(int k) const {
		return aut.degreeStatistics(k);
	}
	string synthesizeWithFullInformation(const string& word, const MorfCharacteristics& current, const MorfCharacteristics& needed) const;
	void synthesize(const string& word, const MorfCharacteristics& current, const MorfCharacteristics& needed, vector<vector<string> >* res) const; 
};