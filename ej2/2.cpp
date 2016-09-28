#include "iostream"
#include <vector>
#include <queue>

using namespace std;

int BFS(vector<vector<int> >& C, vector<vector<int> >& E, int s, int t, vector<vector<int> >& F, int n, vector<int>& P) {
    for (int u = 0; u < n; u++)
        P[u] = -1;
    P[s] = -2; //(make sure source is not rediscovered)
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
	int n, d;
	cin >> n >> d;
	vector<vector<int> > ady(n, vector<int>(n, 0));
	vector<vector<int> > adylista(2*n + 2);
	vector<vector<int> > C(2*n + 2, vector<int>(2*n + 2, 0));
	vector<vector<int> > precios(n, vector<int>(d));
	for (int i = 0; i<n; i++)
		for (int j = 0; j<d; j++) 
			cin >> precios[i][j];
	for (int i = 0; i<d; i++) {
		for (int j = 0; j<n; j++) {
			for (int k = 0; k < n; k++) {
				if (precios[j][i] < precios[k][i]) {
					if (ady[k][j] == 0)
						ady[j][k] = 1;
					else {
						ady[j][k] = -1;
						ady[k][j] = -1;
					}
				} if (precios[j][i] == precios[k][i]) {
					ady[j][k] = -1;
					ady[k][j] = -1;
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j=0; j<n; j++) {
			if (ady[i][j] == 1) {
				adylista[i].push_back(n + j);
				adylista[n + j].push_back(i);
				C[i][n + j] = 1;
			}
		}
	}

    int source = 2*n;
    int sink = 2*n + 1;

	for (int i = 0; i<n; i++) {
		adylista[source].push_back(i);
		adylista[i].push_back(source);
        C[source][i] = 1;
	}

	for (int i = 0; i<n; i++) {
        adylista[n + i].push_back(sink);
		adylista[sink].push_back(n + i);
        C[n+i][sink] = 1;
    }

	cout << n - EdmondsKarp(C, adylista, source, sink, 2*n + 2) << endl;
}
