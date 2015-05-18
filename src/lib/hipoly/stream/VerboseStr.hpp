#ifndef VerboseStrH
#define VerboseStrH
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

#include <HiPoLyMainHeader.hpp>
#include <fstream>
#include <iostream>

// based on :
// http://wordaligned.org/articles/cpp-streambufs

//------------------------------------------------------------------------------

/// CVerboseBuf - buffer for verbose str

class HIPOLY_PACKAGE CVerboseBuf : public std::streambuf {
public:
// public methods --------------------------------------------------------------
    /// constructor - initial output stream buffer is NULL
    CVerboseBuf(void);

    /// attach output to other stream buffer
    void Attach(std::streambuf* p_sb);

    /// attach output to log stream buffer
    void AttachLogBuffer(std::streambuf* p_sb);

    /// enable attached stream buffer
    void Enable(bool set);

// section of private data -----------------------------------------------------
private:
    std::streambuf* StreamBuf;
    std::streambuf* LogStreamBuf;
    bool            Enabled;

    // This verbose buffer has no buffer. So every character "overflows"
    // and can be put directly into the redirected buffer.
    virtual int overflow(int c);

    // Sync buffer.
    virtual int sync(void);
};

//------------------------------------------------------------------------------

/// verbosestr - stream for aplication output management

class HIPOLY_PACKAGE CVerboseStr : public std::ostream {
// public types ----------------------------------------------------------------
public:
    enum EVerbosity {
        none = 0,
        low,
        medium,
        high,
        debug
    };

public:
// public methods --------------------------------------------------------------
    /// constructor - by default all output is anihilated, output verbosity is none
    CVerboseStr(void);

    /// set real output stream
    void Attach(std::ostream& ostr);

    /// set real output stream
    void Attach(std::ostream* ostr);

    /// set log stream - it records everything
    void AttachLogStream(std::ostream& lstr);

    /// set log stream - it records everything
    void AttachLogStream(std::ostream* lstr);

    /// set requested verbosity
    void Verbosity(EVerbosity vrb);

    /// set stream verbosity
    void setv(EVerbosity vrb);

// section of private data -----------------------------------------------------
private:
    CVerboseBuf TBuf;
    EVerbosity  OutputVerbosity;
    EVerbosity  StreamVerbosity;
};

//------------------------------------------------------------------------------

// stream manipulators - verbosity importance
HIPOLY_PACKAGE std::ostream& low(std::ostream& os);        // switch to low importance messages
HIPOLY_PACKAGE std::ostream& medium(std::ostream& os);     // switch to medium importance messages
HIPOLY_PACKAGE std::ostream& high(std::ostream& os);       // switch to high importance messages
HIPOLY_PACKAGE std::ostream& debug(std::ostream& os);      // switch to debug importance messages

//------------------------------------------------------------------------------

#endif
