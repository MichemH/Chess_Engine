#include "evaluation.h"

short PawnTable[64] =	{
  0,  0,  0,  0,  0,  0,  0,  0,
 50, 50, 50, 50, 50, 50, 50, 50,
 10, 10, 20, 30, 30, 20, 10, 10,
  5,  5, 10, 27, 27, 10,  5,  5,
  0,  0,  0, 25, 25,  0,  0,  0,
  5, -5,-10,  0,  0,-10, -5,  5,
  5, 10, 10,-25,-25, 10, 10,  5,
  0,  0,  0,  0,  0,  0,  0,  0, 
};

short KnightTable[64] =	{
 -50,-40,-30,-30,-30,-30,-40,-50,
 -40,-20,  0,  0,  0,  0,-20,-40,
 -30,  0, 10, 15, 15, 10,  0,-30,
 -30,  5, 15, 20, 20, 15,  5,-30,
 -30,  0, 15, 20, 20, 15,  0,-30,
 -30,  5, 10, 15, 15, 10,  5,-30,
 -40,-20,  0,  5,  5,  0,-20,-40,
 -50,-40,-20,-30,-30,-20,-40,-50, 
};

short BishopTable[64] =	{
 -20,-10,-10,-10,-10,-10,-10,-20,
 -10,  0,  0,  0,  0,  0,  0,-10,
 -10,  0,  5, 10, 10,  5,  0,-10,
 -10,  5,  5, 10, 10,  5,  5,-10,
 -10,  0, 10, 10, 10, 10,  0,-10,
 -10, 10, 10, 10, 10, 10, 10,-10,
 -10,  5,  0,  0,  0,  0,  5,-10,
 -20,-10,-40,-10,-10,-40,-10,-20, 
};

short KingTable[64] =	{
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -20, -30, -30, -40, -40, -30, -30, -20,
  -10, -20, -20, -20, -20, -20, -20, -10, 
   20,  20,   0,   0,   0,   0,  20,  20,
   20,  30,  10,   0,   0,  10,  30,  20, 
};

short RookTable[64]	=	{
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
};

short QueenTable[64]=	{
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,-10,-10,-10,-10,	0,	0,
	0,	0,-10,-15,-15,-10,	0,	0,
	0,	0,-10,-15,-15,-10,	0,	0,
	0,	0,-10,-10,-10,-10,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
};

BitBoard CenterBonus	=	0x3C3C3C3C0000;


int	VALUE[13]		=	{P_VALUE, N_VALUE, B_VALUE, R_VALUE, Q_VALUE, K_VALUE, P_VALUE, N_VALUE, B_VALUE, R_VALUE, Q_VALUE, K_VALUE, 0};
int RayLookUp[8]	=	{1,3,5,7,0,2,4,6};

void	UpdatePhase(int turn ){
	using namespace BEGIN_GAME;
}

int	PieceSquareValue(int isPawn, uint16_t from, uint16_t to){
	switch (isPawn % 6){
		case wP: return (PawnTable[to] - PawnTable[from]);
		case wN: return (KnightTable[to] - KnightTable[from]);
		case wB: return (BishopTable[to] - BishopTable[from]);
		case wR: return (RookTable[to] - RookTable[from]);
		case wQ: return (QueenTable[to] - QueenTable[from]);
		case wK: return (KingTable[to] - KingTable[from]);
	}
}

