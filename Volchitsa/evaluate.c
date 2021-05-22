#include "stdio.h"
#include "defs.h"

const int BishopMobility = 5;	//	5>10?
const int RookOpenFile = 40;  // X>20
const int RookSemiOpenFile = 20; // X>10
//const int QueenOpenFile = 10;
//const int QueenSemiOpenFile = 5;

const int BishopPair = 40; // tuned		30<X<50 long ago

const int KnightOutpost = 15;
const int SelfBlock = -20;

const int DoubledPawn = -15;
const int IsolatedPawn = -10; // x<20?(test stopped prematurely, but seemed unlikely)
const int BackwardPawn = -10;

const int PieceAttack = 10; 

const int PawnAttack = 1;
const int KnightPieceAttack = 3;
const int BishopAttack = 2;
const int RookAttack = 3;
const int QueenAttack = 4;
const int KnightDefense = 4; 
const int BishopDefense = 2; 



const int KingPawnShelter[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
20	,	20	,	20	,	20	,	20	,	20	,	20	,	20	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
5	,	5	,	5	,	20	,	20	,	5	,	5	,	5	,
5	,	5	,	5	,	20	,	20	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	20	,	20	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int PawnTablePasser[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
10	,	5	,	5	,	5	,	5	,	5	,	5	,	10	,
15	,	10	,	10	,	10	,	10	,	10	,	10	,	15	,
25	,	20	,	20	,	20	,	20	,	20	,	20	,	25	,
60	,	50	,	50	,	50	,	50	,	50	,	50	,	60	,
110	,	100	,	100	,	100	,	100	,	100	,	100	,	110	,
210	,	200	,	200	,	200	,	200	,	200	,	200	,	210	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int PawnTableO[64] = 
{
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	5	,	5	,	5	,	5	,	5	,	5	,	0	,
-1	,	3	,	0	,	20	,	20	,	0	,	3	,	-1	,
3	,	7	,	10	,	35	,	35	,	10	,	7	,	3	,
8	,	10	,	15	,	33	,	33	,	15	,	10	,	8	,
15	,	15	,	25	,	40	,	40	,	25	,	15	,	15	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int PawnTableE[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
5	,	0	,	0	,	0	,	0	,	0	,	0	,	5	,
15	,	10	,	10	,	10	,	10	,	10	,	10	,	15	,
25	,	20	,	20	,	20	,	20	,	20	,	20	,	25	,
35	,	30	,	30	,	30	,	30	,	30	,	30	,	35	,
45	,	40	,	40	,	40	,	40	,	40	,	40	,	45	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int KnightTable[64] = {
-30	,	-20	,	-12	,	-5	,	-5	,	-12	,	-20	,	-30	,
-20	,	-10	,	-5	,	-1	,	-1	,	-5	,	-10	,	-20	,
-10	,	-5	,	3	,	5	,	5	,	3	,	-5	,	-10	,
-5	,	-2	,	5	,	10	,	10	,	5	,	-2	,	-5	,
-5	,	1	,	5	,	10	,	10	,	5	,	1	,	-5	,
-10	,	-5	,	0	,	5	,	5	,	0	,	-5	,	-10	,
-15	,	-10	,	-5	,	0	,	0	,	-5	,	-10	,	-15	,
-20	,	-15	,	-10	,	-5	,	-5	,	-10	,	-15	,	-20		
};

const int BishopTable[64] = {
-10	,	-10	,	-11	,	-12	,	-12	,	-11	,	-10	,	-10	,
2	,	2	,	2	,	2	,	2	,	2	,	2	,	2	,
2	,	2	,	2	,	2	,	2	,	2	,	2	,	2	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
-2	,	-2	,	-2	,	-2	,	-2	,	-2	,	-2	,	-2	,
-10	,	-10	,	-10	,	-10	,	-10	,	-10	,	-10	,	-10	,
-15	,	-15	,	-15	,	-15	,	-15	,	-15	,	-15	,	-15		
};

const int RookTable[64] = {
-8	,	0	,	5	,	10	,	10	,	5	,	0	,	-8	,
-8	,	0	,	5	,	10	,	10	,	5	,	0	,	-8	,
-5	,	0	,	5	,	10	,	10	,	5	,	0	,	-5	,
-5	,	0	,	5	,	10	,	10	,	5	,	0	,	-5	,
-5	,	0	,	5	,	10	,	10	,	5	,	0	,	-5	,
-5	,	0	,	5	,	10	,	10	,	5	,	0	,	-5	,
25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
-5	,	0	,	5	,	10	,	10	,	5	,	0	,	-5		
};


const int KingTableO[64] = {
15	,	25	,	0	,	-5	,	-5	,	0	,	25	,	15	,
5	,	5	,	-5	,	5	,	5	,	-5	,	5	,	5	,
-20	,	-20	,	-20	,	-20	,	-20	,	-20	,	-20	,	-20	,
-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,
-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,
-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,
-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,
-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50		
};
// or

const int KingTableE[64] = {
-40	,	-30	,	-20	,	-10	,	-10	,	-20	,	-30	,	-40	,
-30	,	-20	,	-10	,	0	,	0	,	-10	,	-20	,	-30	,
-20	,	-10	,	0	,	10	,	10	,	0	,	-10	,	-20	,
-10	,	0	,	10	,	20	,	20	,	10	,	0	,	-10	,
-10	,	0	,	10	,	20	,	20	,	10	,	0	,	-10	,
-20	,	-10	,	0	,	10	,	10	,	0	,	-10	,	-20	,
-30	,	-20	,	-10	,	0	,	0	,	-10	,	-20	,	-30	,
-40	,	-30	,	-20	,	-10	,	-10	,	-20	,	-30	,	-40		
}; 
// double 3775-3725-2500 +2 elo
/*
const int KingTableE[64] = {
-80	,	-60	,	-40	,	-20	,	-20	,	-40	,	-60	,	-80	,
-60	,	-40	,	-20	,	0	,	0	,	-20	,	-40	,	-60	,
-40	,	-20	,	0	,	20	,	20	,	0	,	-20	,	-40	,
-20	,	0	,	20	,	40	,	40	,	20	,	0	,	-20	,
-20	,	0	,	20	,	40	,	40	,	20	,	0	,	-20	,
-40	,	-20	,	0	,	20	,	20	,	0	,	-20	,	-40	,
-60	,	-40	,	-20	,	0	,	0	,	-20	,	-40	,	-60	,
-80	,	-60	,	-40	,	-20	,	-20	,	-40	,	-60	,	-80		
};
*/
/*
const int QueenTableO[64] = {
-3	,	-3	,	-3	,	-3	,	-3	,	-3	,	-3	,	-3	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
-2	,	-2	,	-2	,	-2	,	-2	,	-2	,	-2	,	-2	,
-4	,	-6	,	-10	,	-15	,	-15	,	-10	,	-6	,	-4	,
-8	,	-8	,	-10	,	-15	,	-15	,	-10	,	-8	,	-8	,
-20	,	-20	,	-20	,	-20	,	-20	,	-20	,	-20	,	-20	,
-20	,	-20	,	-20	,	-20	,	-20	,	-20	,	-20	,	-20	,
-20	,	-20	,	-20	,	-20	,	-20	,	-20	,	-20	,	-20		
};
*/

const int QueenTableE[64] = {
-40	,	-30	,	-20	,	-10	,	-10	,	-20	,	-30	,	-40	,
-30	,	-20	,	-10	,	0	,	0	,	-10	,	-20	,	-30	,
-20	,	-10	,	0	,	10	,	10	,	0	,	-10	,	-20	,
-10	,	0	,	10	,	20	,	20	,	10	,	0	,	-10	,
-10	,	0	,	10	,	20	,	20	,	10	,	0	,	-10	,
-20	,	-10	,	0	,	10	,	10	,	0	,	-10	,	-20	,
-30	,	-20	,	-10	,	0	,	0	,	-10	,	-20	,	-30	,
-40	,	-30	,	-20	,	-10	,	-10	,	-20	,	-30	,	-40		
};

const int Mirror[64] = {
56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};

int PawnTablePasserB[64];
int PawnTableEB[64];
int PawnTableOB[64];
int KnightTableB[64];
int BishopTableB[64];
int RookTableB[64];
int KingTableOB[64];
int KingTableEB[64];
int QueenTableOB[64];
int QueenTableEB[64];

void InitBlackPSQT()
{
	int sq = 63;
	for(sq = 0; sq< 63; sq++)
	{
		PawnTablePasserB[sq] = PawnTablePasser[Mirror[sq]];
		PawnTableEB[sq] = PawnTableE[Mirror[sq]];
		PawnTableOB[sq] = PawnTableO[Mirror[sq]];
		KnightTableB[sq] = KnightTable[Mirror[sq]];
		BishopTableB[sq] = BishopTable[Mirror[sq]];
		RookTableB[sq] = RookTable[Mirror[sq]];
		KingTableOB[sq] = KingTableO[Mirror[sq]];
		KingTableEB[sq] = KingTableE[Mirror[sq]];
		//QueenTableOB[sq] = QueenTableO[Mirror[sq]];
		QueenTableEB[sq] = QueenTableE[Mirror[sq]];
	}
}

#define MAX_MAT (2 * PieceVal[wR] + 2 * PieceVal[wN] + 2 * PieceVal[wB] + 8 * PieceVal[wP] + PieceVal[wQ] + 2 * PieceVal[bR] + 2 * PieceVal[bN] + 2 * PieceVal[bB] + 8 * PieceVal[bP] + PieceVal[bQ])

int EvalPosition(const S_BOARD *pos) {

	int pce;
	int pceNum;
	int sq;
	float endgamescore = EvalEndgame(pos);
	if(endgamescore != -0.5)
	{
		if(pos->side == WHITE) 
		{
			return endgamescore;
		} else {
			return -endgamescore;
		}
	}
	int score = pos->material[WHITE] - pos->material[BLACK];
	int GamePhase = (((pos->material[WHITE] + pos->material[BLACK] - PieceVal[wK] - PieceVal [bK])  * 1024)  / MAX_MAT);
	int ReverseGamePhase = 1024 - GamePhase;
	int WhiteAttack = 0;
	int BlackAttack = 0;
	int WhiteDefense = 0;
	int BlackDefense = 0;
	
	U64 e_pawns = pos->pieceBB[P][WHITE];
	
	while(e_pawns)
	{
		sq = POP(&e_pawns);
		
		if( (IsolatedMask[sq] & pos->pieceBB[P][WHITE]) == 0) {
			score += IsolatedPawn;
		}
		
		if( (WhitePassedMask[sq] & pos->pieceBB[P][BLACK]) == 0) {
			score +=  PawnTablePasser[sq];
		}
		else{
			score += ((PawnTableO[sq] * GamePhase) + (PawnTableE[sq] * ReverseGamePhase)) / 1024;
		
			if( (BlackSentryMask[sq] & pos->pieceBB[P][BLACK]) && ((WhiteBackwardMask[sq] & pos->pieceBB[P][WHITE]) == 0) ) 
			{
				score += BackwardPawn;
			}	
		}
		
		if(DoubledMask[sq] & pos->pieceBB[P][WHITE]){
			score += DoubledPawn;
		}
		
		if(AttackRectangle[sq] & pos->pieceBB[K][BLACK])
		{
			WhiteAttack += PawnAttack;
		}
	}
	
	e_pawns = pos->pieceBB[P][BLACK];
	
	while(e_pawns)
	{
		sq = POP(&e_pawns);
		
		if( (IsolatedMask[sq] & pos->pieceBB[P][BLACK]) == 0) {
			score -= IsolatedPawn;
		}
		
		if( (BlackPassedMask[sq] & pos->pieceBB[P][WHITE]) == 0) {
			score -=  PawnTablePasserB[sq];
		}
		else{
			score -= ((PawnTableOB[sq] * GamePhase) + (PawnTableEB[sq] * ReverseGamePhase)) / 1024;
		
			if( (WhiteSentryMask[sq] & pos->pieceBB[P][WHITE]) && ((BlackBackwardMask[sq] & pos->pieceBB[P][BLACK]) == 0) ) 
			{
				score -= BackwardPawn;
			}	
		}
		
		if(DoubledMask[sq] & pos->pieceBB[P][BLACK]){
			score -= DoubledPawn;
		}
		
		if(AttackRectangle[sq] & pos->pieceBB[K][WHITE])
		{
			BlackAttack += PawnAttack;
		}
	}
	
	U64 e_knights = pos->pieceBB[N][WHITE];
	
	while(e_knights)
	{
		sq = POP(&e_knights);
		score += KnightTable[sq];
		
		if (WhiteStupid[sq] & pos->pieceBB[P][WHITE])
		{
			score += SelfBlock;
		}
		
		if (WhiteSupportMask[sq] & pos->pieceBB[P][WHITE])
		{
			if ((WhiteOutpostMask[sq] & pos->pieceBB[P][BLACK]) == 0)
				{
				score += KnightOutpost;
				}
		}	
		
		if(AttackRectangle[sq] & pos->pieceBB[K][BLACK])
		{
			WhiteAttack += KnightPieceAttack;
		}
		if(AttackSquare[sq] & pos->pieceBB[K][WHITE])
		{
			WhiteDefense += KnightDefense;
		}
	}
	
	e_knights = pos->pieceBB[N][BLACK];
	
	while(e_knights)
	{
		sq = POP(&e_knights);
		score -= KnightTableB[sq];
		
		if (BlackStupid[sq] & pos->pieceBB[P][BLACK])
		{
			score -= SelfBlock;
		}
		
		if (BlackSupportMask[sq] & pos->pieceBB[P][BLACK])
		{
			if ((BlackOutpostMask[sq] & pos->pieceBB[P][WHITE]) == 0)
				{
				score -= KnightOutpost;
				}
		}
		
		if(AttackRectangle[sq] & pos->pieceBB[K][WHITE])
		{
			BlackAttack += KnightPieceAttack;
		}
		if(AttackSquare[sq] & pos->pieceBB[K][BLACK])
		{
			BlackDefense += KnightDefense;
		}
		
	}
	
	U64 e_bishops = pos->pieceBB[B][WHITE];
	
	while(e_bishops)
	{
		sq = POP(&e_bishops);
		score += BishopTable[sq];
		
		if (WhiteStupid[sq] & pos->pieceBB[P][WHITE])
		{
			score += SelfBlock;
		}
		
		if(AttackSquare[sq] & pos->pieceBB[K][WHITE])
		{
			WhiteDefense += BishopDefense;
		}
		
		if(AttackRectangle[sq] & pos->pieceBB[K][BLACK])
		{
			WhiteAttack += BishopAttack;
		}

		int t_sq = sq;
		if((RanksBrd[t_sq] != RANK_1)&&(FilesBrd[t_sq] != FILE_H))
		{
		t_sq += -7;
		while(1)
		{
			if(SliderAttack[t_sq] & pos->pieceBB[P][BOTH])
			{
				break;
			}
			score += BishopMobility;
			if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_H))
			{
				break;
			}
			t_sq += -7;
		}
		}
		//1
		t_sq = sq;
		if((RanksBrd[t_sq] != RANK_1)&&(FilesBrd[t_sq] != FILE_A))
		{
		t_sq += -9;
		while(1)
		{
			if(SliderAttack[t_sq] & pos->pieceBB[P][BOTH])
			{
				break;
			}
			score += BishopMobility;
			if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_A))
			{
				break;
			}
			t_sq += -9;
		}
		}
		//2
		t_sq = sq;
		if((RanksBrd[t_sq] != RANK_8)&&(FilesBrd[t_sq] != FILE_A))
		{
		t_sq += 7;
		while(1)
		{
			if(SliderAttack[t_sq] & pos->pieceBB[P][BOTH])
			{
				break;
			}
			score += BishopMobility;
			if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_A))
			{
				break;
			}
			t_sq += 7;
		}
		}
		//3
		t_sq = sq;
		if((RanksBrd[t_sq] != RANK_8)&&(FilesBrd[t_sq] != FILE_H))
		{
		t_sq += 9;
		while(1)
		{
			if(SliderAttack[t_sq] & pos->pieceBB[P][BOTH])
			{
				break;
			}
			score += BishopMobility;
			if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_H))
			{
				break;
			}
			t_sq += 9;
		}
		}	
	}
	
	e_bishops = pos->pieceBB[B][BLACK];
	
	while(e_bishops)
	{
		sq = POP(&e_bishops);
		score -= BishopTableB[sq];
		
		if (BlackStupid[sq] & pos->pieceBB[P][BLACK])
		{
			score -= SelfBlock;
		}
		
		if(AttackSquare[sq] & pos->pieceBB[K][BLACK])
		{
			BlackDefense += BishopDefense;
		}
		
		if(AttackRectangle[sq] & pos->pieceBB[K][WHITE])
		{
			BlackAttack += BishopAttack;
		}
		
		int t_sq = sq;
		if((RanksBrd[t_sq] != RANK_1)&&(FilesBrd[t_sq] != FILE_H))
		{
		t_sq += -7;
		while(1)
		{
			if(SliderAttack[t_sq] & pos->pieceBB[P][BOTH])
			{
				break;
			}
			score -= BishopMobility;
			if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_H))
			{
				break;
			}
			t_sq += -7;
		}
		}
		//1
		t_sq = sq;
		if((RanksBrd[t_sq] != RANK_1)&&(FilesBrd[t_sq] != FILE_A))
		{
		t_sq += -9;
		while(1)
		{
			if(SliderAttack[t_sq] & pos->pieceBB[P][BOTH])
			{
				break;
			}
			score -= BishopMobility;
			if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_A))
			{
				break;
			}
			t_sq += -9;
		}
		}
		//2
		t_sq = sq;
		if((RanksBrd[t_sq] != RANK_8)&&(FilesBrd[t_sq] != FILE_A))
		{
		t_sq += 7;
		while(1)
		{
			if(SliderAttack[t_sq] & pos->pieceBB[P][BOTH])
			{
				break;
			}
			score -= BishopMobility;
			if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_A))
			{
				break;
			}
			t_sq += 7;
		}
		}
		//3
		t_sq = sq;
		if((RanksBrd[t_sq] != RANK_8)&&(FilesBrd[t_sq] != FILE_H))
		{
		t_sq += 9;
		while(1)
		{
			if(SliderAttack[t_sq] & pos->pieceBB[P][BOTH])
			{
				break;
			}
			score -= BishopMobility;
			if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_H))
			{
				break;
			}
			t_sq += 9;
		}
		}
	}
	
	U64 e_rooks = pos->pieceBB[R][WHITE];
	
	while(e_rooks)
	{
		sq = POP(&e_rooks);
		score += RookTable[sq];
		if(!(FileBBMask[FilesBrd[sq]] & pos->pieceBB[P][BOTH]))
		{
			score += RookOpenFile;
		}
		else if(!(FileBBMask[FilesBrd[sq]] & pos->pieceBB[P][WHITE]))
		{
			score += RookSemiOpenFile;
		}
		
		if(RookAttackArea[sq] & pos->pieceBB[K][BLACK])
		{
			if(!(WhiteRookPawnCheck[sq] & pos->pieceBB[P][WHITE]))
			{
				WhiteAttack += RookAttack;
			}
		}
	}
	
	e_rooks = pos->pieceBB[R][BLACK];
	
	while(e_rooks)
	{
		sq = POP(&e_rooks);
		score -= RookTableB[sq];
		if(!(FileBBMask[FilesBrd[sq]] & pos->pieceBB[P][BOTH]))
		{
			score -= RookOpenFile;
		}
		else if(!(FileBBMask[FilesBrd[sq]] & pos->pieceBB[P][BLACK]))
		{
			score -= RookSemiOpenFile;
		}
		
		if(RookAttackArea[sq] & pos->pieceBB[K][WHITE])
		{
			if(!(BlackRookPawnCheck[sq] & pos->pieceBB[P][BLACK]))
			{
				BlackAttack += RookAttack;
			}
		}
	}
	
	U64 e_queens = pos->pieceBB[Q][WHITE];
	
	while(e_queens)
	{
		sq = POP(&e_queens);
		//score += ((QueenTableO[sq] * GamePhase) + (QueenTableE[sq] * ReverseGamePhase)) / 1024;
		score += (QueenTableE[sq] * ReverseGamePhase) / 1024;
		
		if (WhiteStupid[sq] & pos->pieceBB[P][WHITE])
		{
			score += SelfBlock;
		}
		
		if(AttackRectangle[sq] & pos->pieceBB[K][BLACK])
		{
			WhiteAttack += QueenAttack;
		}
		
		//if(!(FileBBMask[FilesBrd[sq]] & pos->pieceBB[P][BOTH]))
		//{
		//	score += QueenOpenFile;
		//}
		//else if(!(FileBBMask[FilesBrd[sq]] & pos->pieceBB[P][WHITE]))
		//{
		//	score += QueenSemiOpenFile;
		//}
	}
	
	e_queens = pos->pieceBB[Q][BLACK];
	
	while(e_queens)
	{
		sq = POP(&e_queens);
		//score -= ((QueenTableOB[sq] * GamePhase) + (QueenTableEB[sq] * ReverseGamePhase)) / 1024;
		score -= (QueenTableEB[sq] * ReverseGamePhase) / 1024;
		
		if (BlackStupid[sq] & pos->pieceBB[P][BLACK])
		{
			score -= SelfBlock;
		}
		
		if(AttackRectangle[sq] & pos->pieceBB[K][WHITE])
		{
			BlackAttack += QueenAttack;
		}
		
		//if(!(FileBBMask[FilesBrd[sq]] & pos->pieceBB[P][BOTH]))
		//{
		//	score -= QueenOpenFile;
		//}
		//else if(!(FileBBMask[FilesBrd[sq]] & pos->pieceBB[P][BLACK]))
		//{
		//	score -= QueenSemiOpenFile;
		//}
	}
	
	sq = pos->KingSq[WHITE];
	U64 e_kings = pos->pieceBB[K][WHITE];
	score += ((KingTableO[sq] * GamePhase) + (KingTableE[sq] * ReverseGamePhase)) / 1024;
	
	///*
	if(GamePhase > 128)
	{
		if(WhiteCastleKingMask & e_kings)
		{
			if(pos->pieceBB[P][WHITE] & SetMask[13])
			{
				score += (KingPawnShelter[13] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[21])
			{
				score += (KingPawnShelter[21] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[29])
			{
				score += (KingPawnShelter[29] * GamePhase) / 1024;
			}
			
			if(pos->pieceBB[P][WHITE] & SetMask[14])
			{
				score += (KingPawnShelter[14] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[22])
			{
				score += (KingPawnShelter[22] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[30])
			{
				score += (KingPawnShelter[30] * GamePhase) / 1024;
			}
			
			if(pos->pieceBB[P][WHITE] & SetMask[15])
			{
				score += (KingPawnShelter[15] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[23])
			{
				score += (KingPawnShelter[23] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[31])
			{
				score += (KingPawnShelter[31] * GamePhase) / 1024;
			}		
		} else if(WhiteCastleQueenMask & e_kings)
		{
			if(pos->pieceBB[P][WHITE] & SetMask[8])
			{
				score += (KingPawnShelter[8] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[16])
			{
				score += (KingPawnShelter[16] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[24])
			{
				score += (KingPawnShelter[24] * GamePhase) / 1024;
			}
			
			if(pos->pieceBB[P][WHITE] & SetMask[9])
			{
				score += (KingPawnShelter[9] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[17])
			{
				score += (KingPawnShelter[17] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[25])
			{
				score += (KingPawnShelter[25] * GamePhase) / 1024;
			}
			
			if(pos->pieceBB[P][WHITE] & SetMask[10])
			{
				score += (KingPawnShelter[10] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[18])
			{
				score += (KingPawnShelter[18] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[26])
			{
				score += (KingPawnShelter[26] * GamePhase) / 1024;
			}	
		} else
		{

			if(pos->pieceBB[P][WHITE] & SetMask[11])
			{
				score += (KingPawnShelter[11] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[19])
			{
				score += (KingPawnShelter[19] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[27])
			{
				score += (KingPawnShelter[27] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[35])
			{
				score += (KingPawnShelter[35] * GamePhase) / 1024;
			}
			
			if(pos->pieceBB[P][WHITE] & SetMask[12])
			{
				score += (KingPawnShelter[12] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[20])
			{
				score += (KingPawnShelter[20] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[28])
			{
				score += (KingPawnShelter[28] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][WHITE] & SetMask[36])
			{
				score += (KingPawnShelter[36] * GamePhase) / 1024;
			}
		}
	}
	//*/
	
	sq = pos->KingSq[BLACK];
	e_kings = pos->pieceBB[K][BLACK];
	score -= ((KingTableOB[sq] * GamePhase) + (KingTableEB[sq] * ReverseGamePhase)) / 1024;
	
	///*
	if(GamePhase > 128)
	{
		if(BlackCastleKingMask & e_kings)
		{
			if(pos->pieceBB[P][BLACK] & SetMask[53])
			{
				score -= (KingPawnShelter[53] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[45])
			{
				score -= (KingPawnShelter[45] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[37])
			{
				score -= (KingPawnShelter[37] * GamePhase) / 1024;
			}
			
			if(pos->pieceBB[P][BLACK] & SetMask[54])
			{
				score -= (KingPawnShelter[54] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[46])
			{
				score -= (KingPawnShelter[46] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[38])
			{
				score -= (KingPawnShelter[38] * GamePhase) / 1024;
			}
			
			if(pos->pieceBB[P][BLACK] & SetMask[55])
			{
				score -= (KingPawnShelter[55] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[47])
			{
				score -= (KingPawnShelter[47] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[39])
			{
				score -= (KingPawnShelter[39] * GamePhase) / 1024;
			}
		} else if(BlackCastleQueenMask & e_kings)
		{
			if(pos->pieceBB[P][BLACK] & SetMask[48])
			{
				score -= (KingPawnShelter[48] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[40])
			{
				score -= (KingPawnShelter[40] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[32])
			{
				score -= (KingPawnShelter[32] * GamePhase) / 1024;
			}
			
			if(pos->pieceBB[P][BLACK] & SetMask[49])
			{
				score -= (KingPawnShelter[49] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[41])
			{
				score -= (KingPawnShelter[41] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[33])
			{
				score -= (KingPawnShelter[33] * GamePhase) / 1024;
			}
			
			if(pos->pieceBB[P][BLACK] & SetMask[50])
			{
				score -= (KingPawnShelter[50] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[42])
			{
				score -= (KingPawnShelter[42] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[34])
			{
				score -= (KingPawnShelter[34] * GamePhase) / 1024;
			}
		} else
		{
			if(pos->pieceBB[P][BLACK] & SetMask[51])
			{
				score -= (KingPawnShelter[51] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[43])
			{
				score -= (KingPawnShelter[43] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[35])
			{
				score -= (KingPawnShelter[35] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[27])
			{
				score -= (KingPawnShelter[27] * GamePhase) / 1024;
			}

			
			if(pos->pieceBB[P][BLACK] & SetMask[52])
			{
				score -= (KingPawnShelter[52] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[44])
			{
				score -= (KingPawnShelter[44] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[36])
			{
				score -= (KingPawnShelter[36] * GamePhase) / 1024;
			} else if(pos->pieceBB[P][BLACK] & SetMask[28])
			{
				score -= (KingPawnShelter[28] * GamePhase) / 1024;
			}
		}
	}
	//*/
	
	if(pos->pceNum[wB] == 2)
	{
		score += BishopPair;
	}
	
	if(pos->pceNum[bB] == 2)
	{
		score -= BishopPair;
	}	
	
	if(WhiteAttack > BlackDefense)
	{
		score += (((WhiteAttack - BlackDefense) * PieceAttack) * GamePhase) / 1024;
	}
	
	if(BlackAttack > WhiteDefense)
	{
		score -= (((BlackAttack - WhiteDefense) * PieceAttack) * GamePhase) / 1024;
	}
	
	
	if(pos->side == WHITE) {
		return score;
	} else {
		return -score;
	}	
} 