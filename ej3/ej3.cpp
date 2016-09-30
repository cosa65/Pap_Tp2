#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <map>

using namespace std;

typedef int Group;

int min(int a, int b) {
	return (a <= b) * a + (a > b) * b;
}

void dfs_get_articulations(int i, vector<list<int> >& la_matris, vector<int>& depth, vector<int>& low, vector<int>& parent, int d, list<int>& articulations) {
	depth[i] = d;
	low[i] = d;
	for (list<int>::iterator it = la_matris[i].begin(); it != la_matris[i].end(); it++) {
		if (parent[*it] == -1) {
			parent[*it] = i;
			dfs_get_articulations(*it, la_matris, depth, low, parent, d+1, articulations);
			if (low[*it] >= depth[i] and i != 0) articulations.push_back(i);
			low[i] = min(low[i], low[*it]);
		} else if (parent[i] != *it) {
			low[i] = min(low[i], depth[*it]);
		}
	}

}

void create_block_cut_tree(vector<list<int> >& la_matris) {

	vector<int> depth(la_matris.size());
	list<int> articulations;
	vector<int> low(la_matris.size());
	vector<int> parent(la_matris.size(), -1);
	int i = 0;
	depth[0] = 0;
	parent[0] = 0;
	low[0] = 0;
	dfs_get_articulations(i, la_matris, depth, low, parent, 0, articulations);

	bool cambio = false;
	int childLow = low[la_matris[0].front()];
	for (list<int>::iterator it = la_matris[0].begin(); it != la_matris[0].end(); it++) {
		if (low[*it] != childLow) {
			cambio = true;
			break;
		}
	}

	if (cambio) articulations.push_back(0);

	cout << "Puntos de articulacion: ";
	for (list<int>::iterator it = articulations.begin(); it != articulations.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}

map<Group, int> get_groups(vector<list<int> >& la_matris, list<int>& articulations) {
	map<Group, int> res;
	int N = la_matris.size();
	for (int i = 0 ; i < N; i++) {
		res.emplace(i, low[i]);
	}
	return res;
}

int query_A() {
	return -1;
}

int query_B() {
	return -1;
}

int query_C() {
	return -1;	
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
	create_block_cut_tree(la_matris);
	get_groups(la_matris, articulations);

	return 0;
}