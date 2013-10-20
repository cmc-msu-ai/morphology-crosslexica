#include "stdafx.h"
#include "morf characteristics.h"

const int MorfCharacteristics::charSizes[10] = {1, 6, 4, 4, 1, 1, 4, 2, 1, 1};
const int MorfCharacteristics::NO_SHORT_ADJECTIVE_FORM = 15;
const int MorfCharacteristics::SHORT_ADJECTIVE = 15;
const int MorfCharacteristics::NUMBER_OF_CASES = 6;

static const wchar_t part[9][8] = {L"сущ", L"глаг", L"прил", L"нар", L"дееприч", L"мест", L"предл", L"союз", L"част"};
static const wchar_t partName[5] = L". ";
static const wchar_t morfclassName[7] = L" мкл. ";
static const wchar_t numberMorfclassName[10] = L" мкл.ч. ";
static const wchar_t shortAdjectiveFormName[15] = L" тип кр. ф. ";
static const wchar_t NoshortAdjectiveForm[15] = L"нет кр. ф. ";
static const wchar_t shortAdjectiveName[10] = L" кр. ф. ";
static const wchar_t cases[8][5] = {L"имен", L"род", L"дат", L"вин", L"твор", L"пред", L"част", L"мест"};
static const wchar_t casesName[5] = L" п. ";
static const wchar_t number[2][5] = {L"един", L"множ"};
static const wchar_t numberName[5] = L" ч. ";
static const wchar_t gender[4][5] = {L"муж", L"жен", L"сред", L"общ"};
static const wchar_t genderName[5] = L" р. ";
static const wchar_t tense[2][5] = {L"прош", L"наст"};
static const wchar_t tenseName[5] = L" в. ";
static const wchar_t person[3][5] = {L"перв", L"втор", L"трет"};
static const wchar_t personName[5] = L" л. ";
static const wchar_t verbFlexes[3][10] = {L"неопр ф. ", L"побуд ф. ", L"прич. "};

bool MorfCharacteristics::satisfiesMask(const MorfCharacteristics& mask) const {
	SpeechPart part = getPartOfSpeech();
	if (part != mask.getPartOfSpeech()) {
		return false;
	}
	switch(part) {
		case NOUN: {
			int morf = mask.getMorfoclass();
			if (morf != 0 && morf != getMorfoclass()) {
				return false;
			}
			int numMorf = mask.getNumberMorfoclass();
			if (numMorf != 0 && numMorf != getNumberMorfoclass()) {
				return false;
			}
			int number = mask.getNumber();
			if (number != 0 && number != getNumber()) {
				return false;
			}
			int gender = mask.getGender();
			if (gender != 0 && gender != getGender()) {
				return false;
			}
			int cs = mask.getCase();
			if (cs != 0 && cs != getCase()) {
				return false;
			}
			return true;
				   }
		case VERB: {
			int morf = mask.getMorfoclass();
			if (morf != 0 && morf != getMorfoclass()) {
				return false;
			}
			int form = mask.getVerbForm();
			if (form != 0 && form != getVerbForm()) {
				return false;
			}
			return true;
				   }
		case ADJECTIVE: {
			int morf = mask.getMorfoclass();
			if (morf != 0 && morf != getMorfoclass()) {
				return false;
			}
			int shortForm = mask.getAdjShortForm();
			if (shortForm != 0 && shortForm != getAdjShortForm()) {
				return false;
			}
			int number = mask.getNumber();
			if (number != 0 && number != getNumber()) {
				return false;
			}
			int gender = mask.getGender();
			if (gender != 0 && gender != getGender()) {
				return false;
			}
			bool isShort = mask.isAdjShort();
			if (isShort && isShort != isAdjShort()) {
				return false;
			}
			if (!isShort) {
				int cs = mask.getCase();
				if (cs != 0 && !isAdjShort() && cs != getCase()) {
					return false;
				}
			}
			return true;
						}
		case ADVERB: case PARTICIPLE: case UNION: case PARTICLE: {
			return true;
					 }
		case PRONOUN: {
			int morf = mask.getMorfoclass();
			if (morf != 0 && morf != getMorfoclass()) {
				return false;
			}
			int number = mask.getNumber();
			if (number != 0 && number != getNumber()) {
				return false;
			}
			int gender = mask.getGender();
			if (gender != 0 && gender != getGender()) {
				return false;
			}
			int cs = mask.getCase();
			if (cs != 0 && cs != getCase()) {
				return false;
			}
			return true;
					  }
		case PREPOSITION: {
			int cs = mask.getCase();
			if (cs != 0 && cs != getCase()) {
				return false;
			}
			return true;
						  }
		default: {
			throw "Unknown part of speech";
				 }
	}
}

