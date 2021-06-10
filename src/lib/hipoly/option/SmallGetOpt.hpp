#ifndef SmallGetOptH
#define SmallGetOptH
//==============================================================================
//    Copyright 2007,2008 Petr Kulhanek
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
#include <SimpleList.hpp>
#include <SimpleIterator.hpp>

//------------------------------------------------------------------------------

#define GO_END_OF_LINE -1
#define GO_BAD_OPTION  -2
#define GO_ERROR       -3

//------------------------------------------------------------------------------

/*! \brief cpp version of getopt functionality
 *
 */

class HIPOLY_PACKAGE CSmallGetOpt {
public:
    CSmallGetOpt(void);
    virtual ~CSmallGetOpt(void);

// setup methods --------------------------------------------------------------
    /// set verbosity
    void SetVerbose(bool set);

    bool IsVerbose(void);

    /// set show usage
    void SetShowUsage(bool set);

    /// set show usage
    void SetShowMiniUsage(bool set);

    /// is usage shown during error?
    bool IsShowUsage(void);

    /// is meni usage shown during error?
    bool IsShowMiniUsage(void);

    /// set allow program arguments
    void SetAllowProgArgs(bool set);

    /// are program arguments allowed?
    bool IsAllowProgArgs(void);

    /// process unrecognized options as arguments
    void SetUnrecognizedOptsAsArgs(bool set);

    /// are unrecognized opts enabled?
    bool AreUnrecognizedOptsAsArgsEnabled(void);

// main methods ---------------------------------------------------------------
    /// set command line
    void SetCmdLine(int argc,char* argv[]);

// description methods -------------------------------------------------------
    /// set program name - it is used in error messages, otherwise setup from SetCmdLine is used
    void SetProgramName(const CSmallString& name);

    /// return program name
    virtual const char* GetProgramName(void) const;

    /// print usage
    virtual void PrintUsage(FILE* fout=NULL);

    /// print program mini usage
    virtual void PrintMiniUsage(FILE* fout=NULL);

    /// print command line
    void PrintCmdLine(FILE* fout=NULL);

// option methods -------------------------------------------------------------
public:
    class COption {
    public:
        COption(void);
        COption(const char _short,const CSmallString& _long,const bool _hasarg,const int _id);

    public:
        char            Short;          // short option name or \0 if not apllied
        CSmallString    Long;           // long option name or null if not applied
        bool            HasArg;         // has argument
        int             OptID;          // option ID
        bool            Set;
    };

    /// enable to re-process options once again, option list is not deleted
    void Restart(void);

    /// remove all registered options
    void ClearOptionList(void);

    /// initialize options from string
    void SetShortOptList(const CSmallString& optstring);

    /// initialize options from option array, terminal item should have GO_BAD_OPTION OptID
    void SetLongOptList(const COption* optlist);

    /// initialize options from option list
    void SetLongOptList(CSimpleList<COption>& optlist);

    /// add single option to the list
    void AddOption(const char _short,const CSmallString& _long,const bool _hasarg,const int _id);

    /// initialize options
    bool InitializeOptions(void);

    /// return next option id from cmd line
    int  GetOpt(void);

    /// return next option from cmd line
    const COption*  GetFullOpt(void);

    /// process all options, each option should be handled in virtual ProcessOption method
    bool ProcessAllOptions(void);

    /// this method is called for every option
    virtual bool ProcessOption(const COption* p_opt);

    /// this method is called for every program argument
    virtual bool ProcessArgument(const int index,const CSmallString& arg);

    /// return current option name as was specified on command line
    const CSmallString& GetCurrOpt(void);

    /// does current option have an argument?
    bool HasOptArg(void);

    /// return option argument
    const CSmallString& GetOptArg(void);

    /// return number of program arguments
    int GetNumberOfProgArgs(void);

    /// return argument with index
    const CSmallString& GetProgArg(int index);

    /// is option set?
    bool IsOptionSet(int optid);

    /// set option set flag?
    void SetOption(int optid);

// section of private data and methods ----------------------------------------
private:
    CSimpleList<CSmallString>       CmdArgs;        // list of arguments
    CSmallString                    ProgramName;    // program name, either argv[0] or via SetProgramName

    // setup
    bool                            Verbose;            // print error messages
    bool                            ShowUsage;          // print usage in the case of error
    bool                            ShowMiniUsage;      // print mini usage in the case of error
    bool                            AllowPrgArgs;       // allow program arguments
    bool                            UnrecognizedOpts;   // treat unrecognized opts as arguments

    // runtime data
    bool                            Initialized;    // initialized
    CSimpleList<COption>            OptList;        // option list
    CSimpleIterator<CSmallString>   CO;             // current option
    int                             GroupPos;       // current option position in the group e.g. -a[b]sd
    CSimpleIterator<CSmallString>   DA;             // option/argument delimiter
    CSimpleIterator<CSmallString>   FA;             // first argument

    bool                            HasArgument;    // current option has argument
    CSmallString                    OptArgument;    // option argument
    CSmallString                    CurrentOpt;     // current option

    bool  IsItShortOption(char sopt,bool& has_arg);
    bool  IsItLongOption(const CSmallString& lopt,bool& has_arg);

    COption*  FindShortOption(char sopt,bool& has_arg);
    COption*  FindLongOption(const CSmallString& lopt,bool& has_arg);

// protected data -------------------------------------------------------------
protected:
    bool                            IsError;        // set to true if error, it is used for formating
};

//------------------------------------------------------------------------------

#endif
