#include<bits/stdc++.h>

using namespace std;

set<string> noTerminales;
set<string> Terminales;
vector<pair<string,vector<string>>> gramatica;
vector<pair<string,set<string>>> setsPrimeros;
vector<pair<string,set<string>>> setsSiguientes;

void definirNoTerminales(string noTer){
	noTerminales.insert(noTer);
}

void definirTerminales(vector<string> posiblesTerminales){
	for(int i=0; i<posiblesTerminales.size();i++){
		if(noTerminales.count(posiblesTerminales[i])==0){
			Terminales.insert(posiblesTerminales[i]);
		}
	}
}

void definirGramatica(string derivacion){
	string noTer;
	vector<string> simbolos;
	int i=0;
	while(derivacion[i]!='-'){
		noTer+=derivacion[i];
		i++;
	}
	i+=1;
	for(int j=i+1;j<derivacion.size();j++){
		string aux;
		if(derivacion[j]=='^'){
			simbolos[simbolos.size()-1]+='^';
		}
		else{
			aux+=derivacion[j];
			simbolos.push_back(aux);
		}
	}
	gramatica.push_back(make_pair(noTer,simbolos));
}

set<string> calcularPrimeros(pair<string,vector<string>> produccion){
	set<string> respuesta;
	for(int i=0; i<produccion.second.size();i++){
		if(noTerminales.count(produccion.second[i])==1 and (respuesta.size()==0 or respuesta.count("e")==1)){
			for(int j=0; j<gramatica.size();j++){
				if(gramatica[j].first==produccion.second[i]){
					set<string> aux;
					aux=calcularPrimeros(gramatica[j]);
					for(auto k:aux){
						respuesta.insert(k);
					}
				}
			}
		}
		else if(noTerminales.count(produccion.second[i])==0){
			respuesta.insert(produccion.second[i]);	
			break;
		}
	}
	return respuesta;
}

set<string> calcularSiguientes(pair<string,vector<string>> produccion,string aux){
	set<string> respuesta;
	for(int i=0; i<produccion.second.size();i++){
		if(aux==produccion.second[i]){
			if(i==produccion.second.size()-1){
				respuesta.insert("$");
				for(int j=0; j<gramatica.size();j++){
					for(int k=0;k<gramatica[j].second.size();k++){
						if(produccion.first==gramatica[j].second[k] and produccion.first!=gramatica[j].first){
							set<string> setAux;
							setAux=calcularSiguientes(gramatica[j],produccion.first);
							for(auto m:setAux){
								respuesta.insert(m);
							}
						}
					}
				}
			}
			else if(i<produccion.second.size()-1){
				if(Terminales.count(produccion.second[i+1])){
					respuesta.insert(produccion.second[i+1]);
				}
				else{
					for(int j=0;j<gramatica.size();j++){
						if(gramatica[j].first==produccion.second[i+1]){
							set<string>setAux;
							setAux=calcularPrimeros(gramatica[j]);
							for(auto k:setAux){
								respuesta.insert(k);
							}
						}
					}
					if(respuesta.count("e")==1){
						respuesta.erase("e");
						for(int n=0; n<gramatica.size();n++){
							if(produccion.first==gramatica[n].first){
								set<string> setAux;
								setAux=calcularSiguientes(gramatica[n],produccion.first);
								for(auto k:setAux){
									respuesta.insert(k);
								}
							}
						}
					}
				}
			}
			break;
		}
	}
	return respuesta;
}



void crearTabla(){
	vector<string> simbolos;
	for(auto i: Terminales){
		if(i!="e"){
			simbolos.push_back(i);
		}
	}
	simbolos.push_back("$");
	vector<pair<string,vector<string>>> filas;
	for(auto i: noTerminales){
		vector<string> aux;
		for(int j=0;j<simbolos.size();j++){
			aux.push_back("-");
		}
		filas.push_back(make_pair(i,aux));
	}

	for(int i=0; i<filas.size();i++){
		vector<string> aux;
		for(int j=0;j<setsPrimeros.size();j++){
			if(filas[i].first==setsPrimeros[j].first){
				string comparar;
				for(auto r:setsPrimeros[j].second){
					comparar=r;
				}
				if(comparar=="e"){
					for(int m=0;m<setsSiguientes.size();m++){
						if(setsSiguientes[m].first==filas[i].first){
							for(int l=0; l<simbolos.size();l++){
								for(auto k:setsSiguientes[m].second){
									if(simbolos[l]==k){
										filas[i].second[l]=to_string(j+1);
									}
								}
							}
						}
					}
				}
				else{
					for(int l=0; l<simbolos.size();l++){
						for(auto k:setsPrimeros[j].second){
							if(simbolos[l]==k){
								filas[i].second[l]=to_string(j+1);
							}
						}
					}
				}
			}
		}
	}

	cout<<"   ";
	for(int i=0; i<simbolos.size();i++){
		cout<<simbolos[i]<<" ";
	}
	cout<<endl;
	for(int i=0; i<filas.size();i++){
		cout<<filas[i].first<<"  ";
		for(int j=0;j<filas[i].second.size();j++){
			cout<<filas[i].second[j]<<" ";
		}
		cout<<endl;
	}
}


int main(){
	int cantidad;
	string derivacion;
	vector<string> derivaciones;
	cin>>cantidad;
	for(int i=0; i<cantidad ;i++){
		cin>>derivacion;
		derivaciones.push_back(derivacion);
	}
	for(int i=0;i<derivaciones.size();i++){
		definirGramatica(derivaciones[i]);
	}
	for(int i=0;i<gramatica.size();i++){
		definirNoTerminales(gramatica[i].first);
	}
	for(int i=0;i<gramatica.size();i++){
		definirTerminales(gramatica[i].second);
	}
	for(int i=0;i<gramatica.size();i++){
		set<string> aux;
		aux = calcularPrimeros(gramatica[i]);
		setsPrimeros.push_back(make_pair(gramatica[i].first,aux));
	}
	
	for(int i=0; i<setsPrimeros.size();i++){
		for(auto j:setsPrimeros[i].second){
			if(j=="e"){
				for(int k=0; k<gramatica.size();k++){
					for(int l=0;l<gramatica[k].second.size();l++){
						if(setsPrimeros[i].first==gramatica[k].second[l] and setsPrimeros[i].first!=gramatica[k].first){
							set<string> auxSet;
							auxSet=calcularSiguientes(gramatica[k],setsPrimeros[i].first);
							setsSiguientes.push_back(make_pair(setsPrimeros[i].first,auxSet));
						}
					}
				}
			}
		}
	}

	crearTabla();

	return 0;
}