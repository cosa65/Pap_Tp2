#include "iostream"
#include <vector>
#include <queue>

using namespace std;

int BFS(vector<vector<int> >& C, vector<vector<int> >& E, int s, int t, vector<vector<int> >& F, int n, vector<int>& P) {
    for (int u = 0; u < n; u++)
        P[u] = -1;
    P[s] = -2; //(make sure source is not rediscovered)
    vector<int> M(n); //(Capacity of found path to node)
    queue<int> Q;
    Q.push(s);
    while (Q.size() > 0) {
        int u = Q.front();
		Q.pop();
        for (int i = 0; i<E[u].size(); i++) {
			int v = E[u][i];
            // (If there is available capacity, and v is not seen before in search)
            if (C[u][v] - F[u][v] > 0 && P[v] == -1) {
                P[v] = u;
                if (v != t) 
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

int EdmondsKarp(vector<vector<int> >& C, vector<vector<int> >& E, int s, int t, int n) {
    int f = 0; //(Initial flow is zero)
    vector<vector<int> > F(n, vector<int>(n)); //(Residual capacity from u to v is C[u,v] - F[u,v])
    vector<int> P(n);
    while(1) {
        int m = BFS(C, E, s, t, F, n, P);
        if (m == 0)
            break;
        f += m;
        //(Backtrack search, and write flow)
        int v = t;
        while (v != s) {
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

int main(){
	int n, m;
	cin >> n >> m;
	vector<int> alumnos;
	vector<int> escuelas;
	vector<vector<int> > ady(2*n + 2);
	vector<vector<int> > C(2*n + 2, vector<int>(2*n + 2, 0));
	char type;
	for (int i=0; i<n; i++) {
		cin >> type;
		if (type == 'A') {
			alumnos.push_back(i);
		} else if (type == 'E') {
			escuelas.push_back(i);
		}
	} 
	int ady1, ady2;
	for (int i = 0; i<m; i++) {
		cin >> ady1 >> ady2;
		ady[ady1-1].push_back(n + ady2-1);
		ady[ady2-1].push_back(n + ady1-1);
		C[ady1-1][n + ady2-1] = 1;
		C[ady2-1][n + ady1-1] = 1;
	}
    //Eje de in-node a out-node
	for (int i = 0; i<n; i++) {
		ady[i].push_back(n + i);
		ady[n + i].push_back(i);
		C[n + i][i] = 1;
	}

    int source = 2*n;
    int sink = 2*n + 1;

    for (int i = 0; i<alumnos.size(); i++) {
        ady[n + alumnos[i]].push_back(source);
        ady[source].push_back(n + alumnos[i]);
       	C[source][n + alumnos[i]] = 1;
	}

	for (int i = 0; i<escuelas.size(); i++) {
        ady[sink].push_back(escuelas[i]);
        ady[escuelas[i]].push_back(sink);
        C[escuelas[i]][sink] = 1;
	}
    
	cout << EdmondsKarp(C, ady, source, sink, 2*n + 2) << endl;
}
