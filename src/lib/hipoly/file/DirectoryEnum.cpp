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

#include <DirectoryEnum.hpp>
#include <ErrorSystem.hpp>
#include <errno.h>
#include <string.h>

#if defined _WIN32 || defined __CYGWIN__
#else
#include <fnmatch.h>
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CDirectoryEnum::CDirectoryEnum(void)
{
#if defined _WIN32 || defined __CYGWIN__
    DirFD = INVALID_HANDLE_VALUE;
    Result = FALSE;
#else
    DirFD = NULL;
#endif
}

//------------------------------------------------------------------------------

CDirectoryEnum::CDirectoryEnum(const CFileName& dirname)
{
    DirName = dirname;
#if defined _WIN32 || defined __CYGWIN__
    DirFD = INVALID_HANDLE_VALUE;
    Result = FALSE;
#else
    DirFD = NULL;
#endif
}

//------------------------------------------------------------------------------

CDirectoryEnum::~CDirectoryEnum(void)
{
    // release all items
    EndFindFile();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CDirectoryEnum::SetDirName(const CFileName& dirname)
{
    DirName = dirname;
    // release previous data
    EndFindFile();
    return(true);
}

//------------------------------------------------------------------------------

const CFileName& CDirectoryEnum::GetDirName(void) const
{
    return(DirName);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CDirectoryEnum::StartFindFile(const CFileName& filter)
{
    Filter = filter;

    // release previous data
    EndFindFile();

#if defined _WIN32 || defined __CYGWIN__
    CFileName dir_search = DirName / filter;
    DirFD = FindFirstFile(dir_search,&Data);
    if( DirFD == INVALID_HANDLE_VALUE ) {
        Result = FALSE;
        return(false);
    } else {
        Result = TRUE;
        return(true);
    }
#else
    // open directory
    DirFD = opendir(DirName);
    if( DirFD == NULL ) {
        CSmallString error;
        error << "'" << DirName << "' (" << strerror(errno) << ")";
        ES_ERROR(error);
        return(false);
    }
    return(true);
#endif
}

//------------------------------------------------------------------------------

bool CDirectoryEnum::FindFile(CFileName& file)
{
#if defined _WIN32 || defined __CYGWIN__
    if( DirFD == INVALID_HANDLE_VALUE ) return(false);
    if( Result == TRUE ) {
        file = Data.cFileName;
    } else {
        return(false);
    }
    Result = FindNextFile(DirFD,&Data);
    return(true);
#else
    if( DirFD == NULL ) {
        ES_ERROR("DirFD is NULL");
        return(false);
    }
    struct dirent* p_dirent;
    while( (p_dirent = readdir(DirFD)) != NULL ) {
        // does name satisfy filter condition?
        if( fnmatch(Filter,p_dirent->d_name,0) == 0 ) {
            file = p_dirent->d_name;
            return(true);
        }
    }
    // not any other entry
    return(false);
#endif

}

//------------------------------------------------------------------------------

bool CDirectoryEnum::EndFindFile(void)
{
#if defined _WIN32 || defined __CYGWIN__
    if( DirFD == INVALID_HANDLE_VALUE ) FindClose(DirFD);
    DirFD = INVALID_HANDLE_VALUE;
    Result = FALSE;
    return(true);
#else
    if( DirFD == NULL ) return(true);
    if( closedir(DirFD) != 0 ) {
        DirFD = NULL;
        ES_ERROR(strerror(errno));
        return(false);
    }
    DirFD = NULL;
    return(true);
#endif
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
