#include "StdAfx.h"
#include "morf analizer.h"
#include "stdio.h"
#include "CLASSES.CPP"

// Endings from Classes.cpp
extern const char xl_nflx[260][8][10];
extern const char xl_aflx[22][5][6][10];
extern const char xl_sflx[11][7][5][10];
extern const char xl_vflx[236][15][15];

extern const char xl_pflx[13][8][6];
extern const int person_of_pronouns[13];
extern const int number_of_pronouns[13];
extern const int gender_of_pronouns[13];
extern const char xl_preps_noun[189][15];
extern const int xl_preps_case[189];
extern const char unions[6][10];
extern const char particles[3][10];

void MorfAnalizer::addSmallWords(AutomatonBuilder& autBuilder, char sentinel) {
	string word, charbuff(1, sentinel);
	charbuff.resize(5);
	charbuff[1] = char(MorfCharacteristics::PRONOUN);
	for (int i = 1; i < 13; ++i) {
		charbuff[2] = char(i);
		charbuff[3] = char((number_of_pronouns[i] << 4) | gender_of_pronouns[i]);
		for (int j = 0; j < MorfCharacteristics::NUMBER_OF_CASES; ++j) {
			charbuff[4] = char(j + 1);
			word = string(xl_pflx[i][j]);
			word.append(charbuff);
			autBuilder.addWord(word);
		}
	}
	charbuff.resize(3);
	charbuff[1] = char(MorfCharacteristics::PREPOSITION);
	for (int i = 1; i < 189; ++i) {
		if (xl_preps_case[i] >= MorfCharacteristics::NUMBER_OF_CASES) {
			continue;
		}
		word = string(xl_preps_noun[i]);
		charbuff[2] = char(xl_preps_case[i]);
		word.append(charbuff);
		autBuilder.addWord(word);
	}
	charbuff.resize(2);
	charbuff[1] = char(MorfCharacteristics::UNION);
	for (int i = 1; i < 6; ++i) {
		word = string(unions[i]);
		word.append(charbuff);
		autBuilder.addWord(word);
	}
	charbuff[1] = char(MorfCharacteristics::PARTICLE);
	for (int i = 1; i < 3; ++i) {
		word = string(particles[i]);
		word.append(charbuff);
		autBuilder.addWord(word);
	}
}

