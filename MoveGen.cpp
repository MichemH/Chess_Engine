#include "MoveGen.h"
/*
 * 
 * Oh My God, finally done with this move generation BS
 * 
 * 
 */

extern int		RECUR_CALL;

void DecodeMove	(ExtMove *A){
	int	from	=	A->move	&	0x3F;
	int to		=	(A->move	&	0xFC0) >> 6;
	int flags	=	(A->move	>> 12) & 0xF;
	std::cout	<< std::dec <<	from << "->" << to << "   Flags = " << flags << "   Value = " << A->value << std::endl;
}

void DecodeMove	(Move A){
	int	from	=	A	&	0x3F;
	int to		=	(A	&	0xFC0) >> 6;
	int flags	=	(A	>> 12) & 0xF;
	std::cout	<< std::dec <<	from << "->" << to << "   Flags = " << flags << std::endl;
}

ExtMove *MoveEncoding (int isPawn, int Sq, BitBoard Target, ExtMove *MoveList, BitBoard Enemy, BOARD C, int *Z){
	uint16_t	from;
	uint16_t	to;
	uint16_t	flags;
	from	=	Sq;
	MoveType	FLG;
	if (isPawn%6	!=	0){
		while (Target != 0){
			to				=	BitPop(Target);
			if ((BIT1 >> to & Enemy) != 0 )	{	FLG		=	CAPTURE;		MoveList->value	=	PieceSquareValue(isPawn, from, to) + SEE(to, C, from, C.Side_to_move);	}
			else 							{	FLG		=	QUIET_MOVES;	MoveList->value	=	PieceSquareValue(isPawn, from, to) + SEE(to, C, from, C.Side_to_move);	}
			flags	=	FLG;
			MoveList->move	=	(from | to << 6 | flags << 12);
			*Z	+=	1;
			MoveList++;
		}
	} else {
		while (Target != 0){
			to				=	BitPop(Target);
			BitBoard		PawnDes		=	BIT1 >> to;
			if ( ( ( PawnDes & Rank1 ) || ( PawnDes & Rank8 ) ) != 0 ){
				if ( ( PawnDes & Enemy ) != 0 )	{
					for (int i =8; i < 12; i++)	{
						flags	=	i;
						MoveList->move	=	(from | to << 6 | flags << 12);
						MoveList->value	=	5 * i * i	+	SEE(to, C, from, C.Side_to_move);
						*Z		+=1;
						MoveList++;
					}
				} else {
					for (int i =12; i < 16; i++)	{
						flags	=	i;
						MoveList->move	=	(from | to << 6 | flags << 12);
						MoveList->value	=	5 * i * i;
						*Z	+=1;
						MoveList++;
					}
				}
			} else {
				if (BIT1 >> to & Enemy != 0 ) {	FLG		=	CAPTURE;		MoveList->value	=	PieceSquareValue(isPawn, from, to) + SEE(to, C, from, C.Side_to_move);	}
				else 						  {	FLG		=	QUIET_MOVES;	MoveList->value	=	PieceSquareValue(isPawn, from, to) + SEE(to, C, from, C.Side_to_move);	}
				flags	=	FLG;
				MoveList->move	=	(from | to << 6 | flags << 12);
				MoveList++;
				*Z	+=	1;
			}
		}
	}
	return MoveList;
};

/*
 * "all in all it's just another brick in the wall"
 */

