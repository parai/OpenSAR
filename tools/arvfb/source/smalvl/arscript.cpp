/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Source Open At: https://github.com/parai/OpenSAR/
 */
#include "analizator.h"
#include "runtime.h"
#include "arvfb.h"
// ==========================[ LOCAL ] =================================

GThread* main_thread = NULL;
static gpointer main_runtime(gpointer param)
{
	Arch_Trace("\n\n// ==================== [ Script Run Start ] ====================\n\n");

	runtime_t::get_instance()->interpretate();

	Arch_Trace("\n\n// ==================== [ Script Run  End  ] ====================\n\n");

	return NULL;
}

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
	if(main_thread != NULL)
	{
		g_thread_unref(main_thread);
	}
	main_thread = g_thread_new("main_runtime",main_runtime,NULL);
}

