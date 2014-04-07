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
 * Sourrce Open At: https://github.com/parai/OpenSAR/
 */
#ifndef ARSCALC_H_
#define ARSCALC_H_



void arsc_copy(ArsValueType* to,ArsValueType* from);
void arsc_add(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2);
void arsc_sub(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2);
void arsc_plus(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2);
void arsc_div(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2);
void arsc_neg(ArsValueType* out,const ArsValueType* in1);
void arsc_pow(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2);
void arsc_eval(ArsValueType* out,const ArsValueType* fnc,const ArsValueType* param);
#endif /* ARSCALC_H_ */
