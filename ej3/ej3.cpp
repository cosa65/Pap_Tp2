#include <iostream>
#include <vector>
#include <list>

using namespace std;

/////////////////////////Auxiliares//////////////////////////////////

int min(int a, int b) {
	return (a <= b) * a + (a > b) * b;
}

void agregar_si_no_esta(list<int>& ls, int num) {
	bool esta = false;

	for (list<int>::iterator it = ls.begin(); it != ls.end(); it++) {
		if (*it == num) esta = true;
	}

	if (not esta) {
		ls.push_back(num);
	}
}

/////////////////////////////////////////////////////////////////////

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

	BlockGraphProcessor(vector<list<int> >& edges_matrix, vector<Edge> calles);
	
	bool bridge(Edge e);

	void start_dfs_set_articulations();
	void dfs_set_articulations(int i, int d);

	void start_dfs_without_bridges();
	int dfs_without_bridges(int d, vector<bool>& visited, list<int>& nodes_in_component);

	int start_dfs_bridges_between_nodes(int e1, int e2);
	bool dfs_bridges_between_nodes(int d, int e2, int& bridges, vector<bool>& visited);

	void add_new_component(Edge e);
	void add_last_component();
	void save_component_size(list<int> in_component, int res);
	int get_unvisited_node(vector<bool> visited);
	
	void solve_queries();
	int query_A(int e1, int e2);
	int query_B(int calle);
	int query_C(int e1);

private:

	vector<list<int> > edges_matrix;
	int N;

	vector<int> depth;					//Elementos usados en start_dfs_set_articulations
	vector<int> low;					//Elementos usados en start_dfs_set_articulations
	vector<int> parent;					//Elementos usados en start_dfs_set_articulations

	stack<int> unvisited;				//Elemento para dfs_without_bridges
	
	vector<bool> articulation;						
	
	vector<Edge> calles;				//Necesito esto para poder encontrar el edge que busco en O(1), ya que la query es por nro de calle
	vector<int> connected_components_sizes;		//Tamaños de componentes conexas luego de sacar los puentes
	vector<vector<bool> > is_bridge;

};

BlockGraphProcessor::BlockGraphProcessor(vector<list<int> >& edges_matrix, vector<Edge> calles) {
	this->edges_matrix = edges_matrix;
	N = edges_matrix.size();
	
	depth.resize(N);
	low.resize(N);
	parent.resize(N, -1);

	articulation.resize(N, false);
	
	this->calles = calles;
	connected_components_sizes.resize(N);
	is_bridge.resize(N, vector<bool> (N));
}

void BlockGraphProcessor::dfs_set_articulations(int i, int d) {	
	depth[i] = d;
	low[i] = d;

	for (list<int>::iterator it = edges_matrix[i].begin(); it != edges_matrix[i].end(); it++) {
		if (parent[*it] == -1) {
			parent[*it] = i;
			dfs_set_articulations(*it, d+1);
			if (low[*it] >= depth[i] and i != 0) {
				articulation[i] = true;
			}
			if (low[*it] >= depth[*it]) {
				is_bridge[i][*it] = true;
				is_bridge[*it][i] = true;
			}
			low[i] = min(low[i], low[*it]);
		} else if (parent[i] != *it) {
			low[i] = min(low[i], depth[*it]);
		}
	}
}

void BlockGraphProcessor::start_dfs_set_articulations() {
	int i = 0;
	depth[0] = 0;
	parent[0] = 0;
	low[0] = 0;
	dfs_set_articulations(i, 0);
	bool cambio = false;
	int childLow = low[edges_matrix[0].front()];
	for (list<int>::iterator it = edges_matrix[0].begin(); it != edges_matrix[0].end(); it++) {
		if (low[*it] != childLow) {
			cambio = true;
			break;
		}
		if (low[*it] >= depth[*it]) {
			is_bridge[0][*it] = true;
			is_bridge[*it][0] = true;
		}
	}
	if (cambio) articulation[0] = true;
}

bool BlockGraphProcessor::bridge(Edge e) {
	return is_bridge[e.v][e.w];
}

void BlockGraphProcessor::save_component_size(list<int> nodes_in_component, int size) {
	for (list<int>::iterator it = nodes_in_component.begin(); it != nodes_in_component.end(); it++) {
		connected_components_sizes[*it] = size;
	}
}

