#include "defs.h"
#include "stdlib.h"

#define RAND_64 (	(U64)rand() | \
					(U64)rand() << 15 | \
					(U64)rand() << 30 | \
					(U64)rand() << 45 | \
					((U64)rand() & 0xf) << 60    )


U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][64];
U64 SideKey;
U64 CastleKeys[16];

int FilesBrd[64];
int RanksBrd[64];

U64 WhitePawnAttack[64];
U64 BlackPawnAttack[64];

U64 KingAttack[64];
U64 KnightAttack[64];
U64 SliderAttack[64];

U64 NoWeAttack[64]; //redundant?
U64 NortAttack[64];
U64 NoEaAttack[64];
U64 EastAttack[64];
U64 SoEaAttack[64];
U64 SoutAttack[64];
U64 SoWeAttack[64];
U64 WestAttack[64];

// EVAL MASKS
U64 FileBBMask[8];
U64 RankBBMask[8];

U64 BlackPassedMask[64];
U64 WhitePassedMask[64];
U64 IsolatedMask[64];
U64 DoubledMask[64];

U64 WhiteSentryMask[64];//White pieceBB[P] !!!
U64 BlackSentryMask[64]; //Black pieceBB[P] !!!
		
U64 WhiteBackwardMask[64]; //White pieceBB[P]
U64 BlackBackwardMask[64]; //Black pieceBB[P]

U64 WhiteSupportMask[64];
U64 BlackSupportMask[64];

U64 WhiteOutpostMask[64];
U64 BlackOutpostMask[64];

U64 WhiteCastleKingMask;
U64 WhiteCastleQueenMask;
U64 BlackCastleKingMask;
U64 BlackCastleQueenMask;

U64	RookAttackArea[64];
U64	WhiteRookPawnCheck[64];
U64	BlackRookPawnCheck[64];
//U64	BishopAttackArea[64];
//U64	BishopPawnCheck[64];
U64	AttackRectangle[64];
U64	AttackSquare[64];

U64 WhiteStupid[64];
U64 BlackStupid[64];

U64 StraightPseudo[64];
U64 DiagionalPseudo[64];
U64 QueenPseudo[64];
U64	SliderObstructionCheck[64][64];


