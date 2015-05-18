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

#include <StdIOFile.hpp>
#include <ErrorSystem.hpp>
#include <errno.h>
#include <string.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CStdIOFile::CStdIOFile(void)
{
    IsOwned = false;
    File = NULL;
}

//------------------------------------------------------------------------------

CStdIOFile::~CStdIOFile(void)
{
    Close();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CStdIOFile::Open(const CSmallString& name,const CSmallString& mode)
{
    if( name == "-" ){
        IsOwned = false;
        if( mode == "r" ){
            File = stdin;
            return(true);
        }
        if( mode == "w" ){
            File = stdout;
            return(true);
        }
        CSmallString error;
        error << "unable to open file '" << name << "' in mode '" << mode << "' (";
        error << "unknown mode" << ")";
        ES_ERROR(error);
        return(false);
    } else {
        IsOwned = false;
        File = fopen(name,mode);
        if( File == NULL ){
            CSmallString error;
            error << "unable to open file '" << name << "' in mode '" << mode << "' (";
            error << strerror(errno) << ")";
            ES_ERROR(error);
            return(false);
        }
        return(true);
    }
}

//------------------------------------------------------------------------------

void CStdIOFile::Close(void)
{
    if( IsOwned ){
        if( File != NULL ){
            fclose(File);
        }
    }
    IsOwned = false;
    File = NULL;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CStdIOFile::operator FILE* ()
{
    return(File);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
