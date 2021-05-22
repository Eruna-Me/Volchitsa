#include "stdio.h"
#include "defs.h"
#include "stdlib.h"


#define PERFTFEN "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"
#define WAC1 "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

int main() {	

	AllInit();		
	
	Uci_Loop();
	
	return 0;
}