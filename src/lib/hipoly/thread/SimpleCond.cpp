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

#include <SimpleCond.hpp>
#include <ErrorSystem.hpp>

#ifdef HAVE_PTHREAD_H
#include  <SimpleCondPThread.hpp>
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSimpleCond::CSimpleCond(void)
{
    SecretImpl = NULL;

#ifdef HAVE_WIN32_THREADS_H
    RUNTIME_ERROR("not implemented");
#endif

#ifdef HAVE_PTHREAD_H
    SecretImpl = new CSimpleCondPThread;
#endif
}

//------------------------------------------------------------------------------

CSimpleCond::~CSimpleCond(void)
{
    if( SecretImpl ) delete SecretImpl;
    SecretImpl = NULL;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSimpleCond::WaitForSignal(CSimpleMutex& mutex)
{
    if( SecretImpl == NULL ) RUNTIME_ERROR("SecretImpl is NULL");
    return( SecretImpl->WaitForSignal(mutex) );
}

//------------------------------------------------------------------------------

bool CSimpleCond::Signal(void)
{
    if( SecretImpl == NULL ) RUNTIME_ERROR("SecretImpl is NULL");
    return( SecretImpl->Signal() );
}

//------------------------------------------------------------------------------

bool CSimpleCond::BroadcastSignal(void)
{
    if( SecretImpl == NULL ) RUNTIME_ERROR("SecretImpl is NULL");
    return( SecretImpl->BroadcastSignal() );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
