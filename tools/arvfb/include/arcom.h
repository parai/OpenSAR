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

// In fact, arvfb is a Virtual Function Bus simulator, it is in charge of
// the data transition from one ECU to the other.
// So arcom only need to display the signals on the VFB, it is not in charge
// of signals' transition, but for debug purpose of AUTOSAR module,
// Somehow, arcom will both display signals and in charge of signals' transition.
#ifndef ARCOM_H_
#define ARCOM_H_


GtkWidget* ArCom(void);

void ArCom_Init(void);
void ArCom_Schedule(void);

void ArCom_RxIndication(const ArCanMsgType* armsg);

void ArCom_DefinePdu( char* Name,uint32 Identifier,uint8 BusID,boolean IsTxEnabled,uint32 Period);

void ArCom_DefineSignal( char* Name,uint8 StartBit,uint8 BitSize,uint32 DefaultValue);

#endif /* ARCOM_H_ */
