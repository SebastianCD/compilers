#include<bits/stdc++.h>

using namespace std;

struct transicion{
	char simbolo;
	int estadoA;
	int estadoB;
};

struct subconjunto{
	vector<int> kernel;
	vector<int> estados;
};

class AFN{
	public:
	int final;
	vector<int> estados;
	vector<transicion> transiciones;

	void numEstados(int num){
		for(int i=0; i<num ;i++){
			estados.push_back(i);
		}
	}
	
	void establecerTransiciones(int a, int b, char simbolo){
		transicion nuevat;
		nuevat.estadoA=a;
		nuevat.estadoB=b;
		nuevat.simbolo=simbolo;
		transiciones.push_back(nuevat);
	}
};

class AFD{
	public:
	int final;
	vector<int> finales;
	vector<int> estados;
	vector<pair<int,subconjunto>> subconjuntos;
	vector<char> simbolos;
	vector<transicion> transiciones;

	void establecerTransiciones(int a, int b, char simbolo){
		transicion nuevat;
		nuevat.estadoA=a;
		nuevat.estadoB=b;
		nuevat.simbolo=simbolo;
		transiciones.push_back(nuevat);
	}

	void establecerInicial(AFN a){
		vector<int> inicial;
		set<int> visitados;
		inicial.push_back(0);
		visitados.insert(0);
		estados.push_back(0);
		for(int i=0; i<int(a.estados.size());i++){
			for(int j=0; j<int(a.transiciones.size());j++){		
				if((visitados.count(i)==1) and (a.transiciones[j].simbolo == 'e') and (i==a.transiciones[j].estadoA)){
					if(visitados.count(a.transiciones[j].estadoB)==0){
						visitados.insert(a.transiciones[j].estadoB);
						inicial.push_back(a.transiciones[j].estadoB);
					}
				}
			}
		}
		subconjunto nuevoSC;
		nuevoSC.kernel=inicial;
		sort(inicial.begin(),inicial.end());
		nuevoSC.estados=inicial;
		subconjuntos.push_back(make_pair(0,nuevoSC));
	}

	set<vector<int>> kernels;
	int numEstado=0;

	void establecerFinales(){
		for(int i=0; i<int(subconjuntos.size()); i++){
			if(subconjuntos[i].second.estados.size()!=0){
				int n = subconjuntos[i].second.estados.size()-1;
				if(subconjuntos[i].second.estados[n]==final){
					finales.push_back(subconjuntos[i].first);
				}
			}
		}
	}

	void comprobarRepeticion(int estadoA, vector<int> kernelRepetido,char simbolo){
		set<vector<int>> analizados;
		for(int i=0; i<int(subconjuntos.size());i++){
			analizados.insert(subconjuntos[i].second.kernel);
			if(analizados.count(kernelRepetido)==1){
				cout<<estadoA<<" "<<subconjuntos[i].first<<" "<<simbolo<<endl;
				establecerTransiciones(estadoA,subconjuntos[i].first,simbolo);
				break;
			}
		}
	}

	void nuevoSubconjunto(int est, vector<int> kernel, vector<int> estados){
		subconjunto sub;
		sub.kernel=kernel;
		sub.estados=estados;
		subconjuntos.push_back(make_pair(est,sub));
	}

	vector<int> mover(subconjunto sub, char c, AFN a){
		vector<int> nuevoKernel;
		for(int j=0; j<int(sub.estados.size()); j++){
			for(int i=0; i<int(a.transiciones.size());i++){
				if(sub.estados[j]==a.transiciones[i].estadoA and c==a.transiciones[i].simbolo){
					nuevoKernel.push_back(a.transiciones[i].estadoB);
				}
			}
		}
		return nuevoKernel;
	}

	vector<int> CerraduraEp(vector<int> kernel, AFN a){
		vector<int> nuevoEpsilon;
		set<int> visitados;
		for(int i=0; i<int(kernel.size());i++){
			visitados.insert(kernel[i]);
			nuevoEpsilon.push_back(kernel[i]);
		}
		for(int i=0; i<int(nuevoEpsilon.size()) ;i++){
			for(int j=0; j<int(a.transiciones.size());j++){
				if((visitados.count(nuevoEpsilon[i])==1) and (a.transiciones[j].simbolo == 'e') and (nuevoEpsilon[i]==a.transiciones[j].estadoA)){
					if(visitados.count(a.transiciones[j].estadoB)==0){
						visitados.insert(a.transiciones[j].estadoB);
						nuevoEpsilon.push_back(a.transiciones[j].estadoB);
					}
				}
			}
		}
		return nuevoEpsilon;
    }

