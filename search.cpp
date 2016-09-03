#include "search.h"

//iterative deepening Minimax Alpha Beta prunning

uint64_t SearchNode = 0;
Move	KillerMove;
Move	PLAYER;
uint64_t Prunning	= 0;
int 	RECUR_CALL	= 0;
int		P_DEPTH[11]	=	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int		FINAL_DEPT;
Move		Bonus;

class STACK{
	int 	DEPTH;
	int 	Current;
	BOARD	Stack[10];
 public:
	STACK			(int A)		{
		DEPTH		=	A;
		Current		=	0;
	}
	void	push	(BOARD A)	{	Stack[Current]		=	A;	Current ++ ;	}
	BOARD	pop		()			{	Current--;	return	Stack[Current];	}
	bool	isEmpty	()			{	return Current  == 0;	}
	BOARD	getTop	()			{	return Stack[Current - 1]; }
};

Move SEARCH_DEPTH(int DEPTH, BOARD A){
	int evaluation;
	int		C_DEPTH	=	DEPTH;
	BOARD	Current_Node;
	STACK	STACK_FRAME(DEPTH);
	
	ExtMove	MOVELIST_AT_DEPTH	[11][256];
	int		TOTALMOVES_AT_DEPTH	[11];
	uint8_t	SEARCH_LEFTOFF		[11];
	int		Alpha_STACK			[11];
	int		Beta_STACK			[11];
	pair<Move, int>	V_STACK		[11];
	pair<Move, int> MOVE_STACK	[11];
	
	SearchNode	=	0;
	bool	PRUNNED;
	int 	breakP;
	bool	OUT_OF_MOVE;
	
	for (int i = 0; i < 10; i++)	{
		TOTALMOVES_AT_DEPTH[i]	=	-1;
		SEARCH_LEFTOFF[i]		=	0;
		MOVE_STACK[i].second	=	MIN_VALUE;
	}
	
	STACK_FRAME.push(A);
	
	Alpha_STACK[C_DEPTH]	=	MIN_VALUE;
	Beta_STACK[C_DEPTH]		=	MAX_VALUE;
	while (!STACK_FRAME.isEmpty() && breakP != -1){
		if (C_DEPTH	==	0 || STACK_FRAME.getTop().Pieces[	wK + 6* (A.Side_to_move^1)	] == 0 ){
			evaluation	=	-QuiesceneSearch(STACK_FRAME.getTop(), MIN_VALUE, MAX_VALUE);
			C_DEPTH++;
			BOARD	tempo	=	STACK_FRAME.pop();
			if (evaluation	>	MOVE_STACK[C_DEPTH].second)		MOVE_STACK[C_DEPTH]	=	make_pair(tempo.PreviousMove, evaluation);
			if (evaluation	>	Alpha_STACK[C_DEPTH])			Alpha_STACK[C_DEPTH]=	evaluation;
			if (Alpha_STACK[C_DEPTH]	>=	Beta_STACK[C_DEPTH]	)	PRUNNED	=	true;
			SearchNode++;
		} else {
			OUT_OF_MOVE	=	true;
			if (TOTALMOVES_AT_DEPTH[C_DEPTH] == -1){		//this node hasnt been explored yet.
				PRUNNED		=	false;
				TOTALMOVES_AT_DEPTH[C_DEPTH]	=	GenerateAllMove(STACK_FRAME.getTop(), MOVELIST_AT_DEPTH[C_DEPTH], STACK_FRAME.getTop().Side_to_move);
				MOVE_STACK[C_DEPTH].second	=	MIN_VALUE;
			}
			for (int i = SEARCH_LEFTOFF[C_DEPTH]; i < TOTALMOVES_AT_DEPTH[C_DEPTH]; i++){
				OUT_OF_MOVE		=	false;
				if (PRUNNED)	{
					int Feed;
					PRUNNED		=	false;
					TOTALMOVES_AT_DEPTH[C_DEPTH] 	= -1;
					SEARCH_LEFTOFF[C_DEPTH]			= 0;
					BOARD	tempo	=	STACK_FRAME.pop();///*
					if (-MOVE_STACK[C_DEPTH].second	>	MOVE_STACK[C_DEPTH+1].second)		MOVE_STACK[C_DEPTH+1]	=	make_pair(tempo.PreviousMove, -MOVE_STACK[C_DEPTH].second);
					if (-MOVE_STACK[C_DEPTH].second	>	Alpha_STACK[C_DEPTH + 1])			Alpha_STACK[C_DEPTH+1]	=	-MOVE_STACK[C_DEPTH].second;
					if (Alpha_STACK[C_DEPTH+1]		>=	Beta_STACK[C_DEPTH+1]	)			PRUNNED	=	true;
					C_DEPTH++;
					break;
				}
				int iMin = i;
				for (int j = i+1; j < TOTALMOVES_AT_DEPTH[C_DEPTH]; j++)	if (MOVELIST_AT_DEPTH[C_DEPTH][j] > MOVELIST_AT_DEPTH[C_DEPTH][iMin])	iMin	=	j;
				if (i != iMin)	{
					ExtMove	tmp						=	MOVELIST_AT_DEPTH[C_DEPTH][i];
					MOVELIST_AT_DEPTH[C_DEPTH][i]	=	MOVELIST_AT_DEPTH[C_DEPTH][iMin];
					MOVELIST_AT_DEPTH[C_DEPTH][iMin]=	tmp;
				}
				SEARCH_LEFTOFF[C_DEPTH]++;
				STACK_FRAME.push(MakeMove(STACK_FRAME.getTop(), MOVELIST_AT_DEPTH[C_DEPTH][i]));
				C_DEPTH--;
				Alpha_STACK[C_DEPTH]	=	-Beta_STACK[C_DEPTH+1];
				Beta_STACK[C_DEPTH]		=	-Alpha_STACK[C_DEPTH+1];
				break;
			}
			if (OUT_OF_MOVE)	{
				TOTALMOVES_AT_DEPTH[C_DEPTH] 	= -1;
				SEARCH_LEFTOFF[C_DEPTH]			= 0;
				BOARD	tempo	=	STACK_FRAME.pop();
				if (-MOVE_STACK[C_DEPTH].second	>	MOVE_STACK[C_DEPTH+1].second)		MOVE_STACK[C_DEPTH+1]	=	make_pair(tempo.PreviousMove, -MOVE_STACK[C_DEPTH].second);
				if (-MOVE_STACK[C_DEPTH].second	>	Alpha_STACK[C_DEPTH + 1])			Alpha_STACK[C_DEPTH+1]	=	-MOVE_STACK[C_DEPTH].second;
				if (Alpha_STACK[C_DEPTH+1]		>=	Beta_STACK[C_DEPTH+1]	)			PRUNNED	=	true;//*/
				C_DEPTH++;
			}
		}
	}
	DecodeMove (MOVE_STACK[DEPTH].first);;
}


