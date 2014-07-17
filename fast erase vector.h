#pragma once
#include <vector>
using std::vector;

// A random access container with fast add and remove operations.
template <class O> class FastEraseVector {
	vector<O> elements;
	vector<int> holes;
public:
	explicit FastEraseVector(int size) {
		elements.reserve(size);
		holes.reserve(size >> 4);
	}
	void init(vector<O>* elems, vector<int>* holesVect) {
		holes.swap(*holesVect);
		elements.swap(*elems);
	}
	// Can't check if there is access violation.
	const O& operator[](int num) const {
		return elements[num];
	}
	// Can't check if there is access violation.
	O& operator[](int num) {
		return elements[num];
	}
	int add(const O& elem) {
		if (holes.empty()) {
			elements.push_back(elem);
			return elements.size() - 1;
		} else {
			int hole = holes.back();
			holes.pop_back();
			elements[hole] = elem;
			return hole;
		}
	}
	void remove(int num) {
		holes.push_back(num);
	}
	// Slow.
	bool isHole(int node) const {
		for (int i = 0; i < (int)holes.size(); ++i) {
			if (holes[i] == node) {
				return true;
			}
		}
		return false;
	}
	vector<int> getHoles() const {
		return holes;
	}
	void clear() {
		elements.clear();
		holes.clear();
	}
	int size() const {
		return elements.size() - holes.size();
	}
	int maxNumber() const {
		return elements.size();
	}
	int memory() const {
		return elements.size() + holes.size();
	}
};