//==============================================================================
//    Copyright 2007,2008,2012 Petr Kulhanek
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

#include <SimpleCondPThread.hpp>
#include <SimpleMutexPThread.hpp>
#include <ErrorSystem.hpp>
#include <string.h>
#include <errno.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSimpleCondPThread::CSimpleCondPThread(void)
{
    int result = pthread_cond_init(&simple_cv,NULL);
    if( result == 0 ) return;
    CSmallString error;
    error << "unable to init condition : " << strerror(errno);
    ES_ERROR(error);
}

//------------------------------------------------------------------------------

CSimpleCondPThread::~CSimpleCondPThread(void)
{
    int result = pthread_cond_destroy(&simple_cv);
    if( result == 0 ) return;
    CSmallString error;
    error << "unable to destroy condition : " << strerror(errno);
    ES_ERROR(error);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSimpleCondPThread::WaitForSignal(CSimpleMutex& mutex)
{
    CSimpleMutexCommon* p_smc = mutex.SecretImpl;
    CSimpleMutexPThread* p_smp = dynamic_cast<CSimpleMutexPThread*>(p_smc);
    if( p_smp == NULL ){
        RUNTIME_ERROR("mutex is not internally CSimpleMutexPThread")
    }

    int result = pthread_cond_wait(&simple_cv,&p_smp->simple_mutex);
    if( result == 0 ) return(true);
    ES_ERROR(strerror(errno));
    return(false);
}

//------------------------------------------------------------------------------

bool CSimpleCondPThread::Signal(void)
{
    int result = pthread_cond_signal(&simple_cv);
    if( result == 0 ) return(true);
    ES_ERROR(strerror(errno));
    return(false);
}

//------------------------------------------------------------------------------

bool CSimpleCondPThread::BroadcastSignal(void)
{
    int result = pthread_cond_broadcast(&simple_cv);
    if( result == 0 ) return(true);
    ES_ERROR(strerror(errno));
    return(false);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
