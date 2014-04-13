#include "analizator.h"

int main (int argc, char** argv) {
	yydebug = 0;

	if (argc < 2) {
		std::cerr << "Input file not specified as first comandline argument. Using stdin as default input stream\n";
	  #if DEBUG_ENABLED
		yyin = fopen("example/main.svl", "r");	  
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