int	EvaluateBOARD	(BOARD A, int side){
	int MATERIAL_BALANCE	=	0;
	int	PAWN_STRUCTURE		=	0;
	int	CENTER_CONTROL		=	0;
	int	MOBILITY			=	0;
	int CASTLING			=	0;
	BitBoard	Pawn[2];
	for (int i = 0; i < 6; i++)	MATERIAL_BALANCE	+=	(population (  A.Pieces[i + 6*side] ) - population ( A.Pieces[i + 6*(side ^ 1)] )) * VALUE[i];
	Pawn[0]	=	A.Pieces[0]; Pawn[1]	=	A.Pieces[6];
	for ( int i = 0; i < 2; i++)
		while (Pawn[i] != 0 )	if ((Pawn_AttackMask[i][BitPop(Pawn[i])]	 & A.Pieces[i*6]) != 0 ) PAWN_STRUCTURE	+=	(1 - 2*i);
	side	==	BLACK ? PAWN_STRUCTURE *= -30 : PAWN_STRUCTURE *= 	30;
	CENTER_CONTROL	=	50*(population(A.CurrentBoard[side] & CenterBonus)	-	population(A.CurrentBoard[side ^ 1] & CenterBonus));
	if ((A.Castling_check & ( 0xE << (side*4))) == (0xE << (side*4))) CASTLING ++ ;
	if ((A.Castling_check & ( 0xE << (side^1*4))) == (0xE << (side^1*4))) CASTLING -- ;
	CASTLING	*=	90;
	return (MATERIAL_BALANCE + PAWN_STRUCTURE + CENTER_CONTROL + MOBILITY + CASTLING);
}

int	SEE	(int SqrTo, BOARD A, int attacker, int Side){
	int SWAP_LIST[32];
	int depth	=	0;
	int CapturedBy;
	SWAP_LIST[0]	=	VALUE[A.Sq[SqrTo]];
	A.Pieces[A.Sq[attacker]]		^=	 ( ( BIT1 >> attacker ) | (BIT1 >> SqrTo) );
	A.Pieces[A.Sq[SqrTo]]			^=	 ( BIT1 >> SqrTo );
	A.CurrentBoard[Side]			^=	 ( ( BIT1 >> attacker ) | (BIT1 >> SqrTo) );
	A.CurrentBoard[Side ^ 1]		^=	 ( BIT1 >> SqrTo );
	A.Sq[SqrTo]		=	A.Sq[attacker];
	A.Sq[attacker]	=	12;
	BitBoard	Occupancy	=	A.CurrentBoard[0] | A.CurrentBoard[1];
	do {
		CapturedBy	=  LeastValuableAttacker(SqrTo, A, depth);
		if (CapturedBy == -1) break;
		depth++;
		SWAP_LIST[depth]	=	VALUE[A.Sq[SqrTo]] - SWAP_LIST[depth -1];
		A.Pieces[A.Sq[CapturedBy]]		^=	 ( ( BIT1 >> CapturedBy ) | (BIT1 >> SqrTo) );
		A.Pieces[A.Sq[SqrTo]]			^=	 ( BIT1 >> SqrTo );
		A.CurrentBoard[(depth ) & 1]				^=	 ( ( BIT1 >> CapturedBy ) | (BIT1 >> SqrTo) );
		A.CurrentBoard[((depth ) & 1) ^ 1]			^=	 ( BIT1 >> SqrTo );
		A.Sq[SqrTo]		=	A.Sq[CapturedBy];
		A.Sq[CapturedBy]	=	12;
		BitBoard	Occupancy	=	A.CurrentBoard[0] | A.CurrentBoard[1];
	} while (true);
	while (--depth >=0)	-SWAP_LIST[depth - 1] > SWAP_LIST[depth] ? SWAP_LIST[depth-1]	=	SWAP_LIST[depth - 1] : SWAP_LIST[depth-1]	=	-SWAP_LIST[depth];
	return SWAP_LIST[0];
}

