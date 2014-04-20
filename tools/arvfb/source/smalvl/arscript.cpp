#include "analizator.h"
#include "runtime.h"

void ArScriptLoad(const char* file)
{
	yydebug = 0;

	if (NULL == file)
	{
		Arch_Trace("\n\n## Load Script: /home/parai/workspace/OpenSAR/binaries/gtk/example/main.svl\n\n\n");
		yyin = fopen("/home/parai/workspace/OpenSAR/binaries/gtk/example/main.svl", "r");
	}
	else
	{
		Arch_Trace("\n\n## Load Script: %s\n\n",file);
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

void ArScriptRun(void)
{
	Arch_Trace("\n\n// ==================== [ Script Run Start ] ====================\n\n");

	runtime_t::get_instance()->interpretate();

	Arch_Trace("\n\n// ==================== [ Script Run  End  ] ====================\n\n");
}

