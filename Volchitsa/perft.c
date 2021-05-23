#include "defs.h"
#include "stdio.h"

long leafNodes;

void Perft(int depth, S_BOARD *pos) {

    //ASSERT(CheckBoard(pos));  

	if(depth == 0) {
        leafNodes++;
        return;
    }	

    S_MOVELIST list[1];
    GenerateAllMoves(pos,list);
      
    int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {	
       
        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }
		int move = list->moves[MoveNum].move;
		//printf("\nMADE:%s\n", PrMove(move));
        Perft(depth - 1, pos);
        TakeMove(pos);
    }

    return;
}


long PerftTest(int depth, S_BOARD *pos, int print) {

    ASSERT(CheckBoard(pos));

    if (print) PrintBoard(pos);
    if (print) printf("\nStarting Test To Depth:%d\n",depth);
	leafNodes = 0;
	int start = GetTimeMs();
    S_MOVELIST list[1];
    GenerateAllMoves(pos,list);	
    
    int move;	    
    int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        move = list->moves[MoveNum].move;
        if ( !MakeMove(pos,move))  {
            continue;
        }
        long cumnodes = leafNodes;
        Perft(depth - 1, pos);
        TakeMove(pos);        
        long oldnodes = leafNodes - cumnodes;

        if(print) printf("move %d : %s : %ld\n",MoveNum+1,PrMove(move),oldnodes);
    }
	
    if (print) printf("\nTest Complete : %ld nodes visited in %dms\n",leafNodes, GetTimeMs()- start);

    return leafNodes;
}
