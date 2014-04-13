#include "analizator.h"

#define DEBUG_ENABLED 1
int arscript_main (int argc, char** argv) {
	yydebug = 0;

	if (argc < 2) {
	  #if DEBUG_ENABLED
		yyin = fopen("./example/main2.svl", "r");
	  #else
		std::cerr << "Input file not specified as first comandline argument. Using stdin as default input stream\n";
	  #endif
	}
	else {
	  yyin = fopen(argv[1], "r");	
	  if( yyin == NULL ) {
	    perror("Can't open source file");
	    return 2;
	  }
	}

	do {
	  yyparse();
	} while( !feof(yyin));
	return 0;
}

