#include "stdafx.h"
#include "automaton builder.h"
#include "prime numbers.h"
#include <iostream>

int AutomatonBuilder::addNode(const Automaton::Node& node, unsigned nodeHash) {
	int res = aut->nodes.add(node);
	if (res < (int)registered.size()) {
		registered[res] = false;
		incoming[res] = 0;
		hash[res] = nodeHash;
	} else {
		registered.resize(res + 1, false);
		incoming.resize(res + 1, 0);
		hash.resize(res + 1, nodeHash);
	}
	for (int i = 0; i < (int)aut->nodes[res].edges.size(); ++i) {
		incoming[aut->nodes[res].edges[i].to] += 1;
	}
	return res;
}

int AutomatonBuilder::findInRegistr(int nodeNum) const {
	const Automaton::Node& node = aut->nodes[nodeNum];
	const HashTable<Automaton::Node, int>::Node* nd = (registr.get(hash[nodeNum])).head;
	int res = -1;
	while (nd != 0) {
		if (aut->nodes[nd->elem] == node) {
			res = nd->elem;
			break;
		}
		nd = nd->next;
	}
	return res;
}

void AutomatonBuilder::link(int node1, int node2, char c) {
	aut->nodes[node1].link(c, node2);
	incoming[node2] += 1;
	hash[node1] += Automaton::Edge(c, node2).hash();
}

void AutomatonBuilder::relink(int node1, int node2, char c) {
	int oldLink = aut->nodes[node1].relink(c, node2);
	if (oldLink != -1) {
		incoming[oldLink] -= 1;
		incoming[node2] += 1;
		hash[node1] += Automaton::Edge(c, node2).hash() - Automaton::Edge(c, oldLink).hash();
	} else {
		incoming[node2] += 1;
		hash[node1] += Automaton::Edge(c, node2).hash();
	}
}

void AutomatonBuilder::removeNode(int node) {
	for (int i = 0; i < (int)aut->nodes[node].edges.size(); ++i) {
		incoming[aut->nodes[node].edges[i].to] -= 1;
	}
	aut->nodes.remove(node);
}

void AutomatonBuilder::replaceOrRegister(int node) {
	for (int i = 0; i < (int)aut->nodes[node].edges.size(); ++i) {
		if (!registered[aut->nodes[node].edges[i].to]) {
			int child = aut->nodes[node].edges[i].to;
			replaceOrRegister(child);
			int equiv = findInRegistr(child);
			if (equiv != -1) {
				bool reg = registered[node];
				if (reg) {
					registr.remove(hash[node], node);
				}
				removeNode(child);
				relink(node, equiv, aut->nodes[node].edges[i].ch);
				if (reg) {
					registr.add(hash[node], node);
				}
			} else {
				registered[child] = true;
				registr.add(hash[child], child);
			}
			break;
		}
	}
}

bool AutomatonBuilder::replaceOrRegisterWithAnswer(int node) {
	bool changed = false;
	for (int i = 0; i < (int)aut->nodes[node].edges.size(); ++i) {
		if (!registered[aut->nodes[node].edges[i].to]) {
			int child = aut->nodes[node].edges[i].to;
			replaceOrRegister(child);
			int equiv = findInRegistr(child);
			if (equiv != -1) {
				changed = true;
				bool reg = registered[node];
				if (reg) {
					registr.remove(hash[node], node);
				}
				removeNode(child);
				relink(node, equiv, aut->nodes[node].edges[i].ch);
				if (reg) {
					registr.add(hash[node], node);
				}
			} else {
				registered[child] = true;
				registr.add(hash[child], child);
			}
			break;
		}
	}
	return changed;
}


void AutomatonBuilder::makeTerminal(int node) {
	Automaton::Node& nnode = aut->nodes[node];
	if (!nnode.isTerm) {
		nnode.isTerm = true;
		hash[node] += primes::bigPrime2 * primes::bigPrime1;
	}
}

void AutomatonBuilder::addBranch(int node, const string& str) {
	bool reg = registered[node];
	if (reg) {
		registr.remove(hash[node], node);
	}
	int nnode = node;
	for (int i = 0; i < (int)str.size(); ++i) {
		int newNode = addNode(Automaton::Node(), 0);
		link(nnode, newNode, str[i]);
		nnode = newNode;
	}
	makeTerminal(nnode);
	if (reg) {
		registr.add(hash[node], node);
	}
}

int AutomatonBuilder::cloneNode(int node) {
	return addNode(Automaton::Node(aut->nodes[node]), hash[node]);
}

void AutomatonBuilder::addWord(const string& word) {
	vector<int> path;
	path.reserve(word.size());
	int firstConfluenceIndex = -1;
	path.push_back(aut->startNode);
	for (int index = 0; index < (int)word.size(); ++index) {
		int nextNode = aut->nodes[path.back()].next(word[index]);
		if (nextNode != -1) {
			path.push_back(nextNode);
			if (firstConfluenceIndex == -1 && incoming[nextNode] > 1) {
				firstConfluenceIndex = index;
			}
		} else {
			break;
		}
	}
	if (firstConfluenceIndex != -1) {
		path[path.size() - 1] = cloneNode(path.back());
		addBranch(path.back(), word.substr(path.size() - 1));
		int currentIndex = firstConfluenceIndex + 1;
		for (int i = path.size() - 2; i >= currentIndex; --i) {
			path[i] = cloneNode(path[i]);
			relink(path[i], path[i+1], word[i]);
			replaceOrRegister(path[i]);
		}
		registered[path[currentIndex - 1]] = false;
		registr.remove(hash[path[currentIndex - 1]], path[currentIndex - 1]);
		relink(path[currentIndex - 1], path[currentIndex], word[currentIndex - 1]);
		replaceOrRegister(path[currentIndex - 1]);
		registr.add(hash[path[currentIndex - 1]], path[currentIndex - 1]);
		registered[path[currentIndex - 1]] = true;
		currentIndex -= 1;
		bool changed = true;
		while (changed && currentIndex > 0) {
			registered[path[currentIndex]] = false;
			registr.remove(hash[path[currentIndex]], path[currentIndex]);
			currentIndex -= 1;
			changed = replaceOrRegisterWithAnswer(path[currentIndex]);
		}
	} else {
		addBranch(path.back(), word.substr(path.size() - 1));
//		replaceOrRegister(path.back());
		int currentIndex = path.size() - 1;
		if (currentIndex == 0) {
			replaceOrRegister(aut->startNode);
		} else {
			bool changed = true;
			while (changed && currentIndex > 0) {
				registered[path[currentIndex]] = false;
				registr.remove(hash[path[currentIndex]], path[currentIndex]);
				currentIndex -= 1;
				changed = replaceOrRegisterWithAnswer(path[currentIndex]);
			}
		}
	}
}

bool AutomatonBuilder::checkMinimality() const {
	for (int i = 0; i < aut->nodes.size(); ++i) {
		if (aut->nodes.isHole(i)) {
			continue;
		}
		for (int j = 0; j < aut->nodes.size(); ++j) {
			if (j != i && !aut->nodes.isHole(j)) {
				if (aut->nodes[i] == aut->nodes[j]) {
					return false;
				}
			}
		}
	}
	return true;
}