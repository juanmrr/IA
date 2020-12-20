#include "Hodor.h"

#include <string>
#include <cstdlib>
#include <limits>
#include <iostream>
using namespace std;

Hodor::Hodor() {
	// Inicializar las variables necesarias para ejecutar la partida
	srand(time(0));

}

Hodor::~Hodor() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void Hodor::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string Hodor::getName() {
	return "Hodor"; // Sustituir por el nombre del bot
}

int Hodor::Heuristica(const GameState &state, const Player &jugador){

	int puntuacion = 0;
	int fichas_propias = 0;
	int fichas_adversario = 0;

	Player adversario;

	if (jugador == J1)
		adversario = J2;
	else if (jugador == J2)
		adversario = J1;
	
	for (int i = 1; i <= 6; i++){
		if (state.getSeedsAt(jugador, (Position) i) == 0)
			puntuacion += 1;
		else if (state.getSeedsAt(jugador, (Position) i) == 1 || state.getSeedsAt(jugador, (Position) i) == 2)
			puntuacion += 6;
		else
			puntuacion += 2;

		if (state.getSeedsAt(adversario, (Position) i) == 0)
			puntuacion += 6;
		else if (state.getSeedsAt(adversario, (Position) i) == 1 || state.getSeedsAt(adversario, (Position) i) == 2)
			puntuacion -= 4;
		else
			puntuacion -= state.getSeedsAt(adversario, (Position) i);

		fichas_propias = state.getSeedsAt(jugador, (Position) i);

		fichas_adversario = state.getSeedsAt(adversario, (Position) i);
	}

	if (fichas_propias > fichas_adversario)
		puntuacion += 10;

	puntuacion += state.getScore(jugador);
	puntuacion -= 4*state.getScore(adversario);

	if(state.getScore(jugador) > 24){
		puntuacion += 10000; 
	}
	else if (state.getScore(adversario) > 24)
		puntuacion -= 10000;
	
	return puntuacion;
}

int Hodor::Poda_alfa_beta (int profundidad, int alfa, int beta, const GameState &state, const Player &jugador){

	if (state.isFinalState() || profundidad == 0)
		return Heuristica (state, jugador);
	if (state.getCurrentPlayer() == jugador){
		for (int i = 1; i <= 6; i++){
			if (state.getSeedsAt(jugador, (Position) i) > 0){
				GameState hijo = state.simulateMove( (Move) i);
				int aux = Poda_alfa_beta(profundidad - 1, alfa, beta, hijo, jugador);
				if (alfa < aux){
					alfa = aux;
				}
				if (beta <= alfa)
					return alfa;
			}
		}
		return alfa;
	}
	else{
		for (int i = 1; i <= 6; i++){
			if (state.getSeedsAt(state.getCurrentPlayer(), (Position) i) > 0){
				GameState hijo = state.simulateMove( (Move) i);
				int aux = Poda_alfa_beta(profundidad - 1, alfa, beta, hijo, jugador);
				if (beta > aux){
					beta = aux;
				}
				if (beta <= alfa)
					return beta;
			}
		}
		return beta;
	}
}


Move Hodor::nextMove(const vector<Move> &adversary, const GameState &state) {

	Player turno= this->getPlayer();
	long timeout= this->getTimeOut();
	Move movimiento;
	int jugada, aux = numeric_limits<int>::min(), max = numeric_limits<int>::min();
	int inf = numeric_limits<int>::max();
	int minf = numeric_limits<int>::min();

	for (int i = 1; i <= 6; i++) {
		if (state.getSeedsAt(turno, (Position) i) > 0){
			GameState hijo = state.simulateMove( (Move) i);
			aux = Poda_alfa_beta (9, minf, inf, hijo, turno);
			if (aux > max){
				max = aux;
				jugada = i;
			}
		}		
	}

	movimiento = (Move) jugada;

	return movimiento;

}
