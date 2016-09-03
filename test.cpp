#include <stdio.h>
#include <iostream>
#include "BitBoard.h"
#include "MoveGen.h"
#include "evaluation.h"
#include "search.h"
#include <time.h> 
//#include <thread>         // std::thread

using namespace std;

BOARD	INIT;
BOARD	I2;

void		InitBoard()				{
	Castling_right	AA;
	for (int j	=	0; j < 12; j++){
		INIT.Pieces[j]		=	EMPTY_BRD;
	}
	//INITIAL SETTING
	for (int t 	=	0; t < 64; t++)	INIT.Sq[t]	=	emptySqr;
	for (int i	=	0;	i<8; i++){
		INIT.Pieces[wP]	|=	BIT1 >> (i+48);
		INIT.Sq[i+48]	=	wP;
		INIT.Pieces[bP]	|=	BIT1 >> (i+8);
		INIT.Sq[i+8]	=	bP;
	}
	INIT.Pieces[bK]	=	BIT1 >> 4;INIT.Sq[4]	=	bK;
	INIT.Pieces[bQ]	=	BIT1 >> 3;INIT.Sq[3]	=	bQ;
	INIT.Pieces[bR]	=	BIT1 >> 0 | BIT1 >> 7;INIT.Sq[0]	=	bR;INIT.Sq[7]	=	bR;
	INIT.Pieces[bB]	=	BIT1 >> 2 | BIT1 >> 5;INIT.Sq[2]	=	bB;INIT.Sq[5]	=	bB;
	INIT.Pieces[bN]	=	BIT1 >> 1 | BIT1 >> 6;INIT.Sq[1]	=	bN;INIT.Sq[6]	=	bN;
	INIT.Pieces[wK]	=	BIT1 >> 60;INIT.Sq[60]	=	wK;
	INIT.Pieces[wQ]	=	BIT1 >> 59;INIT.Sq[59]	=	wQ;
	INIT.Pieces[wR]	=	BIT1 >> 56 | BIT1 >> 63;INIT.Sq[56]	=	wR;INIT.Sq[63]	=	wR;
	INIT.Pieces[wB]	=	BIT1 >> 58 | BIT1 >> 61;INIT.Sq[58]	=	wB;INIT.Sq[61]	=	wB;
	INIT.Pieces[wN]	=	BIT1 >> 57 | BIT1 >> 62;INIT.Sq[57] =	wN;INIT.Sq[62]	=	wN;
	getBoardInfo(INIT);
	for (int i = 0; i < 6; i++){
		INIT.CurrentBoard[WHITE]	|=	INIT.Pieces[i];
		INIT.CurrentBoard[BLACK]	|=	INIT.Pieces[i+6];
	}
	INIT.No_Ply			=	0;
	INIT.Side_to_move	=	0;
	INIT.Castling_check	=	0;
}

void		CheckPromotion()		{
	for (int j	=	0; j < 12; j++)	I2.Pieces[j]		=	EMPTY_BRD;
	for (int t 	=	0; t < 64; t++)	I2.Sq[t]	=	emptySqr;
	I2.Pieces[wP]		=	BIT1 >> 10;	I2.Sq[10]	=	wP;
	I2.Pieces[wP]		|=	BIT1 >> 13;	I2.Sq[13]	=	wP;
	I2.Pieces[bR]		=	BIT1 >> 1 ; I2.Sq[1]	=	bR;
	for (int i = 0; i <6; i++){
		I2.CurrentBoard[WHITE]	|=	I2.Pieces[i];
		I2.CurrentBoard[BLACK]	|=	I2.Pieces[i+6];
	}
	I2.Side_to_move	=	0;
	I2.Castling_check		=	0xC0	|	0x50	|	0x3		|	0xA;
}

int main(){
	pair <Move, int> RES;
	cout	<< "ENGINE ACTIVATED\n";
	time_t now;
	BitBoard	test	=	EMPTY_BRD;
	BitBoard	Own		=	BIT1 >> 36 | BIT1 >> 18 | BIT1 >> 22;
	GenerateMask();
	GenerateMoveData();
	cout	<<	"----------------New_Move_testing_generation-------------------\n";
	InitBoard();
	int N, M = 0;
	getBoardInfo(INIT);
	N	=	time(&now);
	ExtMove ok[265];
	int size;
	cout	<< "\n----------------------------------------------\n";
	ExtMove *lo;
	cin >> M;
	while (N!= -1){
		SearchNode	=	0;
		Prunning	=	0;
		N	=	time(&now);
		ExtMove ok[265];
		int size=	GenerateAllMove(INIT, ok, INIT.Side_to_move);
		ExtMove *lo	=	ok;
		for (int t = 0; t < size; t++){
			cout	<< t << ": ";
			DecodeMove(lo++);
		}
		///*
		RES		=	IterDeepening_SEARCH(INIT, M);
		cout	<< "Number of Search node = : " << SearchNode << endl;
		cout	<< "Number of branches prunned = : " << Prunning << endl;
		cout	<< "Program speed =  : " << time(&now) - N << endl;
		cout	<< "Choosen Move : ";	DecodeMove(RES.first);
		cout	<< "Move Score : " << RES.second << endl;
		INIT	=	MakeMove(INIT, RES.first);
		//*/
		getBoardInfo(INIT);
		size=	GenerateAllMove(INIT, ok, INIT.Side_to_move);
		lo	=	ok;
		for (int t = 0; t < size; t++){
			cout	<< t << ": ";
			DecodeMove(lo++);
		}
		cin >> N;
		INIT	=	MakeMove(INIT,ok[N]);
		getBoardInfo(INIT);
	}
	cout	<< "From = ?\n";
	int From, To;
	cin		>> From;
	cout	<< "To = ?\n";
	cin		>> To;
	cout	<< "surpisingly well\n";
	getBoardInfo(INIT);
	SEET(To, INIT, From, INIT.Side_to_move);
}