void InitEvalMasks() {

	int sq, tsq, r, f;
	
	WhiteCastleKingMask 	= 0ULL;
	WhiteCastleQueenMask 	= 0ULL;
	BlackCastleKingMask		= 0ULL;
	BlackCastleQueenMask	= 0ULL;

	//castle masks
	WhiteCastleQueenMask  	|= (1ULL << 0);
	WhiteCastleQueenMask  	|= (1ULL << 1);
	WhiteCastleQueenMask  	|= (1ULL << 2);
	WhiteCastleQueenMask  	|= (1ULL << 8);
	WhiteCastleQueenMask 	|= (1ULL << 9);
	WhiteCastleQueenMask  	|= (1ULL << 10);
	
	WhiteCastleKingMask   	|= (1ULL << 5);
	WhiteCastleKingMask   	|= (1ULL << 6);
	WhiteCastleKingMask  	|= (1ULL << 7);
	WhiteCastleKingMask   	|= (1ULL << 13);
	WhiteCastleKingMask  	|= (1ULL << 14);
	WhiteCastleKingMask  	|= (1ULL << 15);
	
	BlackCastleQueenMask  	|= (1ULL << 48);
	BlackCastleQueenMask  	|= (1ULL << 49);
	BlackCastleQueenMask  	|= (1ULL << 50);
	BlackCastleQueenMask  	|= (1ULL << 56);
	BlackCastleQueenMask 	|= (1ULL << 57);
	BlackCastleQueenMask  	|= (1ULL << 58);
	
	BlackCastleKingMask   	|= (1ULL << 53);
	BlackCastleKingMask   	|= (1ULL << 54);
	BlackCastleKingMask  	|= (1ULL << 55);
	BlackCastleKingMask   	|= (1ULL << 61);
	BlackCastleKingMask  	|= (1ULL << 62);
	BlackCastleKingMask  	|= (1ULL << 63);
	
	for(sq = 0; sq < 8; ++sq) {
        FileBBMask[sq] = 0ULL;
		RankBBMask[sq] = 0ULL;
	}

	for(r = RANK_8; r >= RANK_1; r--) {
        for (f = FILE_A; f <= FILE_H; f++) {
            sq = r * 8 + f;
            FileBBMask[f] |= (1ULL << sq);
            RankBBMask[r] |= (1ULL << sq);
        }
	}

	for(sq = 0; sq < 64; ++sq) {
		DoubledMask[sq] = 0ULL;
		IsolatedMask[sq] = 0ULL;
		WhitePassedMask[sq] = 0ULL;
		BlackPassedMask[sq] = 0ULL;
		
		WhiteSentryMask[sq] = 0ULL;	//White pieceBB[P] !!!
		BlackSentryMask[sq] = 0ULL; //Black pieceBB[P] !!!
		
		WhiteBackwardMask[sq] = 0ULL; //White pieceBB[P]
		BlackBackwardMask[sq] = 0ULL; //Black pieceBB[P]

		WhiteOutpostMask[sq] = 0ULL;
		BlackOutpostMask[sq] = 0ULL;
		
		WhiteSupportMask[sq] = 0ULL;
		BlackSupportMask[sq] = 0ULL;
		WhiteStupid[sq] = 0ULL;
		BlackStupid[sq] = 0ULL;
    }

	WhiteStupid[19] |= (1ULL << 11);
	WhiteStupid[20] |= (1ULL << 12);
	BlackStupid[43] |= (1ULL << 51);
	BlackStupid[44] |= (1ULL << 52);
	
	for(sq = 0; sq < 64; ++sq) {
		
		
		tsq = sq + 8;

        while(tsq < 64) {
            WhitePassedMask[sq] |= (1ULL << tsq);
            tsq += 8;
        }
		
		tsq = sq + 8;

        while(tsq < 64) {
            DoubledMask[sq] |= (1ULL << tsq);
            tsq += 8;
        }

        tsq = sq - 8;
        while(tsq >= 0) {
            BlackPassedMask[sq] |= (1ULL << tsq);
            tsq -= 8;
        }
		
        if(FilesBrd[sq] > FILE_A) {
            IsolatedMask[sq] |= FileBBMask[FilesBrd[sq] - 1];
			
			tsq = sq + 15;
			if (tsq < 64)
			{
			BlackSentryMask[sq] |= (1ULL << tsq);
			}
		
			tsq = sq - 17;
			if (tsq >= 0)
			{
			WhiteSentryMask[sq] |= (1ULL << tsq);
			}
						
			tsq = sq - 1;
            while((tsq < 64) && (tsq >= 0)) {
                WhiteBackwardMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
			
			tsq = sq - 1;
            while((tsq < 64) && (tsq >= 0)) {
                BlackBackwardMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }
		
            tsq = sq + 7;
            while(tsq < 64) {
                WhitePassedMask[sq] |= (1ULL << tsq);
				WhiteOutpostMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq - 9;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
				BlackOutpostMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
			
			tsq = sq + 7;
			if (tsq < 64)
			{
				BlackSupportMask[sq] |= (1ULL << tsq);
			}
		
			tsq = sq - 9;
			if (tsq >= 0)
			{
				WhiteSupportMask[sq] |= (1ULL << tsq);
			}

            
        }

        if(FilesBrd[sq] < FILE_H) {
			
            IsolatedMask[sq] |= FileBBMask[FilesBrd[sq] + 1];
		
			tsq = sq + 17;
			if (tsq < 64)
			{
			BlackSentryMask[sq] |= (1ULL << tsq);
			}
		
			tsq = sq - 15;
			if (tsq >= 0)
			{
			WhiteSentryMask[sq] |= (1ULL << tsq);
			}
			
			tsq = sq + 1;
            while((tsq < 64) && (tsq >= 0)) {
                WhiteBackwardMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
			
			tsq = sq + 1;
            while((tsq < 64) && (tsq >= 0)) {
                BlackBackwardMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }
			
            tsq = sq + 9;
            while(tsq < 64) {
                WhitePassedMask[sq] |= (1ULL << tsq);
				WhiteOutpostMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq - 7;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
				BlackOutpostMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
			
			
			tsq = sq + 9;
			if (tsq < 64)
			{
			BlackSupportMask[sq] |= (1ULL << tsq);
			}
		
			tsq = sq - 7;
			if (tsq >= 0)
			{
			WhiteSupportMask[sq] |= (1ULL << tsq);
			}
			
        }
	}
}

void InitPieceAttackMasks()
{
	int sq, tsq, r, f;
	
	for(sq = 0; sq < 64; ++sq)
	{
		RookAttackArea[sq] = 0ULL;		//	done
		WhiteRookPawnCheck[sq] = 0ULL;	//	done
		BlackRookPawnCheck[sq] = 0ULL;	//	done
		//BishopAttackArea[sq] = 0ULL;
		//BishopPawnCheck[sq] = 0ULL;
		AttackRectangle[sq] = 0ULL;		//	done
		AttackSquare[sq] = 0ULL;		//	done
	}
	
	for(sq = 0; sq < 64; ++sq)
	{
		RookAttackArea[sq] |= FileBBMask[FilesBrd[sq]];
		
		if(FilesBrd[sq] < FILE_H)
		{
			RookAttackArea[sq] |= FileBBMask[FilesBrd[sq+1]];
			
			tsq = sq + 25;

			if(tsq < 64) {
				AttackRectangle[sq] |= (1ULL << tsq);
			}

			tsq = sq - 23;
			if(tsq >= 0) {
				AttackRectangle[sq] |= (1ULL << tsq);
			}
		}
		if(FilesBrd[sq] < FILE_G)
		{
			tsq = sq + 2;

			if(tsq < 64) {
				AttackSquare[sq] |= (1ULL << tsq);
				AttackRectangle[sq] |= (1ULL << tsq);
			}
			
			tsq = sq + 18;

			if(tsq < 64) {
				AttackSquare[sq] |= (1ULL << tsq);
				AttackRectangle[sq] |= (1ULL << tsq);
			}
			
			tsq = sq + 26;

			if(tsq < 64) {
				AttackRectangle[sq] |= (1ULL << tsq);
			}
			
			tsq = sq - 14;
			if(tsq >= 0) {
				AttackSquare[sq] |= (1ULL << tsq);
				AttackRectangle[sq] |= (1ULL << tsq);
			}
			
			tsq = sq - 22;
			if(tsq >= 0) {
				AttackRectangle[sq] |= (1ULL << tsq);
			}
		}
		
		if(FilesBrd[sq] > FILE_A)
		{
			RookAttackArea[sq] |= FileBBMask[FilesBrd[sq-1]];
			
			tsq = sq + 23;

			if(tsq < 64) {
				AttackRectangle[sq] |= (1ULL << tsq);
			}

			tsq = sq - 25;
			if(tsq >= 0) {
				AttackRectangle[sq] |= (1ULL << tsq);
			}
		}
		if(FilesBrd[sq] > FILE_B)
		{
			tsq = sq - 2;
			if(tsq >= 0) {
				AttackSquare[sq] |= (1ULL << tsq);
				AttackRectangle[sq] |= (1ULL << tsq);
			}
			
			tsq = sq - 18;
			if(tsq >= 0) {
				AttackSquare[sq] |= (1ULL << tsq);
				AttackRectangle[sq] |= (1ULL << tsq);
			}
			
			tsq = sq - 26;
			if(tsq >= 0) {
				AttackRectangle[sq] |= (1ULL << tsq);
			}
			
			tsq = sq + 14;
			if(tsq < 64) {
				AttackSquare[sq] |= (1ULL << tsq);
				AttackRectangle[sq] |= (1ULL << tsq);
			}
			
			tsq = sq + 22;
			if(tsq < 64) {
				AttackRectangle[sq] |= (1ULL << tsq);
			}
		}
		
		AttackSquare[sq] |= KingAttack[sq];
		AttackSquare[sq] |= KnightAttack[sq];
		AttackRectangle[sq] |= KingAttack[sq];
		AttackRectangle[sq] |= KnightAttack[sq];
		
		tsq = sq + 16;

        if(tsq < 64) {
            AttackSquare[sq] |= (1ULL << tsq);
			AttackRectangle[sq] |= (1ULL << tsq);
        }

        tsq = sq - 16;
        if(tsq >= 0) {
            AttackSquare[sq] |= (1ULL << tsq);
			AttackRectangle[sq] |= (1ULL << tsq);
        }
		
		tsq = sq + 24;

        if(tsq < 64) {
            AttackRectangle[sq] |= (1ULL << tsq);
        }

        tsq = sq - 24;
        if(tsq >= 0) {
            AttackRectangle[sq] |= (1ULL << tsq);
        }
		
		tsq = sq;
	
		if(RanksBrd[tsq] != RANK_8)
		{
			tsq += 8;
			while(1)
			{
				WhiteRookPawnCheck[sq] |= (1ULL << tsq);
				
				if((RanksBrd[tsq] == RANK_8))
				{
					break;
				}
				
				tsq += 8;
				
			}
		}
	
		tsq = sq;
	
		if(RanksBrd[tsq] != RANK_1)
		{
			tsq -= 8;
			while(1)
			{
				BlackRookPawnCheck[sq] |= (1ULL << tsq);
				
				if((RanksBrd[tsq] == RANK_1))
				{
					break;
				}
				
				tsq -= 8;
				
			}
		}
	}		
}

void InitAttackMasks()
{
	
	int sq, tsq, r, f;
		
	for(sq = 0; sq < 64; ++sq) {
		WhitePawnAttack[sq] =0ULL;
		BlackPawnAttack[sq] =0ULL;
		KingAttack[sq] =0ULL;
		KnightAttack[sq] =0ULL;
		NoWeAttack[sq] =0ULL;
		NortAttack[sq] =0ULL;
		NoEaAttack[sq] =0ULL;
		EastAttack[sq] =0ULL;
		SoEaAttack[sq] =0ULL;
		SoutAttack[sq] =0ULL;
		SoWeAttack[sq] =0ULL;
		WestAttack[sq] =0ULL;
		SliderAttack[sq] =0ULL;
    }
	
	for(sq = 0; sq < 64; ++sq) {
		
		tsq = sq;
		if (tsq < 64)
			{
			SliderAttack[sq] |= (1ULL << tsq);
			}
		
		tsq = sq + 8;
		if (tsq < 64)
			{
			KingAttack[sq] |= (1ULL << tsq);
			NortAttack[sq] |= (1ULL << tsq);
			}
		
		tsq = sq - 8;
		if (tsq >= 0)
			{
			KingAttack[sq] |= (1ULL << tsq);
			SoutAttack[sq] |= (1ULL << tsq);
			}
		
        if(FilesBrd[sq] > FILE_A) {
			tsq = sq + 7;
			if (tsq < 64)
			{
			BlackPawnAttack[sq] |= (1ULL << tsq);
			KingAttack[sq] |= (1ULL << tsq);
			NoWeAttack[sq] |= (1ULL << tsq);
			}
			
			tsq = sq + 15;
			if (tsq < 64)
			{
			KnightAttack[sq] |= (1ULL << tsq);
			}
		
			tsq = sq - 9;
			if (tsq >= 0)
			{
			WhitePawnAttack[sq] |= (1ULL << tsq);
			KingAttack[sq] |= (1ULL << tsq);
			SoWeAttack[sq] |= (1ULL << tsq);
			}    

			tsq = sq - 17;
			if (tsq >= 0)
			{
			KnightAttack[sq] |= (1ULL << tsq);
			}      			

			tsq = sq - 1;
			if (tsq >= 0)
			{
			KingAttack[sq] |= (1ULL << tsq);
			WestAttack[sq] |= (1ULL << tsq);
			}  			
        }
		
		if(FilesBrd[sq] > FILE_B) {
			tsq = sq + 6;
			if (tsq < 64)
			{
			KnightAttack[sq] |= (1ULL << tsq);
			}
			
			tsq = sq - 10;
			if (tsq >= 0)
			{
			KnightAttack[sq] |= (1ULL << tsq);
			}      			
        }

        if(FilesBrd[sq] < FILE_H) {
			
			tsq = sq + 9;
			if (tsq < 64)
			{
			BlackPawnAttack[sq] |= (1ULL << tsq);
			KingAttack[sq] |= (1ULL << tsq);
			NoEaAttack[sq] |= (1ULL << tsq);
			}
			
			tsq = sq + 17;
			if (tsq < 64)
			{
			KnightAttack[sq] |= (1ULL << tsq);
			}
			
			tsq = sq + 1;
			if (tsq < 64)
			{
			KingAttack[sq] |= (1ULL << tsq);
			EastAttack[sq] |= (1ULL << tsq);
			}  	
		
			tsq = sq - 7;
			if (tsq >= 0)
			{
			WhitePawnAttack[sq] |= (1ULL << tsq);
			KingAttack[sq] |= (1ULL << tsq);
			SoEaAttack[sq] |= (1ULL << tsq);
			}
			
			tsq = sq - 15;
			if (tsq >= 0)
			{
			KnightAttack[sq] |= (1ULL << tsq);
			}
        }
		
		if(FilesBrd[sq] < FILE_G) {
			
			tsq = sq + 10;
			if (tsq < 64)
			{
			KnightAttack[sq] |= (1ULL << tsq);
			}

			tsq = sq - 6;
			if (tsq >= 0)
			{
			KnightAttack[sq] |= (1ULL << tsq);
			}
        }
	}
}

const int BiDir[4] = { -7, -9, 7, 9 };

void InitPseudoAttackMasks()
{
	int sq, t_sq, i, dir, AT, DF;
	
	for(sq = 0; sq < 64; ++sq) {
		StraightPseudo[sq] = 0ULL;
		DiagionalPseudo[sq] = 0ULL;
		QueenPseudo[sq] = 0ULL;
		for(i = 0; i < 64; ++i)
		{
			SliderObstructionCheck[sq][i] = 0ULL;
		}
    }
	
	for(sq = 0; sq < 64; ++sq) {
		StraightPseudo[sq] |= FileBBMask[FilesBrd[sq]];
		StraightPseudo[sq] |= RankBBMask[RanksBrd[sq]];
    }
	
	for(sq = 0; sq < 64; ++sq)
	{
		for(i = 0; i < 4; ++i)
		{
			dir = BiDir[i];
			t_sq = sq;
			int safe = 1;
				if(i == 2 && ((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_A)))
				{
					safe = 0;
				}
				if(i == 3 && ((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_H)))
				{
					safe = 0;
				}
				if(i == 0 && ((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_H)))
				{
					safe = 0;
				}
				if(i == 1 && ((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_A)))
				{
					safe = 0;
				}
				t_sq += dir;
			while(safe)
			{				
				DiagionalPseudo[sq] |= (1ULL << t_sq);
				if(i == 2 && ((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_A)))
				{
					break;
				}
				if(i == 3 && ((RanksBrd[t_sq] == RANK_8)||(FilesBrd[t_sq] == FILE_H)))
				{
					break;
				}
				if(i == 0 && ((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_H)))
				{
					break;
				}
				if(i == 1 && ((RanksBrd[t_sq] == RANK_1)||(FilesBrd[t_sq] == FILE_A)))
				{
					break;
				}
				t_sq += dir;
			}
				
			}
		}
		
		U64 TempBoard = 0ULL;
		
		for(AT = 0; AT < 64; ++AT)
		{
			for(DF = 0; DF < 64; ++DF)
			{
				TempBoard = 0ULL;
				TempBoard |= (1ULL << DF);
				if (StraightPseudo[AT] & TempBoard)
				{
					if(DF-AT > 0 && DF-AT < 8)
					{
						int x = DF;
						while( (x-1) != AT)
						{
							x--;
							SliderObstructionCheck[DF][AT] |= (1ULL << x);
						}
					}
					if(DF-AT < 0 && DF-AT > -8)
					{
						int x = DF;
						while( (x+1) != AT)
						{
							x++;
							SliderObstructionCheck[DF][AT] |= (1ULL << x);
						}
					}
					if(DF-AT > 7)
					{
						int x = DF;
						while( (x-8) != AT)
						{
							x-=8;
							SliderObstructionCheck[DF][AT] |= (1ULL << x);
						}
					}
					if(DF-AT < -7)
					{
						int x = DF;
						while( (x+8) != AT)
						{
							x+=8;
							SliderObstructionCheck[DF][AT] |= (1ULL << x);
						}
					}
				}
				if (DiagionalPseudo[AT] & TempBoard)
				{
					if (DF-AT > 0)
					{
						if(!((DF-AT)%7))
						{
							int x = DF;
							while( (x-7) != AT)
							{
								x-=7;
								SliderObstructionCheck[DF][AT] |= (1ULL << x);
							}
						}
						else
						{
							int x = DF;
							while( (x-9) != AT)
							{
								x-=9;
								SliderObstructionCheck[DF][AT] |= (1ULL << x);
							}
						}
					}
					if (DF-AT < 0)
					{
						if(!((DF-AT)%7))
						{
							int x = DF;
							while( (x+7) != AT)
							{
								x+=7;
								SliderObstructionCheck[DF][AT] |= (1ULL << x);
							}
						}
						else
						{
							int x = DF;
							while( (x+9) != AT)
							{
								x+=9;
								SliderObstructionCheck[DF][AT] |= (1ULL << x);
							}
						}
					}
				}
			}
		}
		
		for(sq = 0; sq < 64; ++sq)
		{
			QueenPseudo[sq] = StraightPseudo[sq];
			QueenPseudo[sq] |= DiagionalPseudo[sq];
		}
}

void InitFilesRanksBrd() 
{
	
	int i = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = 0;
	
	for(i = 0; i < 64; ++i) {
		FilesBrd[i] = 65;
		RanksBrd[i] = 65;
	}
	
	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = (rank) * 8 + file;
			FilesBrd[sq] = file;
			RanksBrd[sq] = rank;
		}
	}
}

void InitHashKeys()
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 13; ++i)
	{
		for(j = 0; j < 64; ++j)
		{
			PieceKeys[i][j] = RAND_64;
		}
	}
	SideKey = RAND_64;
	for(i = 0; i < 16; ++i)
	{
		CastleKeys[i] = RAND_64;
	}
}

void InitBitMasks()
{
	int i = 0;
	
	for(i = 0; i < 64; i++)
	{
		SetMask[i] = 0ULL;
		ClearMask[i] = 0ULL;
	}
	
	for(i = 0; i < 64; i++)
	{
		SetMask[i] |= (1ULL << i);
		ClearMask[i] = ~SetMask[i];
	}
}


void AllInit()
{
	InitBitMasks();
	InitHashKeys();
	InitFilesRanksBrd();
	InitAttackMasks();
	InitEvalMasks();
	InitMvvLva();
	InitBlackPSQT();
	InitPieceAttackMasks();
	InitPseudoAttackMasks();
}