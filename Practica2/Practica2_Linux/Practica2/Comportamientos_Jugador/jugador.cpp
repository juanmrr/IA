#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"


#include <iostream>
#include <cmath>
#include <vector>
#include <stdlib.h>


void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++; 
	}
	cout << endl;
}

void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break; 
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}

Nodo CrearHijo (Nodo aux, int indice, Action action){
	int padre = aux.getPosicion();
	int posicion = indice;
	int fila = aux.getFila();
	int columna = aux.getColumna();
	int orientacion = aux.getOrientacion();

	switch (action){
	  case actTURN_R: 
		orientacion = (orientacion + 1)%4; break;
	  case actTURN_L:
		orientacion = (orientacion + 3)%4; break;
	  case actFORWARD:
		switch (orientacion){
		  case 0: fila--; break;
		  case 1: columna++; break;
		  case 2: fila++; break;
		  case 3: columna--; break;
		}
	}

	Nodo hijo (padre, posicion, fila, columna, orientacion, 0, 0, action);

	return hijo;

}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	
	plan.clear();	
	
	list<Action> acciones;
	acciones.push_back(actFORWARD);
	acciones.push_back(actTURN_L);
	acciones.push_back(actTURN_R);
	list<Action>::iterator it = acciones.begin();
	list<Nodo> abiertos;
	list<Nodo>::iterator it_ab;
	vector<Nodo>::iterator it_ex;
	vector<Nodo> explorados;
	bool encontrado;
	int indice = 1;


	Nodo inicial (-1, 0, origen.fila, origen.columna, origen.orientacion, 0, 0, actIDLE);

	if (origen.fila == destino.fila && origen.columna == destino.columna)
	  return true;
	else{
	  abiertos.push_back(inicial);
	  encontrado = false;;
	}
	//Mientras haya nodos por explorar
	while (!abiertos.empty()){
	  Nodo aux = abiertos.front();
	  abiertos.pop_front();
	  explorados.push_back(aux);
	  for (it = acciones.begin(); it != acciones.end(); ++it){
		  cout << abiertos.size() << " " << explorados.size() << "\n";
	    Nodo hijo = CrearHijo (aux, indice, *it);
	    if (EsValido(hijo)){

	//Miro si está en explorados o en abiertos
	      for (it_ex = explorados.begin(); it_ex != explorados.end() && !encontrado; ++it_ex){
	        if (hijo == *it_ex)
		  encontrado = true;
	      }
	      for (it_ab = abiertos.begin(); it_ab != abiertos.end() && !encontrado; ++it_ab){
	        if (hijo == *it_ab)
		  encontrado = true;
	      }
	//Si no está en explorados ni en abiertos
	      if (!encontrado){	      cout << hijo.getPadre() << " " << hijo.getPosicion() << " " << hijo.getFila() << " " << hijo.getColumna() << " " << hijo.getOrientacion() << " " << hijo.getAccion() << " " << "\n";
	        if (hijo.getFila() == destino.fila && hijo.getColumna() == destino.columna){
	          while (hijo.getPadre() >= 0){
		    plan.push_front(hijo.getAccion()); //ALBERTO: ESTO ES PUSH_FRONT SIN DUDA
			
		    hijo = explorados[hijo.getPadre()];
		    cout << "Busco a mi padre en la posicion " << hijo.getPadre() << "\n";
	          }

		  VisualizaPlan(origen, plan);

	          return true;
	        }
	        else{
	          abiertos.push_back(hijo);
			indice++;
		  cout << abiertos.size() << " " << explorados.size() << "\n";
	        }
	      }
	      encontrado = false;

	    }
	  }
	}

  return false;
}

