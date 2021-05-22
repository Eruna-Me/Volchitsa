#include "defs.h"

U64 GeneratePosKey(const S_BOARD *pos)
{
	int sq = 0;
	U64 finalKey = 0;
	int piece = EMPTY;
	
	for(sq = 0; sq < 64; ++sq)
	{
		piece = pos->pieces[sq];
		if(piece =! EMPTY)
		{
			finalKey ^= PieceKeys[piece][sq];
		}
	}
	
	if (pos->side == WHITE)
	{
		finalKey ^= SideKey;
	}
	
	if (pos->enPas != 65)
	{
		finalKey ^= PieceKeys[EMPTY][pos->enPas];
	}
	
	finalKey ^= CastleKeys[pos->castlePerm];
	
	return finalKey;
}