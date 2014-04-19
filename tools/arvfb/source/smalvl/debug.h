#ifndef _DEBUG_H
#define _DEBUG_H
#include <assert.h>
#if defined(DEBUG_ENABLED)
#define RUNTIME_DEBUG(x, ...) printf("RUNTIME DEBUG: ");\
				printf(x, ##__VA_ARGS__); printf("\n"); fflush(stdout);
#else
#define RUNTIME_DEBUG(x, ...) ;
#endif

extern "C" void Arch_Trace(const char* format,...);

#endif
