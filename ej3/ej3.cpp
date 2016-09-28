#include <iostream>
#include <vector>
#include <list>
#include <stack>

using namespace std;

class PaintedNode() {

	public:
	
	PaintedNode()//int id);
	PaintedNode(list<int> groups);//, int id);
	void add_to_group(int new_col);
	bool in(int group_num);
	void print_groups();

	private:
	
	//int id;
	list<int> groups;
};

PaintedNode::PaintedNode(int id) {}

PaintedNode::PaintedNode(int id, list<int> groups) {
	this.groups = groups;
}

void PaintedNode::add_to_group(int new_col) {
	this.groups.push_back(new_col);
}

bool PaintedNode::in(int group_num) {
	for (list<int>::iterator::it = groups.begin(); it != groups.end(); it++) {
		if (*it == group_num) return true;
	}
	return false;
}

void PaintedNode::print_groups() {
	for (list<int>::iterator::it = groups.begin(); it != groups.end(); it++) {
		cout << " " << *it << " ";
	}
}

void push_unmarked_childs(int curr_node, vector<list<int> >& city, stack<int>& main_stack, vector<bool>& visited) {

	for (list::iterator::it = city[curr_node].begin(); it != city[curr_node].end(); it++) {
		if (not visited[*it]) {
			it.push(*it);
			visited[*it] = true;
		}
	}
}

void create_blockCutTree(vector<list<int> >& city, vector<list<PaintedNode> >& res) {
		//Hasta ahora es solo un dfs, despues le agrego toda la fruta
	vector<int> depths(city.size())
	stack<int> main_stack;
	vector<bool> visited(city.size());

	int left = N;
	int curr_node = 0;
	while (left != 0) {
		main_stack.push_unmarked_childs(curr_node, city, main_stack, visited);
		curr_node = main_stack.top();
		main_stack.pop();
	}
}

int main() {

	int N, M;
	cin >> N >> M;

	vector<list<int> > city(N, list());
	int e1, e2;

	for (int i = 0; i < M; i++) {
		cin >> e1;
		cin >> e2;
		vector[e1].push_back(e2);
		vector[e2].push_back(e1);
	}

	vector<PaintedNode> ();

	return 0;
}