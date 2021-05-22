#include "stdio.h"
#include "defs.h"

#define MOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)]==OFFBOARD)

const int mRkDir[4] = { -1, -8,	1, 8 };
const int mBiDir[4] = { -7, -9, 7, 9 };
const int VictimScore[13] = { 0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600 };
static int MvvLvaScores[13][13];

void InitMvvLva() {
	int Attacker;
	int Victim;
	for(Attacker = wP; Attacker <= bK; ++Attacker) {
		for(Victim = wP; Victim <= bK; ++Victim) {
			MvvLvaScores[Victim][Attacker] = VictimScore[Victim] + 6 - ( VictimScore[Attacker] / 100);
		}
	}		
}

int MoveExists(S_BOARD *pos, const int move) {
	
	S_MOVELIST list[1];
    GenerateAllMoves(pos,list);
      
    int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {	
       
        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }        
        TakeMove(pos);
		if(list->moves[MoveNum].move == move) {
			return 2;
		}
    }
	return 0;
}

static void AddQuietMove( const S_BOARD *pos, int move, S_MOVELIST *list )
{

	list->moves[list->count].move = move;
	
	if(pos->SearchKillers[0][pos->ply] == move) {	
		list->moves[list->count].score = 900000;
	} else if(pos->SearchKillers[1][pos->ply] == move) {	
		list->moves[list->count].score = 800000;
	} else {	
		list->moves[list->count].score = pos->SearchHistory[pos->pieces[FROMSQ(move)]][TOSQ(move)];
	}
	list->count++;
}

static void AddQuietPromotionMove( const S_BOARD *pos, int move, S_MOVELIST *list )
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 1000000;
	list->count++;
}

static void AddUnderPromotionMove( const S_BOARD *pos, int move, S_MOVELIST *list )
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = -10;
	list->count++;
}

static void AddCaptureMove( const S_BOARD *pos, int move, S_MOVELIST *list )
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = MvvLvaScores[CAPTURED(move)][pos->pieces[FROMSQ(move)]] + 1000000;
	list->count++;
}

static void AddEnPassantMove( const S_BOARD *pos, int move, S_MOVELIST *list )
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 1000105;
	list->count++;
}

static void AddWhitePawnCapMove( const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list ) {
	if(RanksBrd[from] == RANK_7) {
		AddCaptureMove(pos, MOVE(from,to,cap,wQ,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,cap,wR,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,cap,wB,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,cap,wN,0), list);
	} else {
		AddCaptureMove(pos, MOVE(from,to,cap,EMPTY,0), list);
	}
}

static void AddWhitePawnMove( const S_BOARD *pos, const int from, const int to, S_MOVELIST *list ) {
	if(RanksBrd[from] == RANK_7) {
		AddQuietPromotionMove(pos, MOVE(from,to,EMPTY,wQ,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,EMPTY,wR,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,EMPTY,wB,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,EMPTY,wN,0), list);
	} else {
		AddQuietMove(pos, MOVE(from,to,EMPTY,EMPTY,0), list);
	}
}

static void AddBlackPawnCapMove( const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list ) {
	if(RanksBrd[from] == RANK_2) {
		AddCaptureMove(pos, MOVE(from,to,cap,bQ,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,cap,bR,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,cap,bB,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,cap,bN,0), list);
	} else {
		AddCaptureMove(pos, MOVE(from,to,cap,EMPTY,0), list);
	}
}

static void AddBlackPawnMove( const S_BOARD *pos, const int from, const int to, S_MOVELIST *list ) {
	if(RanksBrd[from] == RANK_2) {
		AddQuietPromotionMove(pos, MOVE(from,to,EMPTY,bQ,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,EMPTY,bR,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,EMPTY,bB,0), list);
		AddUnderPromotionMove(pos, MOVE(from,to,EMPTY,bN,0), list);
	} else {
		AddQuietMove(pos, MOVE(from,to,EMPTY,EMPTY,0), list);
	}
}