int BlockGraphProcessor::get_unvisited_node(vector<bool> visited) {
	
	for (int i = 0; i < N; ++i) {
		if (not visited[i]) {
			return i;
		}
	}
	return -1;
}

void BlockGraphProcessor::start_dfs_without_bridges() {
	int size;
	int next_d = 0;
	unvisited.push(-1);
	vector<bool> visited(N, false);
	do {
		list<int> nodes_in_component(N);
		size = dfs_without_bridges(next_d, visited, nodes_in_component);
		save_component_size(nodes_in_component, size);
		next_d = unvisited.top();
		unvisited.pop();
	}
	while (next_d != -1);
}

int BlockGraphProcessor::dfs_without_bridges(int d, vector<bool>& visited, list<int>& nodes_in_component) {
	visited[d] = true;
	int amount_in_component = 0;
	cout << "Hola soy " << d << endl;
	for (list<int>::iterator it = edges_matrix[d].begin(); it != edges_matrix[d].end(); it++) {
		if (not visited[*it]) {
			if (not bridge(Edge(*it, d))) {
				amount_in_component += dfs_without_bridges(*it, visited, nodes_in_component);
			} else {
				unvisited.push(*it);
			}
		}
	}
	nodes_in_component.push_back(d);
	return amount_in_component + 1;
}

int BlockGraphProcessor::start_dfs_bridges_between_nodes(int e1, int e2) {
	
	vector<bool> visited(N, false);
	int res = 0;
	dfs_bridges_between_nodes(e1, e2, res, visited);
	return res;
}

bool BlockGraphProcessor::dfs_bridges_between_nodes(int d, int e2, int& bridges, vector<bool>& visited) {
	visited[d] = true;
	bool found = false;

	for (list<int>::iterator it = edges_matrix[d].begin(); it != edges_matrix[d].end(); it++) {
		if (*it == e2) {
			if(bridge(Edge(*it, d))) {
				bridges++;
			}
			return true;
		}
		if (!visited[*it]) {
			found = dfs_bridges_between_nodes(*it, e2, bridges, visited);
			if (found) {
				if(bridge(Edge(*it, d))) {
					bridges++;
				}
				return true;
			}
		}
	}

	return false;
}

int BlockGraphProcessor::query_A(int e1, int e2) {
	return start_dfs_bridges_between_nodes(e1, e2);
}

int BlockGraphProcessor::query_B(int num_calle) {
	Edge calle = calles[num_calle];
	if (bridge(calle)) {
		return 1;
	} else {
		return 0;
	}
}

int BlockGraphProcessor::query_C(int e1) {
	return (connected_components_sizes[e1] - 1);
}

void BlockGraphProcessor::solve_queries() {
	int Q, arg;
	cin >> Q;
	vector<int> results;
	vector<int> inputs;
	list<char> query_types;
	char query_type;

	for (int i = 0; i < Q; i++) {			//Recibiendo input de queries
		cin >> query_type;
		if (query_type == 'A') {
			cin >> arg;
			inputs.push_back(arg-1);
		}
		cin >> arg;
		inputs.push_back(arg-1);
		query_types.push_back(query_type);
	}

	int i = 0;

	for (list<char>::iterator it = query_types.begin(); it != query_types.end(); it++) {				//Procesando queries y guardando resultados
		switch (*it) {
			case 'A':
				results.push_back(query_A(inputs[i], inputs[i + 1]));
				++i;
				break;
			case 'B':
				results.push_back(query_B(inputs[i]));
				break;
			case 'C':
				results.push_back(query_C(inputs[i]));
				break;
		}
		++i;
	}

	for (i = 0; i < Q; ++i) {
		cout << results[i] << endl;
	}
}

int main() {

	int N, M;
	cin >> N >> M;

	vector<list<int> > edges_matrix(N, list<int>());
	vector<Edge> calles;
	int e1, e2;

	for (int i = 0; i < M; i++) {
		cin >> e1;
		cin >> e2;
		e1--; e2--;
		edges_matrix[e1].push_back(e2);
		edges_matrix[e2].push_back(e1);
		calles.push_back(Edge(e1, e2));
	}
	
		BlockGraphProcessor solver(edges_matrix, calles);
		solver.start_dfs_set_articulations();
		solver.start_dfs_without_bridges();						//Preprocesar los datos para query C
		solver.solve_queries();

	return 0;
}