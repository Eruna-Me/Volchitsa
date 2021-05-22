#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

int CheckBoard(const S_BOARD *pos) {   
 
	int t_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int t_bigPce[2] = { 0, 0};
	int t_majPce[2] = { 0, 0};
	int t_minPce[2] = { 0, 0};
	int t_material[2] = { 0, 0};
	
	int sq,t_piece,t_pce_num,colour,pcount;
	
	U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};
	
	t_pawns[WHITE] = pos->pieceBB[P][WHITE];
	t_pawns[BLACK] = pos->pieceBB[P][BLACK];
	t_pawns[BOTH] = pos->pieceBB[P][BOTH];
	
	// check piece count and other counters	
	for(sq = 0; sq < 64; ++sq) {
		t_piece = pos->pieces[sq];
		t_pceNum[t_piece]++;
		colour = PieceCol[t_piece];
		if( PieceBig[t_piece] == 1) t_bigPce[colour]++;
		if( PieceMin[t_piece] == 1) t_minPce[colour]++;
		if( PieceMaj[t_piece] == 1) t_majPce[colour]++;
		
		t_material[colour] += PieceVal[t_piece];
	}
	
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		ASSERT(t_pceNum[t_piece]==pos->pceNum[t_piece]);	
	}
	
	// check bitboards count
	pcount = CNT(t_pawns[WHITE]);
	ASSERT(pcount == pos->pceNum[wP]);
	pcount = CNT(t_pawns[BLACK]);
	ASSERT(pcount == pos->pceNum[bP]);
	pcount = CNT(t_pawns[BOTH]);
	ASSERT(pcount == (pos->pceNum[bP] + pos->pceNum[wP]));
	
	// check bitboards squares
	while(t_pawns[WHITE]) {
		sq = POP(&t_pawns[WHITE]);
		ASSERT(pos->pieces[sq] == wP);
	}
	
	while(t_pawns[BLACK]) {
		sq = POP(&t_pawns[BLACK]);
		ASSERT(pos->pieces[sq] == bP);
	}
	
	while(t_pawns[BOTH]) {
		sq = POP(&t_pawns[BOTH]);
		ASSERT( (pos->pieces[sq] == bP) || (pos->pieces[sq] == wP) );
	}
	
	ASSERT(t_material[WHITE]==pos->material[WHITE] && t_material[BLACK]==pos->material[BLACK]);
	ASSERT(t_minPce[WHITE]==pos->minPce[WHITE] && t_minPce[BLACK]==pos->minPce[BLACK]);
	ASSERT(t_majPce[WHITE]==pos->majPce[WHITE] && t_majPce[BLACK]==pos->majPce[BLACK]);
	ASSERT(t_bigPce[WHITE]==pos->bigPce[WHITE] && t_bigPce[BLACK]==pos->bigPce[BLACK]);	
	
	ASSERT(pos->side==WHITE || pos->side==BLACK);
	ASSERT(GeneratePosKey(pos)==pos->posKey);
	
	ASSERT(pos->enPas==65 || ( RanksBrd[pos->enPas]==RANK_6 && pos->side == WHITE)
		 || ( RanksBrd[pos->enPas]==RANK_3 && pos->side == BLACK));
	
	ASSERT(pos->pieces[pos->KingSq[WHITE]] == wK);
	ASSERT(pos->pieces[pos->KingSq[BLACK]] == bK);
		 
	return 1;

}

void UpdateListsMaterial(S_BOARD *pos) {	
	
	int piece,sq,i,colour;
	pos->pieceBB[ALL][BOTH] = 0ULL;
	
	for(i = 0; i < 64; ++i) {
		sq = i;
		piece = pos->pieces[i];
		if(piece!= EMPTY) {
			colour = PieceCol[piece];
			
		    if( PieceBig[piece] == 1) pos->bigPce[colour]++;
		    if( PieceMin[piece] == 1) pos->minPce[colour]++;
		    if( PieceMaj[piece] == 1) pos->majPce[colour]++;
			
			pos->material[colour] += PieceVal[piece];

			pos->pceNum[piece]++;
			
			if(piece==wK) pos->KingSq[WHITE] = sq;
			if(piece==bK) pos->KingSq[BLACK] = sq;
			
			//sq++;
			
			if(piece==wP) {
				pos->pieceBB[P][WHITE] |= (1ULL << sq);
				pos->pieceBB[P][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][WHITE]|= (1ULL << sq);
			} else if(piece==bP) {
				pos->pieceBB[P][BLACK] |= (1ULL << sq);
				pos->pieceBB[P][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BLACK]|= (1ULL << sq);
			} else if(piece==wN) {
				pos->pieceBB[N][WHITE] |= (1ULL << sq);
				pos->pieceBB[N][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][WHITE]|= (1ULL << sq);
			} else if(piece==bN) {
				pos->pieceBB[N][BLACK] |= (1ULL << sq);
				pos->pieceBB[N][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BLACK]|= (1ULL << sq);
			} else if(piece==wB) {
				pos->pieceBB[B][WHITE] |= (1ULL << sq);
				pos->pieceBB[B][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][WHITE]|= (1ULL << sq);
			} else if(piece==bB) {
				pos->pieceBB[B][BLACK] |= (1ULL << sq);
				pos->pieceBB[B][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BLACK]|= (1ULL << sq);
			} else if(piece==wR) {
				pos->pieceBB[R][WHITE] |= (1ULL << sq);
				pos->pieceBB[R][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][WHITE]|= (1ULL << sq);
			} else if(piece==bR) {
				pos->pieceBB[R][BLACK] |= (1ULL << sq);
				pos->pieceBB[R][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BLACK]|= (1ULL << sq);
			} else if(piece==wQ) {
				pos->pieceBB[Q][WHITE] |= (1ULL << sq);
				pos->pieceBB[Q][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][WHITE]|= (1ULL << sq);
			} else if(piece==bQ) {
				pos->pieceBB[Q][BLACK] |= (1ULL << sq);
				pos->pieceBB[Q][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BLACK]|= (1ULL << sq);
			} else if(piece==wK) {
				pos->pieceBB[K][WHITE] |= (1ULL << sq);
				pos->pieceBB[K][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][WHITE]|= (1ULL << sq);
			} else if(piece==bK) {
				pos->pieceBB[K][BLACK] |= (1ULL << sq);
				pos->pieceBB[K][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BOTH]|= (1ULL << sq);
				pos->pieceBB[ALL][BLACK]|= (1ULL << sq);
			}
			
			
		}
	}
	
	
	

}

