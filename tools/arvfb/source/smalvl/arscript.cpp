#include "analizator.h"
#include "runtime.h"

void ArScript(char* file)
{
	yydebug = 0;

	if (NULL == file)
	{
		Arch_Trace("\n\n## Load Script: /home/parai/workspace/OpenSAR/binaries/gtk/example/main.svl\n\n");
		yyin = fopen("/home/parai/workspace/OpenSAR/binaries/gtk/example/main.svl", "r");
	}
	else
	{
		yyin = fopen(file, "r");
	}

	if (yyin == NULL)
	{
		Arch_Trace("Script: file <%s> is not exist.\n",file);
		return;
	}

	runtime_t::get_instance()->init();

	yyparse();

}

