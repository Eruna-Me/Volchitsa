#include "stdio.h"
#include "defs.h"
//#include "stdlib.h"

void RunTest(char* fen, int depth, long expectedNodes)
{
	AllInit();

	S_BOARD pos[1];

	ParseFen(fen, pos);

	long nodesFound = PerftTest(depth, pos, 0);

	if (nodesFound != expectedNodes)
	{
		printf("Perft failed for FEN: %s\n at depth: %d\n expected: %ld nodes\n found: %ld nodes\n", fen,depth, expectedNodes, nodesFound);
	}
}

//https://www.chessprogramming.org/Perft_Results

void RunAllPerftTests()
{
	RunTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4, 197281); //Pos 1
	RunTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 4, 4085603); //Pos 2
	RunTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 5, 674624); //Pos 3
	RunTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333); //Pos 4
	RunTest("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 4, 422333); //Pos 4 mirrored
	RunTest("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 3, 62379); //Pos 5
	RunTest("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 3, 89890); //Pos 6

	printf("ALL PERFT TESTS FINISHED \n");
}
