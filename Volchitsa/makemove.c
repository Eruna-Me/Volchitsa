#include "defs.h"
#include "stdio.h"

#define HASH_PCE(pce,sq) (pos->posKey ^= (PieceKeys[(pce)][(sq)]))
#define HASH_CA (pos->posKey ^= (CastleKeys[(pos->castlePerm)]))
#define HASH_SIDE (pos->posKey ^= (SideKey))
#define HASH_EP (pos->posKey ^= (PieceKeys[EMPTY][(pos->enPas)]))

const int CastlePerm[64] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11,
};

static void ClearPiece(const int sq, S_BOARD *pos) {
	
    int pce = pos->pieces[sq];
	int col = PieceCol[pce];
	
    HASH_PCE(pce,sq);
	
	pos->pieces[sq] = EMPTY;
    pos->material[col] -= PieceVal[pce];
	
	if (col == WHITE) {
		(pos->pieceBB[pce][WHITE]) &= ClearMask[(sq)];
		(pos->pieceBB[pce][BOTH]) &= ClearMask[(sq)];
		(pos->pieceBB[ALL][BOTH]) &= ClearMask[(sq)];
		(pos->pieceBB[ALL][WHITE]) &= ClearMask[(sq)];
	} else {
		(pos->pieceBB[pce-6][BLACK]) &= ClearMask[(sq)];
		(pos->pieceBB[pce-6][BOTH]) &= ClearMask[(sq)];
		(pos->pieceBB[ALL][BOTH]) &= ClearMask[(sq)];
		(pos->pieceBB[ALL][BLACK]) &= ClearMask[(sq)];
	}	
	if(PieceMaj[pce]) {
		pos->majPce[col]--;	
		pos->bigPce[col]--;
	}		
	if(PieceMin[pce]) {
		pos->minPce[col]--;
		pos->bigPce[col]--;
	} 
	
	pos->pceNum[pce]--;	 
}

static void AddPiece(const int sq, S_BOARD *pos, const int pce) {

	int col = PieceCol[pce];
	
	HASH_PCE(pce,sq);
	
    pos->pieces[sq] = pce;
    pos->material[col] += PieceVal[pce];
	
	if (col == WHITE) {
		(pos->pieceBB[pce][WHITE]) |= SetMask[(sq)];
		(pos->pieceBB[pce][BOTH]) |= SetMask[(sq)];
		(pos->pieceBB[ALL][BOTH]) |= SetMask[(sq)];
		(pos->pieceBB[ALL][WHITE]) |= SetMask[(sq)];
	} else {
		(pos->pieceBB[pce-6][BLACK]) |= SetMask[(sq)];
		(pos->pieceBB[pce-6][BOTH]) |= SetMask[(sq)];
		(pos->pieceBB[ALL][BOTH]) |= SetMask[(sq)];
		(pos->pieceBB[ALL][BLACK]) |= SetMask[(sq)];
	}	
	if(PieceMaj[pce]) {
		pos->majPce[col]++;	
		pos->bigPce[col]++;
	}		
	if(PieceMin[pce]) {
		pos->minPce[col]++;
		pos->bigPce[col]++;
	}
	
	pos->pceNum[pce]++;	  
}

static void MovePiece(const int from, const int to, S_BOARD *pos) {

	int index = 0;
	int pce = pos->pieces[from];	
	int col = PieceCol[pce];

	HASH_PCE(pce,from);
	pos->pieces[from] = EMPTY;
	
	HASH_PCE(pce,to);
	pos->pieces[to] = pce;  

	if (col == WHITE) {
		(pos->pieceBB[pce][WHITE]) |= SetMask[(to)];
		(pos->pieceBB[pce][BOTH]) |= SetMask[(to)];
		(pos->pieceBB[ALL][BOTH]) |= SetMask[(to)];
		(pos->pieceBB[ALL][WHITE]) |= SetMask[(to)];
		(pos->pieceBB[pce][WHITE]) &= ClearMask[(from)];
		(pos->pieceBB[pce][BOTH]) &= ClearMask[(from)];
		(pos->pieceBB[ALL][BOTH]) &= ClearMask[(from)];
		(pos->pieceBB[ALL][WHITE]) &= ClearMask[(from)];
	} else {
		(pos->pieceBB[pce-6][BLACK]) |= SetMask[(to)];
		(pos->pieceBB[pce-6][BOTH]) |= SetMask[(to)];
		(pos->pieceBB[ALL][BOTH]) |= SetMask[(to)];
		(pos->pieceBB[ALL][BLACK]) |= SetMask[(to)];
		(pos->pieceBB[pce-6][BLACK]) &= ClearMask[(from)];
		(pos->pieceBB[pce-6][BOTH]) &= ClearMask[(from)];
		(pos->pieceBB[ALL][BOTH]) &= ClearMask[(from)];
		(pos->pieceBB[ALL][BLACK]) &= ClearMask[(from)];
	}		
}

