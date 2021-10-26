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

#include <TerminalStr.hpp>
#include <fstream>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CTerminalBuffer::CTerminalBuffer(void)
{
    Terminal = NULL;
}

//------------------------------------------------------------------------------

void CTerminalBuffer::SetDevice(CTerminal* p_terminal)
{
    Terminal = p_terminal;
    if( Terminal != NULL ) {
        Terminal->ResetFormattedStream();
    }
}

//------------------------------------------------------------------------------

int CTerminalBuffer::overflow(int c)
{
    if( Terminal != NULL ) {
        Terminal->PrintFormattedChar(c);
    }

    return(0);  // anihilate output
}

//------------------------------------------------------------------------------

int CTerminalBuffer::sync(void)
{
    if( Terminal == NULL ) {
        return(-1);
    }
    return(Terminal->FlushStream());
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CTerminalStr::CTerminalStr(void)
    : std::ostream(&Buffer), File(NULL)
{
    Buffer.SetDevice(&Terminal);
}

//------------------------------------------------------------------------------

CTerminalStr::~CTerminalStr(void)
{
    if( File != NULL ) fclose(File);
}

//------------------------------------------------------------------------------

bool CTerminalStr::Open(const CSmallString& name)
{
    if( File != NULL ) fclose(File);
    File = fopen(name,"w");
    Terminal.SetOutputStream(File);
    return( File != NULL );
}

//------------------------------------------------------------------------------

void CTerminalStr::Close(void)
{
    if( File != NULL ){
        fclose(File);
        Terminal.SetOutputStream(NULL);
    }
    File = NULL;
}

//------------------------------------------------------------------------------

void CTerminalStr::Attach(FILE* fout)
{
    Close();
    Terminal.SetOutputStream(fout);
}

//------------------------------------------------------------------------------

void CTerminalStr::DisconnectFromStdout(void)
{
    Close();
    Terminal.SetOutputStream(NULL);
}

//------------------------------------------------------------------------------

CTerminal& CTerminalStr::GetTerminal(void)
{
    return(Terminal);
}

//------------------------------------------------------------------------------

void CTerminalStr::DisableFormatting(bool set)
{
    Terminal.DisableFormatting(set);
}


//------------------------------------------------------------------------------

std::ostream& disablef(std::ostream& os)
{
    CTerminalStr* p_str = dynamic_cast<CTerminalStr*>(&os);
    if( p_str != NULL ) p_str->DisableFormatting(true);
    return(os);
}

//------------------------------------------------------------------------------

std::ostream& enablef(std::ostream& os)
{
    CTerminalStr* p_str = dynamic_cast<CTerminalStr*>(&os);
    if( p_str != NULL ) p_str->DisableFormatting(true);
    return(os);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
