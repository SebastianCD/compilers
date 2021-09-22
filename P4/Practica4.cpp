#include<bits/stdc++.h>

using namespace std;

vector<char> simbolos;

struct transicion{
	char simbolo;
	int estadoA;
	int estadoB;
};

class AFD{
	public:
	vector<transicion> transiciones;
	vector<transicion> nuevasTransiciones;
	vector<int> estados;
	vector<pair<int,vector<int>>> nuevosEstados;
	vector<char> simbolos;
	vector<int> finales;
	vector<int> nuevosFinales;
	vector<int> nofinales;
	vector<vector<vector<int>>> particiones;
	set<int> setFinales;
	int final;
	int inicial;

	void reducirAFD(){
		obtenerParticiones();
		obtenerNuevosEstados();
		obtenerNuevosFinales();
		obtenerNuevasTransiciones();
		obtenerNuevoInicial();
	}

	void obtenerParticiones(){
		int mx=0;
		obtenerPrimerParticion();
		for(int i=0; i<particiones.size();i++){
			if(particiones[i].size()==mx){
				break;
			}
			mx=particiones[i].size();
			vector<vector<int>> nuevaParticion;
			for(int j=0;j<particiones[i].size();j++){
				vector<int> particionesAlcanzadasA;
				vector<pair<char,int>> estadosAlcanzadosA;
				int a =particiones[i][j][0];
				estadosAlcanzadosA=encontrarEstadosAlcanzados(a);
				particionesAlcanzadasA=encontrarParticionesAlcanzadas(estadosAlcanzadosA,i);
				vector<int> grupoA;
				grupoA.push_back(a);
				vector<int> grupoB;
				for(int k=1;k<particiones[i][j].size();k++){
					vector<int> particionesAlcanzadasB;
					vector<pair<char,int>> estadosAlcanzadosB;
					estadosAlcanzadosB=encontrarEstadosAlcanzados(particiones[i][j][k]);
					particionesAlcanzadasB=encontrarParticionesAlcanzadas(estadosAlcanzadosB,i);
					if(estadosAlcanzadosA.size() == estadosAlcanzadosB.size()){
						bool fl=false;
						for(int l=0; l<particionesAlcanzadasA.size();l++){
							if(particionesAlcanzadasA[l]!=particionesAlcanzadasB[l]){
								grupoB.push_back(particiones[i][j][k]);
								fl=false;
								break;
							}
							else{
								fl=true;
							}
						}
						if(fl){
							grupoA.push_back(particiones[i][j][k]);
						}
					}
					else{
						grupoB.push_back(particiones[i][j][k]);
					}
				}
				if(grupoB.size()>0){
					nuevaParticion.push_back(grupoB);
				}
				if(grupoA.size()>0){
					nuevaParticion.push_back(grupoA);
				}
			}
			if(nuevaParticion.size()>0){
				particiones.push_back(nuevaParticion);
			}
		}
	}

	vector<pair<char,int>> encontrarEstadosAlcanzados(int estado){
		vector<pair<char,int>> respuesta;
		for(int i=0;i<transiciones.size();i++){
			transicion trans;
			trans=transiciones[i];
			if(trans.estadoA==estado){
				respuesta.push_back(make_pair(trans.simbolo,trans.estadoB));
			}
		}
		sort(respuesta.begin(),respuesta.end());
		return respuesta;
	}

	vector<int> encontrarParticionesAlcanzadas(vector<pair<char,int>> estadosAlcanzados,int numParticion){
		vector<int> respuesta;
		for(int i=0; i<estadosAlcanzados.size();i++){
			for(int j=0;j<particiones[numParticion].size();j++){
				for(int k=0;k<particiones[numParticion][j].size();k++){
					if(particiones[numParticion][j][k]==estadosAlcanzados[i].second){
							respuesta.push_back(j);
					}
				}
			}
		}
		return respuesta;
	}

	void obtenerPrimerParticion(){
		vector<vector<int>> primerParticion;
		vector<int> fin;
		vector<int> noFin;
		for(int i=0;i<finales.size();i++){
			setFinales.insert(finales[i]);
			fin.push_back(finales[i]);
		}
		for(int j=0; j<estados.size();j++){
			if(setFinales.count(estados[j])==0){
				noFin.push_back(estados[j]);
			}
		}
		primerParticion.push_back(fin);
		if(noFin.size()>0){
			primerParticion.push_back(noFin);
		}
		particiones.push_back(primerParticion);
	}

	void obtenerNuevoInicial(){
		for(int i=0; i<nuevosEstados.size();i++){
			for(int j=0; j<nuevosEstados[i].second.size();j++){
				if(nuevosEstados[i].second[j]==inicial){
					inicial=i;
				}
			}
		}
	}

	void obtenerNuevasTransiciones(){
		for(int i=0; i<nuevosEstados.size();i++){
			vector<int> particionesAlcanzadasA;
			vector<pair<char,int>> estadosAlcanzadosA;
			int a=nuevosEstados[i].second[0];
			estadosAlcanzadosA=encontrarEstadosAlcanzados(a);
			particionesAlcanzadasA=encontrarParticionesAlcanzadas(estadosAlcanzadosA,particiones.size()-1);
			for(int j=0;j<particionesAlcanzadasA.size();j++){
				nuevaTransicion2(i,particionesAlcanzadasA[j],estadosAlcanzadosA[j].first);
			}
		}
	}

