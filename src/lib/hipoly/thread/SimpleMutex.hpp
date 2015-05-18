#ifndef SimpleMutexH
#define SimpleMutexH
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

#include <HiPoLyMainHeader.hpp>
#include <SimpleMutexCommon.hpp>

//------------------------------------------------------------------------------

/// brief simple mutex implementation

class HIPOLY_PACKAGE CSimpleMutex {
public:
    CSimpleMutex(void);
    ~CSimpleMutex(void);

// main methods ---------------------------------------------------------------
    /// lock mutex
    bool Lock(void);

    /// unlock mutex
    bool Unlock(void);

// section of private data ----------------------------------------------------
private:
    CSimpleMutexCommon* SecretImpl;
    friend class CSimpleCondPThread;
};

//------------------------------------------------------------------------------
#endif
