#pragma once
#include <vector>
#include <string>
#include "stdlib.h"
using std::string;
using std::wstring;
using std::vector;

class MorfAnalizer;

class MorfCharacteristics {
public:
	friend class MorfAnalizer;
	static const int NO_SHORT_ADJECTIVE_FORM;
	static const int SHORT_ADJECTIVE;
	static const int NUMBER_OF_CASES;
	enum SpeechPart{UNKNOWN = 0, NOUN = 1, VERB = 2, ADJECTIVE = 3, ADVERB = 4, PARTICIPLE = 5, PRONOUN = 6, PREPOSITION = 7, UNION = 8, PARTICLE = 9};
private:
	char* characteristics;
	const static int charSizes[10];
	explicit MorfCharacteristics(const string& charact) {
		characteristics = new char[charact.size()];
		for (int i = 0; i < (int)charact.size(); ++i) {
			characteristics[i] = charact[i];
		}
	}
public:
	explicit MorfCharacteristics(SpeechPart part = UNKNOWN) {
		characteristics = new char[charSizes[part]];
		characteristics[0] = char(part);
		for (int i = 1; i < charSizes[part]; ++i) {
			characteristics[i] = 0;
		}
	}
	MorfCharacteristics(const MorfCharacteristics& mc) {
		const int size = charSizes[mc.characteristics[0]];
		characteristics = new char[size];
		for (int i = 0; i < size; ++i) {
			characteristics[i] = mc.characteristics[i];
		}
	}
	~MorfCharacteristics() {
		delete [] characteristics;
	}
	MorfCharacteristics& operator=(const MorfCharacteristics& mc) {
		char* toDelete = characteristics;
		const int size = charSizes[mc.characteristics[0]];
		characteristics = new char[size];
		for (int i = 0; i < size; ++i) {
			characteristics[i] = mc.characteristics[i];
		}
		delete [] toDelete;
		return *this;
	}
	void setPartOfSpeech(SpeechPart part) {
		char* toDelete = characteristics;
		characteristics = new char[charSizes[part]];
		delete [] toDelete;
		characteristics[0] = char(part);
		for (int i = 1; i < charSizes[part]; ++i) {
			characteristics[i] = 0;
		}
	}
	SpeechPart getPartOfSpeech() const {
		return SpeechPart(unsigned(characteristics[0]));
	}
	void setMorfoclass(unsigned morf) {
		if (unsigned(characteristics[0]) == NOUN || unsigned(characteristics[0]) == VERB) {
			characteristics[1] = char(morf >> 8);
			characteristics[2] = char(morf & 255);
		} else if (unsigned(characteristics[0]) == ADJECTIVE || unsigned(characteristics[0]) == PRONOUN) {
			characteristics[1] = char(morf);
		} else {
			throw "Can't set morfoclass to this part of speech";
		}
	}
	unsigned getMorfoclass() const {
		if (unsigned(characteristics[0]) == NOUN || unsigned(characteristics[0]) == VERB) {
			return (unsigned(characteristics[1]) << 8) | unsigned(characteristics[2]);
		} else if (characteristics[0] == ADJECTIVE || characteristics[0] == PRONOUN) {
			return unsigned(characteristics[1]);
		} else {
			throw "Can't get morfoclass from this part of speech";
		}
	}
	void setNumberMorfoclass(unsigned morf) {
		if (unsigned(characteristics[0]) == NOUN) {
			characteristics[3] = char(morf);
		} else {
			throw "Can't set number morfoclass to this part of speech";
		}
	}
	unsigned getNumberMorfoclass() const {
		if (unsigned(characteristics[0]) == NOUN) {
			return unsigned(characteristics[3]);
		} else {
			throw "Can't get number morfoclass from this part of speech";
		}
	}
	void setNumber(int num) {
		if (num != 1 && num != 2) {
			throw "Illegal number in setNumber function";
		}
		if (unsigned(characteristics[0]) == NOUN) {
			characteristics[4] = char((num << 4) | (unsigned(characteristics[4]) & 15));
		} else if (unsigned(characteristics[0]) == ADJECTIVE) {
			characteristics[3] = char((unsigned(characteristics[3]) & 15) | (num << 4));
		} else if (unsigned(characteristics[0]) == PRONOUN) {
			characteristics[2] = char((unsigned(characteristics[2]) & 15) | (num << 4));
		} else if (unsigned(characteristics[0]) == PRONOUN) {
			characteristics[2] = char((num << 4) | (unsigned(characteristics[3]) & 15));
		} else {
			throw "Can't set number characteristic to this part of speech";
		}
	}
	int getNumber() const {
		if (unsigned(characteristics[0]) == NOUN) {
			return unsigned(characteristics[4]) >> 4;
		} else if (unsigned(characteristics[0]) == ADJECTIVE) {
			return unsigned(characteristics[3]) >> 4;
		} else if (unsigned(characteristics[0]) == PRONOUN) {
			return unsigned(characteristics[2]) >> 4;
		} else {
			throw "Can't get number from this part of speech";
		}
	}
	void setGender(int gen) {
		if (gen < 1 || gen > 4) {
			throw "Illegal gender in setGender function";
		}
		if (unsigned(characteristics[0]) == NOUN) {
			characteristics[4] = char(((characteristics[4] >> 4) << 4) | (gen & 15));
		} else if (unsigned(characteristics[0]) == ADJECTIVE) {
			characteristics[3] = char(((unsigned(characteristics[3]) >> 4) << 4) | gen);
		} else if (unsigned(characteristics[0]) == PRONOUN) {
			characteristics[2] = char(((unsigned(characteristics[3]) >> 4) << 4) | (gen & 15));
		} else {
			throw "Can't set gender characteristic to this art of speech";
		}
	}
	int getGender() const {
		if (unsigned(characteristics[0]) == NOUN) {
			return unsigned(characteristics[4]) & 15;
		} else if (unsigned(characteristics[0]) == ADJECTIVE) {
			return unsigned(characteristics[3]) & 15;
		} else if (unsigned(characteristics[0]) == PRONOUN) {
			return unsigned(characteristics[2]) & 15;
		} else {
			throw "Can't get gender from this part of speech";
		}
	}
	void setCase(int cs) {
		if (cs < 1 || cs > 8) {
			throw "Illegal case in setCase function";
		}
		if (unsigned(characteristics[0]) == NOUN) {
			characteristics[5] = char(cs);
		} else if (characteristics[0] == ADJECTIVE) {
			if (cs > 6) {
				throw "Illegal case for adjective in setCase function";
			}
			if ((unsigned(characteristics[2]) & 15) == SHORT_ADJECTIVE) {
				throw "Can't set case for short adjective";
			}
			characteristics[2] = char(((unsigned(characteristics[2]) >> 4) << 4) | (cs & 15));
		} else if (unsigned(characteristics[0]) == PRONOUN) {
			characteristics[3] = char(cs);
		} else if (unsigned(characteristics[0]) == PREPOSITION) {
			characteristics[1] = char(cs);
		} else {
			throw "Can't set case to this part of speech";
		}
	}
	int getCase() const {
		if (unsigned(characteristics[0]) == NOUN) {
			return unsigned(characteristics[5]);
		} else if (unsigned(characteristics[0]) == ADJECTIVE) {
			if ((unsigned(characteristics[2]) & 15) == SHORT_ADJECTIVE) {
				throw "Can't get case from short adjective";
			}
			return unsigned(characteristics[2]) & 15;
		} else if (unsigned(characteristics[0]) == PRONOUN) {
			return unsigned(characteristics[3]);
		} else if (unsigned(characteristics[0]) == PREPOSITION) {
			return unsigned(characteristics[1]);
		} else {
			throw "Can't get case for this part of speech";
		}
	}
	void setVerbForm(int form) {
		if (form < 0 || form > 14) {
			throw "Illegal verb form in setVerbForm function";
		}
		if (unsigned(characteristics[0]) == VERB) {
			characteristics[3] = char(form);
		} else {
			throw "Can't set verb form for this part of speech";
		}
	}
	int getVerbForm() const {
		if (unsigned(characteristics[0]) == VERB) {
			return unsigned(characteristics[3]);
		} else {
			throw "Can't get verb form from this part of speech";
		}
	}
	void setAdjShortForm(int form) {
		if (form < 0 || form > 5) {
			throw "Illegal adjective short form in setAdjShortForm function";
		}
		if (unsigned(characteristics[0]) == ADJECTIVE) {
			characteristics[2] = char((unsigned(characteristics[2]) & 15) | (form << 4));
		} else {
			throw "Can't set short form for this part of speech";
		}
	}
	int getAdjShortForm() const {
		if (unsigned(characteristics[0]) == ADJECTIVE) {
			return unsigned(characteristics[2]) >> 4;
		} else {
			throw "Can't get short form for this part of speech";
		}
	}
	void setAdjShort(bool form) {
		if (unsigned(characteristics[0]) == ADJECTIVE) {
			if (unsigned(characteristics[2]) == NO_SHORT_ADJECTIVE_FORM) {
				throw "No short adjective form";
			}
			if (!form) {
				characteristics[2] = char((unsigned(characteristics[2]) >> 4) << 4);
			} else {
				characteristics[2] = char(((unsigned(characteristics[2]) >> 4) << 4) | SHORT_ADJECTIVE);
			}
		} else {
			throw "Can't set short form for this part of speech";
		}
	}
	bool isAdjShort() const {
		if (characteristics[0] == ADJECTIVE) {
			return (unsigned(characteristics[2]) & 15) == SHORT_ADJECTIVE;
		} else {
			throw "Can't get short form for this part of speech";
		}
	}

	bool satisfiesMask(const MorfCharacteristics& mask) const;
	void copyConstantCharacteristics(const MorfCharacteristics& from);
	vector<MorfCharacteristics> possibleCharacteristicsSatisfiingMask() const;
	template <class outStream> friend outStream& operator<<(outStream& out, const MorfCharacteristics& morfChar) {
		out << morfChar.toString();
		return out;
	}
	wstring toString() const;
};
