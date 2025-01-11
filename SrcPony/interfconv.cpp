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

#include <QString>
#include <QStringList>
#include <QDebug>

#include "types.h"
#include "globals.h"
#include "interfconv.h"

struct Interf2Index
{
	int vector;
	int index;
	QString name;
	HInterfaceType type;
	VidPid vp;
	InterfPins pins;
	int port;
};

//RaspberryPi default pins
#define DEF_GPIO_CTRL			23  //Rst pin 16
#define DEF_GPIO_DATAIN			27  //Miso pin 13
#define DEF_GPIO_DATAOUT		17  //Mosi pin 11
#define DEF_GPIO_CLOCK			24  //Clock pin 18

#define DEF_MPSSE_CTRL			9  //ACBUS1
#define DEF_MPSSE_DATAIN		2  //ADBUS2
#define DEF_MPSSE_DATAOUT		1  //ADBUS1
#define DEF_MPSSE_CLOCK			0  //ADBUS0

static QVector<Interf2Index> index_interface =
{
	//Interfaces vector
	{INTERF_COM, 0, "SI-ProgAPI", SIPROG_API},
	{INTERF_COM, 1, "SI-ProgI/O", SIPROG_IO},
	{INTERF_COM, 2, "JDM-API", JDM_API},
	{INTERF_LPT, 0, "AvrISP-API", AVRISP},
	{INTERF_LPT, 1, "AvrISP-I/O", AVRISP_IO},
	{INTERF_LPT, 2, "DT-006-API", DT006_API},
	{INTERF_LPT, 3, "DT-006-I/O", DT006_IO},
	{INTERF_LPT, 4, "EasyI2C-API", EASYI2C_API},
	{INTERF_LPT, 5, "EasyI2C-I/O", EASYI2C_IO},
	{INTERF_USB, 0, "FTDI PonyProgFT", PONYPROG_FT, VidPid(0x0403, 0x6e38), {3, 2, 1, 0, /* 4 */ -1, 5, 7, 6}, FTDI_PORTA},
	{INTERF_USB, 1, "FTDI JtagKey", FTDI_JTAGKEY, VidPid(0x0403, 0xcff8), {9, 2, 1, 0, -1, -1, -1, -1}, FTDI_PORTA},
	{INTERF_USB, 2, "EGGLINK V3", EGGLINK_V3, VidPid(0x0403, 0x69e0), {3, 2, 1, 0, /* 4 */ -1, 5, 7, 6}, FTDI_PORTB},
	{INTERF_GPIO, 0, "Linux SysFs GPIO", LINUXSYSFS_IO, VidPid(), {23, 27, 17, 24, -1, -1, -1, -1}},
	//{INTERF_GPIO, 1, "PonyProgRPi", PONYPROG_RPI},
};

int TypeToInterfPort(HInterfaceType type)
{
	for (int k = 0; k < index_interface.count(); k++)
	{
		if (index_interface.at(k).type == type)
		{
			return index_interface.at(k).port;
		}
	}

	return -1;
}

bool TypeToInterfPins(HInterfaceType type, InterfPins &pins)
{
	for (int k = 0; k < index_interface.count(); k++)
	{
		if (index_interface.at(k).type == type)
		{
			pins = index_interface.at(k).pins;
			return true;
		}
	}

	return false;
}

bool TypeToInterfVidPid(HInterfaceType type, unsigned int &vid, unsigned int &pid)
{
	for (int k = 0; k < index_interface.count(); k++)
	{
		if (index_interface.at(k).type == type)
		{
			vid = index_interface.at(k).vp.vid;
			pid = index_interface.at(k).vp.pid;
			return true;
		}
	}

	return false;
}

VidPid TypeToInterfVidPid(HInterfaceType type)
{
	VidPid rv;

	for (int k = 0; k < index_interface.count(); k++)
	{
		if (index_interface.at(k).type == type)
		{
			rv = index_interface.at(k).vp;
			break;
		}
	}

	return rv;
}

QStringList GetInterfList(int vector)
{
	QStringList lst;

	for (int k = 0; k < index_interface.count(); k++)
	{
		if (index_interface.at(k).vector == vector)
		{
			lst << index_interface.at(k).name;
		}
	}

	return lst;
}

HInterfaceType NameToInterfType(const QString &name)
{
	if (name.length())
	{
		QString n = name;
		n.remove(QChar('-'));
		n.remove(QChar(' '));
		n.remove(QChar('/'));

		for (int k = 0; k < index_interface.count(); k++)
		{
			QString v = index_interface.at(k).name;
			v.remove(QChar('-'));
			v.remove(QChar(' '));
			v.remove(QChar('/'));

			if (v.indexOf(n) >= 0)
			{
				return index_interface.at(k).type;
			}
		}
	}

	return LAST_HT;
}

QString TypeToInterfName(HInterfaceType type)
{
	for (int k = 0; k < index_interface.count(); k++)
	{
		if (index_interface.at(k).type == type)
		{
			return index_interface.at(k).name;
		}
	}

	return "";
}

int TypeToInterfVector(HInterfaceType type)
{
	for (int k = 0; k < index_interface.count(); k++)
	{
		if (index_interface.at(k).type == type)
		{
			return index_interface.at(k).vector;
		}
	}

	return -1;
}

int TypeToInterfIndex(HInterfaceType type)
{
	for (int k = 0; k < index_interface.count(); k++)
	{
		if (index_interface.at(k).type == type)
		{
			return index_interface.at(k).index;
		}
	}

	return -1;
}

HInterfaceType VindexToInterfType(int vector, int index)
{
	for (int k = 0; k < index_interface.count(); k++)
	{
		if (index_interface.at(k).vector == vector &&
				index_interface.at(k).index == index)
		{
			return index_interface.at(k).type;
		}
	}

	return LAST_HT;
}
