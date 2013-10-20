#pragma once
#include "fast_erase_vector.h"
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;

class AutomatonBuilder;

class Automaton {
	struct Edge {
		char ch;
		int to;
		explicit Edge(char c = '\0', int t = -1): ch(c), to(t) { }
		bool operator<(const Edge& edge) const {
			if (ch < edge.ch) {
				return true;
			} else if (ch > edge.ch) {
				return false;
			} else {
				return to < edge.to;
			}
		}
		bool operator==(const Edge& edge) const {
			return ch == edge.ch && to == edge.to;
		}
		unsigned hash() const;
	};
	class Node {
	public:
		bool isTerm;
		vector<Edge> edges;

		Node(): isTerm(false) { }
		explicit Node(bool term): isTerm(term) { }
		Node(const Node& node): isTerm(node.isTerm), edges(node.edges) { }
		unsigned hash() const;
		int next(char c) const;
		void link(char c, int node);
		int relink(char c, int node);
		bool operator==(const Node& node) const;
		void writeToBinaryStream(ofstream& out) const;
		bool readFromBinaryStream(ifstream& in);
	};
private:
	int startNode;
	FastEraseVector<Node> nodes;
	void rightLanguage(int node, vector<string>& strs, string& myString) const;
public:
	friend class AutomatonBuilder;
	explicit Automaton(int size = 0): nodes(size) { 
		startNode = nodes.add(Node(false));
	}
	void allSuffixes(const string& word, vector<string>* res) const;
	void writeToBinaryStream(ofstream& out) const;
	void readFromBinaryStream(ifstream& in);
};