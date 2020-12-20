#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <iostream>
#include <cmath>
#include <vector>
#include <stdlib.h>

class Nodo {

private:

	int padre;
	int posicion;
	int fil;
	int col;
	int orienta;
	int costo;
	int costo_total;
	Action accion;

public:

	Nodo(){
	  this->padre = -1;
	  this->posicion = -1;
	  this->fil = -1;
	  this->col =  -1;
	  this->orienta = -1;
	  this->costo = 0;
	  this->costo_total = 0;
	  this->accion = actIDLE;
	}

	Nodo (Nodo const &nodo){
	  this->padre = nodo.padre;
	  this->posicion = nodo.posicion;
	  this->fil = nodo.fil;
	  this->col = nodo.col;
	  this->orienta = nodo.orienta;
	  this->costo = nodo.costo;
	  this->costo_total = nodo.costo_total;
	  this->accion = nodo.accion;
	}

	Nodo (int padre, int posicion, int fila, int columna, int orientacion, int costo, int costo_total, Action accion){
	  this->padre = padre;
	  this->posicion = posicion;
	  this->fil = fila;
	  this->col = columna;
	  this->orienta = orientacion;
	  this->costo = costo;
	  this->costo_total = costo_total;
	  this->accion = accion;
	}

  bool operator == (Nodo &nodo){ //ALBERTO: EN ESTE OPERADOR EN ANCHURA YO SOLO COMPARO EL ESTADO, ES DECIR: FILA, COLUMNA Y ORIENTACION
	bool resultado = true;
	
	if (this->fil != nodo.fil)
	  return false;
	if (this->col != nodo.col)
	  return false;
	if (this->orienta != nodo.orienta)
	  return false;
	if (this->costo != nodo.costo)
	  return false;
	if (this->costo_total != nodo.costo_total)
	  return false;

	return resultado;
  }

  Nodo & operator = (Nodo const &nodo){
	this->padre = nodo.padre;
	this->posicion = nodo.posicion;
	this->fil = nodo.fil;
	this->col = nodo.col;
	this->orienta = nodo.orienta;
	this->costo = nodo.costo;
	this->costo_total = nodo.costo_total;
	this->accion = nodo.accion;

	return *this;
  }

  int getPadre () const{
	return padre;
  }

  Action getAccion () const{
	return accion;
  }

  int getColumna () const{
	return col;
  }

  int getFila () const{
	return fil;
  }

  int getOrientacion () const{
	return orienta;
  }

  int getPosicion () const{
	return posicion;
  }


  bool operator <(const Nodo & nodo) const{
	bool salida = false;
	
	if (fil<nodo.fil)
	  salida = true;
	else if (fil == nodo.fil && col<nodo.col)
	  salida = true;
	else if (fil == nodo.fil && col == nodo.col && orienta<nodo.orienta)
	  salida = true;

	return salida;
  }


};

struct estado {
  int fila;
  int columna;
  int orientacion;
};

class ComportamientoJugador : public Comportamiento {
  public:

    bool EsValido (Nodo const &nodo){
	bool resultado;
	if (mapaResultado[nodo.getFila()][nodo.getColumna()] == 'S' || mapaResultado[nodo.getFila()][nodo.getColumna()] == 'T' || mapaResultado[nodo.getFila()][nodo.getColumna()] == 'K' || mapaResultado[nodo.getFila()][nodo.getColumna()] == '?'){

	  resultado = true;
	}else
	  resultado = false;

	return resultado;
    }
 
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      hayPlan = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      hayPlan = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);

    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, brujula;
    estado destino;
    list<Action> plan;

    // Nuevas Variables de Estado
    Action ultimaAccion;
    bool hayPlan;
    bool valido = false;



    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    void PintaPlan(list<Action> plan);
};


#endif
