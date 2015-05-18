#ifndef SmartThreadPThreadH
#define SmartThreadPThreadH
//==============================================================================
//    Copyright 2006,2008 Petr Kulhanek
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
#include <SmartThreadCommon.hpp>
#include <pthread.h>

//------------------------------------------------------------------------------

class CSmartThread;

//------------------------------------------------------------------------------

/// simple thread class - pthread implemetation

class CSmartThreadPThread : public CSmartThreadCommon {
public:
    CSmartThreadPThread(CSmartThread* p_owner);
    virtual ~CSmartThreadPThread(void);

    /// start execution of thread
    virtual bool StartThread(void);

    /// wait until thread is finished
    virtual EThreadWaitFor WaitForThread(int msectime);

    /// return thread status
    virtual EThreadStatus GetThreadStatus(void);

// section of private data -----------------------------------------------------
private:
    CSmartThread*       SmartThread;
    pthread_t           ThreadID;       // thread ID
    pthread_mutex_t     WaitMutex;      // wait mutex
    pthread_cond_t      WaitCond;       // wait condition

    // entry point for thread
    static void* ThreadFunc(void* p_class);
};

//---------------------------------------------------------------------------

#endif
