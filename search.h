#ifndef SEARCH_H_
#define SEARCH_H_

#include "evaluation.h"
#include "MoveGen.h"
#include <utility>      // std::pair, std::make_pair
#include <stdio.h>

using namespace std;

extern uint64_t SearchNode;
extern uint64_t Prunning;
extern int		FINAL_DEPT;
extern int		RECUR_CALL;
extern int 		P_DEPTH[11];

pair<Move, int>	Search (BOARD A, int DEPTH,  int Alpha, int Beta, int TURN);

Move SEARCH_DEPTH(int DEPTH, BOARD A);

int	QuiesceneSearch(BOARD A, int Alpha, int Beta);

pair<Move, int> IterDeepening_SEARCH(BOARD A, int MAX_DEPTH);

#endif 
