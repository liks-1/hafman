﻿#include <queue>
#include <iostream>
#include <sstream>
using namespace std;
string cod[256];//таблица кодов

struct Node {
	string value, code;
	
	unsigned amount;
	Node* left;
	Node* right;

	bool operator() (const Node& x, const Node& y) const {
		return x.amount > y.amount;
	}

	// конструктор по умолчанию 
	Node(const string& value = "", unsigned amount = 0, Node * left = 0, Node * right = 0) {
		this->value = value; // множество символом узла
		this->code = ""; //  кода узла
		this->amount = amount; // сколько раз втретилось
		this->left = left; // левый ребенок
		this->right = right; // правый ребенок
	}

	// генерация описания полученного дерева 
	string to_str() {
		ostringstream x;
		if (left != 0 || right != 0) { // дерево такоe, что либо есть оба ребенка либо нет ни одного
			x << "\t" << code << ": " << value << "[" << amount << "] -> " << left->code << ":" << left->value << "[" << left->amount << "]\n";
			x << left->to_str();
			x << "\t" << code << ": " << value << "[" << amount << "] -> " << right->code << ":" << right->value << "[" << right->amount << "]\n";
			x << right->to_str();
		}
		else {
			x << "\t" << code << ": " << value << "[" << amount << "] list\n";
			cod[int(value[0])] = code;//заполнение таблицы кодов

		}
		
		return (x.str());
	}
	// объединение деревьев
	Node* join(Node x) {
		return new Node(x.value + value, x.amount + amount, new Node(x), this);
	}
	// проход по дереву с генерацией кода
	void transform_code(string code) {
		this->code = code;
		if (left != 0 || right != 0) {
			left->transform_code(code + "1");
			right->transform_code(code + "0");
		}
	}

	// строим дерево по алгоритму Хаффмана
	static Node* tree_hafman(priority_queue<Node, vector<Node>, Node> tree) {
		while (tree.size() > 1) {
			Node* n = new Node(tree.top());
			tree.pop();
			tree.push(*n->join(*new Node(tree.top())));
			tree.pop();
		}
		return new Node(tree.top());
	}
	void transform_decode(string s, int len=0) {//декодирование по дереву 
		len++;

		if (left != 0 || right != 0) {
			if (s[len-1] == '1')
				left->transform_decode(s, len);
			else
				right->transform_decode(s, len);
		}
		else
			cout<<(value);
		
	}
};

unsigned amounts[256]; // массив счетчиков встречаемости символов
int main() {
	string s, s_p, s_code[255];
	int l;
	getline(std::cin, s); 
	// читаем строку вместе с пробелами
	s_p = s;
	for (auto i : s) amounts[i]++;
	priority_queue<Node, vector<Node>, Node> graph;

	for (int i = char(32); i <= char(126); i++) // записываем в очередь с приоритетами 
		if (amounts[i] > 0) graph.emplace(s = (char)i, amounts[i]);
	Node* n = Node::tree_hafman(graph);
	n->transform_code("");

	l = s_p.length();

	cout << "{\n" << n->to_str() << "}\n";
	for (int i = 0; i < s_p.length(); i++) {
		s_code[i] = cod[int(s_p[i])];
		cout << cod[int(s_p[i])]<<' ';

	}
	cout << '\n';
	for (int i = 0; i < l; i++) {
		n->transform_decode(s_code[i]);

	}
}