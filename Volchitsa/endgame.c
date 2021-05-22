#include "stdio.h"
#include "defs.h"

const int PushToCorners[64] = {
	2000, 1900, 1800, 1700, 1600, 1500, 1400, 1300,
	1900, 1800, 1700, 1600, 1500, 1400, 1300, 1400,
	1800, 1700, 1550, 1400, 1400, 1250, 1400, 1500,
	1700, 1600, 1400, 1200, 1100, 1400, 1500, 1600,
	1600, 1500, 1400, 1100, 1200, 1400, 1600, 1700,
	1500, 1400, 1250, 1400, 1400, 1550, 1700, 1800,
	1400, 1300, 1400, 1500, 1600, 1700, 1800, 1900,
	1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000
};


S_EndgameEval EvalEndgame(const S_BOARD* pos)
{
	S_EndgameEval returnValue = { 0, 0 };

	if (!pos->pceNum[wP] && !pos->pceNum[bP]) {	//no pieceBB[P]

		if (!pos->pceNum[wR] && !pos->pceNum[bR] && !pos->pceNum[wQ] && !pos->pceNum[bQ]) 	// no major pieces
		{
			if (!pos->pceNum[bB] && !pos->pceNum[wB]) 		//no pieceBB[B]
			{
				if (pos->pceNum[wN] < 3 && pos->pceNum[bN] < 3) {
					if (pos->side == WHITE)
					{
						return  (S_EndgameEval) { Contempt, 1 };
					}
					else
					{
						return (S_EndgameEval) { -Contempt, 1 };
					}
				}
			}
			else if (!pos->pceNum[wN] && !pos->pceNum[bN]) 	//No pieceBB[N]
			{
				if (abs(pos->pceNum[wB] - pos->pceNum[bB]) < 2)
				{
					if (pos->side == WHITE)
					{
						return  (S_EndgameEval) { Contempt, 1 };
					}
					else
					{
						return (S_EndgameEval) { -Contempt, 1 };
					}
				}
			}
			else if ((pos->pceNum[wN] < 3 && !pos->pceNum[wB]) || (pos->pceNum[wB] == 1 && !pos->pceNum[wN])) //white has less than 3 pieceBB[N] or a lone bishop
			{
				if ((pos->pceNum[bN] < 3 && !pos->pceNum[bB]) || (pos->pceNum[bB] == 1 && !pos->pceNum[bN]))
				{
					if (pos->side == WHITE) {
						return  (S_EndgameEval) { Contempt, 1 };
					}
					else {
						return (S_EndgameEval) { -Contempt, 1 };
					}
				} //black has less than 3 pieceBB[N] or a lone bishop
				else if (pos->pceNum[wB] == 0 && pos->pceNum[wN] == 0 && pos->pceNum[bB] == 1 && pos->pceNum[bN] == 1) // black NB
				{
					return (S_EndgameEval) {
						-PushToCorners[pos->KingSq[WHITE]], 1
					};
				}
			}
			else if (pos->pceNum[wB] == 1 && pos->pceNum[wN] == 1 && pos->pceNum[bB] == 0 && pos->pceNum[bN] == 0) // white NB
			{
				return (S_EndgameEval) { PushToCorners[pos->KingSq[BLACK]], 1 };
			}
		}
		else if (!pos->pceNum[wQ] && !pos->pceNum[bQ])
		{
			if (pos->pceNum[wR] == 1 && pos->pceNum[bR] == 1)
			{
				if ((pos->pceNum[wN] + pos->pceNum[wB]) < 2 && (pos->pceNum[bN] + pos->pceNum[bB]) < 2) {
					if (pos->side == WHITE) {
						return  (S_EndgameEval) { Contempt, 1 };
					}
					else {
						return (S_EndgameEval) { -Contempt, 1 };
					}
				}
			}
			else if (pos->pceNum[wR] == 1 && !pos->pceNum[bR])
			{
				if ((pos->pceNum[wN] + pos->pceNum[wB] == 0) && (((pos->pceNum[bN] + pos->pceNum[bB]) == 1) || ((pos->pceNum[bN] + pos->pceNum[bB]) == 2))) {
					if (pos->side == WHITE) {
						return  (S_EndgameEval) { Contempt, 1 };
					}
					else {
						return (S_EndgameEval) { -Contempt, 1 };
					}
				}
			}
			else if (pos->pceNum[bR] == 1 && !pos->pceNum[wR])
			{
				if ((pos->pceNum[bN] + pos->pceNum[bB] == 0) && (((pos->pceNum[wN] + pos->pceNum[wB]) == 1) || ((pos->pceNum[wN] + pos->pceNum[wB]) == 2))) {
					if (pos->side == WHITE) {
						return  (S_EndgameEval) { Contempt, 1 };
					}
					else {
						return (S_EndgameEval) { -Contempt, 1 };
					}
				}
			}
		}
	}
	/*else if(!pos->pceNum[bP] && pos->pceNum[wP] == 1) //white has a single pawn
	{

	} else if(!pos->pceNum[wP] && pos->pceNum[bP] == 1) //black has a single pawn
	{

	}*/

	return (S_EndgameEval) { 0, 0 };
}