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

#include "types.h"
#include "globals.h"

#include "e2profil.h"
#include "microbus.h"
#include "errcode.h"

#include "e2cmdw.h"

#include <QDebug>
#include <QtCore>

MicroWireBus::MicroWireBus(BusInterface *ptr)
	: BusIO(ptr)
{
	qDebug() << Q_FUNC_INFO;
}

MicroWireBus::~MicroWireBus()
{
	qDebug() << Q_FUNC_INFO;
}

void MicroWireBus::SetDelay()
{
	int val = E2Profile::GetMicroWireSpeed();
	int n;

	switch (val)
	{
	case TURBO:
		n = 1;		// as fast as your PC can
		break;

	case FAST:
		n = 5;
		break;

	case SLOW:
		n = 50;
		break;

	case VERYSLOW:
		n = 200;
		break;

	case ULTRASLOW:
		n = 800;
		break;

	default:
		n = 10;		//Default (< 100KHz)
		break;
	}

	Q_CHECK_PTR(busI);
	busI->SetDelay(n);

	qDebug() << Q_FUNC_INFO << "=" << n;
}

int MicroWireBus::SendDataBit(int b)
{
	//clearCLK();			//si assicura che SCK low
	//bitDI(b);
	//ShotDelay();
	//setCLK();				//device latch data bit now!
	//ShotDelay();
	//clearCLK();

	int err = OK;
	busI->xferBit(err, b, SPI_MODE_0 | xMODE_WRONLY);

	return err;
}

int MicroWireBus::RecDataBit()
{
	//clearCLK();			//si assicura che SCK low
	//ShotDelay();
	//setCLK();
	//ShotDelay();
	//b = getDO();
	//clearCLK();

	int err = OK;
	int rv = busI->xferBit(err, 1, SPI_MODE_1 | xMODE_RDONLY);
	if (err == OK)
	{
		return rv;
	}
	else
	{
		return err;
	}
}

int MicroWireBus::RecDataBitShort()
{
	clearCLK();				//be sure SCK low
	ShotDelay();
	return getDO();
}

int MicroWireBus::SendDataWord(int wo, int wlen, bool lsb)
{
	int err = OK;
	clearCLK();
	busI->xferWord(err, wo, SPI_MODE_0 | xMODE_WRONLY, wlen, lsb);
	clearDI();

	return err;
}

//Standard Receive data word
int MicroWireBus::RecDataWord(int wlen, bool lsb)
{
	int err = OK;
	clearCLK();
	int rv = busI->xferWord(err, 0xffff, SPI_MODE_1 | xMODE_RDONLY, wlen, lsb);
	if (err == OK)
	{
		return rv;
	}
	else
	{
		return err;
	}
}

//Receive Data word with the first clock pulse shortened.
//  In case of the device doesn't leave a clock pulse to switch
//  from DataOut to DataIn after the command
int MicroWireBus::RecDataWordShort(int wlen, bool lsb)
{
	int valb = 0, valw = 0;

	Q_ASSERT(wlen > 1);

	clearCLK();
	valb = RecDataBitShort() ? 1 : 0;
	valw = RecDataWord(wlen - 1, lsb);

	if (lsb)
	{
		return (valw << 1) | valb;
	}
	else
	{
		return (valb << (wlen - 1)) | valw;
	}
}

int MicroWireBus::WaitReadyAfterWrite(long timeout)
{
	clearCLK();
	ClearReset();		//27/05/98
	ShotDelay();
	SetReset();			//27/05/98
	ShotDelay();
	clearCLK();

	long k;
	for (k = timeout; k > 0 && !getDO(); k--)
	{
		WaitUsec(1);
	}

	qDebug() << Q_FUNC_INFO << "=" << k;

	return k ? OK : E2P_TIMEOUT;
}

int MicroWireBus::Reset(void)
{
	qDebug() << Q_FUNC_INFO;

	SetDelay();

	clearCLK();
	//clearDI();	//27/05/98
	setDI();

	ClearReset();	//Pulse the reset
	WaitMsec(1);	//27/05/98
	SetReset();

	WaitMsec(50);

	return OK;
}

int MicroWireBus::CalcAddressSize(int mem_size) const
{
	if (mem_size-- <= 0)
	{
		return -1;
	}

	int k;

	for (k = 15; k > 0; k--)
	{
		if (mem_size & (1 << k))
		{
			break;
		}
	}

	return k + 1;
}
