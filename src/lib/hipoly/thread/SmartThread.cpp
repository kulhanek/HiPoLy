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

#include <SmartThread.hpp>
#include <ErrorSystem.hpp>
#include <SmartThreadPThread.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmartThread::CSmartThread(void)
{
    ThreadTerminated = false;
    Status = ETS_PREPARED;
    SecretImpl = NULL;

#ifdef HAVE_WIN32_THREADS_H
    RUNTIME_ERROR("not implemented");
#endif

#ifdef HAVE_PTHREAD_H
    SecretImpl = new CSmartThreadPThread(this);
#endif
}

// -----------------------------------------------------------------------------

CSmartThread::~CSmartThread(void)
{
    if( SecretImpl != NULL ) delete SecretImpl;
    SecretImpl = NULL;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSmartThread::StartThread(void)
{
    if( SecretImpl == NULL ) RUNTIME_ERROR("SecretImpl is NULL");

    if( (Status == ETS_STARTING) || (Status == ETS_RUNNING) ) {
        ES_ERROR("thread is not prepared or finished (e.g. it is running, ...)");
        return(false);
    }

    Status = ETS_STARTING;
    ThreadTerminated = false;

    if( SecretImpl->StartThread() == false ){
        ES_ERROR("unable to start thread");
        Status = ETS_PREPARED;
        return(false);
    }

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmartThread::ExecuteThread(void)
{
    // nothing to be here
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmartThread::TerminateThread(void)
{
    ThreadTerminated = true;
}

//------------------------------------------------------------------------------

EThreadStatus CSmartThread::GetThreadStatus(void)
{   
    if( SecretImpl == NULL ) RUNTIME_ERROR("SecretImpl is NULL");
    return( SecretImpl->GetThreadStatus() );
}

//------------------------------------------------------------------------------

EThreadWaitFor CSmartThread::WaitForThread(int msectime)
{
    if( SecretImpl == NULL ) RUNTIME_ERROR("SecretImpl is NULL");
    return( SecretImpl->WaitForThread(msectime) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


