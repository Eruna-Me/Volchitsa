#include "stdio.h"
#include "defs.h"
#include "string.h"

#define INPUTBUFFER 400 * 6
	
int MaxDepth = 64;
int Contempt = 0;

void ParseGo(char* line, S_SEARCHINFO *info, S_BOARD *pos) {
	int depth = -1, movestogo = 30,movetime = -1;
	int time = -1, inc = 0;
    char *ptr = NULL;
	info->timeset = 0;
	
	if ((ptr = strstr(line,"infinite"))) {
		;
	} 
	
	if ((ptr = strstr(line,"binc")) && pos->side == BLACK) {
		inc = atoi(ptr + 5);
	}
	
	if ((ptr = strstr(line,"winc")) && pos->side == WHITE) {
		inc = atoi(ptr + 5);
	} 
	
	if ((ptr = strstr(line,"wtime")) && pos->side == WHITE) {
		time = atoi(ptr + 6);
	} 
	  
	if ((ptr = strstr(line,"btime")) && pos->side == BLACK) {
		time = atoi(ptr + 6);
	} 
	  
	if ((ptr = strstr(line,"movestogo"))) {
		movestogo = atoi(ptr + 10);
	} 
	  
	if ((ptr = strstr(line,"movetime"))) {
		movetime = atoi(ptr + 9);
	}
	  
	if ((ptr = strstr(line,"depth"))) {
		depth = atoi(ptr + 6);
	} 
	
	if(movetime != -1) {
		time = movetime;
		movestogo = 1;
	}
	
	info->starttime = GetTimeMs();
	info->depth = depth;
	
	if(time != -1) {
		info->timeset = 1;
		time /= movestogo;
		time -= 50;		
		info->stoptime = info->starttime + time + inc;
	} 
	
	if(depth == -1) {
		info->depth = MAXDEPTH;
	}
	
	printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
		time,info->starttime,info->stoptime,info->depth,info->timeset);
	SearchPosition(pos, info);
}
void ParsePosition(char* lineIn, S_BOARD *pos) {
	
	lineIn += 9;
    char *ptrChar = lineIn;
	
    if(strncmp(lineIn, "startpos", 8) == 0){
        ParseFen(START_FEN, pos);
    } else {
        ptrChar = strstr(lineIn, "fen");
        if(ptrChar == NULL) {
            ParseFen(START_FEN, pos);
        } else {
            ptrChar+=4;
            ParseFen(ptrChar, pos);
        }
    }
	
	ptrChar = strstr(lineIn, "moves");
	int move;
	
	if(ptrChar != NULL) {
        ptrChar += 6;
        while(*ptrChar) {
              move = ParseMove(ptrChar,pos);
			  if(move == 0) break;
			  MakeMove(pos, move);
              pos->ply=0;
              while(*ptrChar && *ptrChar!= ' ') ptrChar++;
              ptrChar++;
        }
    }
	PrintBoard(pos);
	
}

void Uci_Loop() {
	
	setbuf(stdin, NULL);
    setbuf(stdout, NULL);
	
	char line[INPUTBUFFER];

	
	
    S_BOARD pos[1];
	S_SEARCHINFO info[1] = { { 0 } };
    InitHashTable(pos->HashTable);
	
	while (1) {
		memset(&line[0], 0, sizeof(line));
        fflush(stdout);
		//InitPvTable(pos->PvTable);
        if (!fgets(line, INPUTBUFFER, stdin))
        continue;

        if (line[0] == '\n')
        continue;

        if (!strncmp(line, "isready", 7)) {
            printf("readyok\n");
            continue;
        } else if (!strncmp(line, "position", 8)) {
            ParsePosition(line, pos);
        } else if (!strncmp(line, "ucinewgame", 10)) {
            ParsePosition("position startpos\n", pos);
			ClearHashTable(pos->HashTable);
        } else if (!strncmp(line, "go", 2)) {
            ParseGo(line, info, pos);
        } else if (!strncmp(line, "quit", 4)) {
            info->quit = 1;
            break;
        } else if (!strncmp(line, "uci", 3)) {
            printf("id name %s\n",NAME);
            printf("id author %s\n", AUTHOR);
			printf("option name MaxDepth type spin default 64 min 1 max 64\n");
			printf("option name Contempt type spin default 0 min -100 max 100\n");
            printf("uciok\n");
        } else if (!strncmp(line, "setoption name MaxDepth value ", 30)) {			
			sscanf(line,"%*s %*s %*s %*s %d",&MaxDepth);
			if(MaxDepth < 1) MaxDepth = 1;
			if(MaxDepth > 64) MaxDepth = 64;
			printf("Set MaxDepth to %d\n",MaxDepth);
		} else if (!strncmp(line, "setoption name Contempt value ", 30)) {			
			sscanf(line,"%*s %*s %*s %*s %d",&Contempt);
			printf("Set Contempt to %d\n",Contempt);
			Contempt = 0-Contempt;
		}
		if(info->quit) break;
    }
}