pair<Move, int>	IterDeepening_SEARCH (BOARD A, int MAX_DEPTH){
	ExtMove FirstBatch[255];
	pair<Move, int> DES_MOVE;
	Bonus	=	0;
	FINAL_DEPT	=	MAX_DEPTH - 2;
	DES_MOVE	=	Search(A, MAX_DEPTH - 2, MIN_VALUE, MAX_VALUE, WHITE);
	FINAL_DEPT	=	MAX_DEPTH;
	Bonus		=	DES_MOVE.first;
	DecodeMove(Bonus);
	SearchNode	=	0;
	Prunning	=	0;
	DES_MOVE	=	Search(A, MAX_DEPTH, MIN_VALUE, MAX_VALUE, WHITE);
	return DES_MOVE;
}


pair<Move, int>	Search (BOARD A, int DEPTH, int Alpha, int Beta, int TURN){
	int 	v;
	pair<Move, int> result;
	if (DEPTH	==	0 || A.Pieces[wK + 6* (A.Side_to_move)] == 0) {
		SearchNode++;
		if ( A.Pieces[wK + 6* (A.Side_to_move)] == 0 ){
			if ( A.Side_to_move ==	BLACK ) return  make_pair(A.PreviousMove, 999999);
			else 							return  make_pair(A.PreviousMove, -999999);
		}
		int evaluation	=	QuiesceneSearch(A, MIN_VALUE, MAX_VALUE);
		if ( A.Side_to_move	==	BLACK ) evaluation	*=	-1;
		return make_pair(A.PreviousMove, evaluation);
	}
	int	BestVal;
	if (TURN ==	WHITE){
		v	=	MIN_VALUE;
		ExtMove 		MoveList[256];
		ExtMove *lo		=	MoveList;
		int size		=	GenerateAllMove(A, lo, TURN);
		if (DEPTH	==	FINAL_DEPT) {
			cout	<< "DEPTH = " << DEPTH << endl;
			for (int i = 0; i < size; i++) {
				if (MoveList[i].move == Bonus) {
					MoveList[i].value	+=	2000;
					cout	<< "Adding bonus" << endl;
					break;
				}
			}
		}
		for (int i = 0; i < size; i++){
			int iMin = i;
			for (int j = i+1; j < size; j++)	if (MoveList[j] > MoveList[iMin])	iMin	=	j;
			if (i != iMin)	{
				ExtMove	tmp		=	MoveList[i];
				MoveList[i]		=	MoveList[iMin];
				MoveList[iMin]	=	tmp;
			}
			BestVal	=	v;
			v		=	max(v, Search( MakeMove(A, MoveList[i]),  DEPTH - 1,  Alpha,  Beta,  TURN ^ 1).second);
			if ( BestVal != v )		result	=	make_pair(MoveList[i].move, v);
			if ( Alpha < v) 			Alpha	=	v;
			if ( Alpha >= Beta ) {	Prunning++; break; }
			
		}
	} else {
		v	=	MAX_VALUE;
		ExtMove 		MoveList[256];
		ExtMove *lo		=	MoveList;
		int size		=	GenerateAllMove(A, lo, TURN);
		for (int i = 0; i < size; i++){
			int iMin = i;
			for (int j = i+1; j < size; j++)	if (MoveList[j] > MoveList[iMin])	iMin	=	j;
			if (i != iMin)	{
				ExtMove	tmp		=	MoveList[i];
				MoveList[i]		=	MoveList[iMin];
				MoveList[iMin]	=	tmp;
			}
			BestVal	=	v;
			v		=	min(v, Search( MakeMove(A, MoveList[i]),  DEPTH - 1,  Alpha,  Beta,  TURN ^ 1).second);
			if (BestVal != v) 	result	=	make_pair(MoveList[i].move, v);
			if (Beta > v) 		Beta	=	v;
			if ( Alpha >= Beta )  {	Prunning++;	break; }
		}
	}
	return result;
}