int	SEET(int SqrTo, BOARD A, int attacker, int Side){
	int SWAP_LIST[32];
	int depth	=	0;
	int CapturedBy;
	SWAP_LIST[0]	=	VALUE[A.Sq[SqrTo]];
	std::cout	<< "SWAP_LIST[0] = " << SWAP_LIST[0] << std::endl;
	A.Pieces[A.Sq[attacker]]		^=	 ( ( BIT1 >> attacker ) | (BIT1 >> SqrTo) );
	A.Pieces[A.Sq[SqrTo]]			^=	 ( BIT1 >> SqrTo );
	A.CurrentBoard[Side]			^=	 ( ( BIT1 >> attacker ) | (BIT1 >> SqrTo) );
	A.CurrentBoard[Side ^ 1]		^=	 ( BIT1 >> SqrTo );
	A.Sq[SqrTo]		=	A.Sq[attacker];
	A.Sq[attacker]	=	12;
	getBoardInfo(A);
	BitBoard	Occupancy	=	A.CurrentBoard[0] | A.CurrentBoard[1];
	do {
		CapturedBy	=  LeastValuableAttacker(SqrTo, A, depth);
		if (CapturedBy == -1) {
			std::cout	<< "No more attacker from side\n";
			break;
		}
		depth++;
		SWAP_LIST[depth]	=	VALUE[A.Sq[SqrTo]] - SWAP_LIST[depth -1];
		A.Pieces[A.Sq[CapturedBy]]		^=	 ( ( BIT1 >> CapturedBy ) | (BIT1 >> SqrTo) );
		A.Pieces[A.Sq[SqrTo]]			^=	 ( BIT1 >> SqrTo );
		A.CurrentBoard[(depth ) & 1]				^=	 ( ( BIT1 >> CapturedBy ) | (BIT1 >> SqrTo) );
		A.CurrentBoard[((depth ) & 1) ^ 1]			^=	 ( BIT1 >> SqrTo );
		A.Sq[SqrTo]		=	A.Sq[CapturedBy];
		A.Sq[CapturedBy]	=	12;
		std::cout	<< "SWAP_LIST[" << depth << "] = " << SWAP_LIST[depth] << std::endl;
		BitBoard	Occupancy	=	A.CurrentBoard[0] | A.CurrentBoard[1];
	} while (true);
	std::cout	<< "Final Depth = " << depth << std::endl;
	while (--depth >= 0)	-SWAP_LIST[depth - 1] > SWAP_LIST[depth] ? SWAP_LIST[depth-1]	=	SWAP_LIST[depth - 1] : SWAP_LIST[depth-1]	=	-SWAP_LIST[depth];
	std::cout	<< "SWAP_LIST[0] = " << SWAP_LIST[0] << std::endl;
	return SWAP_LIST[0];
}

// "There is vomit on his code already. Mom Spaghetti"

int LeastValuableAttacker	(int att_Sqr, BOARD A, int side){
	int	QPosition	=	-1;
	BitBoard SlidingPiece;
	BitBoard B;
	int position	=	-1;
	SlidingPiece	=	generateQueenMove(att_Sqr, A.CurrentBoard[side ^ 1] | BIT1 >> att_Sqr, A.CurrentBoard[side]);
	B	=	(Pawn_AttackMask[side][att_Sqr]	&	A.Pieces[wP + 6*side]);
	while (B!=0){
		position	=	BitPop(B);
		if (A.Sq[position]	==	(wP + 6 * side ))	return position;
	}
	B	=	Knight_AttackMask[att_Sqr]	&	A.Pieces[wN	+	6 * side];
	while (B!=0){
		position	=	BitPop(B);
		if (A.Sq[position]	==	(wN + 6 * side ))	return position;
	}
	for (int i = 0; i < 8; i++){
		B	=	AttackRay[RayLookUp[i]][att_Sqr] & SlidingPiece ;
		while (B!=0){
			if (i < 4)	{
				RayLookUp[i]	<	4? position	=	BitPopR(B) : position	=	BitPop(B);
				if (A.Sq[position]	==	(wB + 6 * side ))	return position;
				if (A.Sq[position]	==	(wQ + 6 * side )) 	QPosition	=	position;
			}	else {
				RayLookUp[i]	<	4? position	=	BitPopR(B) : position	=	BitPop(B);
				if (A.Sq[position]	==	(wR + 6 * side ))	return position;
				if (A.Sq[position]	==	(wQ + 6 * side )) 	QPosition	=	position;
			}
		}
	}
	B	=	King_AttackMask[att_Sqr]	&	A.Pieces[wK	+	6 * side];
	position	=	BitPop(B);
	if (A.Sq[position]	==	(wN + 6 * side ))	if (QPosition	==	-1) return position;
	return QPosition;
}
