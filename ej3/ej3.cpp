#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <map>

using namespace std;

struct BlockNode{
	BlockNode(int block_id, int first_node);

	int block_id;
	list<int> nodes;
};

BlockNode::BlockNode(int block_id, int first_node) {
	this->block_id = block_id;
	nodes.push_back(first_node);
}

int min(int a, int b) {
	return (a <= b) * a + (a > b) * b;
}

struct Edge {

	Edge(int v, int w) {
		this->v = v;
		this->w = w;
	}

	bool operator== (Edge other) {
		return ((v == other.v) and (w == other.w)) or ((v == other.w) and (w == other.v));
	}

	bool operator!= (Edge other) {
		return !(*this == other);
	}

	int v;
	int w;
};

ostream& operator<< (ostream& stream, Edge& e) {
	stream << "(" << e.v << ", " << e.w << ")";
}

class BlockGraphProcessor {
public:

	BlockGraphProcessor(vector<list<int> >& la_matris);
	void process_articulation_points();
	void dfs_set_articulations(int i, int d);
	void add_new_component(Edge e);
	void add_last_component();
	void create_block_cut_tree();
	bool bridge(Edge e);
	void print_components();
	void print_block_cut_tree();
	void print_components_of_nodes();
	int query_A();
	int query_B();
	int query_C();

private:

	vector<list<int> > la_matris;
	vector<list<int> > block_cut_tree;
	vector<int> depth;
	vector<int> low;
	vector<int> parent;
	vector<bool> articulation;
	vector<int> component;
	list<list<Edge> > components;
	vector<int> components_sizes;
	stack<Edge> members_of_component;
	int components_num;
};

BlockGraphProcessor::BlockGraphProcessor(vector<list<int> >& la_matris) {
	this->la_matris = la_matris;
	//members_of_component;
	//articulation;
	depth.resize(la_matris.size());
	low.resize(la_matris.size());
	parent.resize(la_matris.size(), -1);
	articulation.resize(la_matris.size(), false);
	component.resize(la_matris.size(), -1);
	components_num = 0;
}

void BlockGraphProcessor::dfs_set_articulations(int i, int d) {
	depth[i] = d;
	low[i] = d;
	if(parent[i] != i) {
		members_of_component.push(Edge(i, parent[i]));
	}
	for (list<int>::iterator it = la_matris[i].begin(); it != la_matris[i].end(); it++) {
		if (parent[*it] == -1) {
			parent[*it] = i;
			dfs_set_articulations(*it, d+1);
			if (low[*it] >= depth[i] and i != 0) {
				articulation[i] = true;
				add_new_component(Edge(*it, i));
			}
			low[i] = min(low[i], low[*it]);
		} else if (parent[i] != *it) {
			low[i] = min(low[i], depth[*it]);
		}
	}
}

void BlockGraphProcessor::process_articulation_points() {

	int i = 0;
	depth[0] = 0;
	parent[0] = 0;
	low[0] = 0;
	dfs_set_articulations(i, 0);
	bool cambio = false;
	int childLow = low[la_matris[0].front()];
	for (list<int>::iterator it = la_matris[0].begin(); it != la_matris[0].end(); it++) {
		if (low[*it] != childLow) {
			cambio = true;
			break;
		}
	}

	if (cambio) articulation[0] = true;
	
	add_last_component();			//Agrego al componente del nodo raiz, 
									//que no lo incluye en el proceso normal porque es un caso borde.


	cout << "Puntos de articulacion: ";
	int N = articulation.size();
	for (i = 0; i < N; i++) {
		if (articulation[i]) cout << i << " ";
	}
	cout << endl;

	for(i = 0; i < N; i++) {		//Ahora genero componentes de cada punto de articulacion
		if(articulation[i]) {
			component[i] = components_num;
			components_sizes.push_back(1);
			components_num++;
		}
	}
}

void BlockGraphProcessor::add_new_component(Edge e) {
	list<Edge> new_component;
	Edge top = members_of_component.top();
//	if(not bridge(top)){
		new_component.push_back(top);
		component[top.v] = components_num;
		component[top.w] = components_num;
		while(members_of_component.top() != e) {
			members_of_component.pop();
			top = members_of_component.top();
			new_component.push_back(top);
			component[top.v] = components_num;
			component[top.w] = components_num;
		}
		members_of_component.pop();
		components.push_back(new_component);
		components_num++;
//	} else {
//		members_of_component.pop();
//	}
}

void BlockGraphProcessor::add_last_component() {
	list<Edge> new_component;
	while (not members_of_component.empty()) {
		new_component.push_back(members_of_component.top());
		component[members_of_component.top().v] = components_num;
		component[members_of_component.top().w] = components_num;
		members_of_component.pop();
	}
	if (not new_component.empty()) {
		components_num++;
		components.push_back(new_component);
	}
}

void BlockGraphProcessor::create_block_cut_tree() {
	int i = 0;
	int component_of_v;
	int component_of_w;
	block_cut_tree.resize(components_num);
	for(list<list<Edge> >::iterator iti = components.begin(); iti != components.end(); iti++) {
		list<int> edges_of_new_node;
		cout << "Estoy con el componente " << i << endl;
		for(list<Edge>::iterator itj = iti->begin(); itj != iti->end(); itj++) {
			if(articulation[itj->v] or articulation[itj->w]) {
				component_of_v = component[itj->v];
				component_of_w = component[itj->w];
				block_cut_tree[component_of_v].push_back(component_of_w);
				block_cut_tree[component_of_w].push_back(component_of_v);
			} else {
				components_sizes[component_of_v]+=2;
			}
		}
		i++;
	}
}

bool BlockGraphProcessor::bridge(Edge e) {
	return articulation[e.v] and articulation[e.w];
}

void BlockGraphProcessor::print_components() {
	int i = 0;
	for(list<list<Edge> >::iterator iti = components.begin(); iti != components.end(); iti++) {
		cout << "Componente " << i << endl;
		for(list<Edge>::iterator itj = iti->begin(); itj != iti->end(); itj++) {
			cout << *itj << " ";
		}
		cout << endl;
		i++;
	}
}

void BlockGraphProcessor::print_block_cut_tree() {
	int n = block_cut_tree.size();
	for (int i = 0; i < n; i++) {
		cout << "Component " << i << endl;
		for (list<int>::iterator it = block_cut_tree[i].begin(); it != block_cut_tree[i].end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}

void BlockGraphProcessor::print_components_of_nodes() {
	int N = la_matris.size();
	for(int i = 0; i < N; i++) {
		cout << "Node " << i << " Component " << component[i] << endl;
	}
}

int BlockGraphProcessor::query_A(int e1, int e2) {
	
}

int BlockGraphProcessor::query_B() {
	return -1;
}

int BlockGraphProcessor::query_C() {
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
	
	BlockGraphProcessor solver(la_matris);
	solver.process_articulation_points();
	solver.print_components();
	solver.create_block_cut_tree();
	solver.print_block_cut_tree();
	solver.print_components_of_nodes();

	return 0;
}