int GenerateAllMove( struct BOARD A, ExtMove *MoveList, int Color ){
	int 		MOVELIST_TOTAL_ITEMS	=	0;
	MoveType	FLAG;
	BitBoard	Pieces[12];
	BitBoard	Moves[12];
	BitBoard	EnemyPieces, OwnPieces;
	Castling_right	castK, castQ;
	for (int wtf	=	0; 	wtf < 6; wtf++) Moves[wtf]	=	0;
	if (Color	==	WHITE){
		for (int i	=	0; i < 6; i++)	Pieces[i]	=	A.Pieces[i];
		OwnPieces	=	A.CurrentBoard[WHITE];
		EnemyPieces	=	A.CurrentBoard[BLACK];
		castK		=	CastlingKW;
		castQ		=	CastlingQW;
		if ((A.Castling_check	&	castK)	!=	castK) {
			if (LeastValuableAttacker(60, A, BLACK)	!=	-1) {}
			else if (A.Sq[62]	==	emptySqr&& A.Sq[61]	==	emptySqr){
				MoveList->move	=	(60 | 62 << 6 | KING_CASTLE << 12);
				Moves[wK]		=	BIT1 >> 62;
				MoveList->value			=	250;
				MoveList++;
				MOVELIST_TOTAL_ITEMS	+=	1;
			}
		}
		if (A.Castling_check	&	castQ	!=	castQ){
			if (LeastValuableAttacker(60, A, BLACK)	!=	-1) {}
			else if (A.Sq[57]	==	emptySqr&& A.Sq[58]	==	emptySqr&& A.Sq[59]	==	emptySqr){
				MoveList->move	=	(60 | 58 << 6 | QUEEN_CASTLE << 12);
				Moves[wK]		=	BIT1 >> 58;
				MoveList->value			=	250;
				MoveList++;
				MOVELIST_TOTAL_ITEMS	+=	1;
			}
		}
	}
	else { 
		for (int i	=	0; i < 6; i++)	Pieces[i]	=	A.Pieces[i+6];
		OwnPieces	=	A.CurrentBoard[BLACK];
		EnemyPieces	=	A.CurrentBoard[WHITE];
		castK		=	CastlingKB;
		castQ		=	CastlingQB;
		if ((A.Castling_check	&	castK)	!=	castK) {
			if (LeastValuableAttacker(5, A, WHITE)	!=	-1) {}
			else if (A.Sq[5]	==	emptySqr&& A.Sq[6]	==	emptySqr){
				MoveList->move	=	(4 | 6 << 6 | KING_CASTLE << 12);
				Moves[bK]		=	BIT1 >> 6;
				MoveList->value			=	250;
				MoveList++;
				MOVELIST_TOTAL_ITEMS	+=	1;
			}
		}
		if ((A.Castling_check	&	castQ)	!=	castQ){
			if (LeastValuableAttacker(5, A, WHITE)	!=	-1) {}
			else if (A.Sq[1]	==	emptySqr&& A.Sq[2]	==	emptySqr&& A.Sq[3]	==	emptySqr){
				MoveList->move	=	(4 | 2 << 6 | QUEEN_CASTLE << 12);
				Moves[bK]		=	BIT1 >> 2;
				MoveList->value			=	250;
				MoveList++;
				MOVELIST_TOTAL_ITEMS	+=	1;
			}
		}
	}
	for (int i	=	0; i < 6; i++){
		while (Pieces[i] != 0){
			BitBoard		MoveHolder;
			int position	=	BitPop(Pieces[i]);
			MoveHolder		=	Picker(i, position, OwnPieces, EnemyPieces, Color, EMPTY_BRD);
			MoveList		=	MoveEncoding(i, position, MoveHolder, MoveList, EnemyPieces, A, &MOVELIST_TOTAL_ITEMS);
			Moves[i]		|=	MoveHolder;
		}	
	}	
	//std::cout	<< "______________Ending_move_generation_____________" << std::endl;
	return MOVELIST_TOTAL_ITEMS;
}

int GenerateCaptureMove( struct BOARD A, ExtMove *MoveList, int Color ){
	MoveType	FLAG;
	BitBoard	Pieces[6];
	BitBoard	Moves[6];
	BitBoard	EnemyPieces, OwnPieces;
	int 		MOVELIST_TOTAL_ITEMS	=	0;
	for (int wtf	=	0; 	wtf < 6; wtf++) Moves[wtf]	=	0;
	if (Color	==	WHITE){
		for (int i	=	0; i < 6; i++)	Pieces[i]	=	A.Pieces[i];
		OwnPieces	=	A.CurrentBoard[WHITE];
		EnemyPieces	=	A.CurrentBoard[BLACK];
	}
	else { 
		for (int i	=	0; i < 6; i++)	Pieces[i]	=	A.Pieces[i+6];
		OwnPieces	=	A.CurrentBoard[BLACK];
		EnemyPieces	=	A.CurrentBoard[WHITE];
	}
	for (int i	=	0; i < 6; i++){
		while (Pieces[i] != 0){
			BitBoard		MoveHolder;
			int position	=	BitPop(Pieces[i]);
			MoveHolder		=	Picker(i, position, OwnPieces, EnemyPieces, Color, EMPTY_BRD) & EnemyPieces;
			MoveList		=	MoveEncoding(i, position, MoveHolder, MoveList, EnemyPieces, A, &MOVELIST_TOTAL_ITEMS);
			Moves[i]		|=	MoveHolder;
		}
	}
	return MOVELIST_TOTAL_ITEMS;
};

