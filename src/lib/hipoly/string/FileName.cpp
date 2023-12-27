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

#include <FileName.hpp>
#include <stdlib.h>
#include <string.h>
#include <FileSystem.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>

//------------------------------------------------------------------------------

using namespace std;
using namespace boost;
using namespace boost::algorithm;

//------------------------------------------------------------------------------

#if defined _WIN32 || defined __CYGWIN__
const char CFileName::PathDelimiter = '\\';
#else
const char CFileName::PathDelimiter = '/';
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CFileName::CFileName(void)
{
}

//------------------------------------------------------------------------------

CFileName::CFileName(const CFileName& copypath)
    : CSmallString(copypath)
{
}

//------------------------------------------------------------------------------

CFileName::CFileName(const CSmallString& copystring)
    : CSmallString(copystring)
{
}

//------------------------------------------------------------------------------

CFileName::CFileName(const std::string& string)
    : CSmallString(string)
{
}

//------------------------------------------------------------------------------

CFileName::CFileName(const char* p_string)
    : CSmallString(p_string)
{
}

//------------------------------------------------------------------------------

CFileName::~CFileName(void)
{
}

//------------------------------------------------------------------------------

void CFileName::SetFromUNIX(const CSmallString& path)
{
    *this = path.GetSubstitute('/',PathDelimiter);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

char CFileName::GetPathDelimiter(void)
{
    return(PathDelimiter);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CFileName::operator+=(const CFileName& string)
{
    if( string.LengthOfBuffer == 0 ) return;

    if( LengthOfBuffer == 0 ) LengthOfBuffer++;

    Buffer = (char*)realloc(Buffer,LengthOfBuffer+string.LengthOfBuffer-1);
    memcpy(&Buffer[LengthOfBuffer-1],string.Buffer,string.LengthOfBuffer);
    LengthOfBuffer += string.LengthOfBuffer-1;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CFileName CFileName::operator/(const char* p_string) const
{
    CSmallString ret_string(Buffer);

    ret_string += PathDelimiter;
    ret_string += p_string;

    return(ret_string);
}

//---------------------------------------------------------------------------

CFileName CFileName::operator/(const CFileName &string) const
{
    CSmallString ret_string(Buffer);

    ret_string += PathDelimiter;
    ret_string += string;

    return(ret_string);
}

// -----------------------------------------------------------------------------

CFileName  CFileName::operator/(const CSmallString& string) const
{
    CSmallString ret_string(Buffer);

    ret_string += PathDelimiter;
    ret_string += string;

    return(ret_string);
}

// -----------------------------------------------------------------------------

CFileName HIPOLY_PACKAGE operator/(const char* string1,const CFileName& string2)
{
    CSmallString ret_string(string1);

    ret_string += CFileName::PathDelimiter;
    ret_string += string2;

    return(ret_string);
}

// -----------------------------------------------------------------------------

void  CFileName::operator/=(const char *p_string)
{
    *this += CSmallString(CFileName::PathDelimiter);
    *this += p_string;
}

// -----------------------------------------------------------------------------

void  CFileName::operator/=(const CFileName& cs)
{
    *this += CSmallString(CFileName::PathDelimiter);
    *this += cs;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CFileName CFileName::GetFileDirectory(void) const
{
    int from=0;
    int length=0;

    char slash = PathDelimiter;

    for(int i=LengthOfBuffer-2; i>=1; i--) {
        if( Buffer[i] == slash ) {
            length = i;
            break;
        }
    }

    return(GetSubString(from,length));
}

// -----------------------------------------------------------------------------

CFileName CFileName::GetFileName(void) const
{
    int from=0;
    char slash = PathDelimiter;

    for(int i=LengthOfBuffer-2; i>=0; i--) {
        if( Buffer[i] == slash ) {
            from = i+1;
            break;
        }
    }

    int length=LengthOfBuffer-1-from;

    return(GetSubString(from,length));
}

// -----------------------------------------------------------------------------

CFileName CFileName::GetFileNameExt(void) const
{
    int from=LengthOfBuffer-1;

    for(int i=LengthOfBuffer-1; i>=0; i--) {
        if( Buffer[i] == '.') {
            from = i;
            break;
        }
    }
    int length=LengthOfBuffer-1-from;
    return(GetSubString(from,length));
}

//---------------------------------------------------------------------------

CFileName CFileName::GetFileNameWithoutExt(void) const
{
    int from=0;
    int to=LengthOfBuffer-2;
    char slash = PathDelimiter;

    for(int i=LengthOfBuffer-2; i>=1; i--) {
        if( Buffer[i] == slash ) {
            break;
        }
        if( Buffer[i] == '.') {
            to = i-1;
            break;
        }
    }

    for(int i=LengthOfBuffer-2; i>=0; i--) {
        if( Buffer[i] == slash ) {
            from = i+1;
            break;
        }
    }

    return(GetSubString(from,to-from+1));
}

//------------------------------------------------------------------------------

CFileName CFileName::RelativeTo(const CFileName& path_to)
{
    // https://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path

    std::string sfrom(GetBuffer());
    std::string sto(path_to);

    std::vector<std::string> from;
    std::vector<std::string> to;

#if defined _WIN32 || defined __CYGWIN__
    split(from,sfrom,is_any_of("\\"));
    split(to,sto,is_any_of("\\"));
#else
    split(from,sfrom,is_any_of("/"));
    split(to,sto,is_any_of("/"));
#endif

    // Start at the root path and while they are the same then do nothing then when they first
    // diverge take the entire from path, swap it with '..' segments, and then append the remainder of the to path.
    auto fromIter = from.begin();
    auto toIter = to.begin();

    // Loop through both while they are the same to find nearest common directory
    while( fromIter != from.end() && toIter != to.end() && *toIter == *fromIter ) {
       ++toIter;
       ++fromIter;
    }

    // Replace to path segments with '..' (from => nearest common directory)
    CFileName finalPath;
    while( toIter != to.end() ) {
        if( finalPath == NULL ) {
            finalPath = "..";
        } else {
            finalPath /= "..";
        }
        ++toIter;
    }

    // Append the remainder of the from path (nearest common directory => from)
    while( fromIter != from.end() ) {
        if( finalPath == NULL ) {
            finalPath = CFileName(*fromIter);
        } else {
            finalPath /= CFileName(*fromIter);
        }
       ++fromIter;
    }

    return finalPath;
}

//------------------------------------------------------------------------------

void CFileName::AbsolutizePath(void)
{
    CSmallString curr_dir;
    CFileSystem::GetCurrentDir(curr_dir);
    AbsolutizePath(CFileName(curr_dir));
}

//------------------------------------------------------------------------------

void CFileName::AbsolutizePath(const CFileName& base)
{
    if( IsEmpty() ){
        *this = base;
        return;
    }

    // decode current path
    // FIXME
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#ifdef HAVE_QT5

CFileName::CFileName(const QString& string)
    : CSmallString(string)
{

}

//------------------------------------------------------------------------------

CFileName CFileName::operator/(const QString &string) const
{
    CSmallString ret_string(Buffer);

    ret_string += PathDelimiter;
    ret_string += string;

    return(ret_string);
}

#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