	void obtenerNuevosFinales(){
		set<int> setNuevosFinales;
		for(int i=0; i<nuevosEstados.size();i++){
			for(int j=0; j<nuevosEstados[i].second.size();j++){
				if(setFinales.count(nuevosEstados[i].second[j])==1 and setNuevosFinales.count(nuevosEstados[i].first)==0){
					setNuevosFinales.insert(nuevosEstados[i].first);
					nuevosFinales.push_back(nuevosEstados[i].first);
				}
			}
		}
	}

	void obtenerNuevosEstados(){
		int numEst=0;
		for(int j=0; j<particiones[particiones.size()-1].size();j++){
			vector<int> estado;
			for(auto k:particiones[particiones.size()-1][j]){
				estado.push_back(k);
			}
			nuevosEstados.push_back(make_pair(numEst,estado));
			numEst++;
		}
	}

	void nuevaTransicion2(int estadoA, int estadoB, char simbolo){
		transicion aux;
		aux.estadoA=estadoA;
		aux.estadoB=estadoB;
		aux.simbolo=simbolo;
		nuevasTransiciones.push_back(aux);
	}

	void establecerTransicion(string stringTransicion){
		string aux;
		int edoA=-1;
		int edoB=-1;
		char sim;
		int escaner=0;
		while(escaner<stringTransicion.size()){
			if(stringTransicion[escaner]!=' '){
				aux+=stringTransicion[escaner];
			}
			else{
				if(edoA==-1){
					edoA=stoi(aux);
				}
				else{
					edoB=stoi(aux);
				}
				aux="";
			}
			escaner++;
		}
		sim=aux[0];
		transicion nuevaTrans;
		nuevaTrans.estadoA=edoA;
		nuevaTrans.estadoB=edoB;
		nuevaTrans.simbolo=sim;
		transiciones.push_back(nuevaTrans);
	}


	void establecerFinales(string stringFinales){
		string aux;
		int escaner=0;
		while(escaner<stringFinales.size()){
			if(stringFinales[escaner]!=' '){
				aux+=stringFinales[escaner];
			}
			else{
				finales.push_back(stoi(aux));
				aux="";
			}
			escaner++;
		}
		finales.push_back(stoi(aux));
	}

	void establecerSimbolos(string stringSimbolos){
		char aux;
		int escaner=0;
		while(escaner<stringSimbolos.size()){
			if(stringSimbolos[escaner]!=' '){
				aux=stringSimbolos[escaner];
			}
			else{
				simbolos.push_back(aux);
			}
			escaner++;
		}
		simbolos.push_back(aux);
	}

	void establecerEstados(string stringEstados){
		string aux;
		int escaner=0;
		while(escaner<stringEstados.size()){
			if(stringEstados[escaner]!=' '){
				aux+=stringEstados[escaner];
			}
			else{
				estados.push_back(stoi(aux));
				aux="";
			}
			escaner++;
		}
		estados.push_back(stoi(aux));
	}

	void establecerInicial(int ini){
		inicial=ini;
	}

	void mostrar(){
		transicion trans;
		cout<<"digraph AFN{\nrankdir = LR;"<<endl;
		cout<<"node [shape = doublecircle];";
		for(int i=0; i<int(nuevosFinales.size());i++){
			cout<<" "<<nuevosFinales[i];
		}
		cout<<";"<<endl;
		cout<<"node [shape = point ];"<<" qi"<<endl;
		cout<<"node [shape = circle];"<<endl;
		cout<<"qi"<<" -> "<<inicial<<endl;
		for(int i = 0; i < int(nuevasTransiciones.size()); i++) {
			trans = nuevasTransiciones[i];
			cout<<trans.estadoA<<" -> "<<trans.estadoB<<" [label =\""<<trans.simbolo<<"\"];"<<endl;
		}
		cout<<"}"<<endl;
	}
};

int main(){
	ifstream archivo;
	string line;
	vector<string> lines;
	archivo.open("input",ios::in); //Abrimos el archivo en modo lectura
	
	if(archivo.fail()){
		cout<<"No se pudo abrir el archivo";
		exit(1);
	}
	
	while(!archivo.eof()){ //mientras no sea final del archivo
		getline(archivo,line);
		lines.push_back(line);
	}
	
	archivo.close();
	AFD respuestaAFD;
	respuestaAFD.establecerEstados(lines[0]);
	respuestaAFD.establecerSimbolos(lines[1]);
	respuestaAFD.establecerInicial(stoi(lines[2]));
	respuestaAFD.establecerFinales(lines[3]);
	for(int i=4; i<lines.size();i++){
		respuestaAFD.establecerTransicion(lines[i]);
	}
	respuestaAFD.reducirAFD();
	respuestaAFD.mostrar();
	return 0;
}