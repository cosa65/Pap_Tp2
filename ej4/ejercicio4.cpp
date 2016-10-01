#include <iostream>
#include <vector>
#include <string>
#include <algorithm> //max
#include <fstream>
#include <deque>

using namespace std;

int A;
int P;
int Q;
int com_con;


void recorrer(vector<bool> &dfsRes, vector <vector <int> > &adyacencias, int nodo, deque <int> &finalizados, vector<int> &en_componente, int tipo)
{

	dfsRes[nodo] = true; //ya fue descubierto

	for (int i = 0; i < adyacencias[nodo].size(); ++i)
	{
		int nodo_vecino = adyacencias[nodo][i];
		if (dfsRes[nodo_vecino] == false) //ese vecino aun no fue descubierto
			recorrer(dfsRes, adyacencias, nodo_vecino, finalizados, en_componente, tipo);		
	}

	if (tipo == 0)
		finalizados.push_front(nodo);//me guardo los nodos segun el orden de finalización para el caso de GT
	if (tipo == 1)
		en_componente[nodo] = com_con;//me guardo la componente conexa a la cual pertenece el nodo
}


void DFS (deque<int> &nodos, vector<bool> &dfsRes, vector <vector <int> > &adyacencias, deque <int> &finalizados, vector<int> &en_componente, int tipo)
{

	for(int i = 0; i < A; i++)
		if (dfsRes[nodos[i]] == false) //si no descubri ya ese nodo
		{
			recorrer(dfsRes, adyacencias, nodos[i], finalizados, en_componente, tipo);
			if (tipo == 1)
				com_con++;
		}
}

////////////////////////////////
int main()
{

    cin >> A >> P;

	vector <vector <int> > adyacenciasG(A); // vector de adyacencias para G
	vector <vector <int> > adyacenciasGT(A); //vector de adyacencias para G traspuesto
		
	for (int i = 0; i < P; ++i)
	{
		int a_i, b_i;
		cin >> a_i >>  b_i;
		adyacenciasG[a_i - 1].push_back(b_i - 1);
		adyacenciasGT[b_i - 1].push_back(a_i - 1);
	}

	cin >> Q;

	vector <pair <int,int> > questions;

	for (int i = 0; i < Q; ++i)
	{
		int q_i, q_j;
		cin >> q_i >> q_j;
		pair<int, int> elem (q_i -1, q_j - 1);
		questions.push_back(elem); 
	}

	vector<bool> dfsG (A); //resultado del recorrido dfs de G
	vector<bool> dfsGT (A); //resultado del recorrido dfs de GT
	
	for (int i = 0; i < A; ++i)
	{
		dfsG[i] = false; 
 		dfsGT[i] = false; 
	}

	deque<int> finalizados;

	deque<int> nodos;
	for (int i = 0; i < A; ++i)
		nodos.push_back(i);

	vector<int> en_componente (A);

	DFS(nodos, dfsG, adyacenciasG, finalizados, en_componente, 0); 

	com_con = 0; //esto me va a decir en que componente conexa esta cada nodo

	DFS(finalizados, dfsGT, adyacenciasGT, finalizados, en_componente, 1); 

	for (int i = 0; i < Q; ++i)
	{
		if (en_componente[questions[i].first] == en_componente[questions[i].second])
			cout << "S" << endl;
		else
			cout << "N" << endl;
	}

	return 0;
}