BOARD	MakeMove(BOARD initial, ExtMove transformer){
	uint8_t	from	=	transformer.getFrom();
	uint8_t to		=	transformer.getTo();
	uint8_t flag	=	transformer.getFlags();
	
	if (initial.Sq[from] == (wR + 6*initial.Side_to_move)){
		if 		(from	==	(63 - (56*initial.Side_to_move)))	initial.Castling_check |=	0x08 << (4*initial.Side_to_move);
		else if (from	==	(56 - (56*initial.Side_to_move)))	initial.Castling_check |=	0x04 << (4*initial.Side_to_move);
	}
	if (initial.Sq[from] == (wK + 6*initial.Side_to_move)){
		initial.Castling_check |=	(0xC << (4*initial.Side_to_move));
	}
	//"So make all your last demand, for i will forsake you".
	initial.Pieces[initial.Sq[from] ]	=	initial.Pieces[initial.Sq[from] ]	& ~(BIT1 >> from) | (BIT1 >> to);
	initial.Pieces[initial.Sq[to] ]	=	initial.Pieces[initial.Sq[to]	]	& ~(BIT1 >> to);
	initial.Sq[to]		=	initial.Sq[from];
	initial.Sq[from]	=	emptySqr;
	if (flag	==	KING_CASTLE){
		initial.Pieces[initial.Side_to_move*6 +	wR]	=	initial.Pieces[initial.Side_to_move*6 +	wR]	& ~ (BIT1 >> (from + 3)) | (BIT1 >> (from + 1));
		initial.Sq[from + 1]	=	initial.Sq[from + 3];
		initial.Sq[from + 3]	=	emptySqr;
		initial.Castling_check	|=	( 0xE << (initial.Side_to_move*4));
	}
	if (flag	==	QUEEN_CASTLE){
		initial.Pieces[initial.Side_to_move*6 +	wR]	=	initial.Pieces[initial.Side_to_move*6 +	wR]	& ~ (BIT1 >> (from -4)) | (BIT1 >> (from -1));
		initial.Sq[from - 1]	=	initial.Sq[from - 4];
		initial.Sq[from - 4]	=	emptySqr;
		initial.Castling_check	|=	( 0xE << (initial.Side_to_move*4));
	}
	if (flag > 7) {
		initial.Pieces[initial.Side_to_move]	^=	(BIT1 >> to);
		initial.Pieces[(flag - 8) % 4 + 1 + 6*initial.Side_to_move]		|=	(BIT1 >> to);
		initial.Sq[to]		=	(flag - 8) % 4 + 1 + 6*initial.Side_to_move;
	}
	initial.CurrentBoard[0]	=	initial.Pieces[wP] | initial.Pieces[wN] | initial.Pieces[wR] | initial.Pieces[wB] | initial.Pieces[wK] | initial.Pieces[wQ];
	initial.CurrentBoard[1]	=	initial.Pieces[bP] | initial.Pieces[bN] | initial.Pieces[bR] | initial.Pieces[bB] | initial.Pieces[bK] | initial.Pieces[bQ];
	initial.Side_to_move	^=	0x01;
	initial.PreviousMove	=	transformer.move;
	initial.No_Ply++;
	return initial;
}


