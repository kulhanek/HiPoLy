//==============================================================================
//    Copyright 2000,2004,2008 Petr Kulhanek
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

#include <string.h>
#include <stdio.h>
#include <HiPoLyMainHeader.hpp>
#include <UUID.hpp>

#if defined _WIN32 || defined __CYGWIN__
// required to generate UUID
#include <rpc.h>
#else
#ifdef HAVE_UUID
// required to generate UUID
#include <uuid/uuid.h>
#else
#include <fstream>
using namespace std;
#endif
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CUUID::CUUID(void)
{
    Clear();
}

//------------------------------------------------------------------------------

CUUID::CUUID(const CSmallString& data)
{
    SetFromStringForm(data);
}

//------------------------------------------------------------------------------

void CUUID::Clear(void)
{
    Main = 0;
    First = 0;
    Second = 0;
    Third = 0;
    memset(Extended,0,sizeof(Extended));
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CUUID::operator==(const CUUID& left) const
{
    if( Main != left.Main ) return(false);
    if( First != left.First ) return(false);
    if( Second != left.Second ) return(false);
    if( Third != left.Third ) return(false);
    for(unsigned int i=0; i < sizeof(Extended); i++) {
        if( Extended[i] != left.Extended[i] ) return(false);
    }
    return(true);
}

//------------------------------------------------------------------------------

bool CUUID::operator!=(const CUUID& left) const
{
    if( Main != left.Main ) return(true);
    if( First != left.First ) return(true);
    if( Second != left.Second ) return(true);
    if( Third != left.Third ) return(true);
    for(unsigned int i=0; i<sizeof(Extended); i++) {
        if( Extended[i] != left.Extended[i] ) return(true);
    }
    return(false);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CUUID::SetFromStringForm(const CSmallString& data)
{
    unsigned int n1,n2,n3,n4,n5,n6,n7,n8,n9,n10;
    bool result;

    if( data == NULL ) return(false);
    if( data[0] == 'Z' ) {
        Main = 0;
        First = 0;
        Second = 0;
        Third = 0;
        Extended[0] = 0;
        Extended[1] = 0;
        Extended[2] = 0;
        Extended[3] = 0;
        Extended[4] = 0;
        Extended[5] = 0;
        return(true);
    }

    result = sscanf(data,"%8x-%4x-%4x-%4x-%2x%2x%2x%2x%2x%2x",&n1,&n2,&n3,&n4,&n5,&n6,
                    &n7,&n8,&n9,&n10);
    Main = n1;
    First = n2;
    Second = n3;
    Third = n4;
    Extended[0] = n5;
    Extended[1] = n6;
    Extended[2] = n7;
    Extended[3] = n8;
    Extended[4] = n9;
    Extended[5] = n10;

    return(result);
}

//------------------------------------------------------------------------------

CSmallString CUUID::GetStringForm(void) const
{
    char buffer[37];

    sprintf(buffer,"%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x",(int)Main,(int)First,(int)Second,(int)Third,
            (int)Extended[0],(int)Extended[1],(int)Extended[2],(int)Extended[3],(int)Extended[4],(int)Extended[5]);
    return(CSmallString(buffer));
}

//------------------------------------------------------------------------------

bool CUUID::IsValidUUID(void) const
{
    bool result = false;

    result |= Main != 0;
    result |= First != 0;
    result |= Second != 0;
    result |= Third != 0;
    result |= Extended[0] != 0;
    result |= Extended[1] != 0;
    result |= Extended[2] != 0;
    result |= Extended[3] != 0;
    result |= Extended[4] != 0;
    result |= Extended[5] != 0;
    return(result);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CUUID::CreateUUID(void)
{
#if defined _WIN32 || defined __CYGWIN__
    RPC_STATUS status;
    UUID uuid;

    status = UuidCreate(&uuid);
    if( status != RPC_S_OK ) return(false);

    unsigned char* p_string = NULL;
    status = UuidToString(&uuid,&p_string);
    if( status != RPC_S_OK ) return(false);

    bool result = SetFromStringForm((char*)p_string);

    RpcStringFree(&p_string);

    return(result);
#else
#ifdef HAVE_UUID
    uuid_t out;
    uuid_generate(out);
    if( sizeof(uuid_t) != sizeof(CUUID) ) return(false);
    memcpy(this,&out,sizeof(CUUID));
    return(true);
#else
    ifstream ifs("/dev/urandom",ios::binary);
    ifs.read((char*)this,sizeof(CUUID));
    return(ifs);
#endif
#endif
}

//------------------------------------------------------------------------------

CSmallString CUUID::GetGeneratorType(void) const
{
#if defined _WIN32 || defined __CYGWIN__
    return("Win32 RPC");
#else
#ifdef HAVE_UUID
    return("libuuid");
#else
    return("/dev/urandom");
#endif
#endif
}

//------------------------------------------------------------------------------

//! comparison operator
bool CUUID::operator < (const CUUID& left) const
{
    if( Main < left.Main ) return(true);
    if( Main > left.Main ) return(false);

    if( First < left.First ) return(true);
    if( First > left.First ) return(false);

    if( Second < left.Second ) return(true);
    if( Second > left.Second ) return(false);

    if( Third < left.Third ) return(true);
    if( Third > left.Third ) return(false);

    for(unsigned int i=0; i<sizeof(Extended); i++) {
        if( Extended[i] < left.Extended[i] ) return(true);
        if( Extended[i] > left.Extended[i] ) return(false);
    }

    // boths are actually equal
    return(false);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

