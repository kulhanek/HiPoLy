#ifndef ThreadPoolH
#define ThreadPoolH
//==============================================================================
//    Copyright 2010 Petr Kulhanek
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
#include <SimpleList.hpp>
#include <SmartThread.hpp>
#include <Runnable.hpp>
#include <SimpleMutex.hpp>
#include <SimpleCond.hpp>

//------------------------------------------------------------------------------

class CThreadPool;

//------------------------------------------------------------------------------

/*! \brief thread pool
 *
 */

class HIPOLY_PACKAGE CThreadPool {
public:
    CThreadPool(void);
    ~CThreadPool(void);

// main methods ---------------------------------------------------------------
    /// set max thread count in the pool
    bool SetMaxThreadCount(int maxThreadCount);

    /// get max thread count
    int  GetMaxThreadCount(void);

    /// get number of active threads
    int  GetActiveThreadCount(void);

    /// insert task into queue for further processing
    void AddTask(CRunnable* p_runnable);

    /// waits for each thread to exit and removes all threads from the thread pool
    void WaitForDone(void);

// section of private data ----------------------------------------------------
private:
//------------------------------------------------------------------------------
    class HIPOLY_PACKAGE CTPThread : public CSmartThread {
    public:
        CTPThread(CThreadPool* p_owner);
        ~CTPThread(void);

        // main methods ---------------------------------------------------------------
        /// redefine execution point
        virtual void ExecuteThread(void);

        // section of private data ----------------------------------------------------
    private:
        CThreadPool*    Owner;
    };

    int                             MaxThreadCount;
    int                             NumOfActiveThreads;
    CSimpleList<CTPThread>          Threads;        // pool threads
    CSimpleList<CRunnable>          Tasks;          // task for thread pool
    CSimpleMutex                    AccessMutex;    // synchronization mutex
    CSimpleCond                     WaitCond;       // wait condition
    bool                            WaitForDoneState;

    friend class CThreadPoolThread;
};

//------------------------------------------------------------------------------
#endif
