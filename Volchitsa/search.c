#include "stdio.h"
#include "defs.h"


const int SafetyMargin[2] = { 600, -600 };
int rootDepth;

static void CheckUp(S_SEARCHINFO *info) {
	// .. check if time up, or interrupt from GUI
	if(info->timeset == 1 && GetTimeMs() > info->stoptime) {
		info->stopped = 1;
	}
	
	ReadInput(info);
}

static void PickNextMove(int moveNum, S_MOVELIST *list) {

	S_MOVE temp;
	int index = 0;
	int bestScore = 0; 
	int bestNum = moveNum;
	
	for (index = moveNum; index < list->count; ++index) {
		if (list->moves[index].score > bestScore) {
			bestScore = list->moves[index].score;
			bestNum = index;
		}
	}
	temp = list->moves[moveNum];
	list->moves[moveNum] = list->moves[bestNum];
	list->moves[bestNum] = temp;
}

static int IsRepetition(const S_BOARD *pos) {
	int index = 0;

	for(index = pos->hisPly - pos->fiftyMove; index < pos->hisPly-1; ++index) {
		if(pos->posKey == pos->history[index].posKey) {
			return 1;
		}
	}
	
	return 0;
}

static void ClearForSearch(S_BOARD *pos, S_SEARCHINFO *info) 
{
	int i = 0;
	int j = 0;
	
	for(i = 0; i < 13; ++i) {
		for(j = 0; j < 64; ++j) {
			pos->SearchHistory[i][j] = 0;
		}
	}
	
	for(i = 0; i < 2; ++i) {
		for(j = 0; j < MAXDEPTH; ++j) {
			pos->SearchKillers[i][j] = 0;
		}
	}	
	
	pos->HashTable->overWrite = 0;	
	pos->HashTable->hit = 0;	
	pos->HashTable->cut = 0;		
	pos->ply = 0;
	
	//info->starttime = GetTimeMs();
	info->stopped = 0;
	info->nodes = 0;
	//info->fh = 0;
	//info->fhf = 0;
	
}

static int Quiescence(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info)
{
	//ASSERT(CheckBoard(pos));
	
	if(( info->nodes & 2047 ) == 0) {
		CheckUp(info);
	}
	info->nodes++; //such lies
	
	if((IsRepetition(pos) || pos->fiftyMove >= 100) &&  pos->ply) {
		return 0;
	}
	
	if(pos->ply > MAXDEPTH - 1) {
		return EvalPosition(pos);
	}
	
	int Score = EvalPosition(pos);
	
	if(Score >= beta) {
		return beta;
	}
	
	if(Score > alpha) {
		alpha = Score;
	}
	
	S_MOVELIST list[1];
    GenerateAllCaps(pos,list);
      
    int MoveNum = 0;
	int Legal = 0;
	int OldAlpha = alpha;
	int BestMove = 0;
	Score = -INFINITE;
	int PvMove = ProbePvMove(pos);
	
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {	
			
		PickNextMove(MoveNum, list);	
		
        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }
        
		Legal++;
		Score = -Quiescence( -beta, -alpha, pos, info);		
        TakeMove(pos);
		
		if(Score > alpha) {
			if(Score >= beta) {
				if(Legal==1) {
					//info->fhf++;
				}
				//info->fh++;	
				return beta;
			}
			alpha = Score;
			BestMove = list->moves[MoveNum].move;
		}		
    }
	return alpha;
	
}

