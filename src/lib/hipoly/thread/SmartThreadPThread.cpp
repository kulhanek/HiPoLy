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

#include <SmartThreadPThread.hpp>
#include <ErrorSystem.hpp>
#include <SmartThread.hpp>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#define NSEC_PER_USEC 1000

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmartThreadPThread::CSmartThreadPThread(CSmartThread* p_owner)
{
    SmartThread = p_owner;

    int result;
    result = pthread_mutex_init(&WaitMutex,NULL);
    if( result != 0 ){
        CSmallString error;
        error << "unable to create mutex: " << strerror(errno);
        RUNTIME_ERROR(error);
    }

    result = pthread_cond_init(&WaitCond,NULL);
    if( result != 0 ){
        CSmallString error;
        error << "unable to create cond: " << strerror(errno);
        RUNTIME_ERROR(error);
    }
}

// -----------------------------------------------------------------------------

CSmartThreadPThread::~CSmartThreadPThread(void)
{
    SmartThread->TerminateThread();
    WaitForThread(_THR_INFINITE);

    int result;
    result = pthread_cond_destroy(&WaitCond);
    if( result != 0 ){
        CSmallString error;
        error << "unable to destroy cond: " << strerror(errno);
        RUNTIME_ERROR(error);
    }

    result = pthread_mutex_destroy(&WaitMutex);
    if( result != 0 ){
        CSmallString error;
        error << "unable to destroy mutex: " << strerror(errno);
        RUNTIME_ERROR(error);
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSmartThreadPThread::StartThread(void)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int result;
    result = pthread_create(&ThreadID,&attr,CSmartThreadPThread::ThreadFunc,this);
    pthread_attr_destroy(&attr);

    if( result != 0 ) {
        CSmallString error;
        error << "unable to creaate thread: " << strerror(errno);
        ES_ERROR(error);
        return(false);
    }

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void* CSmartThreadPThread::ThreadFunc(void* p_class)
{
    CSmartThreadPThread* p_thclass = static_cast<CSmartThreadPThread*>(p_class);
    if( p_thclass == NULL ) return(NULL);

    pthread_mutex_lock(&p_thclass->WaitMutex);
    p_thclass->SmartThread->Status = ETS_RUNNING;
    pthread_mutex_unlock(&p_thclass->WaitMutex);

    try {
        // execute thread
        p_thclass->SmartThread->ExecuteThread();
    } catch(...) {
        ES_ERROR("uncatched exception in thread");
    }

    pthread_mutex_lock(&p_thclass->WaitMutex);
    p_thclass->SmartThread->Status = ETS_FINISHED;
    pthread_cond_broadcast(&p_thclass->WaitCond);
    pthread_mutex_unlock(&p_thclass->WaitMutex);

    return(NULL);
}

//------------------------------------------------------------------------------

EThreadStatus CSmartThreadPThread::GetThreadStatus(void)
{   
    // mutex is necessary because of change of status in CSmartThreadPThread::ThreadFunc
    pthread_mutex_lock(&WaitMutex);
    EThreadStatus st = SmartThread->Status;
    pthread_mutex_unlock(&WaitMutex);
    return(st);
}

//------------------------------------------------------------------------------

EThreadWaitFor CSmartThreadPThread::WaitForThread(int msectime)
{
    pthread_mutex_lock(&WaitMutex);

    if( SmartThread->Status == ETS_PREPARED ) {
        pthread_mutex_unlock(&WaitMutex);
        return(ETWF_TERMINATED);
    }

    timeval     x;
    timespec    ts;

    gettimeofday(&x,NULL);
    ts.tv_sec = x.tv_sec;
    ts.tv_nsec = x.tv_usec*NSEC_PER_USEC;

    ts.tv_sec += msectime / 1000;
    ts.tv_nsec += msectime % 1000  * 1000000;

    while(SmartThread->Status != ETS_FINISHED) {
        int result;
        if( msectime == _THR_INFINITE ) {
            result = pthread_cond_wait(&WaitCond, &WaitMutex);
        } else {
            result = pthread_cond_timedwait(&WaitCond, &WaitMutex, &ts);
        }
        if( result == ETIMEDOUT ) {
            pthread_mutex_unlock(&WaitMutex);
            return(ETWF_TIMEDOUT);
        }
        if( result != 0 ) {
            pthread_mutex_unlock(&WaitMutex);
            return(ETWF_ERROR);
        }
    }

    pthread_mutex_unlock(&WaitMutex);
    return(ETWF_TERMINATED);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


