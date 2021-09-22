#include<bits/stdc++.h>

using namespace std;

struct transicion{
	char simbolo;
	int estadoA;
	int estadoB;
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

	void mostrar(){
		transicion trans;
		cout<<"digraph AFN{\nrankdir = LR;"<<endl;
		cout<<"node [shape = doublecircle];"<<" "<<final<<";"<<endl;
		cout<<"node [shape = point ];"<<" qi"<<endl;
		cout<<"node [shape = circle];"<<endl;
		cout<<"qi"<<" -> "<<"0"<<endl;
		for(int j = 0; j < estados.size(); j++){
			for(int i = 0; i < transiciones.size(); i++) {
				trans = transiciones[i];
				if(trans.estadoA == j){
					if(trans.simbolo == 'e'){
						cout<<trans.estadoA<<" -> "<<trans.estadoB<<" [label =\"&#949;\"];"<<endl;
					}
					else{
						cout<<trans.estadoA<<" -> "<<trans.estadoB<<" [label =\""<<trans.simbolo<<"\"];"<<endl;
					}
					
				}
			}
		}
		cout<<"}"<<endl;
	}
};

AFN kleene(AFN a) {
	AFN aux;
	aux.numEstados(a.estados.size()+2);
	aux.establecerTransiciones(0, 1, 'e');
	for(int i = 0; i<a.transiciones.size(); i++) {
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

	for(int i = 0; i < operando1.transiciones.size(); i++) {
		aux.establecerTransiciones(operando1.transiciones[i].estadoA , operando1.transiciones[i].estadoB, operando1.transiciones[i].simbolo);
	}

	for(int i=0; i<operando2.transiciones.size(); i++) {
		aux.establecerTransiciones(operando2.transiciones[i].estadoA + operando1.estados.size()-1, operando2.transiciones[i].estadoB + operando1.estados.size()-1, operando2.transiciones[i].simbolo);
	}

	aux.final=operando1.estados.size() + operando2.estados.size()-2;
	return aux;
}

AFN orAFN(AFN operando1, AFN operando2) {
	AFN aux;
	aux.numEstados(operando1.estados.size() + operando2.estados.size()+2);	
	aux.establecerTransiciones(0,1,'e');
	for(int i=0; i<operando1.transiciones.size();i++){
		aux.establecerTransiciones(operando1.transiciones[i].estadoA + 1, operando1.transiciones[i].estadoB + 1, operando1.transiciones[i].simbolo);
	}
	aux.final=operando1.estados.size()+operando2.estados.size()+1;
	aux.establecerTransiciones(operando1.estados.size(),aux.final,'e');
	aux.establecerTransiciones(0, operando1.estados.size() + 1,'e');
	for(int i=0; i<operando2.transiciones.size();i++){
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
	for(int i=0; i<regex.size();i++){
		nuevoRegex+=regex[i];
		if(regex[i]==')' or regex[i]=='*'){
			if(i+1<regex.size()){
				if((regex[i+1]!='*') and (regex[i+1]!='|') and (regex[i+1]!=')')){
					nuevoRegex+='.';
				}
			}
		}
		else{
			if((regex[i]>='a' and regex[i]<='z') or (regex[i]>='A' and regex[i]<='Z')){
				if((regex[i+1]!='*') and (regex[i+1]!='|') and (regex[i+1]!=')') and i+1<regex.size()){
					nuevoRegex+='.';
				}
			}
		}
	}
	return nuevoRegex;
}

int main(){
	string regex;
	cin>>regex;
	regex=definirConcatenaciones(regex);
	string post;
	post=postfija(regex);
	AFN respuesta;
	respuesta=analisisRegex(post);
	respuesta.mostrar();
	return 0;
}