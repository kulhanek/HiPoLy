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

#include <SimpleMutexPThread.hpp>
#include <ErrorSystem.hpp>
#include <string.h>
#include <errno.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSimpleMutexPThread::CSimpleMutexPThread(void)
{
    int result = pthread_mutex_init(&simple_mutex,NULL);
    if( result == 0 ) return;
    ES_ERROR(strerror(errno));
}

//------------------------------------------------------------------------------

CSimpleMutexPThread::~CSimpleMutexPThread(void)
{
    if( pthread_mutex_trylock(&simple_mutex) == 0 ) {
        pthread_mutex_unlock(&simple_mutex);
    } else {
        if( errno == EBUSY ) {
            ES_ERROR("mutex is still locked");
        }
    }
    int result = pthread_mutex_destroy(&simple_mutex);
    if( result == 0 ) return;
    ES_ERROR(strerror(errno));
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSimpleMutexPThread::Lock(void)
{
    int result = pthread_mutex_lock(&simple_mutex);
    if( result == 0 ) return(true);
    ES_ERROR(strerror(errno));
    return(false);
}

//------------------------------------------------------------------------------

bool CSimpleMutexPThread::Unlock(void)
{
    int result = pthread_mutex_unlock(&simple_mutex);
    if( result == 0 ) return(true);
    ES_ERROR(strerror(errno));
    return(false);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