static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int DoNull)
{
	//ASSERT(CheckBoard(pos)); 
	
	if(depth == 0) {
		return Quiescence(alpha, beta, pos, info);
		// return EvalPosition(pos);
	}
	
	if(( info->nodes & 2047 ) == 0) {
		CheckUp(info);
	}
	info->nodes++;
	
	if(IsRepetition(pos) || pos->fiftyMove >= 100) {
		if(pos->side == WHITE) {
			return Contempt;
		} else {
			return -Contempt;
		}
	}
	
	if(pos->ply > MAXDEPTH - 1) {
		return EvalPosition(pos);
	}
	
	//if pawn on 7th rank, depth++!
	
	int InCheck = SqAttacked(pos->KingSq[pos->side],pos->side^1,pos);
	
	if(InCheck == 1) {
		depth++;
	}
	
	int Score = -INFINITE;
	int PvMove = NOMOVE;
	
	if( ProbeHashEntry(pos, &PvMove, &Score, alpha, beta, depth) == 1)
	{
		pos->HashTable->cut++;
		return Score;
	}
	
	if( DoNull && !InCheck && pos->ply && (pos->bigPce[pos->side] > 1) && depth >= 4) {
		MakeNullMove(pos);
		Score = -AlphaBeta( -beta, -beta + 1, depth-4, pos, info, 0);
		TakeNullMove(pos);
		if(info->stopped == 1) {
			return 0;
		}
		if (Score >= beta) {		 
		  return beta;
		}	
	}
	
	S_MOVELIST list[1];
    GenerateAllMoves(pos,list);
      
    int MoveNum = 0;
	int Legal = 0;
	int OldAlpha = alpha;
	int BestMove = 0;
	Score = -INFINITE;
	int BestScore = -INFINITE;
	
	
	if (PvMove != 0 )
	{
		for(MoveNum = 0; MoveNum < list->count; ++MoveNum)
		{
			if( list->moves[MoveNum].move == PvMove )
			{
				list->moves[MoveNum].score = 2000000;
			}
		}
	}
	
	int FoundPv = 0;
	
	int moves_searched = 0;
	
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {//	
       
		PickNextMove(MoveNum, list);	
		
		
		
        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }
        
		Legal++;
		
		int InCheckS = SqAttacked(pos->KingSq[pos->side],pos->side^1,pos);
		
		if(moves_searched == 0) // First move, use full-window search
		{
			Score = -AlphaBeta( -beta, -alpha, depth-1, pos, info, 1 );
		}
		else if(FoundPv == 1)
		{
			Score = -AlphaBeta ( -alpha -1, -alpha, depth-1, pos, info, 1 );
			if(Score > alpha && Score < beta)
			{
				Score = -AlphaBeta( -beta, -alpha, depth-1, pos, info, 1 );
			}
		}  else {
			
			if(	MoveNum 									>= 4 	&&		//at this node
				depth										>= 3    &&		//depth?
				pos->bigPce[pos->side] 						>= 2	&&		//not a endgame
				InCheckS									== 0	&&		//doesn't put the enemy in check
				InCheck										== 0	&&		//when not in check
				list->moves[MoveNum].score					<  700000	)
			{
					Score = -AlphaBeta( -alpha -1, -alpha, depth-2, pos, info, 1);
			}
		else
			Score = -AlphaBeta( -beta, -alpha, depth-1, pos, info, 1);	
		}
		
        TakeMove(pos);
		
		moves_searched++;
		
		
		if(info->stopped == 1) {
			return 0;
		}		
		
		
		
		if(Score > BestScore)
		{
			BestScore = Score;
			BestMove = list->moves[MoveNum].move;
		if(Score > alpha) {
			if(Score >= beta) {
				if(Legal==1) {
					//info->fhf++;
				}
				//info->fh++;		

				if(!(list->moves[MoveNum].move & MFLAGCAP))
				{
					pos->SearchKillers[1][pos->ply] = pos->SearchKillers[0][pos->ply];
					pos->SearchKillers[0][pos->ply] = list->moves[MoveNum].move;
				}
				
				StoreHashEntry(pos, BestMove, beta, HFBETA, depth);
				
				return beta;
			}
			FoundPv = 1;
			alpha = Score;
			if(!(list->moves[MoveNum].move & MFLAGCAP))
				{
					pos->SearchHistory[pos->pieces[FROMSQ(BestMove)]][TOSQ(BestMove)] += depth * depth;
				}
			}	
		}
	
				
    }//
	
	if(Legal == 0) {
		if(SqAttacked(pos->KingSq[pos->side],pos->side^1,pos)) {
			return -INFINITE + pos->ply;
		} else {
			return 0;
		}
	}
	
	if(alpha != OldAlpha) {
		StoreHashEntry(pos, BestMove, BestScore, HFEXACT, depth);
	} else {
		StoreHashEntry(pos, BestMove, alpha, HFALPHA, depth);
	}
	
	return alpha;
}

void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info) 
{
	int bestMove = 0;
	int bestScore = -INFINITE;
	int currentDepth = 0;
	int pvMoves = 0;
	int pvNum = 0;
	
	ClearForSearch(pos,info);
	
	// iterative deepening
	for( currentDepth = 1; currentDepth <= info->depth; ++currentDepth ) {
							// alpha	 beta
		bestScore = AlphaBeta(-INFINITE, INFINITE, currentDepth, pos, info, 1);
		
		if(info->stopped == 1) {
			break;
		}
		
		if(currentDepth > MaxDepth) {
			break;
		}
		
		pvMoves = GetPvLine(currentDepth, pos);
		bestMove = pos->PvArray[0];
		
		printf("info score cp %d depth %d nodes %ld time %d ",
			bestScore,currentDepth,info->nodes,GetTimeMs()-info->starttime);
			
		pvMoves = GetPvLine(currentDepth, pos);	
		printf("pv");		
		for(pvNum = 0; pvNum < pvMoves; ++pvNum) {
			printf(" %s",PrMove(pos->PvArray[pvNum]));
		}
		printf("\n");
		//printf("Ordering:%.2f\n",(info->fhf/info->fh));
	}
	printf("bestmove %s\n",PrMove(bestMove));

}