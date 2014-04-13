#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef DEBUG_ENABLED
#define RUNTIME_DEBUG(x, ...) printf("RUNTIME DEBUG: ");\
				printf(x, ##__VA_ARGS__); printf("\n"); fflush(stdout);
#else
#define RUNTIME_DEBUG(x, ...) ;
#endif

#endif