	void crearSubconjuntos(AFN a){
		for(int i=0; i<int(subconjuntos.size()); i++){
			for(int j=0; j<int(simbolos.size());j++){
				vector<int> crearKernel;
				vector<int> crearEpsilon;
				crearKernel=mover(subconjuntos[i].second,simbolos[j],a);
				if(kernels.count(crearKernel)==0 and crearKernel.size()>0){
					kernels.insert(crearKernel);
					numEstado++;
					estados.push_back(numEstado);
					crearEpsilon=CerraduraEp(crearKernel,a);
					sort(crearEpsilon.begin(),crearEpsilon.end());
					nuevoSubconjunto(numEstado,crearKernel,crearEpsilon);
					establecerTransiciones(i,numEstado,simbolos[j]);
				}
				else if(crearKernel.size()==0){
					if(kernels.count(crearKernel)==0){
						kernels.insert(crearKernel);
					}
				}
				else{
					comprobarRepeticion(subconjuntos[i].first,crearKernel,simbolos[j]);
				}
			}
		}
	}

	void mostrar(){
		for(int i=0;i<subconjuntos.size();i++){
			cout<<subconjuntos[i].first<<endl;
			for(int j=0; j<subconjuntos[i].second.estados.size();j++){
				cout<<subconjuntos[i].second.estados[j]<<" ";
			}
			cout<<endl;
		}
		for(int i=0; i<transiciones.size();i++){
			cout<<transiciones[i].estadoA<<" "<<transiciones[i].estadoB<<" "<<transiciones[i].simbolo<<endl;
		}
		transicion trans;
		cout<<"digraph AFN{\nrankdir = LR;"<<endl;
		cout<<"node [shape = doublecircle];";
		for(int i=0; i<int(finales.size());i++){
			cout<<" "<<finales[i];
		}
		cout<<";"<<endl;
		cout<<"node [shape = point ];"<<" qi"<<endl;
		cout<<"node [shape = circle];"<<endl;
		cout<<"qi"<<" -> "<<"0"<<endl;
		for(int i = 0; i < int(transiciones.size()); i++) {
			trans = transiciones[i];
			cout<<trans.estadoA<<" -> "<<trans.estadoB<<" [label =\""<<trans.simbolo<<"\"];"<<endl;
		}
		cout<<"}"<<endl;
	}
};

vector<char> simbolos;
set<char> setSimbolos;

AFD construirAfd(AFN a){
	AFD *nuevoAFD = new AFD;
	nuevoAFD->simbolos=simbolos;
	nuevoAFD->establecerInicial(a);
	nuevoAFD->crearSubconjuntos(a);
	nuevoAFD->final = a.final;
	nuevoAFD->establecerFinales();
	return *nuevoAFD;
}

AFN kleene(AFN a) {
	AFN aux;
	aux.numEstados(a.estados.size()+2);
	aux.establecerTransiciones(0, 1, 'e');
	for(int i = 0; i<int(a.transiciones.size()); i++) {
		aux.establecerTransiciones(a.transiciones[i].estadoA+1,a.transiciones[i].estadoB+1,a.transiciones[i].simbolo);
	}
	int nEstados=a.estados.size();
	aux.establecerTransiciones(nEstados,nEstados+1,'e');
	aux.establecerTransiciones(nEstados,1, 'e');
	aux.establecerTransiciones(0,nEstados+1, 'e');
	aux.final = nEstados+1;
	return aux;
}

AFN concatenar(AFN operando1, AFN operando2) {
	AFN aux;
	aux.numEstados(operando1.estados.size() + operando2.estados.size()-1);

	for(int i = 0; i < int(operando1.transiciones.size()); i++) {
		aux.establecerTransiciones(operando1.transiciones[i].estadoA , operando1.transiciones[i].estadoB, operando1.transiciones[i].simbolo);
	}

	for(int i=0; i< int(operando2.transiciones.size()); i++) {
		aux.establecerTransiciones(operando2.transiciones[i].estadoA + operando1.estados.size()-1, operando2.transiciones[i].estadoB + operando1.estados.size()-1, operando2.transiciones[i].simbolo);
	}

	aux.final=operando1.estados.size() + operando2.estados.size()-2;
	return aux;
}