Action ComportamientoJugador::think(Sensores sensores) {

	if (sensores.destinoF == fil && sensores.destinoC == col)
		hayPlan = false;

	for (int i = 0; i < mapaResultado.size(); i++){
		cout  << "\n";
		for (int j = 0; j < mapaResultado[i].size(); j ++)
			cout << mapaResultado[i][j];
	}

  if (sensores.mensajeF != -1){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		//Si recibo un mensaje con mi posicion, actualizo el mapa
		switch(brujula){
			case 0: mapaResultado[fil][col] = sensores.terreno[0];
				mapaResultado[fil-1][col] = sensores.terreno[2];
				mapaResultado[fil-2][col] = sensores.terreno[6];
				mapaResultado[fil-3][col] = sensores.terreno[12];
				mapaResultado[fil-1][col-1] = sensores.terreno[1];
				mapaResultado[fil-1][col+1] = sensores.terreno[3];
				mapaResultado[fil-2][col-1] = sensores.terreno[5];
				mapaResultado[fil-2][col-2] = sensores.terreno[4];
				mapaResultado[fil-2][col+1] = sensores.terreno[7];
				mapaResultado[fil-2][col+2] = sensores.terreno[8];
				mapaResultado[fil-3][col-1] = sensores.terreno[11];
				mapaResultado[fil-3][col-2] = sensores.terreno[10];
				mapaResultado[fil-3][col-3] = sensores.terreno[9];
				mapaResultado[fil-3][col+1] = sensores.terreno[13];
				mapaResultado[fil-3][col+2] = sensores.terreno[14];
				mapaResultado[fil-3][col+3] = sensores.terreno[15]; break;
			case 1: mapaResultado[fil][col] = sensores.terreno[0];
				mapaResultado[fil][col+1] = sensores.terreno[2];
				mapaResultado[fil][col+2] = sensores.terreno[6];
				mapaResultado[fil][col+3] = sensores.terreno[12];
				mapaResultado[fil-1][col+1] = sensores.terreno[1];
				mapaResultado[fil+1][col+1] = sensores.terreno[3];
				mapaResultado[fil-1][col+2] = sensores.terreno[5];
				mapaResultado[fil-2][col+2] = sensores.terreno[4];
				mapaResultado[fil+1][col+2] = sensores.terreno[7];
				mapaResultado[fil+2][col+2] = sensores.terreno[8];
				mapaResultado[fil-1][col+3] = sensores.terreno[11];
				mapaResultado[fil-2][col+3] = sensores.terreno[10];
				mapaResultado[fil-3][col+3] = sensores.terreno[9];
				mapaResultado[fil+1][col+3] = sensores.terreno[13];
				mapaResultado[fil+2][col+3] = sensores.terreno[14];
				mapaResultado[fil+3][col+3] = sensores.terreno[15]; break;
			case 2: mapaResultado[fil][col] = sensores.terreno[0];
				mapaResultado[fil+1][col] = sensores.terreno[2];
				mapaResultado[fil+2][col] = sensores.terreno[6];
				mapaResultado[fil+3][col] = sensores.terreno[12];
				mapaResultado[fil+1][col+1] = sensores.terreno[1];
				mapaResultado[fil+1][col-1] = sensores.terreno[3];
				mapaResultado[fil+2][col+1] = sensores.terreno[5];
				mapaResultado[fil+2][col+2] = sensores.terreno[4];
				mapaResultado[fil+2][col-1] = sensores.terreno[7];
				mapaResultado[fil+2][col-2] = sensores.terreno[8];
				mapaResultado[fil+3][col+1] = sensores.terreno[11];
				mapaResultado[fil+3][col+2] = sensores.terreno[10];
				mapaResultado[fil+3][col+3] = sensores.terreno[9];
				mapaResultado[fil+3][col-1] = sensores.terreno[13];
				mapaResultado[fil+3][col-2] = sensores.terreno[14];
				mapaResultado[fil+3][col-3] = sensores.terreno[15]; break;
			case 3: mapaResultado[fil][col] = sensores.terreno[0];
				mapaResultado[fil][col-1] = sensores.terreno[2];
				mapaResultado[fil][col-2] = sensores.terreno[6];
				mapaResultado[fil][col-3] = sensores.terreno[12];
				mapaResultado[fil-1][col-1] = sensores.terreno[1];
				mapaResultado[fil+1][col-1] = sensores.terreno[3];
				mapaResultado[fil-1][col-2] = sensores.terreno[5];
				mapaResultado[fil-2][col-2] = sensores.terreno[4];
				mapaResultado[fil+1][col-2] = sensores.terreno[7];
				mapaResultado[fil+2][col-2] = sensores.terreno[8];
				mapaResultado[fil-1][col-3] = sensores.terreno[11];
				mapaResultado[fil-2][col-3] = sensores.terreno[10];
				mapaResultado[fil-3][col-3] = sensores.terreno[9];
				mapaResultado[fil+1][col-3] = sensores.terreno[13];
				mapaResultado[fil+2][col-3] = sensores.terreno[14];
				mapaResultado[fil+3][col-3] = sensores.terreno[15]; break;
		}
	valido = true;
  }

	for (int i = 0; i < mapaResultado.size(); i++){
		cout  << "\n";
		for (int j = 0; j < mapaResultado[i].size(); j ++)
			cout << mapaResultado[i][j];
	}

  Action reactivo;

  if (!valido){
	cout << "xd";
	srand(time(NULL));
	int num = rand()%3;
		if ((num == 0 || num == 1) && ((sensores.terreno[2] == 'S' || sensores.terreno[2] == 'T' || sensores.terreno[2] == 'K') && sensores.superficie[2] != 'a'))
			reactivo = actFORWARD;
		else{
			if (num == 2){
				reactivo = actTURN_R;
				brujula = (brujula+1)%4;
				cout << brujula << " ";
			} 
			else{
			reactivo = actTURN_L;
			brujula = (brujula+3)%4;
			cout << brujula << " ";
			}
		}

	return reactivo;

  }


	// Actualizar el efecto de la ultima accion
	
  if (valido){
	switch (ultimaAccion){
		case actTURN_R: brujula = (brujula+1)%4; break;
		case actTURN_L: brujula = (brujula+3)%4; break;
		case actFORWARD:
			switch (brujula){
				case 0:  fil--;
					mapaResultado[fil][col] = sensores.terreno[0];
					mapaResultado[fil-1][col] = sensores.terreno[2];
					mapaResultado[fil-2][col] = sensores.terreno[6];
					mapaResultado[fil-3][col] = sensores.terreno[12];
					mapaResultado[fil-1][col-1] = sensores.terreno[1];
					mapaResultado[fil-1][col+1] = sensores.terreno[3];
					mapaResultado[fil-2][col-1] = sensores.terreno[5];
					mapaResultado[fil-2][col-2] = sensores.terreno[4];
					mapaResultado[fil-2][col+1] = sensores.terreno[7];
					mapaResultado[fil-2][col+2] = sensores.terreno[8];
					mapaResultado[fil-3][col-1] = sensores.terreno[11];
					mapaResultado[fil-3][col-2] = sensores.terreno[10];
					mapaResultado[fil-3][col-3] = sensores.terreno[9];
					mapaResultado[fil-3][col+1] = sensores.terreno[13];
					mapaResultado[fil-3][col+2] = sensores.terreno[14];
					mapaResultado[fil-3][col+3] = sensores.terreno[15];
 break;
				case 1:  col++;
					mapaResultado[fil][col] = sensores.terreno[0];
					mapaResultado[fil][col+1] = sensores.terreno[2];
					mapaResultado[fil][col+2] = sensores.terreno[6];
					mapaResultado[fil][col+3] = sensores.terreno[12];
					mapaResultado[fil-1][col+1] = sensores.terreno[1];
					mapaResultado[fil+1][col+1] = sensores.terreno[3];
					mapaResultado[fil-1][col+2] = sensores.terreno[5];
					mapaResultado[fil-2][col+2] = sensores.terreno[4];
					mapaResultado[fil+1][col+2] = sensores.terreno[7];
					mapaResultado[fil+2][col+2] = sensores.terreno[8];
					mapaResultado[fil-1][col+3] = sensores.terreno[11];
					mapaResultado[fil-2][col+3] = sensores.terreno[10];
					mapaResultado[fil-3][col+3] = sensores.terreno[9];
					mapaResultado[fil+1][col+3] = sensores.terreno[13];
					mapaResultado[fil+2][col+3] = sensores.terreno[14];
					mapaResultado[fil+3][col+3] = sensores.terreno[15];
 break;
				case 2: fil++;
					mapaResultado[fil][col] = sensores.terreno[0];
					mapaResultado[fil+1][col] = sensores.terreno[2];
					mapaResultado[fil+2][col] = sensores.terreno[6];
					mapaResultado[fil+3][col] = sensores.terreno[12];
					mapaResultado[fil+1][col+1] = sensores.terreno[1];
					mapaResultado[fil+1][col-1] = sensores.terreno[3];
					mapaResultado[fil+2][col+1] = sensores.terreno[5];
					mapaResultado[fil+2][col+2] = sensores.terreno[4];
					mapaResultado[fil+2][col-1] = sensores.terreno[7];
					mapaResultado[fil+2][col-2] = sensores.terreno[8];
					mapaResultado[fil+3][col+1] = sensores.terreno[11];
					mapaResultado[fil+3][col+2] = sensores.terreno[10];
					mapaResultado[fil+3][col+3] = sensores.terreno[9];
					mapaResultado[fil+3][col-1] = sensores.terreno[13];
					mapaResultado[fil+3][col-2] = sensores.terreno[14];
					mapaResultado[fil+3][col-3] = sensores.terreno[15];
  break;
				case 3: col--;
					mapaResultado[fil][col] = sensores.terreno[0];
					mapaResultado[fil][col-1] = sensores.terreno[2];
					mapaResultado[fil][col-2] = sensores.terreno[6];
					mapaResultado[fil][col-3] = sensores.terreno[12];
					mapaResultado[fil-1][col-1] = sensores.terreno[1];
					mapaResultado[fil+1][col-1] = sensores.terreno[3];
					mapaResultado[fil-1][col-2] = sensores.terreno[5];
					mapaResultado[fil-2][col-2] = sensores.terreno[4];
					mapaResultado[fil+1][col-2] = sensores.terreno[7];
					mapaResultado[fil+2][col-2] = sensores.terreno[8];
					mapaResultado[fil-1][col-3] = sensores.terreno[11];
					mapaResultado[fil-2][col-3] = sensores.terreno[10];
					mapaResultado[fil-3][col-3] = sensores.terreno[9];
					mapaResultado[fil+1][col-3] = sensores.terreno[13];
					mapaResultado[fil+2][col-3] = sensores.terreno[14];
					mapaResultado[fil+3][col-3] = sensores.terreno[15]; 
 break;
			}
			cout << "fil: " << fil << "  col: " << col << " Or: " << brujula << endl;
	}
  }


	// Determinar si ha cambiado el destino desde la ultima planificacion
	if (hayPlan and (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna)){
		cout << "El destino ha cambiado\n" << sensores.destinoF << "y" << sensores.destinoC << "\n";
		hayPlan = false;
	}

	// Determinar si tengo que construir un plan
	if (valido && !hayPlan){
cout << "dfaishfiashfaish";
		estado origen;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;

    		hayPlan = pathFinding(origen,destino,plan);
	}


	// Ejecutar el plan
	Action sigAccion;
	if (hayPlan and plan.size()>0){
		if (sensores.superficie[2] == 'a')
			sigAccion = actIDLE;
		else{
		sigAccion = plan.front();
		plan.erase(plan.begin());
		}
	}
	else {
		sigAccion = actIDLE; cout << "en serio";
	}

	ultimaAccion = sigAccion;
	return sigAccion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
