#include "iostream"
#include "matrizDeFlujo.cpp"

int main(){
	int n, m;
	cin >> n >> m;
	vector<int> alumnos;
	vector<int> escuelas;
	char type;
    int source = 2*n;
    int sink = 2*n + 1;
	MatrizDeFlujo mat(2*n + 2, source, sink);
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
		mat.agregarEje(ady1-1, n + ady2-1, 1, 0);
		mat.agregarEje(ady2-1, n + ady1-1, 1, 0);
	}
    //Eje de in-node a out-node
	for (int i = 0; i<n; i++) 
		mat.agregarEje(n+i, i, 1, 0);

    for (int i = 0; i<alumnos.size(); i++)
		mat.agregarEje(source, n + alumnos[i], 1, 0);

	for (int i = 0; i<escuelas.size(); i++)
		mat.agregarEje(escuelas[i], sink, 1, 0);
    
	cout << mat.EdmondsKarp() << endl;
}