void GenerateAllMoves(  const S_BOARD *pos,  S_MOVELIST *list )	{
	//ASSERT(CheckBoard(pos));
	list->count = 0;	
	int pce = EMPTY;
	int side = pos->side;
	int sq = 0; 
	int t_sq = 0;
	int pceNum = 0;
	int dir = 0;
	int i = 0;
	
	if(side == WHITE) {
		U64 t_pawns = pos->pieceBB[P][WHITE];
		U64 t_kings = pos->pieceBB[K][WHITE];
		U64 t_knights = pos->pieceBB[N][WHITE];
		U64 t_rooks = pos->pieceBB[R][WHITE];
		U64 t_queens = pos->pieceBB[Q][WHITE];
		U64 t_bishops = pos->pieceBB[B][WHITE];
		// pieceBB[P] !!!
		while(t_pawns) {
			sq = POP(&t_pawns);
			if((NortAttack[sq] & pos->pieceBB[ALL][BOTH]) == 0)	{
				AddWhitePawnMove(pos, sq, sq+8, list);
				if(RanksBrd[sq] == RANK_2 && (NortAttack[sq+8] & pos->pieceBB[ALL][BOTH]) == 0) {
					AddQuietMove(pos, MOVE(sq,(sq+16),EMPTY,EMPTY,MFLAGPS),list);
				}
			} 
			if(NoWeAttack[sq] & pos->pieceBB[ALL][BLACK]) {
				AddWhitePawnCapMove(pos, sq, sq+7, pos->pieces[sq + 7], list);
			}  
			if(NoEaAttack[sq] & pos->pieceBB[ALL][BLACK]) {
				AddWhitePawnCapMove(pos, sq, sq+9, pos->pieces[sq + 9], list);
			} 							
		}
		if(pos->enPas < 65)	{
			sq = pos->enPas;
			U64 t_enPas = (WhitePawnAttack[sq] & pos->pieceBB[P][WHITE]);
			while(t_enPas)	{
				t_sq = POP(&t_enPas);
				AddEnPassantMove(pos, MOVE(t_sq,sq,EMPTY,EMPTY,MFLAGEP), list);				
			}
		}
		// QUIET KING MOVES !!!
		sq = POP(&t_kings);
		U64 quietKingM = (KingAttack[sq] & ~pos->pieceBB[ALL][BOTH]);
		while(quietKingM)	{
			t_sq = POP(&quietKingM);
			AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);	
		}
		// CAP KING MOVES
		U64 capKingM = (KingAttack[sq] & pos->pieceBB[ALL][BLACK]);
		while(capKingM)	{
			t_sq = POP(&capKingM);
			AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
		}
		// KNIGHT MOVES
		while(t_knights)	{
			sq = POP(&t_knights);
			U64 quietKnightM = (KnightAttack[sq] & ~pos->pieceBB[ALL][BOTH]);
			while(quietKnightM)	{
				t_sq = POP(&quietKnightM);
				AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);	
			}
			U64 capKnightM = (KnightAttack[sq] & pos->pieceBB[ALL][BLACK]);
			while(capKnightM)	{
				t_sq = POP(&capKnightM);
				AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
			}
		}
		//	ROOK MOVES
		while(t_rooks)	{
			sq = POP(&t_rooks);
			if(FilesBrd[sq] != FILE_A)	{
				t_sq = sq-1;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						}	else	{
						break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);	
					if(FilesBrd[t_sq] == FILE_A)	{
						break;
					}
					t_sq += -1;
				}
			}
			if(RanksBrd[sq] != RANK_1)	{
				t_sq = sq-8;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
						AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
						break;
						}	else	{
						break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(RanksBrd[t_sq] == RANK_1)	{
						break;
					}
					t_sq += -8;
				}
			}
			if(FilesBrd[sq] != FILE_H)	{
				t_sq = sq+1;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						}	else{
						break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(FilesBrd[t_sq] == FILE_H)	{
						break;
					}
					t_sq += 1;
				}
			}
			if(RanksBrd[sq] != RANK_8)	{
				t_sq = sq+8;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						}	else	{
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(RanksBrd[t_sq] == RANK_8)	{
						break;
					}
					t_sq += 8;
				}
			}		
		}
		// QUEEN MOVES
		while(t_queens)
		{
			sq = POP(&t_queens);	
			if(FilesBrd[sq] != FILE_A)	{
				t_sq = sq-1;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);	
					if(FilesBrd[t_sq] == FILE_A) {
						break;
					}
					t_sq += -1;
				}
			}
			if(RanksBrd[sq] != RANK_1)	{
				t_sq = sq-8;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(RanksBrd[t_sq] == RANK_1)	{
						break;
					}
					t_sq += -8;
				}
			}
			if(FilesBrd[sq] != FILE_H)	{	
				t_sq = sq+1;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(FilesBrd[t_sq] == FILE_H)	{
						break;
					}
					t_sq += 1;
				}
			}
			if(RanksBrd[sq] != RANK_8)	{
				t_sq = sq+8;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(RanksBrd[t_sq] == RANK_8)	{
						break;
					}
					t_sq += 8;
				}
			}			
			if((RanksBrd[sq] != RANK_1)&&(FilesBrd[sq] != FILE_H))	{
				t_sq = sq-7;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])
						{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_H))	{
						break;
					}
					t_sq += -7;
				}
			}
			if((RanksBrd[sq] != RANK_1)&&(FilesBrd[sq] != FILE_A))	{
				t_sq = sq-9;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_A))	{
						break;
					}
					t_sq += -9;
				}
			}
			if((RanksBrd[sq] != RANK_8)&&(FilesBrd[sq] != FILE_A))	{
				t_sq = sq+7;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_A))	{
						break;
					}
					t_sq += 7;
				}
			}
			if((RanksBrd[sq] != RANK_8)&&(FilesBrd[sq] != FILE_H))	{
				t_sq = sq+9;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else	{
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_H))	{
						break;
					}
					t_sq += 9;
				}
			}
		}
		// BISHOP MOVES
		while(t_bishops)	{
			sq = POP(&t_bishops);
			if((RanksBrd[sq] != RANK_1)&&(FilesBrd[sq] != FILE_H))	{
				t_sq = sq-7;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_H))	{
						break;
					}
					t_sq += -7;
				}
			}
			if((RanksBrd[sq] != RANK_1)&&(FilesBrd[sq] != FILE_A))	{
				t_sq = sq-9;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_A))	{
						break;
					}
					t_sq += -9;
				}
			}
			if((RanksBrd[sq] != RANK_8)&&(FilesBrd[sq] != FILE_A))	{
				t_sq = sq+7;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_A))	{
						break;
					}
					t_sq += 7;
				}
			}
			if((RanksBrd[sq] != RANK_8)&&(FilesBrd[sq] != FILE_H))	{
				t_sq = sq+9;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][BLACK])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_H))	{
						break;
					}
					t_sq += 9;
				}
			}
		}
		// castling
		if(pos->castlePerm & WKCA) {
			if(pos->pieces[5] == EMPTY && pos->pieces[6] == EMPTY) {
				if(!SqAttacked(4,BLACK,pos) && !SqAttacked(5,BLACK,pos) ) {
					AddQuietMove(pos, MOVE(4,6,EMPTY,EMPTY,MFLAGCA),list);
				}
			}
		}
		if(pos->castlePerm & WQCA) {
			if(pos->pieces[3] == EMPTY && pos->pieces[2] == EMPTY && pos->pieces[1] == EMPTY) {
				if(!SqAttacked(4,BLACK,pos) && !SqAttacked(3,BLACK,pos) ) {
					AddQuietMove(pos, MOVE(4,2,EMPTY,EMPTY,MFLAGCA),list);
				}
			}
		}
	}	else	{
		U64 t_pawns = pos->pieceBB[P][BLACK];
		U64 t_kings = pos->pieceBB[K][BLACK];
		U64 t_knights = pos->pieceBB[N][BLACK];
		U64 t_rooks = pos->pieceBB[R][BLACK];
		U64 t_queens = pos->pieceBB[Q][BLACK];
		U64 t_bishops = pos->pieceBB[B][BLACK];
		
		// pieceBB[P] !!!
		while(t_pawns) {
			sq = POP(&t_pawns);
			if((SoutAttack[sq] & pos->pieceBB[ALL][BOTH]) == 0)	{
				AddBlackPawnMove(pos, sq, sq-8, list);
				if(RanksBrd[sq] == RANK_7 && (SoutAttack[sq-8] & pos->pieceBB[ALL][BOTH]) == 0) {
					AddQuietMove(pos, MOVE(sq,(sq-16),EMPTY,EMPTY,MFLAGPS),list);
				}
			} 
			if(SoEaAttack[sq] & pos->pieceBB[ALL][WHITE]) {
				AddBlackPawnCapMove(pos, sq, sq-7, pos->pieces[sq - 7], list);
			}  
			if(SoWeAttack[sq] & pos->pieceBB[ALL][WHITE]) {
				AddBlackPawnCapMove(pos, sq, sq-9, pos->pieces[sq - 9], list);
			} 	
		}
		
		if(pos->enPas < 65)	{
			sq = pos->enPas;
			U64 t_enPas = (BlackPawnAttack[sq] & pos->pieceBB[P][BLACK]);
			while(t_enPas)
			{
				t_sq = POP(&t_enPas);
				AddEnPassantMove(pos, MOVE(t_sq,sq,EMPTY,EMPTY,MFLAGEP), list);				
			}
		}
		// QUIET KING MOVES !!!
		sq = POP(&t_kings);
		U64 quietKingM = (KingAttack[sq] & ~pos->pieceBB[ALL][BOTH]);
		while(quietKingM)	{
			t_sq = POP(&quietKingM);
			AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);	
		}
		// CAP KING MOVES
		U64 capKingM = (KingAttack[sq] & pos->pieceBB[ALL][WHITE]);
		while(capKingM)	{
			t_sq = POP(&capKingM);
			AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
		}	
		// KNIGHT MOVES
		while(t_knights)	{
			sq = POP(&t_knights);
			U64 quietKnightM = (KnightAttack[sq] & ~pos->pieceBB[ALL][BOTH]);
			while(quietKnightM)	{
				t_sq = POP(&quietKnightM);
				AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);	
			}
			U64 capKnightM = (KnightAttack[sq] & pos->pieceBB[ALL][WHITE]);
			while(capKnightM)	{
				t_sq = POP(&capKnightM);
				AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
			}
		}
		//	ROOK MOVES
		while(t_rooks)	{
			sq = POP(&t_rooks);
			if(FilesBrd[sq] != FILE_A)	{
				t_sq = sq-1;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						}	else	{
						break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);	
					if(FilesBrd[t_sq] == FILE_A)	{
						break;
					}
					t_sq += -1;
				}
			}
			if(RanksBrd[sq] != RANK_1)	{
				t_sq = sq-8;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
						AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
						break;
						}	else	{
						break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(RanksBrd[t_sq] == RANK_1)	{
						break;
					}
					t_sq += -8;
				}
			}
			if(FilesBrd[sq] != FILE_H)	{
				t_sq = sq+1;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
						break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(FilesBrd[t_sq] == FILE_H)	{
						break;
					}
					t_sq += 1;
				}
			}
			if(RanksBrd[sq] != RANK_8)	{
				t_sq = sq+8;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						}	else	{
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(RanksBrd[t_sq] == RANK_8)	{
						break;
					}
					t_sq += 8;
				}
			}		
		}
		// QUEEN MOVES
		while(t_queens)
		{
			sq = POP(&t_queens);	
			if(FilesBrd[sq] != FILE_A)	{
				t_sq = sq-1;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);	
					if(FilesBrd[t_sq] == FILE_A) {
						break;
					}
					t_sq += -1;
				}
			}
			if(RanksBrd[sq] != RANK_1)	{
				t_sq = sq-8;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(RanksBrd[t_sq] == RANK_1)	{
						break;
					}
					t_sq += -8;
				}
			}
			if(FilesBrd[sq] != FILE_H)
			{	
				t_sq = sq+1;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(FilesBrd[t_sq] == FILE_H)	{
						break;
					}
					t_sq += 1;
				}
			}
			if(RanksBrd[sq] != RANK_8)
			{
				t_sq = sq+8;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if(RanksBrd[t_sq] == RANK_8)	{
						break;
					}
					t_sq += 8;
				}
			}			
			if((RanksBrd[sq] != RANK_1)&&(FilesBrd[sq] != FILE_H))	{
				t_sq = sq-7;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])
						{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_H))	{
						break;
					}
					t_sq += -7;
				}
			}
			if((RanksBrd[sq] != RANK_1)&&(FilesBrd[sq] != FILE_A))	{
				t_sq = sq-9;
				while(1)
				{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_A))	{
						break;
					}
					t_sq += -9;
				}
			}
			if((RanksBrd[sq] != RANK_8)&&(FilesBrd[sq] != FILE_A))	{
				t_sq = sq+7;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_A))	{
						break;
					}
					t_sq += 7;
				}
			}
			if((RanksBrd[sq] != RANK_8)&&(FilesBrd[sq] != FILE_H))	{
				t_sq = sq+9;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])
					{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_H))	{
						break;
					}
					t_sq += 9;
				}
			}
		}
		// BISHOP MOVES
		while(t_bishops)	{
			sq = POP(&t_bishops);
			if((RanksBrd[sq] != RANK_1)&&(FilesBrd[sq] != FILE_H))	{
				t_sq = sq-7;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_H))	{
						break;
					}
					t_sq += -7;
				}
			}
			if((RanksBrd[sq] != RANK_1)&&(FilesBrd[sq] != FILE_A))	{
			t_sq = sq-9;
			while(1)	{
				if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_A))	{
						break;
					}
					t_sq += -9;
				}
			}
			if((RanksBrd[sq] != RANK_8)&&(FilesBrd[sq] != FILE_A))	{
				t_sq = sq+7;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_A))	{
						break;
					}
					t_sq += 7;
				}
			}
			t_sq = sq;
			if((RanksBrd[t_sq] != RANK_8)&&(FilesBrd[t_sq] != FILE_H))	{
				t_sq += 9;
				while(1)	{
					if(SliderAttack[t_sq] & pos->pieceBB[ALL][BOTH])	{
						if(SliderAttack[t_sq] & pos->pieceBB[ALL][WHITE])	{
							AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
							break;
						} else {
							break;
						}
					}
					AddQuietMove(pos, MOVE(sq,t_sq,EMPTY,EMPTY,0),list);
					if((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_H))	{
						break;
					}
					t_sq += 9;
				}
			}
		}
		// castling
		if(pos->castlePerm & BKCA) {
			if(pos->pieces[61] == EMPTY && pos->pieces[62] == EMPTY) {
				if(!SqAttacked(60,WHITE,pos) && !SqAttacked(61,WHITE,pos) ) {
					AddQuietMove(pos, MOVE(60,62,EMPTY,EMPTY,MFLAGCA),list);
				}
			}
		}
		
		if(pos->castlePerm & BQCA) {
			if(pos->pieces[59] == EMPTY && pos->pieces[58] == EMPTY && pos->pieces[57] == EMPTY) {
				if(!SqAttacked(60,WHITE,pos) && !SqAttacked(59,WHITE,pos) ) {
					AddQuietMove(pos, MOVE(60,58,EMPTY,EMPTY,MFLAGCA),list);
				}
			}
		}
	}
}
void GenerateAllCaps(  const S_BOARD *pos,  S_MOVELIST *list )	{
	//ASSERT(CheckBoard(pos));
	list->count = 0;	
	int pce = EMPTY;
	int side = pos->side;
	int sq = 0; 
	int t_sq = 0;
	int pceNum = 0;
	int dir = 0;
	int i = 0;
	
	if(side == WHITE) {
		U64 t_pawns = pos->pieceBB[P][WHITE];
		U64 t_kings = pos->pieceBB[K][WHITE];
		U64 t_knights = pos->pieceBB[N][WHITE];
		U64 t_rooks = pos->pieceBB[R][WHITE];
		U64 t_queens = pos->pieceBB[Q][WHITE];
		U64 t_bishops = pos->pieceBB[B][WHITE];
		// pieceBB[P] !!!
		while(t_pawns) {		
			sq = POP(&t_pawns);
			if(NoWeAttack[sq] & pos->pieceBB[ALL][BLACK]) {
				AddWhitePawnCapMove(pos, sq, sq+7, pos->pieces[sq + 7], list);
			}  
			if(NoEaAttack[sq] & pos->pieceBB[ALL][BLACK]) {
				AddWhitePawnCapMove(pos, sq, sq+9, pos->pieces[sq + 9], list);
			} 		
		}
		//enpas captures
		if(pos->enPas < 65)	{
			sq = pos->enPas;
			U64 t_enPas = (WhitePawnAttack[sq] & pos->pieceBB[P][WHITE]);
			while(t_enPas)	{
				t_sq = POP(&t_enPas);
				AddEnPassantMove(pos, MOVE(t_sq,sq,EMPTY,EMPTY,MFLAGEP), list);				
			}
		}
		// KING MOVES
		sq = POP(&t_kings);
		U64 capKingM = (KingAttack[sq] & pos->pieceBB[ALL][BLACK]);
		while(capKingM)	{
			t_sq = POP(&capKingM);
			AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
		}
		// KNIGHT MOVES
		while(t_knights)	{
			sq = POP(&t_knights);
			U64 capKnightM = (KnightAttack[sq] & pos->pieceBB[ALL][BLACK]);
			while(capKnightM)	{
				t_sq = POP(&capKnightM);
				AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
			}
		}
		//	ROOK MOVES
		while(t_rooks)	{
			sq = POP(&t_rooks);
			U64 TempBoard = StraightPseudo[sq] & pos->pieceBB[ALL][BLACK];
			while(TempBoard)	{
				int DF = POP(&TempBoard);
				if (!(SliderObstructionCheck[DF][sq] & pos->pieceBB[ALL][BOTH]))	{
					AddCaptureMove(pos, MOVE(sq,DF,pos->pieces[DF],EMPTY,0),list);
				}
			}
		}
		//	QUEEN MOVES
		while(t_queens)	{
			sq = POP(&t_queens);
			U64 TempBoard = QueenPseudo[sq] & pos->pieceBB[ALL][BLACK];
			while(TempBoard)	{
				int DF = POP(&TempBoard);
				if (!(SliderObstructionCheck[DF][sq] & pos->pieceBB[ALL][BOTH]))	{
					AddCaptureMove(pos, MOVE(sq,DF,pos->pieces[DF],EMPTY,0),list);
				}
			}
		}
		// BISHOP MOVES
		while(t_bishops)	{
			sq = POP(&t_bishops);
			U64 TempBoard = DiagionalPseudo[sq] & pos->pieceBB[ALL][BLACK];
			while(TempBoard)	{
				int DF = POP(&TempBoard);
				if (!(SliderObstructionCheck[DF][sq] & pos->pieceBB[ALL][BOTH]))	{
					AddCaptureMove(pos, MOVE(sq,DF,pos->pieces[DF],EMPTY,0),list);
				}
			}
		}
	} else {
		U64 t_pawns = pos->pieceBB[P][BLACK];
		U64 t_kings = pos->pieceBB[K][BLACK];
		U64 t_knights = pos->pieceBB[N][BLACK];
		U64 t_rooks = pos->pieceBB[R][BLACK];
		U64 t_queens = pos->pieceBB[Q][BLACK];
		U64 t_bishops = pos->pieceBB[B][BLACK];
		// PAWN MOVES
		while(t_pawns) {		
			sq = POP(&t_pawns);
			if(SoEaAttack[sq] & pos->pieceBB[ALL][WHITE]) {
				AddBlackPawnCapMove(pos, sq, sq-7, pos->pieces[sq - 7], list);
			}  
			if(SoWeAttack[sq] & pos->pieceBB[ALL][WHITE]) {
				AddBlackPawnCapMove(pos, sq, sq-9, pos->pieces[sq - 9], list);
			} 	
		}
		if(pos->enPas < 65)	{
			sq = pos->enPas;
			U64 t_enPas = (BlackPawnAttack[sq] & pos->pieceBB[P][BLACK]);
			while(t_enPas)	{
				t_sq = POP(&t_enPas);
				AddEnPassantMove(pos, MOVE(t_sq,sq,EMPTY,EMPTY,MFLAGEP), list);				
			}
		}
		// KING MOVES
		sq = POP(&t_kings);
		U64 capKingM = (KingAttack[sq] & pos->pieceBB[ALL][WHITE]);
		while(capKingM)	{
			t_sq = POP(&capKingM);
			AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
		}
		// KNIGHT MOVES
		while(t_knights)	{		
			sq = POP(&t_knights);
			U64 capKnightM = (KnightAttack[sq] & pos->pieceBB[ALL][WHITE]);
			while(capKnightM)	{
				t_sq = POP(&capKnightM);
				AddCaptureMove(pos, MOVE(sq,t_sq,pos->pieces[t_sq],EMPTY,0),list);
			}
		}
		//	ROOK MOVES
		while(t_rooks)	{
			sq = POP(&t_rooks);
			U64 TempBoard = StraightPseudo[sq] & pos->pieceBB[ALL][WHITE];
			while(TempBoard)	{
				int DF = POP(&TempBoard);
				if (!(SliderObstructionCheck[DF][sq] & pos->pieceBB[ALL][BOTH]))	{
					AddCaptureMove(pos, MOVE(sq,DF,pos->pieces[DF],EMPTY,0),list);
				}
			}
		}
		//	QUEEN MOVES
		while(t_queens)	{
			sq = POP(&t_queens);
			U64 TempBoard = QueenPseudo[sq] & pos->pieceBB[ALL][WHITE];
			while(TempBoard)	{
				int DF = POP(&TempBoard);
				if (!(SliderObstructionCheck[DF][sq] & pos->pieceBB[ALL][BOTH]))	{
					AddCaptureMove(pos, MOVE(sq,DF,pos->pieces[DF],EMPTY,0),list);
				}
			}
		}
		// BISHOP MOVES
		while(t_bishops)	{
			sq = POP(&t_bishops);
			U64 TempBoard = DiagionalPseudo[sq] & pos->pieceBB[ALL][WHITE];
			while(TempBoard)	{
				int DF = POP(&TempBoard);
				if (!(SliderObstructionCheck[DF][sq] & pos->pieceBB[ALL][BOTH]))	{
					AddCaptureMove(pos, MOVE(sq,DF,pos->pieces[DF],EMPTY,0),list);
				}
			}
		}	
	}
}