int ParseFen(char *fen, S_BOARD *pos) {
	
	int  rank = RANK_8;
    int  file = FILE_A;
    int  piece = 0;
    int  count = 0;
    int  i = 0; 
	int  sq64 = 0; 
	
	ResetBoard(pos);
	
	while ((rank >= RANK_1) && *fen) {
	    count = 1;
		switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = *fen - '0';
                break;

            case '/':
            case ' ':
                rank--;
                file = FILE_A;
                fen++;
                continue;              

            default:
                printf("FEN error \n");
                return -1;
        }		
		
		for (i = 0; i < count; i++) {			
            sq64 = rank * 8 + file;
            if (piece != EMPTY) {
                pos->pieces[sq64] = piece;
            }
			file++;
        }
		fen++;
	}
	
	pos->side = (*fen == 'w') ? WHITE : BLACK;
	fen += 2;
	
	for (i = 0; i < 4; i++) {
        if (*fen == ' ') {
            break;
        }		
		switch(*fen) {
			case 'K': pos->castlePerm |= WKCA; break;
			case 'Q': pos->castlePerm |= WQCA; break;
			case 'k': pos->castlePerm |= BKCA; break;
			case 'q': pos->castlePerm |= BQCA; break;
			default:	     break;
        }
		fen++;
	}
	fen++;
	
	if (*fen != '-') {        
		file = fen[0] - 'a';
		rank = fen[1] - '1';
		
		pos->enPas = (rank) * 8 + file;		
    }
	
	pos->posKey = GeneratePosKey(pos); 
	
	UpdateListsMaterial(pos);
	
	return 0;
}

void ResetBoard(S_BOARD *pos) {

	int i = 0;
	
	for(i = 0; i < 64; ++i) {
		pos->pieces[i] = EMPTY;
	}
	
	for(i = 0; i < 2; ++i) {
		pos->bigPce[i] = 0;
		pos->majPce[i] = 0;
		pos->minPce[i] = 0;
		pos->material[i] = 0;
	}
	
	for(i = 0; i < 3; ++i)
	{
		pos->pieceBB[P][i] = 0ULL;
		pos->pieceBB[ALL][i] = 0ULL;
		pos->pieceBB[Q][i] = 0ULL;
		pos->pieceBB[K][i] = 0ULL;
		pos->pieceBB[R][i] = 0ULL;
		pos->pieceBB[N][i] = 0ULL;
		pos->pieceBB[B][i] = 0ULL;
	}
	
	for(i = 0; i < 13; ++i) {
		pos->pceNum[i] = 0;
	}
	
	pos->KingSq[WHITE] = pos->KingSq[BLACK] = 65;
	
	pos->side = BOTH;
	pos->enPas = 65;
	pos->fiftyMove = 0;
	
	pos->ply = 0;
	pos->hisPly = 0;
	
	pos->castlePerm = 0;
	
	pos->posKey = 0ULL;
	
	//InitPvTable(pos->PvTable);
}

void PrintBoard(const S_BOARD *pos)
{
	
	int sq,file,rank,piece;
	
	printf("\nGame Board:\n\n");
	
	for(rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ",rank+1);
		for(file = FILE_A; file <= FILE_H; file++) {
			sq = (rank) * 8 + file;
			piece = pos->pieces[sq];
			printf("%3c",PceChar[piece]);
		}
		printf("\n");
	}
	
	printf("\n   ");
	for(file = FILE_A; file <= FILE_H; file++) {
		printf("%3c",'a'+file);	
	}
	printf("\n");
	printf("side:%c\n",SideChar[pos->side]);
	printf("enPas:%d\n",pos->enPas);
	printf("castle:%c%c%c%c\n",
			pos->castlePerm & WKCA ? 'K' : '-',
			pos->castlePerm & WQCA ? 'Q' : '-',
			pos->castlePerm & BKCA ? 'k' : '-',
			pos->castlePerm & BQCA ? 'q' : '-'	
			);
	printf("PosKey:%llX\n",pos->posKey);
	}