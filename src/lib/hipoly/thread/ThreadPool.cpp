//==============================================================================
//    Copyright 2007,2008 Petr Kulhanek
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

#include <ThreadPool.hpp>
#include <SimpleIterator.hpp>
#include <ErrorSystem.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CThreadPool::CTPThread::CTPThread(CThreadPool* p_owner)
{
    Owner =  p_owner;
}

//------------------------------------------------------------------------------

CThreadPool::CTPThread::~CTPThread(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CThreadPool::CTPThread::ExecuteThread(void)
{
    for(;;) {
        CRunnable* p_task = NULL;

        Owner->AccessMutex.Lock();
        if( ThreadTerminated == true ) {
            Owner->AccessMutex.Unlock();
            break;
        }

        // check for new task
        if( Owner->Tasks.NumOfMembers() > 0 ) {
            p_task = Owner->Tasks.RemoveFromBegin();
        } else {
            // wait for other job
            Owner->WaitCond.WaitForSignal(Owner->AccessMutex);
            Owner->AccessMutex.Unlock();
            continue;
        }

        // execute task
        if( p_task != NULL ) {
            Owner->NumOfActiveThreads++;

            Owner->AccessMutex.Unlock();
            p_task->Run();
            if( p_task->ShouldBeAutoDeleted() ) {
                delete p_task;
                p_task = NULL;
            }
            Owner->AccessMutex.Lock();

            Owner->NumOfActiveThreads--;
        }

        Owner->AccessMutex.Unlock();
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CThreadPool::CThreadPool(void)
{
    MaxThreadCount = 0;
    NumOfActiveThreads = 0;
    WaitForDoneState = false;
}

//------------------------------------------------------------------------------

CThreadPool::~CThreadPool(void)
{
    WaitForDone();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CThreadPool::SetMaxThreadCount(int maxThreadCount)
{
    if( maxThreadCount < 0 ) {
        ES_ERROR("number of threads cannot be lower than zero");
        return(false);
    }

    MaxThreadCount = maxThreadCount;
    return(true);
}

//------------------------------------------------------------------------------

int  CThreadPool::GetMaxThreadCount(void)
{
    return(MaxThreadCount);
}

//------------------------------------------------------------------------------

int  CThreadPool::GetActiveThreadCount(void)
{
    return(NumOfActiveThreads);
}

//------------------------------------------------------------------------------

void CThreadPool::AddTask(CRunnable* p_runnable)
{
    if( p_runnable == NULL ) return;

    AccessMutex.Lock();
    if( WaitForDoneState ) {
        // do not accept any new jobs
        if( p_runnable->ShouldBeAutoDeleted() ) {
            delete p_runnable;
        }
        AccessMutex.Unlock();
        return;
    }

    // create new thread if necessary
    if( Threads.NumOfMembers() < MaxThreadCount ) {
        CTPThread* p_thread = new CTPThread(this);
        Threads.InsertToEnd(p_thread,0,true);
        p_thread->StartThread();
    }

    // add task into queue
    Tasks.InsertToEnd(p_runnable);

    // wake up any sleeping thread
    WaitCond.Signal();

    AccessMutex.Unlock();
}

//------------------------------------------------------------------------------

void CThreadPool::WaitForDone(void)
{
    AccessMutex.Lock();
    WaitForDoneState = true;

    CRunnable*  p_task;

    while( (p_task = Tasks.RemoveFromBegin()) != NULL ) {
        if( p_task->ShouldBeAutoDeleted() ) {
            delete p_task;
            p_task = NULL;
        }
    }

    CSimpleIterator<CTPThread>  I(Threads);
    CTPThread*  p_thread;

    while( (p_thread = I.Current()) != NULL ) {
        p_thread->TerminateThread();
        I++;
    }

    WaitCond.BroadcastSignal();
    AccessMutex.Unlock();

    // now wait
    p_thread = NULL;

    do {
        if( p_thread != NULL ) delete p_thread;
        AccessMutex.Lock();
        p_thread = Threads.RemoveFromBegin(false);
        AccessMutex.Unlock();
        if( p_thread != NULL ) p_thread->WaitForThread();
    } while( p_thread != NULL );


    AccessMutex.Lock();
    WaitForDoneState = false;
    AccessMutex.Unlock();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