int	QuiesceneSearch(BOARD A, int Alpha, int Beta){
	SearchNode++;
	ExtMove CaptureList[150];
	ExtMove *iter	=	CaptureList;
	ExtMove *lol	=	CaptureList;
	int evaluation	=	EvaluateBOARD(A, A.Side_to_move);
	if (evaluation 	>=	Beta) 	{	return Beta;	}
	if (evaluation  >=	Alpha) 	{	Alpha 	=	evaluation;	}
	int CAPTURE_LIST_SIZE	=	GenerateCaptureMove(A, iter, A.Side_to_move); 
	for (int i = 0; i < CAPTURE_LIST_SIZE; i++){
		int iMin = i;
		for (int j = i+1; j < CAPTURE_LIST_SIZE; j++)	if (CaptureList[j] > CaptureList[iMin])	iMin	=	j;
		if ( i != iMin )	{
			ExtMove	tmp			=	CaptureList[i];
			CaptureList[i]		=	CaptureList[iMin];
			CaptureList[iMin]	=	tmp;
		}
		evaluation	=	-QuiesceneSearch(MakeMove(A, CaptureList[i]), -Beta, -Alpha);
		if (evaluation 	>=	Beta) 	{	return Beta;	}
		if (evaluation  >=	Alpha) 	{	Alpha 	=	evaluation;	}
    }
    return Alpha;
}

int max (int A, int B){	if ( A > B ) return A; return B;	}

int min (int A, int B){	if ( A <= B ) return A; return B;	}

