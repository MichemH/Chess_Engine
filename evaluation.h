#ifndef EVALUATION_H_
#define EVALUATION_H_

/*
 * The floor under our feet whisper out
 * "Come on in, come on in
 *  Where it all begin.."
 */

#include  "BitBoard.h"
#include  "MoveGen.h"
#include  "evaluation.h"


extern short PawnTable[64];
extern short KnightTable[64];
extern short BishopTable[64];
extern short KingTable[64];
extern short RookTable[64];
extern short QueenTable[64];
extern BitBoard	CenterBonus;
int	PieceSquareValue(int isPawn, uint16_t from, uint16_t to);

int	EvaluateBOARD	(BOARD A, int side);

int	SEE	(int Sqr, BOARD A, int attacker, int turn);

int	SEET(int SqrTo, BOARD A, int attacker, int Side);

int LeastValuableAttacker	(int att_Sqr, BOARD A, int side);

#endif 