void MorfCharacteristics::copyConstantCharacteristics(const MorfCharacteristics& from) {
	SpeechPart part = from.getPartOfSpeech();
	if (getPartOfSpeech() != part) {
		setPartOfSpeech(part);
	}
	switch(part) {
		case NOUN: {
			setMorfoclass(from.getMorfoclass());
			setNumberMorfoclass(from.getNumberMorfoclass());
			setGender(from.getGender());
			break;
				   }
		case VERB: {
			setMorfoclass(from.getMorfoclass());
			break;
				   }
		case ADJECTIVE: {
			setMorfoclass(from.getMorfoclass());
			setAdjShortForm(from.getAdjShortForm());
			break;
						}
		case ADVERB: case PARTICIPLE: case UNION: case PARTICLE: {
			break;
					 }
		case PRONOUN: {
			setMorfoclass(from.getMorfoclass());
			setNumber(from.getNumber());
			setGender(from.getGender());
			break;
					  }
		case PREPOSITION: {
			setCase(from.getCase());
			break;
						  }
		default: {
			throw "Unknown part of speech";
				 }
	}
}

vector<MorfCharacteristics> MorfCharacteristics::possibleCharacteristicsSatisfiingMask() const {
	vector<MorfCharacteristics> res;
	switch(getPartOfSpeech()) {
		case NOUN: {
			MorfCharacteristics buff(*this);
			int number = getNumber();
			int cs = getCase();
			if (number == 0 || number == 1) {
				buff.setNumber(1);
				if (cs == 0) {
					for (cs = 1; cs <= NUMBER_OF_CASES; ++cs) {
						buff.setCase(cs);
						res.push_back(buff);
					}
				} else {
					buff.setCase(cs);
					res.push_back(buff);
				}
			}
			if (number == 0 || number == 2) {
				buff.setNumber(2);
				if (cs == 0) {
					for (cs = 1; cs <= NUMBER_OF_CASES; ++cs) {
						buff.setCase(cs);
						res.push_back(buff);
					}
				} else {
					buff.setCase(cs);
					res.push_back(buff);
				}
			}
			break;
				   }
		case VERB: {
			int form = getVerbForm();
			if (form == 0) {
				MorfCharacteristics buff(*this);
				for (form = 1; form <= 15; ++form) {
					buff.setVerbForm(form);
					res.push_back(buff);
				}
			} else {
				res.push_back(*this);
			}
			break;
				   }
		case ADJECTIVE: {
			MorfCharacteristics buff(*this);
			int number = getNumber();
			int gender = getGender();
			int shortForm = getAdjShortForm();
			bool isShort = isAdjShort();
			if (isShort) {
				if (number == 0 || number == 1) {
					buff.setNumber(1);
					if (gender == 0) {
						for (int gender = 1; gender < 4; ++gender) {
							buff.setGender(gender);
							res.push_back(buff);
						}
					} else {
						buff.setGender(gender);
						res.push_back(buff);
					}
				}
				if ((number == 0 || number == 2) && (gender == 0 || gender == 4)) {
					buff.setNumber(2);
					buff.setGender(4);
					res.push_back(buff);
				}
			} else {
				int cs = getCase();
				if (number == 0 || number == 1) {
					buff.setNumber(1);
					if (gender == 0) {
						for (int gender = 1; gender < 4; ++gender) {
							buff.setGender(gender);
							if (cs == 0) {
								buff.setAdjShort(false);
								for (int cs = 1; cs <= NUMBER_OF_CASES; ++cs) {
									buff.setCase(cs);
									res.push_back(buff);
								}
								if (shortForm != NO_SHORT_ADJECTIVE_FORM) {
									buff.setAdjShort(true);
									res.push_back(buff);
								}
							} else {
								buff.setCase(cs);
								res.push_back(buff);
							}
						}
					} else {
						buff.setGender(gender);
						if (cs == 0) {
							buff.setAdjShort(false);
							for (int cs = 1; cs <= NUMBER_OF_CASES; ++cs) {
								buff.setCase(cs);
								res.push_back(buff);
							}
							if (shortForm != NO_SHORT_ADJECTIVE_FORM) {
								buff.setAdjShort(true);
								res.push_back(buff);
							}
						} else {
							buff.setCase(cs);
							res.push_back(buff);
						}
					}
				}
				if ((number == 0 || number == 2) && (gender == 0 || gender == 4)) {
					buff.setNumber(2);
					buff.setGender(4);
					if (cs == 0) {
						buff.setAdjShort(false);
						for (cs = 1; cs <= NUMBER_OF_CASES; ++cs) {
							buff.setCase(cs);
							res.push_back(buff);
						}
						if (shortForm != NO_SHORT_ADJECTIVE_FORM) {
							buff.setAdjShort(true);
							res.push_back(buff);
						}
					} else {
						buff.setCase(cs);
						res.push_back(buff);
					}
				}
			}
			break;
						}
		case ADVERB: case PARTICIPLE: case PREPOSITION: case UNION: case PARTICLE: {
			res.push_back(*this);
			break;
					 }
		case PRONOUN: {
			int cs = getCase();
			if (cs == 0) {
				MorfCharacteristics buff(*this);
				for (cs = 1; cs <= NUMBER_OF_CASES; ++cs) {
					buff.setCase(cs);
					res.push_back(buff);
				}
			} else {
				res.push_back(*this);
			}
			break;
					  }
	}
	return res;
}

