#include <iostream>
#include <vector>
#include <list>
#include <stack>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*class PaintedNode() {

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
	for (list<int>::iterator it = groups.begin(); it != groups.end(); it++) {
		if (*it == group_num) return true;
	}
	return false;
}

void PaintedNode::print_groups() {
	for (list<int>::iterator it = groups.begin(); it != groups.end(); it++) {
		cout << " " << *it << " ";
	}
}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int min(int a, int b) {
	return (a >= b) * a + (a < b) * b;
}

void dfs_get_articulations(int curr_node, vector<list<int> >& city, vector<int>& depth, vector<int>& low, vector<int>& parent, int d) {

	for (list<int>::iterator it = city[curr_node].begin(); it != city[curr_node].end(); it++) {
		depth[*it] = d;
		low[*it] = d;
		if (parent[*it] == -1) {
			parent[*it] = curr_node;
			dfs_get_articulations(*it, city, depth, low, parent, d+1);
			++d;
		} else if(parent[curr_node] != *it) {
			low[curr_node] = min(low[curr_node], depth[*it]);
		}
	}
}

void create_block_cut_tree(vector<list<int> >& city) {

	vector<int> depth(city.size());
	vector<int> low(city.size());
	vector<int> parent(city.size(), -1);
	int curr_node = 0;
	depth[0] = 0;
	//parent[0] = 0;
	low[0] = 0;
	int d = 0;
	dfs_get_articulations(curr_node, city, depth, low, parent, d);
}

int main() {

	int N, M;
	cin >> N >> M;

	vector<list<int> > la_matris(N, list<int>());
	int e1, e2;

	for (int i = 0; i < M; i++) {
		cin >> e1;
		cin >> e2;
		la_matris[e1].push_back(e2);
		la_matris[e2].push_back(e1);
	}

	return 0;
}