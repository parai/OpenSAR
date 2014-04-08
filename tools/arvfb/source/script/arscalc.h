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
 * but WITHOUT ANY WARRANTY; withrvar even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Sourrce Open At: https://github.com/parai/OpenSAR/
 */
#ifndef ARSCALC_H_
#define ARSCALC_H_

// ==================================== [ MACROS    ] ==========================================

void arsc_copy(yvar_t* to,yvar_t* from);
void arsc_add(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2);
void arsc_sub(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2);
void arsc_plus(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2);
void arsc_div(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2);
void arsc_neg(yvar_t* rvar,const yvar_t* var1);
void arsc_pow(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2);
void arsc_read(yvar_t* rvar,const yvar_t* ref);
void arsc_eval(yvar_t* rvar,const yvar_t* fnc,const yvar_t* param);
void arsc_print(yvar_t* var);
#endif /* ARSCALC_H_ */
