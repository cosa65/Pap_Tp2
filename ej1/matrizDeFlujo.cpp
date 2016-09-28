#include <vector>
#include <queue>

using namespace std;

struct MatrizDeFlujo {
	public:
		MatrizDeFlujo(int n, int source, int sink);
		void agregarEje(int nodoA, int nodoB);
		void agregarEje(int nodoA, int nodoB, int capacidadAB, int capacidadBA);
		int EdmondsKarp();
	private:
		const vector<int>& getAdy(int nodo);
		const int getCap(int nodoA, int nodoB);
		int BFS(const vector<vector<int> >& F, vector<int>& P);

		vector<vector<int> > cap;
		vector<vector<int> > ady;
		const int n;
		const int sink;
		const int source;
};

MatrizDeFlujo::MatrizDeFlujo(int n, int source, int sink) : 
	n(n),
	source(source),
	sink(sink),
	cap(n, vector<int>(n, 0)),
	ady(n) {
}

void MatrizDeFlujo::agregarEje(int nodoA, int nodoB) {
	ady[nodoA].push_back(nodoB);
	ady[nodoB].push_back(nodoA);
}

void MatrizDeFlujo::agregarEje(int nodoA, int nodoB, int capacidadAB, int capacidadBA) {
	agregarEje(nodoA, nodoB);
	cap[nodoA][nodoB] = capacidadAB;
	cap[nodoB][nodoA] = capacidadBA;
}

const vector<int>& MatrizDeFlujo::getAdy(int nodo) {
	return ady[nodo];
}

const int MatrizDeFlujo::getCap(int nodoA, int nodoB) {
	return cap[nodoA][nodoB];
}

int MatrizDeFlujo::BFS(const vector<vector<int> >& F, vector<int>& P) {
    for (int u = 0; u < n; u++)
        P[u] = -1;
    P[source] = -2; //(make sure source is not rediscovered)
    queue<int> Q;
    Q.push(source);
    while (Q.size() > 0) {
        int u = Q.front();
		Q.pop();
		vector<int> E = getAdy(u);
        for (int i = 0; i<E.size(); i++) {
			int v = E[i];
            // (If there is available capacity, and v is not seen before in search)
            if (getCap(u, v) - F[u][v] > 0 && P[v] == -1) {
                P[v] = u;
                if (v != sink) 
                    Q.push(v);
                else
                    return 1;
		    }
		}
	}
    return 0;
//M[t]: Capacity of path found
//P: Parent table
}

int MatrizDeFlujo::EdmondsKarp() {
    int f = 0; //(Initial flow is zero)
    vector<vector<int> > F(n, vector<int>(n)); //(Residual capacity from u to v is C[u,v] - F[u,v])
    vector<int> P(n);
    while(1) {
        int m = BFS(F, P);
        if (m == 0)
            break;
        f += m;
        //(Backtrack search, and write flow)
        int v = sink;
        while (v != source) {
            int u = P[v];
            F[u][v] += m;
            F[v][u] -= m;
            v = u;
		}
	}
    return f;
//f: max flow
//F: flow path
}
