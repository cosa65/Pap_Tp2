#include "iostream"
#include "matrizDeFlujo.cpp"

using namespace std;

int main(){
	int n, d;
	cin >> n >> d;
    int source = 2*n;
    int sink = 2*n + 1;
	MatrizDeFlujo mat(2*n + 2, source, sink);
	vector<vector<int> > ady(n, vector<int>(n, 0));
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
				mat.agregarEje(i, n+j, 1, 0);
			}
		}
	}

	for (int i = 0; i<n; i++)
		mat.agregarEje(source, i, 1, 0);

	for (int i = 0; i<n; i++)
		mat.agregarEje(n + i, sink, 1, 0);

	cout << n - mat.EdmondsKarp() << endl;
}