void MorfAnalizer::translateDictionary(ifstream& in, int size, ofstream& out, char sentinell) {
	Automaton autom(size);
	AutomatonBuilder autBuilder(&autom, size);
	addSmallWords(autBuilder, sentinell);
//	int counter = 0;
	while (in.peek() == ' ' || in.peek() == '\n') {
		in.get();
	}
	int partOfSpeech, morfClass, thirdNumber, forthNumber, fifthNumber;
	string word, buff, charbuff(1, sentinell);
	word.reserve(50);
	buff.reserve(50);
	charbuff.reserve(50);
	while (in.peek() != EOF) {
		in >> partOfSpeech >> morfClass >> thirdNumber >> forthNumber;
		if (partOfSpeech == MorfCharacteristics::NOUN) {
			in >> fifthNumber;
		}
		in >> word;
		if (word[word.size() - 1] >= '0' && word[word.size() - 1] <= '9') {
			word = word.substr(0, word.size() - 1);
		}
//		++counter;
//		if ((counter & 1023) == 0) {
//			std::cout << counter << ' ';
//		}
		switch(partOfSpeech) {
			case MorfCharacteristics::NOUN: {
				int endLength = strlen(xl_nflx[morfClass][0]);
				word = word.substr(0,word.length() - endLength);
				charbuff.resize(7);
				charbuff[1] = char(MorfCharacteristics::NOUN);
				charbuff[2] = char(morfClass >> 8);
				charbuff[3] = char(morfClass);
				charbuff[4] = char(fifthNumber);
				charbuff[5] = char((thirdNumber << 4) + forthNumber);
				for (int i = 0; i < MorfCharacteristics::NUMBER_OF_CASES; ++i) {
					buff = word;
					buff.append(xl_nflx[morfClass][i]);
					charbuff[6] = char(i + 1);
					buff.append(charbuff);
					autBuilder.addWord(buff);
				}
				break;
					   }
			case MorfCharacteristics::VERB: {
				int endLength = strlen(xl_vflx[morfClass][0]);
				word = word.substr(0,word.length() - endLength);
				charbuff.resize(5);
				charbuff[1] = char(MorfCharacteristics::VERB);
				charbuff[2] = char(morfClass >> 8);
				charbuff[3] = char(morfClass);
				for (int i = 0; i < 15; ++i) {
					buff = word;
					buff.append(xl_vflx[morfClass][i]);
					charbuff[4] = char(i + 1);
					buff.append(charbuff);
					autBuilder.addWord(buff);
				}
				break;
					  }
			case MorfCharacteristics::ADJECTIVE:{
				charbuff.resize(5);
				charbuff[1] = char(MorfCharacteristics::ADJECTIVE);
				charbuff[2] = char(morfClass);
				if (forthNumber == 0) {
					forthNumber = MorfCharacteristics::NO_SHORT_ADJECTIVE_FORM;
				}
				if (morfClass != 0) {
					int endLength = strlen(xl_aflx[morfClass][1][0]);
					word = word.substr(0,word.length() - endLength);
					for (int i = 1; i < 5; ++i) {
						for (int j = 0; j < 6; ++j) {
							buff = word;
							buff.append(xl_aflx[morfClass][i][j]);
							charbuff[3] = char(forthNumber << 4 | (j + 1));
							if (i != 4) {
								charbuff[4] = char((1 << 4) | i);
							} else {
								charbuff[4] = char((2 << 4) | i);
							}
							buff.append(charbuff);
							autBuilder.addWord(buff);
						}
					}
					word.append(xl_aflx[morfClass][1][0]);
				}
				if (forthNumber != MorfCharacteristics::NO_SHORT_ADJECTIVE_FORM) {
					int endLength = strlen(xl_sflx[morfClass][forthNumber][0]);
					word = word.substr(0, word.length() - endLength);
					for (int i = 1; i < 5; ++i) {
						buff = word;
						buff.append(xl_sflx[morfClass][forthNumber][i]);
						charbuff[3] = char(forthNumber << 4 | (MorfCharacteristics::SHORT_ADJECTIVE));
						if (i != 4) {
							charbuff[4] = char((1 << 4) | i);
						} else {
							charbuff[4] = char((2 << 4) | i);
						}
						buff.append(charbuff);
						autBuilder.addWord(buff);
					}
				}
				break;
					  }
			case MorfCharacteristics::ADVERB: {
				buff = word;
				charbuff.resize(2);
				charbuff[1] = char(MorfCharacteristics::ADVERB);
				buff.append(charbuff);
				autBuilder.addWord(buff);
				break;
						 }
			case MorfCharacteristics::PARTICIPLE: {
				buff = word;
				charbuff.resize(2);
				charbuff[1] = char(MorfCharacteristics::PARTICIPLE);
				buff.append(charbuff);
				autBuilder.addWord(buff);
				break;
						 }
		}
		while (in.peek() == ' ' || in.peek() == '\n') {
			in.get();
		}
	}
//	std::cout << '\n';
//	autBuilder.printStats();
	out.write(&sentinell, 1);
	autom.writeToBinaryStream(out);
}

static string append(const string& str, char c) {
	string res = str;
	res.append(1, c);
	return res;
}

void MorfAnalizer::analize(const string& word, vector<MorfCharacteristics>* result) const {
	vector<string> suffixes;
	aut.allSuffixes(append(word, sentinel), &suffixes);
	result->clear();
	result->reserve(suffixes.size());
	for (int i = 0; i < (int)suffixes.size(); ++i) {
		result->push_back(MorfCharacteristics(suffixes[i]));
	}
}


void MorfAnalizer::writeToBinaryStream(ofstream& out) const {
	out.write(&sentinel, 1);
	aut.writeToBinaryStream(out);
}

void MorfAnalizer::readFromBinaryStream(ifstream& in) {
	in.read(&sentinel, 1);
	aut.readFromBinaryStream(in);
}