BOARD	MakeMove(BOARD initial, Move transformer){
	uint8_t	from	=	transformer & 0x3f;
	uint8_t to		=	transformer >> 6 & 0x3f;
	uint8_t flag	=	transformer >> 12 & 0x0f;;
	if (initial.Sq[from] == (wR + 6*initial.Side_to_move)){
		if 		(from	==	(63 - (56*initial.Side_to_move)))	initial.Castling_check |=	0x08 << (4*initial.Side_to_move);
		else if (from	==	(56 - (56*initial.Side_to_move)))	initial.Castling_check |=	0x04 << (4*initial.Side_to_move);
	}
	if (initial.Sq[from] == (wK + 6*initial.Side_to_move)){
		initial.Castling_check |=	(0xC << (4*initial.Side_to_move));
	}
	initial.Pieces[initial.Sq[from] ]	=	initial.Pieces[initial.Sq[from] ]	& ~(BIT1 >> from) | (BIT1 >> to);
	initial.Pieces[initial.Sq[to] ]	=	initial.Pieces[initial.Sq[to]	]	& ~(BIT1 >> to);
	initial.Sq[to]		=	initial.Sq[from];
	initial.Sq[from]	=	emptySqr;
	if (flag	==	KING_CASTLE){
		initial.Pieces[initial.Side_to_move*6 +	wR]	=	initial.Pieces[initial.Side_to_move*6 +	wR]	& ~ (BIT1 >> from + 3) | (BIT1 >> (from + 1));
		initial.Sq[from + 1]	=	initial.Sq[from + 3];
		initial.Sq[from + 3]	=	emptySqr;
		initial.Castling_check	|=	( 0xE << (initial.Side_to_move*4));
	}
	if (flag	==	QUEEN_CASTLE){
		initial.Pieces[initial.Side_to_move*6 +	wR]	=	initial.Pieces[initial.Side_to_move*6 +	wR]	& ~ (BIT1 >> from -4) | (BIT1 >> (from -1));
		initial.Sq[from - 1]	=	initial.Sq[from - 4];
		initial.Sq[from - 4]	=	emptySqr;
		initial.Castling_check	|=	( 0xE << (initial.Side_to_move*4));
	}
	if (flag > 7) {
		initial.Pieces[initial.Side_to_move]	^=	(BIT1 >> to);
		initial.Pieces[(flag - 8) % 4 + 1 + 6*initial.Side_to_move]		|=	(BIT1 >> to);
		initial.Sq[to]		=	(flag - 8) % 4 + 1 + 6*initial.Side_to_move;
	}
	initial.CurrentBoard[0]	=	initial.Pieces[wP] | initial.Pieces[wN] | initial.Pieces[wR] | initial.Pieces[wB] | initial.Pieces[wK] | initial.Pieces[wQ];
	initial.CurrentBoard[1]	=	initial.Pieces[bP] | initial.Pieces[bN] | initial.Pieces[bR] | initial.Pieces[bB] | initial.Pieces[bK] | initial.Pieces[bQ];
	initial.Side_to_move	^=	0x01;
	initial.PreviousMove	=	transformer;
	initial.No_Ply++;
	return initial;
}


BOARD	UnmakeMove(BOARD &B, ExtMove	transformer){
	uint8_t	from	=	transformer.getFrom();
	uint8_t to		=	transformer.getTo();
	uint8_t flag	=	transformer.getFlags();
	B.Pieces[	B.Sq[to]	]		=	B.Pieces[	B.Sq[to]	] & ~(BIT1 >> to) | (BIT1 >> from);
}

BitBoard Picker(int chooser, int pos, BitBoard OwnPieces, BitBoard EnemyPieces, int Color, BitBoard capture){
	BitBoard result;
	switch (chooser){
		case wP:	result	=	generatePawnMove( pos,  OwnPieces,  EnemyPieces,  Color, capture);	break;
		case wB:	result	=	generateBishopMove( pos,  OwnPieces,  EnemyPieces);					break;
		case wR:	result	=	generateRookMove( pos,  OwnPieces,  EnemyPieces);					break;
		case wQ:	result	=	generateQueenMove( pos,  OwnPieces,  EnemyPieces);					break;
		case wN:	result	=	generateKnightMove( pos,  OwnPieces,  EnemyPieces);					break;
		case wK:	result	=	generateKingMove( pos,  OwnPieces,  EnemyPieces);					break;
	}
	return result;
}

