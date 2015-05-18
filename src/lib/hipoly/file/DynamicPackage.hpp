#ifndef DynamicPackageH
#define DynamicPackageH
//==============================================================================
//    Copyright 2001,2002,2003,2004,2006,2008 Petr Kulhanek
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
#include <SmallString.hpp>

#if defined _WIN32 || defined __CYGWIN__
#include <windows.h>
#endif

//------------------------------------------------------------------------------

// handle to method
typedef void (*TProcAdress)(void);

//------------------------------------------------------------------------------

#if ! (defined _WIN32 || defined __CYGWIN__)
// warning: ISO C++ forbids casting between pointer-to-function and pointer-to-object
union __CDPU__ {
    TProcAdress p2f;
    void*       p2d;
};
#endif

//------------------------------------------------------------------------------

/// access to dynamic library

class HIPOLY_PACKAGE CDynamicPackage {
public:
// constructor/destructor -----------------------------------------------------
    /// constructor
    CDynamicPackage(void);
    /// destructor - unload package
    ~CDynamicPackage(void);

// IO methods -----------------------------------------------------------------
    /// load package with name
    bool Open(const CSmallString& name);
    /// unload package
    bool Close(void);

// general methods -----------------------------------------------------------
    /// check if package is loaded
    bool IsOpen(void);

    /// return LibraryHandle as void*
    operator const void*(void) const;

    /// get adress of point from package
    TProcAdress GetProcAddress(const CSmallString& name);

    /// get address of point from package
    void* GetObjAddress(const CSmallString& name);

// section of private data ----------------------------------------------------
private:
    CSmallString    LibraryName;
#if defined _WIN32 || defined __CYGWIN__
    HMODULE LibraryHandle;  // library handle
#else
    void*   LibraryHandle;  // library handle
#endif
};

//------------------------------------------------------------------------------

#endif