int MakeMove(S_BOARD *pos, int move) {

	int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos->side;
	
	pos->history[pos->hisPly].posKey = pos->posKey;
	
	if(move & MFLAGEP) {
        if(side == WHITE) {
            ClearPiece(to-8,pos);
        } else {
            ClearPiece(to+8,pos);
        }
    } else if (move & MFLAGCA) {
        switch(to) {
            case 2:
                MovePiece(0, 3, pos);
			break;
            case 58:
                MovePiece(56, 59, pos);
			break;
            case 6:
                MovePiece(7, 5, pos);
			break;
            case 62:
                MovePiece(63, 61, pos);
			break;
            default:  break;
        }
    }	
	
	if(pos->enPas != 65) HASH_EP;
    HASH_CA;
	
	pos->history[pos->hisPly].move = move;
    pos->history[pos->hisPly].fiftyMove = pos->fiftyMove;
    pos->history[pos->hisPly].enPas = pos->enPas;
    pos->history[pos->hisPly].castlePerm = pos->castlePerm;

    pos->castlePerm &= CastlePerm[from];
    pos->castlePerm &= CastlePerm[to];
    pos->enPas = 65;

	HASH_CA;
	
	int captured = CAPTURED(move);
    pos->fiftyMove++;
	
	if(captured != EMPTY) {
        ClearPiece(to, pos);
        pos->fiftyMove = 0;
    }
	
	pos->hisPly++;
	pos->ply++;
	
	if(PiecePawn[pos->pieces[from]]) {
        pos->fiftyMove = 0;
        if(move & MFLAGPS) {
            if(side==WHITE) {
                pos->enPas=from+8;
            } else {
                pos->enPas=from-8;
            }
            HASH_EP;
        }
    }
	
	MovePiece(from, to, pos);
	
	int prPce = PROMOTED(move);
    if(prPce != EMPTY)   {
        ClearPiece(to, pos);
        AddPiece(to, pos, prPce);
    }
	if(PieceKing[pos->pieces[to]]) {
        pos->KingSq[pos->side] = to;
    }
	
	pos->side ^= 1;
    HASH_SIDE;
		
	if(SqAttacked(pos->KingSq[side],pos->side,pos))  {
        TakeMove(pos);
        return 0;
    }
	return 1;
}

void TakeMove(S_BOARD *pos) {
	
	pos->hisPly--;
    pos->ply--;
	
    int move = pos->history[pos->hisPly].move;
    int from = FROMSQ(move);
    int to = TOSQ(move);	
	
	if(pos->enPas != 65) HASH_EP;
    HASH_CA;

    pos->castlePerm = pos->history[pos->hisPly].castlePerm;
    pos->fiftyMove = pos->history[pos->hisPly].fiftyMove;
    pos->enPas = pos->history[pos->hisPly].enPas;

    if(pos->enPas != 65) HASH_EP;
    HASH_CA;

    pos->side ^= 1;
    HASH_SIDE;
	
	if(MFLAGEP & move) {
        if(pos->side == WHITE) {
            AddPiece(to-8, pos, bP);
        } else {
            AddPiece(to+8, pos, wP);
        }
    } else if(MFLAGCA & move) {
        switch(to) {
            case 2: MovePiece(3, 0, pos); break;
            case 58: MovePiece(59, 56, pos); break;
            case 6: MovePiece(5, 7, pos); break;
            case 62: MovePiece(61, 63, pos); break;
            default: break;
        }
    }
	
	MovePiece(to, from, pos);
	
	if(PieceKing[pos->pieces[from]]) {
        pos->KingSq[pos->side] = from;
    }
	int captured = CAPTURED(move);
    if(captured != EMPTY) {
        AddPiece(to, pos, captured);
    }	
	if(PROMOTED(move) != EMPTY)   {
        ClearPiece(from, pos);
        AddPiece(from, pos, (PieceCol[PROMOTED(move)] == WHITE ? wP : bP));
    }
}

void MakeNullMove(S_BOARD *pos) {

    pos->ply++;
    pos->history[pos->hisPly].posKey = pos->posKey;

    if(pos->enPas != 65) HASH_EP;

    pos->history[pos->hisPly].move = NOMOVE;
    pos->history[pos->hisPly].fiftyMove = pos->fiftyMove;
    pos->history[pos->hisPly].enPas = pos->enPas;
    pos->history[pos->hisPly].castlePerm = pos->castlePerm;
    pos->enPas = 65;

    pos->side ^= 1;
    pos->hisPly++;
    HASH_SIDE;
} 

void TakeNullMove(S_BOARD *pos) {

    pos->hisPly--;
    pos->ply--;

    if(pos->enPas != 65) HASH_EP;

    pos->castlePerm = pos->history[pos->hisPly].castlePerm;
    pos->fiftyMove = pos->history[pos->hisPly].fiftyMove;
    pos->enPas = pos->history[pos->hisPly].enPas;

    if(pos->enPas != 65) HASH_EP;
    pos->side ^= 1;
    HASH_SIDE;
}