BitBoard generatePawnMove		(int pos, BitBoard OwnPieces, BitBoard EnemyPieces, int Color, BitBoard cap){
	BitBoard ValidMoves	=	0;
	BitBoard position	=	BIT1 >> pos;
	if (Color	==	WHITE){
		ValidMoves	|=	( position	<< 8 ) & ~( OwnPieces | EnemyPieces ) & ~cap;	
		ValidMoves  |=  (ValidMoves & Rank3) << 8 & ~( OwnPieces | EnemyPieces ) & ~cap;
		ValidMoves	|=	( (	( position	<< 7 ) & FileH )  ^ (position << 7 )	) & ( EnemyPieces | cap );
		ValidMoves	|=	( (	( position	<< 9 ) & FileA )  ^ (position << 9 )	) & ( EnemyPieces | cap );
	} else {
		ValidMoves	=	(position	>> 8 ) & ~( OwnPieces | EnemyPieces ) & ~cap;
		ValidMoves  |=  (ValidMoves & Rank6) >> 8 & ~( OwnPieces | EnemyPieces ) & ~cap;
		ValidMoves	|=	( (	( position	>> 7 ) & FileA )  ^ (position >> 7 )	) & ( EnemyPieces | cap );
		ValidMoves	|=	( (	( position	>> 9 ) & FileH )  ^ (position >> 9 )	) & ( EnemyPieces | cap );
	}
	return ValidMoves;
}

BitBoard generateKnightMove		(int pos, BitBoard OwnPieces, BitBoard EnemyPieces){
	BitBoard ValidMoves	=	0;
	BitBoard position	=	BIT1 >> pos;
	ValidMoves	=	Knight_AttackMask[pos] & ~OwnPieces;
	return ValidMoves;
}

BitBoard generateBishopMove		(int pos, BitBoard OwnPieces, BitBoard EnemyPieces){
	BitBoard ValidMoves	=	0;
	BitBoard position	=	BIT1 >> pos;
	int magicIndex;
	magicIndex	=	(int)( ((OwnPieces | EnemyPieces) & Bishop_AttackMask[pos]) * magicNumberBishop[63 - pos] >> Shift_B[63 - pos]);
	ValidMoves	=	BishopMoveDatabase[pos][magicIndex] & ~OwnPieces;
	return ValidMoves;
}

BitBoard generateRookMove		(int pos, BitBoard OwnPieces, BitBoard EnemyPieces){
	BitBoard ValidMoves	=	0;
	BitBoard position	=	BIT1 >> pos;
	int magicIndex;
	magicIndex	=	(int)( ((OwnPieces | EnemyPieces) & Rook_AttackMask[pos]) * magicNumberRook[63 - pos] >> Shift_R[63 - pos]);
	ValidMoves	=	RookMoveDatabase[pos][magicIndex] & ~OwnPieces;
	return ValidMoves;
}

BitBoard generateQueenMove		(int pos, BitBoard OwnPieces, BitBoard EnemyPieces){
	BitBoard ValidMoves	=	0;
	BitBoard position	=	BIT1 >> pos;
	int magicIndex;
	magicIndex	=	(int)( ((OwnPieces | EnemyPieces) & Bishop_AttackMask[pos]) * magicNumberBishop[63 - pos] >> Shift_B[63 - pos]);
	ValidMoves	=	BishopMoveDatabase[pos][magicIndex] & ~OwnPieces;
	magicIndex	=	(int)( ((OwnPieces | EnemyPieces) & Rook_AttackMask[pos]) * magicNumberRook[63 - pos] >> Shift_R[63 - pos]);
	ValidMoves	|=	RookMoveDatabase[pos][magicIndex] & ~OwnPieces;
	return ValidMoves;
}

BitBoard generateKingMove		(int pos, BitBoard OwnPieces, BitBoard EnemyPieces){
	BitBoard ValidMoves	=	0;
	BitBoard position	=	BIT1 >> pos;
	ValidMoves	=	King_AttackMask[pos] & ~OwnPieces;
	return ValidMoves;
}

bool isCheck	(int Side){
}

/*
 * "all in all it's just another brick in the wall"
 */
