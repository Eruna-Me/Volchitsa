
#ifndef DEFS_H
#define DEFS_H

#include "stdlib.h"


//#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed",#n); \
printf("On %s ",__DATE__); \
printf("At %s ",__TIME__); \
printf("In File %s ",__FILE__); \
printf("At Line %d\n",__LINE__); \
exit(1);}
#endif


typedef unsigned long long U64;

#define NAME "Volchitsa 1.0"
#define AUTHOR "Hidde Bartelink"

#define NOMOVE 0
#define MAXGAMEMOVES 2048
#define MAXPOSITIONMOVES 256
#define MAXDEPTH 64

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define INFINITEx 30000
#define ISMATE (INFINITEx - MAXDEPTH)

//ENUMS

enum { ALL, P, N, B, R, Q, K};
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
enum { WHITE, BLACK, BOTH };

enum {WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

//STRUCTS

typedef struct
{
	int move;
	int score;
} S_MOVE ;

typedef struct
{
	S_MOVE moves[MAXPOSITIONMOVES];
	int count;
} S_MOVELIST ;

enum { HFNONE, HFALPHA, HFBETA, HFEXACT};

typedef struct
{
	U64 posKey;
	int move;
	int score;
	int depth;
	int flags;
} S_HASHENTRY;

typedef struct
{
	S_HASHENTRY *pTable;
	int numEntries;
	int newWrite;
	int overWrite;
	int hit;
	int cut;
} S_HASHTABLE;

typedef struct 
{
	int move;
	int castlePerm;
	int enPas;
	int fiftyMove;
	U64 posKey;
	
} S_UNDO ;

typedef struct 
{
	
	int pieces[64];
	U64 pieceBB[7][3];
	
	int KingSq[2];
	
	int side;
	int enPas;
	int fiftyMove;
	
	int ply;
	int hisPly;
	
	int castlePerm;
	
	U64 posKey;
	
	int pceNum[13];
	int bigPce[2];
	int majPce[2];
	int minPce[2];
	int material[2];
	
	S_UNDO history[MAXGAMEMOVES];
	
	S_HASHTABLE HashTable[1];
	int PvArray[MAXDEPTH];
	
	int SearchHistory[13][64];
	int SearchKillers[2][MAXDEPTH];
	
} S_BOARD ;


typedef struct {

	int starttime;
	int stoptime;
	int depth;
	int depthset;
	int timeset;
	int movestogo;
	int infinite;
	
	long nodes;
	
	int quit;
	int stopped;
	
	float fh;
	float fhf;

} S_SEARCHINFO;

// MACROS

#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

#define POP(b) PopBit(b)
#define CNT(b) CountBits(b)
#define CLRBIT (bb,sq) ((bb) &= ClearMask[(sq)])
#define SETBIT (bb,sq) ((bb) |= SetMask[(sq)])

#define IsBQ(p) (PieceBishopQueen[(p)])
#define IsRQ(p) (PieceRookQueen[(p)])
#define IsKn(p) (PieceKnight[(p)])
#define IsKi(p) (PieceKing[(p)])

// GLOBALS



extern U64 SetMask[64];
extern U64 ClearMask[64];

extern U64 PieceKeys[13][64];
extern U64 SideKey;
extern U64 CastleKeys[16];
extern int MaxDepth;
extern int Contempt;

// Piece attack bitboards
extern U64 WhitePawnAttack[64];
extern U64 BlackPawnAttack[64];
extern U64 KingAttack[64];
extern U64 KnightAttack[64];
extern U64 NoWeAttack[64];
extern U64 NortAttack[64];
extern U64 NoEaAttack[64];
extern U64 EastAttack[64];
extern U64 SoEaAttack[64];
extern U64 SoutAttack[64];
extern U64 SoWeAttack[64];
extern U64 WestAttack[64];
extern U64 SliderAttack[64];

extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

extern int PieceBig[13];
extern int PieceMaj[13];
extern int PieceMin[13];
extern int PieceVal[13];
extern int PieceCol[13];

extern int FilesBrd[64];
extern int RanksBrd[64];

extern int PiecePawn[13];
extern int PieceKnight[13];
extern int PieceKing[13];
extern int PieceRookQueen[13];
extern int PieceBishopQueen[13];

//Piece Eval

extern U64 FileBBMask[8];
extern U64 RankBBMask[8];

extern U64 BlackPassedMask[64];
extern U64 WhitePassedMask[64];
extern U64 IsolatedMask[64];
extern U64 DoubledMask[64];

extern U64 WhiteSentryMask[64];	//White pieceBB[P] !!!
extern U64 BlackSentryMask[64]; //Black pieceBB[P] !!!
		
extern U64 WhiteBackwardMask[64]; //White pieceBB[P]
extern U64 BlackBackwardMask[64]; //Black pieceBB[P]

extern U64 WhiteOutpostMask[64];
extern U64 BlackOutpostMask[64];

extern U64 WhiteSupportMask[64];
extern U64 BlackSupportMask[64];

extern U64 WhiteCastleKingMask;
extern U64 WhiteCastleQueenMask;
extern U64 BlackCastleKingMask;
extern U64 BlackCastleQueenMask;

extern U64	RookAttackArea[64];
extern U64	WhiteRookPawnCheck[64];
extern U64	BlackRookPawnCheck[64];
extern U64	AttackRectangle[64];
extern U64	AttackSquare[64];
extern U64	WhiteStupid[64];
extern U64	BlackStupid[64];

extern U64 StraightPseudo[64];
extern U64 DiagionalPseudo[64];
extern U64 QueenPseudo[64];
extern U64 SliderObstructionCheck[64][64];

// FUNCTIONS

// init.c
extern void AllInit();

// bitboards.c
extern void PrintBitBoard(U64 bb);
extern int PopBit (U64 *bb);
extern int CountBits(U64 b);

// hashkeys.c
extern U64 GeneratePosKey(const S_BOARD *pos);

// board.c

extern void ResetBoard(S_BOARD *pos);
extern int ParseFen(char *fen, S_BOARD *pos);
extern void PrintBoard(const S_BOARD *pos);
extern void UpdateListsMaterial(S_BOARD *pos);
extern int CheckBoard(const S_BOARD *pos);

// attack.c

extern int SqAttacked(const int sq, const int side, const S_BOARD *pos);

// io.c

extern char *PrSq(const int sq);
extern char *PrMove(const int move);
extern void PrintMoveList(const S_MOVELIST *list);
extern int ParseMove(char *ptrChar, S_BOARD *pos);

//  movegen.c

extern void GenerateAllMoves(  const S_BOARD *pos, S_MOVELIST *list );
extern void GenerateAllCaps(  const S_BOARD *pos,  S_MOVELIST *list );
extern int MoveExists(S_BOARD *pos, const int move);
extern void InitMvvLva();

//  makemove.c

extern void TakeMove(S_BOARD *pos);
extern int MakeMove(S_BOARD *pos, int move);
extern void MakeNullMove(S_BOARD *pos);
extern void TakeNullMove(S_BOARD *pos);

// perft.c
extern void PerftTest(int depth, S_BOARD *pos);

// search.c
extern void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info);
//extern int IsRepetition(const S_BOARD *pos);

// misc.c
extern int GetTimeMs();
extern void ReadInput(S_SEARCHINFO *info);

// pvtable.c
extern void InitHashTable(S_HASHTABLE *table);
extern void StoreHashEntry(S_BOARD *pos, const int move, int score, const int flags, const int depth);
extern int ProbeHashEntry(S_BOARD *pos, int *move, int *score, int alpha, int beta, int depth);
extern int ProbePvMove(const S_BOARD *pos);
extern int GetPvLine(const int depth, S_BOARD *pos);
extern void ClearHashTable(S_HASHTABLE *table);
// evaluate.c

extern int EvalPosition(const S_BOARD *pos);
extern void InitBlackPSQT();

// uci.c

extern void Uci_Loop();

// endgame.c

extern float EvalEndgame(const S_BOARD *pos);

#endif