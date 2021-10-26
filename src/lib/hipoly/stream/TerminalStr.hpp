#ifndef TerminalStrH
#define TerminalStrH
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
#include <Terminal.hpp>
#include <iostream>

// -----------------------------------------------------------------------------

class HIPOLY_PACKAGE CTerminalBuffer : public std::streambuf {
public:
// public methods --------------------------------------------------------------
    CTerminalBuffer(void);

// setup methods ---------------------------------------------------------------
    /// set output "device" to hipoly terminal
    void SetDevice(CTerminal* p_terminal);

// section of private data -----------------------------------------------------
private:
    CTerminal*      Terminal;

    // This verbose buffer has no buffer. So every character "overflows"
    // and can be put directly into the redirected buffer.
    virtual int overflow(int c);

    // Sync buffer.
    virtual int sync(void);
};

// -----------------------------------------------------------------------------

class HIPOLY_PACKAGE CTerminalStr : public std::ostream {
public:
// constructor and destructors ------------------------------------------------
    CTerminalStr(void);
    ~CTerminalStr(void);

// executive methods ----------------------------------------------------------
    /// open file instead of specified terminal
    bool Open(const CSmallString& name);

    /// close file opened by Open
    void Close(void);

    /// attach to fout
    void Attach(FILE* fout);

    /// disconnect from stdout
    void DisconnectFromStdout(void);

    /// get terminal
    CTerminal& GetTerminal(void);

    /// disable/enable formatting
    void DisableFormatting(bool set);

// section of private data ----------------------------------------------------
private:
    CTerminal               Terminal;
    CTerminalBuffer         Buffer;
    FILE*                   File;
};

// -----------------------------------------------------------------------------

HIPOLY_PACKAGE std::ostream& disablef(std::ostream& os);   // disable formatting
HIPOLY_PACKAGE std::ostream& enablef(std::ostream& os);    // enable formatting

// -----------------------------------------------------------------------------

#endif
