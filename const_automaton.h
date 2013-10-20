#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;

// Automaton that cannot be changed - just read it from binary .aut file.
class ConstAutomaton {
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
	};

	int startNode;
	vector<int> nodes;
	vector<bool> isTerm;
	vector<Edge> edges;
	int nextNode(int node, char c) const;
	void rightLanguage(int node, vector<string>& strs, string& myString) const;
public:
	ConstAutomaton() { }
	void allSuffixes(const string& word, vector<string>* res) const;
	void writeToBinaryStream(ofstream& out) const;
	void readFromBinaryStream(ifstream& in);
	vector<int> degreeStatistics(const int k) const {
		vector<int> res(k + 1, 0);
		for (int i = 0; i < (int)nodes.size() - 1; ++i) {
			if (nodes[i + 1] == nodes[i] && !isTerm[i]) {
				continue;
			}
			if (nodes[i + 1] - nodes[i] > k) {
				++res[k];
			} else {
				++res[nodes[i + 1] - nodes[i]];
			}
		}
		return res;
	}
};