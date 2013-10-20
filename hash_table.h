#pragma once
#include <vector>
using std::vector;

// A separate chaining hash table, that contains only objects (without keys).
template <class K, class O> class HashTable {
public:
	struct Node {
		O elem;
		Node *next;
		Node (const O &element, Node* nextNode): elem(element), next(nextNode) { }
	};
	struct List {
		Node * head;
		List (): head(0) { }
		~List() {
			Node *next;
			while (head != 0) {
				next = head;
				head = head->next;
				delete next;
			}
		}
		void add(const O& elem) {
			head = new Node(elem, head);
		}
		void remove(const O& obj) {
			if (head->elem == obj) {
				Node *toDelete = head;
				head = head->next;
				delete toDelete;
				return;
			}
			Node *prevNode = head;
			Node *nextNode = head->next;
			while (nextNode->elem != obj) {
				prevNode = nextNode;
				nextNode = nextNode->next;
			}
			prevNode->next = nextNode->next;
			delete nextNode;
		}
		int size() const {
			int res = 0;
			for (Node* node = head; node != 0; node = node->next) {
				++res;
			}
			return res;
		}
	};
private:
	List *table;
	const unsigned maxSize;
public:
	// maxSize == 2^n.
	const unsigned n;
	HashTable(unsigned N): n(N), maxSize(1 << N) {
		table = new List[maxSize];
	}
	~HashTable() {
		delete [] table;
	}
	void add_(const K& key, const O& obj) {
		table[key.hash() >> (8*sizeof(unsigned)-n)].add(obj);
	}
	void add(const unsigned& hash, const O& obj) {
		table[hash >> (8*sizeof(unsigned)-n)].add(obj);
	}
	const List& get_(const K& key) const {
		return table[key.hash() >> (8*sizeof(unsigned)-n)];
	}
	const List& get(const unsigned& hash) const {
		return table[hash >> (8*sizeof(unsigned)-n)];
	}
	void remove_(const K& key, const O& obj) {
		table[key.hash() >> (8*sizeof(unsigned)-n)].remove(obj);
	}
	void remove(const unsigned& hash, const O& obj) {
		table[hash >> (8*sizeof(unsigned)-n)].remove(obj);
	}
	int collisions() const {
		int res = 0;
		for (int i = 0; i < (int)maxSize; ++i) {
			int s = table[i].size();
			if (s != 0) {
				res += s - 1;
			}
		}
		return res;
	}
};