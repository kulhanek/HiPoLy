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

#include <DynamicPackage.hpp>
#include <ErrorSystem.hpp>

#if ! (defined _WIN32 || defined __CYGWIN__)
#include <dlfcn.h>
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CDynamicPackage::CDynamicPackage(void)
{
    LibraryHandle = NULL;
}

//------------------------------------------------------------------------------

CDynamicPackage::~CDynamicPackage(void)
{
    if( LibraryHandle == NULL ) return;

#if defined _WIN32 || defined __CYGWIN__
    FreeLibrary(LibraryHandle);
#else
    dlclose(LibraryHandle);
#endif
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CDynamicPackage::Open(const CSmallString& name)
{
    if( name == NULL ) return(false);  // name was not specified

    if( LibraryHandle != NULL ) Close();   // close previously opened library

#if defined _WIN32 || defined __CYGWIN__
    try {
        SetErrorMode(0);
        LibraryHandle = LoadLibrary(name);
        if( LibraryHandle != NULL ){
            LibraryName = name;
        } else {
            DWORD error = GetLastError();
            CSmallString serr;
            serr << "LoadLibrary return code = " << error;
            ES_ERROR(serr);
        }
    } catch(...) {
    }
#else
    LibraryHandle = dlopen(name,RTLD_NOW|RTLD_GLOBAL);
    LibraryName = name;
    if( LibraryHandle == NULL ) {
        ES_ERROR(dlerror());
    }
#endif

    return( LibraryHandle != NULL);
}

//------------------------------------------------------------------------------

bool CDynamicPackage::Close(void)
{
    if( LibraryHandle == NULL ) return(false); // package is not opened

    LibraryName = NULL;

    int result = 0;

#if defined _WIN32 || defined __CYGWIN__
    FreeLibrary(LibraryHandle);
#else
    result = dlclose(LibraryHandle);
#endif

    LibraryHandle = NULL;
    return(result == 0);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CDynamicPackage::IsOpen(void)
{
    return(LibraryHandle != NULL);
}

//------------------------------------------------------------------------------

CDynamicPackage::operator const void*(void) const
{
    return((void*)LibraryHandle);
}

//------------------------------------------------------------------------------

TProcAdress CDynamicPackage::GetProcAddress(const CSmallString& name)
{
    if( LibraryHandle == NULL ) return(NULL);

#if defined _WIN32 || defined __CYGWIN__
    return((TProcAdress)::GetProcAddress(LibraryHandle,name));
#else
    // warning: ISO C++ forbids casting between pointer-to-function and pointer-to-object
    // this stupid thing bypass this warning
    __CDPU__ ret_value;
    ret_value.p2d = dlsym(LibraryHandle,name);
    return(ret_value.p2f);
#endif
}

//------------------------------------------------------------------------------

void* CDynamicPackage::GetObjAddress(const CSmallString& name)
{
    if( LibraryHandle == NULL ) return(NULL);

#if defined _WIN32 || defined __CYGWIN__
    return((void*)::GetProcAddress(LibraryHandle,name));
#else
    // warning: ISO C++ forbids casting between pointer-to-function and pointer-to-object
    // this stupid thing bypass this warning
    __CDPU__ ret_value;
    ret_value.p2d = dlsym(LibraryHandle,name);
    return(ret_value.p2d);
#endif
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

