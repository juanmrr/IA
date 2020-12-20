#include "Bot.h"

#ifndef HODOR_H_
#define HODOR_H_

class Hodor:Bot {
public:
	Hodor();
	~Hodor();


	void initialize();
	string getName();
	int Heuristica(const GameState &state, const Player &jugador);
int heuristica2 (const GameState &st, Player yo);
	int Poda_alfa_beta (int profundidad, int alfa, int beta, const GameState &state, const Player &jugador);
	Move nextMove(const vector<Move> &adversary, const GameState &state);
};

#endif /* MANUPCBOT_H_ */
