#ifndef TerminalH
#define TerminalH
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

#include <HiPoLyMainHeader.hpp>
#include <SmallString.hpp>
#include <stdio.h>
#include <vector>
#include <sstream>

//------------------------------------------------------------------------------

class HIPOLY_PACKAGE CTerminal {
public:
    // constructor and destructor ------------------------------------------------
    CTerminal(void);

    CTerminal(FILE* fout);

//-----------------------------------------------------------------------------
    static bool IsTerminal(FILE* p_f);

//-----------------------------------------------------------------------------
    /// color definition
    enum EColor {
        EC_NONE = -1,
        EC_BLACK = 0,
        EC_RED = 1,
        EC_GREEN = 2,
        EC_YELLOW = 3,
        EC_BLUE = 4,
        EC_PURPLE = 5,
        EC_CYAN = 6,
        EC_WHITE = 7
    };

// setup methods --------------------------------------------------------------
    /// set output stream
    void SetOutputStream(FILE* fout);

    /// detect if it is terminal (from output file or stdout)
    void DetectTerminal(void);

    /// force colors and underlying terminal from stdin
    void ForceColors(void);

    /// set number of columns
    void SetNumberOfColumns(int columns);

// information methods --------------------------------------------------------
    /// return number of columns
    int GetNumberOfColumns(void) const;

    /// return number of rows
    int GetNumberOfRows(void) const;

    /// are colors available by given terminal?
    bool AreColorsAvailable(void) const;

    /// get terminal dimmension (from stdin)
    static bool GetSize(int &nrow,int &ncolumns);

// executive methods ----------------------------------------------------------
    /// set foreground and background colors
    bool SetColors(int foreground,int background) const;

    /// set foreground and background colors
    bool SetColors(EColor foreground,EColor background=EC_NONE) const;

    /// restore default setup
    bool SetDefault(bool only_colors=false) const;

    /// to bold text
    bool SetBold(bool set=true) const;

    /// to underline text
    bool SetUnderline(bool set=true) const;

    /// to italic text
    bool SetItalic(bool set=true) const;

    /// disable/enable colours
    void EnableColors(bool set);

// list operations ------------------------------------------------------------
    /// add item to the list
    void AddItem(const CSmallString& text);

    /// clear entire list
    void ClearList(void);

    /// print the list as column sorted list
    bool PrintColumnSortedList(unsigned int column_width=0);

    /// print the list as row sorted list
    bool PrintRowSortedList(unsigned int column_width=0);

    /// print the list as plain text
    bool PrintPlainList(void);

// titles ---------------------------------------------------------------------
    /// print title
    bool PrintTitle(const CSmallString& title,
                                 char delimiter,int beginning=1);

// text  ---------------------------------------------------------------------
    /// print text
    bool PrintText(const CSmallString& text,
                                int column_width=-1,const char* p_leading=NULL);

    /// printf
    int Printf(const char* format,...);

    /// print formated text
    bool PrintFormatted(const char* text);

    /// print formatted charcter
    bool PrintFormattedChar(int c);

    /// reset formatted stream
    void ResetFormattedStream(void);

    /// flush buffer to terminal
    int FlushStream(void);

// section of private data ----------------------------------------------------
private:
    FILE*                       FOut;
    int                         NRows;              // number of rows
    int                         NColumns;           // number of columns
    bool                        ColorsAvailable;    // are colors supported?
    bool                        ColorsEnabled;      // are colors enabled
    std::vector<CSmallString>   List;
    // formatted output
    int                         CommandLevel;
    std::stringstream           Command;
    int                         BlockCount;
    bool                        BlockMode;
    int                         LeftOffset;
};

//------------------------------------------------------------------------------

#endif
