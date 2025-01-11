//=========================================================================//
//                                                                         //
//  PonyProg - Serial Device Programmer                                    //
//                                                                         //
//  Copyright (C) 1997-2025   Claudio Lanconelli                           //
//                                                                         //
//  https://github.com/lancos/ponyprog                                        //
//                                                                         //
//-------------------------------------------------------------------------//
//                                                                         //
// This program is free software; you can redistribute it and/or           //
// modify it under the terms of the GNU  General Public License            //
// as published by the Free Software Foundation; either version2 of        //
// the License, or (at your option) any later version.                     //
//                                                                         //
// This program is distributed in the hope that it will be useful,         //
// but WITHOUT ANY WARRANTY; without even the implied warranty of          //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       //
// General Public License for more details.                                //
//                                                                         //
// You should have received a copy of the GNU  General Public License      //
// along with this program (see LICENSE);     if not, write to the         //
// Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. //
//                                                                         //
//=========================================================================//

#ifndef INTERFCONV_H
#define INTERFCONV_H

#include "globals.h"

enum
{
	FTDI_PORTA = 0,
	FTDI_PORTB,
	FTDI_PORTC,
	FTDI_PORTD
};

struct InterfPins
{
	int ctrl;
	int datain;
	int dataout;
	int clock;
	int clockin;
	int poweron;
	int enbus;
	int ctrlin;
};

enum
{
	//Directly correspond to setup interface dialog tab index
	INTERF_USB = 0,
	INTERF_COM,
	INTERF_LPT,
	INTERF_GPIO
};

extern QStringList GetInterfList(int vector);
extern HInterfaceType NameToInterfType(const QString &name);
extern QString TypeToInterfName(HInterfaceType type);
extern int TypeToInterfVector(HInterfaceType type);
extern int TypeToInterfIndex(HInterfaceType type);
extern HInterfaceType VindexToInterfType(int vector, int index);
extern bool TypeToInterfVidPid(HInterfaceType type, unsigned int &vid, unsigned int &pid);
extern VidPid TypeToInterfVidPid(HInterfaceType type);
extern bool TypeToInterfPins(HInterfaceType type, InterfPins &pins);
extern int TypeToInterfPort(HInterfaceType type);

#endif // INTERFCONV_H