string MorfAnalizer::synthesizeWithFullInformation(const string& word, const MorfCharacteristics& current, const MorfCharacteristics& needed) const {
	const MorfCharacteristics::SpeechPart part = current.getPartOfSpeech();
	string res = word;
	switch(part) {
		case MorfCharacteristics::NOUN: {
			int morf = current.getMorfoclass();
			int curNumber = current.getNumber();
			int needNumber = needed.getNumber();
			if (curNumber != needNumber) {
				int numMorf = current.getNumberMorfoclass();
				int cs = current.getCase();
				int cut = strlen(xl_nflx[morf][cs - 1]);
				res = res.substr(0, res.size() - cut);
				res.append(xl_nflx[morf][0]);
				if (curNumber == 1) {
					cut = strlen(number_morfs[numMorf][0]);
					res = res.substr(0, res.size() - cut);
					res.append(number_morfs[numMorf][1]);
				} else {
					cut = strlen(number_morfs[numMorf][1]);
					res = res.substr(0, res.size() - cut);
					res.append(number_morfs[numMorf][0]);
				}
				vector<MorfCharacteristics> variants;
				analize(res, &variants);
				for (int i = 0; i < variants.size(); ++i) {
					if (variants[i].getPartOfSpeech() == MorfCharacteristics::NOUN && variants[i].getNumber() == needNumber && variants[i].getCase() == 1) {
						int newMorf = variants[i].getMorfoclass();
						int cut = strlen(xl_nflx[newMorf][0]);
						res = res.substr(0, res.size() - cut);
						res.append(xl_nflx[newMorf][needed.getCase() - 1]);
						break;
					}
				}
			} else {
				int cut = strlen(xl_nflx[morf][current.getCase() - 1]);
				res = res.substr(0, res.size() - cut);
				res.append(xl_nflx[morf][needed.getCase() - 1]);
			}
			break;
				   }
		case MorfCharacteristics::VERB: {
			int morf = current.getMorfoclass();
			int cut = strlen(xl_vflx[morf][current.getVerbForm() - 1]);
			res = res.substr(0, res.size() - cut);
			res.append(xl_vflx[morf][needed.getVerbForm() - 1]);
			break;
				   }
		case MorfCharacteristics::ADJECTIVE: {
			int morf = current.getMorfoclass();
			if (!current.isAdjShort()) {
				if (!needed.isAdjShort()) {
					int cut = strlen(xl_aflx[morf][current.getGender()][current.getCase() - 1]);
					res = res.substr(0, res.size() - cut);
					res.append(xl_aflx[morf][needed.getGender()][needed.getCase() - 1]);
				} else {
					int shortMorf = current.getAdjShortForm();
					int cut = strlen(xl_sflx[morf][shortMorf][0]);
					res = res.substr(0, res.size() - cut);
					res.append(xl_sflx[morf][shortMorf][needed.getGender()]);
				}
			} else {
				if (!needed.isAdjShort()) {
					int shortMorf = current.getAdjShortForm();
					int cut = strlen(xl_sflx[morf][shortMorf][current.getGender()]);
					res = res.substr(0, res.size() - cut);
					res.append(xl_sflx[morf][shortMorf][0]);
					cut = strlen(xl_aflx[morf][1][0]);
					res = res.substr(0, res.size() - cut);
					res.append(xl_aflx[morf][needed.getGender()][needed.getCase() - 1]);
				} else {
					int shortMorf = current.getAdjShortForm();
					int cut = strlen(xl_sflx[morf][shortMorf][current.getGender()]);
					res = res.substr(0, res.size() - cut);
					res.append(xl_sflx[morf][shortMorf][needed.getGender()]);
				}
			}
			break;
						}
		case MorfCharacteristics::ADVERB: case MorfCharacteristics::PARTICIPLE: case MorfCharacteristics::PREPOSITION: case MorfCharacteristics::UNION: case MorfCharacteristics::PARTICLE:{
			break;
					 }
		case MorfCharacteristics::PRONOUN: {
			int morf = current.getMorfoclass();
			int cut = strlen(xl_pflx[morf][current.getCase() - 1]);
			res = res.substr(0, res.size() - cut);
			res.append(xl_pflx[morf][needed.getCase() - 1]);
			break;
					  }
		default: {
			throw "Unknown part of speech";
				 }
	}
	return res;
}

vector<vector<string> > MorfAnalizer::synthesize(const string& word, const MorfCharacteristics& current, const MorfCharacteristics& needed) const {
	vector<MorfCharacteristics> allVariants;
	analize(word, &allVariants);
	vector<MorfCharacteristics> possibleVariants;
	for (int i = 0; i < allVariants.size(); ++i) {
		if (allVariants[i].satisfiesMask(current)) {
			possibleVariants.push_back(allVariants[i]);
		}
	}
	vector<vector<string> > res(possibleVariants.size());
	for (int i = 0; i < possibleVariants.size(); ++i) {
		MorfCharacteristics buff(needed);
		buff.copyConstantCharacteristics(possibleVariants[i]);
		vector<MorfCharacteristics> variants = buff.possibleCharacteristicsSatisfiingMask();
		for (int j = 0; j < variants.size(); ++j) {
			res[i].push_back(synthesizeWithFullInformation(word, possibleVariants[i], variants[j]));
		}
	}
	return res;
}