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

#ifndef _WAIT_H
#define _WAIT_H

#include <QtCore>

#ifdef Q_OS_WIN32
#include "windows.h"
#endif

class Wait
{
  public:

	Wait();
	virtual ~Wait();

	void WaitMsec(unsigned int msec);
	void WaitUsec(unsigned int usec);

	void SetBogoKips();

	int GetHwTimer() const
	{
		return htimer;
	}
	void SetHwTimer(int ok = -1);
	int CheckHwTimer();

  protected:

	int GetBogoKips();

  private:

	static int bogokips;
	static int htimer;

#ifdef Q_OS_WIN32
	static LARGE_INTEGER mlpf;
#endif
};
#endif
