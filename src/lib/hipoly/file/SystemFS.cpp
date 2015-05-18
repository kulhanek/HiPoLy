//==============================================================================
//    Copyright 2008 Petr Kulhanek
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

#include <SystemFS.hpp>
#include <stdlib.h>

#if defined _WIN32 || defined __CYGWIN__
#include <shlobj.h>
#else
#include "../binreloc/prefix.h"
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

const CFileName CSystemFS::GetProgramDir(void)
{
    CFileName dir;
#if defined _WIN32 || defined __CYGWIN__
    char path_buffer[MAX_PATH];
    memset(path_buffer,0,MAX_PATH);
    HMODULE hmod = GetModuleHandle(NULL);
    if( hmod != NULL ) {
        GetModuleFileName(hmod,path_buffer,MAX_PATH);
        dir = CFileName(path_buffer).GetFileDirectory();
        dir = dir.GetFileDirectory();
    }
#else
    dir = PREFIX;
#endif
    return(dir);
}

//------------------------------------------------------------------------------

const CFileName CSystemFS::GetProgramSubDir(const CFileName& sub_dir)
{
    return( GetProgramDir() / sub_dir );
}

//------------------------------------------------------------------------------

const CFileName CSystemFS::GetUserSetupDir(void)
{
    CFileName dir;

#if defined _WIN32 || defined __CYGWIN__
    char path_buffer[MAX_PATH];
    memset(path_buffer,0,MAX_PATH);
    SHGetFolderPath(NULL,CSIDL_APPDATA,NULL,0,path_buffer);
    dir = path_buffer;
#else
    dir = CSystemFS::GetSystemVariable("HOME");
#endif

    return(dir);
}

//------------------------------------------------------------------------------

const CFileName CSystemFS::GetUserDocumentsDir(void)
{
    CFileName dir;

#if defined _WIN32 || defined __CYGWIN__
    char path_buffer[MAX_PATH];
    memset(path_buffer,0,MAX_PATH);
    SHGetFolderPath(NULL,CSIDL_APPDATA,NULL,0,path_buffer);
    dir = path_buffer;
#else
    dir = CSystemFS::GetSystemVariable("HOME");
#endif

    return(dir);
}

//------------------------------------------------------------------------------

const CFileName  CSystemFS::GetLibraryPrefix(void)
{
    CFileName name("lib");
    return(name);
}

//------------------------------------------------------------------------------

const CFileName  CSystemFS::GetLibraryAffix(void)
{
#if defined _WIN32 || defined __CYGWIN__
    CFileName name(".dll");
#else
    CFileName name(".so");
#endif
    return(name);
}

//------------------------------------------------------------------------------

const CSmallString CSystemFS::GetSystemVariable(const CSmallString& name)
{
    if( name == NULL ) return("");
    return(CSmallString(getenv(name)));
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
