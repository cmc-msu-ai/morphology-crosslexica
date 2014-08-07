#include "StdAfx.h"
#include "const automaton.h"

int ConstAutomaton::nextNode(int node, char c) const {
	int lower = std::lower_bound(edges.begin() + nodes[node], edges.begin() + nodes[node + 1], Edge(c)) - edges.begin();
	if (lower < (int)edges.size() && edges[lower].ch == c)
		return edges[lower].to;
	else
		return -1;
}

void ConstAutomaton::rightLanguage(int node, vector<string>& strs, string& myString = string()) const {
	if (isTerm[node]) {
		strs.push_back(myString);
	}
	for (int i = nodes[node]; i < nodes[node + 1]; ++i) {
		myString.push_back(edges[i].ch);
		rightLanguage(edges[i].to, strs, myString);
		myString.resize(myString.size() - 1);
	}
}

void ConstAutomaton::allSuffixes(const string& word, vector<string>* res) const {
	res->clear();
	int node = startNode;
	for (int i = 0; i < (int)word.size(); ++i) {
		int next = nextNode(node, word[i]);
		if (next != -1) {
			node = next;
		} else {
			return;
		}
	}
	rightLanguage(node, *res);
}

void ConstAutomaton::writeToBinaryStream(ofstream& out) const {
	out.write(reinterpret_cast<const char*>(&startNode), 4);
	int maxNumber = nodes.size() - 1;
	out.write(reinterpret_cast<const char*>(&maxNumber), 4);
	for (int i = 0; i < maxNumber; ++i) {
		if (nodes[i] == nodes[i + 1] && !isTerm[i]) {
			char c = char(-1);
			out.write(&c, 1);
		} else {
			if (isTerm[i]) {
				char c = char(1);
				out.write(&c, 1);
			} else {
				char c = char(0);
				out.write(&c, 1);
			}
			const int size = nodes[i + 1] - nodes[i];
			out.write(reinterpret_cast<const char*>(&size), 4);
			for (int j = nodes[i]; j < nodes[i + 1]; ++j) {
				out.write(&edges[j].ch, 1);
				out.write(reinterpret_cast<const char*>(&edges[j].to), 4);
			}
		}
	}
}

void ConstAutomaton::readFromBinaryStream(ifstream& in) {
	nodes.clear();
	edges.clear();
	isTerm.clear();
	char buff[4];
	in.read(buff, 4);
	startNode = *(reinterpret_cast<int*>(buff));
	in.read(buff, 4);
	const int maxNumber = *(reinterpret_cast<int*>(buff));
	nodes.resize(maxNumber + 1);
	isTerm.resize(maxNumber);
	edges.reserve(5 * maxNumber);
	for (int i = 0; i < maxNumber; ++i) {
		nodes[i] = edges.size();
		in.read(buff, 1);
		if (int(buff[0]) == 0) {
			isTerm[i] = false;
		} else if (int(buff[0]) == 1) {
			isTerm[i] = true;
		} else {
			continue;
		}
		in.read(buff, 4);
		const int size = *(reinterpret_cast<int*>(buff));
		edges.resize(edges.size() + size);
		for (int j = nodes[i]; j < (int)edges.size(); ++j) {
			in.read(&edges[j].ch, 1);
			in.read(buff, 4);
			edges[j].to = *(reinterpret_cast<int*>(buff));
		}
	}
	nodes[maxNumber] = edges.size(); 
}