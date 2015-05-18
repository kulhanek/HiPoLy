#ifndef SmartThreadH
#define SmartThreadH
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

//------------------------------------------------------------------------------

#ifndef _THR_INFINITE
#define _THR_INFINITE -1
#endif

//------------------------------------------------------------------------------

/// smart thread class

class HIPOLY_PACKAGE CSmartThread {
public:
    CSmartThread(void);
    virtual ~CSmartThread(void);

// thread manipulation ---------------------------------------------------------
    /// start execution of thread
    bool StartThread(void);

    /// set ThreadTerminated to true
    void TerminateThread(void);

    /// wait until thread is finished
    EThreadWaitFor WaitForThread(int msectime=_THR_INFINITE);

    /// return thread status
    EThreadStatus GetThreadStatus(void);

// main entry point ------------------------------------------------------------
    /// main execution entry
    virtual void ExecuteThread(void);

// section of private data -----------------------------------------------------
protected:
    // this variable indicates thread termination upon user request
    // implementation of ExecuteThread should respond on its change
    bool                ThreadTerminated;

// section of private data -----------------------------------------------------
private:
    EThreadStatus       Status;         // thread status
    CSmartThreadCommon* SecretImpl;
    friend class CSmartThreadPThread;
};

//---------------------------------------------------------------------------

#endif
