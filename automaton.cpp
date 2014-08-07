#include "StdAfx.h"
#include "automaton.h"
#include "prime numbers.h"
#include <algorithm>

inline unsigned Automaton::Edge::hash() const {
	return (primes::bigPrime2 * unsigned(ch) + unsigned(to)) * primes::bigPrime1;
}

unsigned Automaton::Node::hash() const {
	unsigned res = 0;
	if (isTerm) {
		res = primes::bigPrime3;
	}
	for (int i = 0; i < (int)edges.size(); ++i) {
		res += edges[i].hash();
	}
	return res;
}

int Automaton::Node::next(char c) const {
	int lower = std::lower_bound(edges.begin(), edges.end(), Edge(c)) - edges.begin();
	if (lower < (int)edges.size() && edges[lower].ch == c)
		return edges[lower].to;
	else
		return -1;
}

void Automaton::Node::link(char c, int node) {
	int lower = std::lower_bound(edges.begin(), edges.end(), Edge(c)) - edges.begin();
	edges.insert(edges.begin() + lower, Edge(c, node));
}

int Automaton::Node::relink(char c, int node) {
	int lower = std::lower_bound(edges.begin(), edges.end(), Edge(c)) - edges.begin();
	if (edges[lower].ch == c) {
		int res = edges[lower].to;
		edges[lower].to = node;
		return res;
	} else {
		edges.insert(edges.begin() + lower, Edge(c, node));
		return -1;
	}
}

bool Automaton::Node::operator==(const Node& node) const {
	if (isTerm != node.isTerm) {
		return false;
	}
	return edges == node.edges;
}

void Automaton::rightLanguage(int node, vector<string>& strs, string& myString) const {
	const Node& curNode = nodes[node];
	if (curNode.isTerm) {
		strs.push_back(myString);
	}
	for (int i = 0; i < (int)curNode.edges.size(); ++i) {
		myString.push_back(curNode.edges[i].ch);
		rightLanguage(curNode.edges[i].to, strs, myString);
		myString.resize(myString.size() - 1);
	}
}

void Automaton::allSuffixes(const string& word, vector<string>* res) const {
	res->clear();
	int node = startNode;
	for (int i = 0; i < (int)word.size(); ++i) {
		int nextNode = nodes[node].next(word[i]);
		if (nextNode != -1) {
			node = nextNode;
		} else {
			return;
		}
	}
	rightLanguage(node, *res, string(""));
}

void Automaton::Node::writeToBinaryStream(ofstream& out) const {
	if (isTerm) {
		char c = char(1);
		out.write(&c, 1);
	} else {
		char c = char(0);
		out.write(&c, 1);
	}
	const int size = edges.size();
	out.write(reinterpret_cast<const char*>(&size), 4);
	for (int i = 0; i < size; ++i) {
		out.write(&edges[i].ch, 1);
		out.write(reinterpret_cast<const char*>(&edges[i].to), 4);
	}
}

bool Automaton::Node::readFromBinaryStream(ifstream& in) {
	char buff[4];
	in.read(buff, 1);
	if (int(buff[0]) == 0) {
		isTerm = false;
	} else if (int(buff[0]) == 1) {
		isTerm = true;
	} else {
		return false;
	}
	in.read(buff, 4);
	const int size = *(reinterpret_cast<int*>(buff));
	edges.resize(size);
	for (int i = 0; i < size; ++i) {
		in.read(&edges[i].ch, 1);
		in.read(buff, 4);
		edges[i].to = *(reinterpret_cast<int*>(buff));
	}
	return true;
}


void Automaton::writeToBinaryStream(ofstream& out) const {
	out.write(reinterpret_cast<const char*>(&startNode), 4);
	int maxNumber = nodes.maxNumber();
	vector<int> holes = nodes.getHoles();
	std::sort(holes.begin(), holes.end());
	while (!holes.empty() && holes.back() == maxNumber - 1) {
		holes.pop_back();
		--maxNumber;
	}
	out.write(reinterpret_cast<const char*>(&maxNumber), 4);
	int nextHole = 0;
	for (int i = 0; i < maxNumber; ++i) {
		if (nextHole < (int)holes.size() && i == holes[nextHole]) {
			++nextHole;
			char c = char(-1);
			out.write(&c, 1);
		} else {
			nodes[i].writeToBinaryStream(out);
		}
	}
}

void Automaton::readFromBinaryStream(ifstream& in) {
	nodes.clear();
	char buff[4];
	in.read(buff, 4);
	startNode = *(reinterpret_cast<int*>(buff));
	in.read(buff, 4);
	const int maxNumber = *(reinterpret_cast<int*>(buff));
	vector<Node> nodeBuff(maxNumber);
	vector<int> holesBuff;
	for (int i = 0; i < maxNumber; ++i) {
		if (!nodeBuff[i].readFromBinaryStream(in)) {
			holesBuff.push_back(i);
		}
	}
	nodes.init(&nodeBuff, &holesBuff);
}