AFN orAFN(AFN operando1, AFN operando2) {
	AFN aux;
	aux.numEstados(operando1.estados.size() + operando2.estados.size()+2);	
	aux.establecerTransiciones(0,1,'e');
	for(int i=0; i< int(operando1.transiciones.size());i++){
		aux.establecerTransiciones(operando1.transiciones[i].estadoA + 1, operando1.transiciones[i].estadoB + 1, operando1.transiciones[i].simbolo);
	}
	aux.final=operando1.estados.size()+operando2.estados.size()+1;
	aux.establecerTransiciones(operando1.estados.size(),aux.final,'e');
	aux.establecerTransiciones(0, operando1.estados.size() + 1,'e');
	for(int i=0; i< int(operando2.transiciones.size());i++){
		aux.establecerTransiciones(operando2.transiciones[i].estadoA + operando1.estados.size() + 1, operando2.transiciones[i].estadoB + operando1.estados.size() + 1, operando2.transiciones[i].simbolo);
	}
	aux.establecerTransiciones(operando1.estados.size() + operando2.estados.size() , aux.final,'e');
	return aux;
}

AFN analisisRegex(string regex){
	stack<AFN> pilaAFNs;
	AFN operando1;
	AFN operando2;
	AFN aux;
	for(char i:regex){
		if((i!='*') and (i!='|') and (i!='.')){
			AFN *nuevoAFN;
			nuevoAFN=new AFN;
			nuevoAFN->numEstados(2);
			nuevoAFN->establecerTransiciones(0,1,i);
			nuevoAFN->final=2;
			pilaAFNs.push(*nuevoAFN);
			if(setSimbolos.count(i)==0){
				setSimbolos.insert(i);
				simbolos.push_back(i);
			}
		}
		else if(i=='*'){
			operando1 = pilaAFNs.top();
			pilaAFNs.pop();
			pilaAFNs.push(kleene(operando1));
		}
		else if(i=='|'){
			operando2 = pilaAFNs.top();
			pilaAFNs.pop();
			operando1 = pilaAFNs.top();
			pilaAFNs.pop();
			pilaAFNs.push(orAFN(operando1,operando2));
		}
		else{
			operando2 = pilaAFNs.top();
			pilaAFNs.pop();
			operando1 = pilaAFNs.top();
			pilaAFNs.pop();
			pilaAFNs.push(concatenar(operando1,operando2));
		}
	}
	return pilaAFNs.top();
}

int precedencia(char c) {
    if(c == '*'){
 		return 3;
	}
    else if(c == '.'){
		return 2;
	}   
    else if(c == '|'){
		return 1;
	}   
    else{
		return 0;
	}
}
 
string postfija(string regex) {
    stack<char> pila;
    string post;
    for(char i: regex){
        if((i!='(') and (i!=')') and (i!='.') and (i!='*') and (i!='|')){
        	post+= i;
		}
        else if(i == '('){
        	pila.push('(');
		}
        else if(i == ')'){
            while(!pila.empty() && pila.top() != '('){
                post += pila.top();
                pila.pop();
            }
            pila.pop();
        }
        else {
            while(!pila.empty() && precedencia(i) <= precedencia(pila.top())) {
                post+= pila.top();
                pila.pop();
            }
            pila.push(i);
        }
    }
    while(!pila.empty()) {
        post+= pila.top();
        pila.pop();
    }
 	return post;
}

string definirConcatenaciones(string regex){
	string nuevoRegex;
	for(int i=0; i<int(regex.size());i++){
		nuevoRegex+=regex[i];
		if(regex[i]==')' or regex[i]=='*'){
			if(i+1<int(regex.size())){
				if((regex[i+1]!='*') and (regex[i+1]!='|') and (regex[i+1]!=')')){
					nuevoRegex+='.';
				}
			}
		}
		else{
			if((regex[i]>='a' and regex[i]<='z') or (regex[i]>='A' and regex[i]<='Z')){
				if((regex[i+1]!='*') and (regex[i+1]!='|') and (regex[i+1]!=')') and i+1<int(regex.size())){
					nuevoRegex+='.';
				}
			}
		}
	}
	return nuevoRegex;
}

int main(){
	string regex,post;
	AFN inicialAFN;
	AFD respuesta;
	cin>>regex;
	regex=definirConcatenaciones(regex);
	post=postfija(regex);
	inicialAFN=analisisRegex(post);
	respuesta=construirAfd(inicialAFN);
	respuesta.mostrar();
	return 0;
}