wstring MorfCharacteristics::toString() const {
	wchar_t buff[10];
	switch(unsigned(characteristics[0])) {
		case UNKNOWN: {
			return wstring(L"неизв ч. р.");
					  }
		case NOUN: {
			wstring res(part[NOUN - 1]);
			res.append(partName);
			_itow_s((unsigned(characteristics[1]) << 8) | unsigned(characteristics[2]), buff, 10);
			res.append(buff);
			res.append(morfclassName);
			_itow_s(unsigned(characteristics[3]), buff, 10);
			res.append(buff);
			res.append(numberMorfclassName);
			res.append(number[(unsigned(characteristics[4]) >> 4) - 1]);
			res.append(numberName);
			res.append(gender[(unsigned(characteristics[4]) & 15) - 1]);
			res.append(genderName);
			res.append(cases[unsigned(characteristics[5]) - 1]);
			res.append(casesName);
			return res;
				   }
		case VERB: {
			wstring res(part[VERB - 1]);
			res.append(partName);
			_itow_s((unsigned(characteristics[1]) << 8) | unsigned(characteristics[2]), buff, 10);
			res.append(buff);
			res.append(morfclassName);
			const int form = unsigned(characteristics[3]) - 1;
			if (form == 0) {
				res.append(verbFlexes[0]);
			} else if (form >= 1 && form <= 4) {
				res.append(tense[0]);
				res.append(tenseName);
				if (form == 4) {
					res.append(number[1]);
					res.append(numberName);
				} else {
					res.append(number[0]);
					res.append(numberName);
				}
				res.append(gender[form - 1]);
				res.append(genderName);
			} else if (form >= 5 && form <= 10) {
				res.append(tense[1]);
				res.append(tenseName);
				if (form <= 7) {
					res.append(number[0]);
					res.append(numberName);
					res.append(person[form - 5]);
					res.append(personName);
				} else {
					res.append(number[1]);
					res.append(numberName);
					res.append(person[form - 8]);
					res.append(personName);
				}
			} else if (form >= 11 && form <= 12) {
				res.append(verbFlexes[1]);
				res.append(number[form - 11]);
				res.append(numberName);
			} else if (form >= 13 && form <= 14) {
				res.append(verbFlexes[2]);
				res.append(tense[form - 13]);
				res.append(tenseName);
			} else {
				throw "Unknown verb flex";
			}
			return res;
				   }
		case ADJECTIVE: {
			wstring res(part[ADJECTIVE - 1]);
			res.append(partName);
			_itow_s(unsigned(characteristics[1]), buff, 10);
			res.append(buff);
			res.append(morfclassName);
			if (unsigned(characteristics[2]) >> 4 == MorfCharacteristics::NO_SHORT_ADJECTIVE_FORM) {
				res.append(NoshortAdjectiveForm);
			} else {
				_itow_s(unsigned(characteristics[2]) >> 4, buff, 10);
				res.append(buff);
				res.append(shortAdjectiveFormName);
			}
			if ((unsigned(characteristics[2]) & 15) == MorfCharacteristics::SHORT_ADJECTIVE) {
				res.append(shortAdjectiveName);
				res.append(number[(unsigned(characteristics[3]) >> 4) - 1]);
				res.append(numberName);
				res.append(gender[(unsigned(characteristics[3]) & 15) - 1]);
				res.append(genderName);
			} else {
				res.append(number[(unsigned(characteristics[3]) >> 4) - 1]);
				res.append(numberName);
				res.append(gender[(unsigned(characteristics[3]) & 15) - 1]);
				res.append(genderName);
				res.append(cases[(unsigned(characteristics[2]) & 15) - 1]);
				res.append(casesName);
			}
			return res;
						}
		case ADVERB: {
			wstring res(part[ADVERB - 1]);
			res.append(partName);
			return res;
					 }
		case PARTICIPLE: {
			wstring res(part[PARTICIPLE - 1]);
			res.append(partName);
			return res;
					 }
		case PRONOUN: {
			wstring res(part[PRONOUN - 1]);
			res.append(partName);
			_itow_s(unsigned(characteristics[1]), buff, 10);
			res.append(buff);
			res.append(morfclassName);
			if (unsigned(characteristics[2]) >> 4 == 1) {
				res.append(number[0]);
			} else {
				res.append(number[1]);
			}
			res.append(numberName);
			res.append(gender[unsigned(characteristics[2]) & 15 - 1]);
			res.append(genderName);
			res.append(cases[unsigned(characteristics[3]) - 1]);
			res.append(casesName);
			return res;
					 }
		case PREPOSITION: {
			wstring res(part[PREPOSITION - 1]);
			res.append(partName);
			res.append(cases[unsigned(characteristics[1]) - 1]);
			res.append(casesName);
			return res;
					 }
		case UNION: {
			wstring res(part[UNION - 1]);
			res.append(partName);
			return res;
					 }
		case PARTICLE: {
			wstring res(part[PARTICLE - 1]);
			res.append(partName);
			return res;
					 }
		default: {
			throw "Unknown part of speech";
				 }
	}
}