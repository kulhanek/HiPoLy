//==============================================================================
//    Copyright 2010 Petr Kulhanek
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

#include <VerboseStr.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CVerboseBuf::CVerboseBuf(void)
    : StreamBuf(NULL), LogStreamBuf(NULL), Enabled(false)
{
}

//------------------------------------------------------------------------------

void CVerboseBuf::Attach(std::streambuf* p_sb)
{
    StreamBuf = p_sb;
}

//------------------------------------------------------------------------------

void CVerboseBuf::AttachLogBuffer(std::streambuf* p_sb)
{
    LogStreamBuf = p_sb;
}

//------------------------------------------------------------------------------

void CVerboseBuf::Enable(bool set)
{
    Enabled = set;
}

//------------------------------------------------------------------------------

int CVerboseBuf::overflow(int c)
{
    if( LogStreamBuf ) LogStreamBuf->sputc(c); // always record

    if( Enabled == false ) return(!EOF); // anihilate = success
    if( StreamBuf == NULL ) return(!EOF); // anihilate = success

    if( c == EOF ) {
        return(!EOF);
    } else {
        int const r1 = StreamBuf->sputc(c);
        return( r1 == EOF ? EOF : c );
    }
}

//------------------------------------------------------------------------------

// Sync both teed buffers.
int CVerboseBuf::sync()
{
    if( LogStreamBuf ) LogStreamBuf->pubsync(); // always sync

    if( Enabled == false ) return(0);    // anihilate = success
    if( StreamBuf == NULL ) return(0);  // anihilate = success
    int const r1 = StreamBuf->pubsync();
    return r1 == 0 ? 0 : -1;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CVerboseStr::CVerboseStr(void)
    : std::ostream(&TBuf)
{
    OutputVerbosity = none;
    StreamVerbosity = low;
    TBuf.Enable(OutputVerbosity <= StreamVerbosity);
}

//------------------------------------------------------------------------------

void CVerboseStr::Attach(std::ostream& ostr)
{
    TBuf.Attach(ostr.rdbuf());
}

//------------------------------------------------------------------------------

void CVerboseStr::Attach(std::ostream* ostr)
{
    if( ostr ){
        TBuf.Attach(ostr->rdbuf());
    } else {
        TBuf.Attach(NULL);
    }
}

//------------------------------------------------------------------------------

void CVerboseStr::AttachLogStream(std::ostream& lstr)
{
    TBuf.AttachLogBuffer(lstr.rdbuf());
}

//------------------------------------------------------------------------------

void CVerboseStr::AttachLogStream(std::ostream* lstr)
{
    if( lstr ){
        TBuf.AttachLogBuffer(lstr->rdbuf());
    } else {
        TBuf.AttachLogBuffer(NULL);
    }
}

//------------------------------------------------------------------------------

void CVerboseStr::Verbosity(EVerbosity vrb)
{
    OutputVerbosity = vrb;
    TBuf.Enable(OutputVerbosity >= StreamVerbosity);
}

//------------------------------------------------------------------------------

void CVerboseStr::setv(EVerbosity vrb)
{
    StreamVerbosity = vrb;
    TBuf.Enable(OutputVerbosity >= StreamVerbosity);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

std::ostream& debug(std::ostream& os)
{
    CVerboseStr* p_str = dynamic_cast<CVerboseStr*>(&os);
    if( p_str != NULL ) p_str->setv(CVerboseStr::debug);
    return(os);
}

//------------------------------------------------------------------------------

std::ostream& low(std::ostream& os)
{
    CVerboseStr* p_str = dynamic_cast<CVerboseStr*>(&os);
    if( p_str != NULL ) p_str->setv(CVerboseStr::low);
    return(os);
}

//------------------------------------------------------------------------------

std::ostream& medium(std::ostream& os)
{
    CVerboseStr* p_str = dynamic_cast<CVerboseStr*>(&os);
    if( p_str != NULL ) p_str->setv(CVerboseStr::medium);
    return(os);
}

//------------------------------------------------------------------------------

std::ostream& high(std::ostream& os)
{
    CVerboseStr* p_str = dynamic_cast<CVerboseStr*>(&os);
    if( p_str != NULL ) p_str->setv(CVerboseStr::high);
    return(os);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


