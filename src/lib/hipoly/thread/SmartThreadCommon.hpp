#ifndef SmartThreadCommonH
#define SmartThreadCommonH
//==============================================================================
//    Copyright 2012 Petr Kulhanek
//
//    This file is part of HiPoLy library.
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor,
//    Boston, MA  02110-1301  USA
//==============================================================================

#include <HiPoLyMainHeader.hpp>

//------------------------------------------------------------------------------

enum EThreadStatus {
    ETS_UNKNOWN  = -1,          // unknown status
    ETS_PREPARED =  0,          // thread is prepared for run
    ETS_STARTING =  1,          // thread is starting
    ETS_RUNNING  =  2,          // thread is running
    ETS_FINISHED =  3           // thread was finished
};

//------------------------------------------------------------------------------

enum EThreadWaitFor {
    ETWF_ERROR      = -1,       // an error occured in WaitForThread
    ETWF_TERMINATED =  0,       // thread is either prepared or terminated
    ETWF_TIMEDOUT   =  1        // thread was not completed till timeout
};

//------------------------------------------------------------------------------

/// smart thread class - common class

class CSmartThreadCommon {
public:
    CSmartThreadCommon(void);
    virtual ~CSmartThreadCommon(void);

    /// start execution of thread
    virtual bool StartThread(void);

    /// wait until thread is finished
    virtual EThreadWaitFor WaitForThread(int msectime);

    /// return thread status
    virtual EThreadStatus GetThreadStatus(void);
};

//---------------------------------------------------------------------------

#endif
