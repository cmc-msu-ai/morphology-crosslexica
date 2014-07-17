#pragma once
#include <iostream>
#include "automaton.h"
#include "hash_table.h"
#include <set>

static int maxOneBit(int num) {
	int res = 0;
	while (num != 0) {
		num >>= 1;
		++res;
	}
	return res - 1;
}

// A wrap class of Automaton, that allows to add words to it keeping it minimal.
class AutomatonBuilder {
	Automaton* const aut;
	HashTable<Automaton::Node, int> registr;
	vector<bool> registered;
	vector<int> incoming;
	vector<unsigned> hash;
public:
	AutomatonBuilder(Automaton* const automaton, unsigned int size): 
	  aut(automaton), registr(maxOneBit(size)), registered(size, false), incoming(size, 0), hash(size, 0) {
		  registered[aut->startNode] = true;
		  registr.add(hash[aut->startNode], aut->startNode);
	}
private:
	int cloneNode(int node);
	void link(int node1, int node2, char c);
	void relink(int node1, int node2, char c);
	int addNode(const Automaton::Node& node, unsigned nodeHash);
	int findInRegistr(int node) const;
	void removeNode(int node);
	void addBranch(int node, const string& str);
	void replaceOrRegister(int node);
	bool replaceOrRegisterWithAnswer(int node);
	void makeTerminal(int node);
public:
	void addWord(const string& word); 
	void printStats() {
		std::cout << registr.collisions() << " / " << aut->nodes.size() << "\n";
	}
	// Slow. For debuging.
	bool checkMinimality() const;
};