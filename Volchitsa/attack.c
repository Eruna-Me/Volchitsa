#include "defs.h"


//const int BiDir[4] = { -7, -9, 7, 9 };


int SqAttacked(const int sq, const int side, const S_BOARD *pos)
{
	int pce, i, t_sq, dir;
	
	if(side == WHITE && WhitePawnAttack[sq] & pos->pieceBB[P][WHITE])
	{
			return 1;
	}
	if(side == BLACK && BlackPawnAttack[sq] & pos->pieceBB[P][BLACK])
	{
			return 1;
	}
		
	if(KnightAttack[sq] & pos->pieceBB[N][side])
		{
			return 1;
		}
	if(KingAttack[sq] & pos->pieceBB[K][side])
		{
			return 1;
		}
		
	U64 TempBoard = QueenPseudo[sq] & pos->pieceBB[Q][side];
	TempBoard |= StraightPseudo[sq] & pos->pieceBB[R][side];
	TempBoard |= DiagionalPseudo[sq] & pos->pieceBB[B][side];
	
	while(TempBoard)
	{
		int DF = POP(&TempBoard);
		if (!(SliderObstructionCheck[DF][sq] & pos->pieceBB[ALL][BOTH]))
		{
			return 1;
		}
	}
	
	return 0;
	
}
	
