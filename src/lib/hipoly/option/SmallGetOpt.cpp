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

#include <SmallGetOpt.hpp>
#include <string.h>
#include <ctype.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmallGetOpt::CSmallGetOpt(void)
    : CO(CmdArgs), DA(CmdArgs), FA(CmdArgs)
{
    Verbose = true;
    ShowUsage = false;
    AllowPrgArgs = false;
    UnrecognizedOpts = false;

    Restart();
}

//------------------------------------------------------------------------------

CSmallGetOpt::~CSmallGetOpt(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmallGetOpt::SetVerbose(bool set)
{
    Verbose = set;
}

//------------------------------------------------------------------------------

bool CSmallGetOpt::IsVerbose(void)
{
    return(Verbose);
}

//------------------------------------------------------------------------------

void CSmallGetOpt::SetShowUsage(bool set)
{
    ShowUsage = set;
}

//------------------------------------------------------------------------------

void CSmallGetOpt::SetShowMiniUsage(bool set)
{
    ShowMiniUsage = set;
}

//------------------------------------------------------------------------------

bool CSmallGetOpt::IsShowUsage(void)
{
    return(ShowUsage);
}

//------------------------------------------------------------------------------

bool CSmallGetOpt::IsShowMiniUsage(void)
{
    return(ShowMiniUsage);
}

//------------------------------------------------------------------------------

void CSmallGetOpt::SetAllowProgArgs(bool set)
{
    AllowPrgArgs = set;
}

//------------------------------------------------------------------------------

bool CSmallGetOpt::IsAllowProgArgs(void)
{
    return(AllowPrgArgs);
}

//------------------------------------------------------------------------------

void CSmallGetOpt::SetUnrecognizedOptsAsArgs(bool set)
{
    UnrecognizedOpts = set;
}

//------------------------------------------------------------------------------

bool CSmallGetOpt::AreUnrecognizedOptsAsArgsEnabled(void)
{
    return(UnrecognizedOpts);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmallGetOpt::SetCmdLine(int argc,char* argv[])
{
    // if initialized - restart iterators
    if( Initialized == true ) Restart();

    // set program name
    ProgramName = argv[0];

    // make local copy of arguments
    CmdArgs.RemoveAll();
    for(int i=1; i < argc; i++) {
        CSmallString* p_str = new CSmallString(argv[i]);
        CmdArgs.InsertToEnd(p_str,i,true); // index is original arg index
    }
}

//------------------------------------------------------------------------------

void CSmallGetOpt::SetProgramName(const CSmallString& name)
{
    ProgramName = name;
}

//------------------------------------------------------------------------------

const char* CSmallGetOpt::GetProgramName(void) const
{
    return(ProgramName);
}

//------------------------------------------------------------------------------

void CSmallGetOpt::PrintUsage(FILE* fout)
{
    if( fout == NULL ) {
        fout = stdout;  // if output stream is not specified use standard output
    }

    fprintf(fout,"\n");

    // print header --------------------------------
    const char* p_progname = GetProgramName();
    fprintf(fout,"Usage: %s [OPTIONS] [--] [ARGUMENTS]\n",p_progname);

    fprintf(fout,"\n");
}

//------------------------------------------------------------------------------

void CSmallGetOpt::PrintMiniUsage(FILE* fout)
{
    // nothing to print
}

//------------------------------------------------------------------------------

void CSmallGetOpt::PrintCmdLine(FILE* fout)
{
    if( fout == NULL ) {
        fout = stdout;  // if output stream is not specified use standard output
    }

    fprintf(fout,"%s",(char*)GetProgramName());

    for(int i=0; i < CmdArgs.NumOfMembers(); i++) {
        fprintf(fout," %s",CmdArgs[i]->GetBuffer());
    }
    fprintf(fout,"\n");
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmallGetOpt::Restart(void)
{
    Initialized = false;
    CO.SetToBegin();
    FA.Invalidate();
    DA.Invalidate();

    IsError = false;

    HasArgument = false;
    OptArgument = NULL;
    CurrentOpt = NULL;

    // resort arguments to original order - by brutal force
    bool sorted = false;

    while(sorted != true) {
        CSimpleIterator<CSmallString>   I(CmdArgs);
        CSmallString*                   p_str1;
        CSmallString*                   p_str2;
        int                             id1,id2;

        I.SetToBegin();
        sorted = true;

        while( (p_str1 = I.Current()) != NULL ) {
            id1 = I.CurrentID();
            I++;
            if( I.Current() == NULL ) break;
            p_str2 = I.Current();
            id2 = I.CurrentID();
            if( id1 > id2 ) {
                CmdArgs.Swap(p_str1,p_str2);
                sorted = false;
            }
        }
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmallGetOpt::ClearOptionList(void)
{
    OptList.RemoveAll();
}

//------------------------------------------------------------------------------

void CSmallGetOpt::SetShortOptList(const CSmallString& optstring)
{
    // if initialized - restart iterators
    if( Initialized == true ) Restart();

    // apply only short options - option ID is option name
    for(unsigned int i=0; i < optstring.GetLength(); i++) {
        char sopt = optstring[i];
        if( sopt != ':' ) {
            COption* p_opt;
            p_opt = new COption;
            p_opt->Short = sopt;
            p_opt->OptID = sopt;
            i++;
            if( (i < optstring.GetLength()) && (optstring[i] == ':') ) {
                p_opt->HasArg = true;
            } else {
                i--;
            }
            OptList.InsertToEnd(p_opt,0,true);
        }
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSmallGetOpt::SetLongOptList(const COption* optlist)
{
    // if initialized - restart iterators
    if( Initialized == true ) Restart();

    if( optlist == NULL ) return;

    // apply only short options - option ID is option name
    int index = 0;

    while( optlist[index].OptID != GO_BAD_OPTION ) {
        COption* p_opt;
        p_opt = new COption;
        // copy data
        p_opt->Short  = optlist[index].Short;
        p_opt->Long   = optlist[index].Long;
        p_opt->HasArg = optlist[index].HasArg;
        p_opt->OptID  = optlist[index].OptID;

        OptList.InsertToEnd(p_opt,0,true);
        index++;
    }
}

//------------------------------------------------------------------------------

void CSmallGetOpt::SetLongOptList(CSimpleList<COption>& optlist)
{
    // if initialized - restart iterators
    if( Initialized == true ) Restart();

    // apply only short options - option ID is option name
    CSimpleIterator<COption> I(optlist);
    COption* p_sopt;

    while( (p_sopt = I.Current()) != NULL ) {
        COption* p_opt;

        p_opt = new COption;
        // copy data
        p_opt->Short  = p_sopt->Short;
        p_opt->Long   = p_sopt->Long;
        p_opt->HasArg = p_sopt->HasArg;
        p_opt->OptID  = p_sopt->OptID;

        OptList.InsertToEnd(p_opt,0,true);
        I++;
    }
}

//------------------------------------------------------------------------------

void CSmallGetOpt::AddOption(const char _short,const CSmallString& _long,
        const bool _hasarg,const int _id)
{
    // if initialized - restart iterators
    if( Initialized == true ) Restart();

    // add option to the list
    COption* p_opt;
    p_opt = new COption;

    p_opt->Short = _short;
    p_opt->Long = _long;
    p_opt->HasArg = _hasarg;
    p_opt->OptID = _id;

    OptList.InsertToEnd(p_opt,0,true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CSmallGetOpt::GetOpt(void)
{
    const COption* p_opt = GetFullOpt();
    if( p_opt == NULL ) return(GO_ERROR);
    return(p_opt->OptID);
}

//------------------------------------------------------------------------------

const CSmallGetOpt::COption*  CSmallGetOpt::GetFullOpt(void)
{
    static COption end_of_line('\0',"",false,GO_END_OF_LINE);
    static COption bad_opt('\0',"",false,GO_BAD_OPTION);
    static COption erorr('\0',"",false,GO_ERROR);

    HasArgument = false;
    OptArgument = NULL;
    CurrentOpt = NULL;

    // initialize if necessary
    if( Initialized == false ) {
        if( InitializeOptions() == false) {
            return(&erorr);
        }
    }

    // is it last option?
    if( CO == DA || CO.Current() == NULL ) {
        return(&end_of_line); // end of line
    }

    CSmallString* p_str = CO.Current();
    COption* p_opt;

    if( (p_str->GetLength() >= 2) && (p_str->GetBuffer()[0] == '-') ) {
        bool has_arg = false;
        if( p_str->GetBuffer()[1] == '-' ) {
            if( p_str->GetLength() > 2 ) {
                // process long option
                if( (p_opt = FindLongOption(&(p_str->GetBuffer()[2]),has_arg)) != NULL ) {
                    CurrentOpt = &(p_str->GetBuffer()[2]);
                    CO++;
                    if( has_arg == true ) {
                        HasArgument = has_arg;
                        p_str = CO.Current();
                        if( p_str != NULL ) OptArgument = *p_str;
                        CO++;
                    }
                    return(p_opt);
                }
            }
        } else {
            // process short option/options
            for(unsigned int i=1; i < p_str->GetLength(); i++) {
                if( (p_opt = FindShortOption(p_str->GetBuffer()[i],has_arg)) != NULL ) {
                    CurrentOpt = (p_str->GetBuffer()[i]);
                    CO++;
                    if( has_arg == true ) {
                        HasArgument = has_arg;
                        p_str = CO.Current();
                        if( p_str != NULL ) OptArgument = *p_str;
                        CO++;
                    }
                    return(p_opt);
                }
            }
        }
    }

    // this should not happen - because it was handled in InitializeOptions
    CO++;

    return(&bad_opt);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSmallGetOpt::ProcessAllOptions(void)
{
    if( InitializeOptions() == false ) return(false);

    // process options
    for(;;) {
        const COption* p_opt = GetFullOpt();
        if( p_opt == NULL ) return(false);
        if( p_opt->OptID < 0 ) return(false);
        if( ProcessOption(p_opt) == false ) return(false);
    }

    // process arguments
    for(int i=0; i<GetNumberOfProgArgs(); i++) {
        if( ProcessArgument(i,GetProgArg(i)) == false ) return(false);
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CSmallGetOpt::ProcessOption(const COption* p_opt)
{
    return(true);
}

//------------------------------------------------------------------------------

bool CSmallGetOpt::ProcessArgument(const int index,const CSmallString& arg)
{
    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSmallGetOpt::InitializeOptions(void)
{
    // if initialized - restart iterators
    if( Initialized == true ) Restart();
    Initialized = false;

    // test input options --------------------
    if( OptList.IsEmpty() == true ) {
        // NULL string -> error
        fprintf(stderr," >>> RUNTIME ERROR [InitializeOptions]: No options are requested (empty list)!\n");
        return(false); // RUNTIME error - this is programmer error, do not print usage
    }

    // test for option duplicity -------------------
    bool result = true;
    CSimpleIterator<COption>   I1(OptList);
    CSimpleIterator<COption>   I2(OptList);
    COption*                   p_opt1;
    COption*                   p_opt2;

    while( (p_opt1 = I1.Current()) != NULL ) {
        I1++;
        I2 = I1;
        while( (p_opt2 = I2.Current()) != NULL ) {
            if( (p_opt1->Short != '\0') && (p_opt1->Short == p_opt2->Short) ) {
                // duplicit short option
                fprintf(stderr," >>> RUNTIME ERROR [InitializeOptions]: Duplicit short option '%c' in option list!\n", p_opt1->Short);
                result = false;
            }
            if( (p_opt1->Long != NULL) && (p_opt1->Long == p_opt2->Long) ) {
                // duplicit short option
                fprintf(stderr," >>> RUNTIME ERROR [InitializeOptions]: Duplicit long option '%s' in option list!\n", (const char*)p_opt1->Long);
                result = false;
            }
            I2++;
        }
    }
    if( result == false) return(false);    // RUNTIME error - this is programmer error, do not print usage

    // now find border between options and arguments

    // find --, it is delimiter between options and program arguments
    // this is optional option and it is even permitted for short option mode
    CSmallString*                 p_str;

    DA.SetToBegin();

    while( (p_str = DA.Current()) != NULL ) {
        if( *p_str == "--" )break;
        DA++;
    }

    FA = DA;
    FA++;      // first argument if +1 pos after DA

    // find additional program arguments in the range <0,LastOptData>
    CSimpleIterator<CSmallString> A(CmdArgs);  // arguments
    CSimpleIterator<CSmallString> O(CmdArgs);  // options

    O.SetToBegin();
    A=DA;

    while( ((p_str = O.Current()) != NULL) && (DA != O) ) {
        // is it short option?
        if( (p_str->GetLength() >= 2) && (p_str->GetBuffer()[0] == '-') ) {
            bool found = true;
            bool has_arg = false;
            bool is_arg = false;
            if( p_str->GetBuffer()[1] == '-' ) {
                if( p_str->GetLength() > 2 ) {
                    // process long option
                    if( IsItLongOption(&(p_str->GetBuffer()[2]),has_arg) == false ) {
                        if( UnrecognizedOpts && AllowPrgArgs ){
                            is_arg = true;
                        } else {
                            if( Verbose ) {
                                if( IsError == false ) fprintf(stderr,"\n");
                                fprintf(stderr,"%s: illegal long option '%s'\n", (char*)GetProgramName(),&(p_str->GetBuffer()[2]));
                                IsError = true;
                            }
                            result = false;
                            found = false;
                        }
                    }
                } else {
                    if( UnrecognizedOpts && AllowPrgArgs ){
                        is_arg = true;
                    } else {
                        if(Verbose) {
                            if( IsError == false ) fprintf(stderr,"\n");
                            fprintf(stderr,"%s: missing long option name\n",(char*)GetProgramName());
                            IsError = true;
                        }
                        result = false;
                        found = false;
                    }
                }
            } else {
                // process short option/options
                for(unsigned int i=1; i < p_str->GetLength(); i++) {
                    if( IsItShortOption(p_str->GetBuffer()[i],has_arg) == false ) {
                        if( UnrecognizedOpts && AllowPrgArgs ){
                            is_arg = true;
                            break;
                        } else {
                            if( Verbose ) {
                                if( IsError == false ) fprintf(stderr,"\n");
                                fprintf(stderr,"%s: illegal short option '%c'\n", (char*)GetProgramName(),p_str->GetBuffer()[i]);
                                IsError = true;
                            }
                            result = false;
                            found = false;
                        }
                    }
                    if( (has_arg == true) && (p_str->GetLength() > 2) ) {
                        if( Verbose ) {
                            if( IsError == false ) fprintf(stderr,"\n");
                            fprintf(stderr,"%s: short option '%c' requires an argument but it was specified in the group '%s'\n",(char*)GetProgramName(),p_str->GetBuffer()[i],p_str->GetBuffer());
                            IsError = true;
                        }
                        result = false;
                    }
                }
            }
            if( is_arg == true ){
                int           id = O.CurrentID();
                CSmallString* p_tstr = O.Remove(false);   // remove object from list but do not destroy it
                A.InsertAfter(p_tstr,id,true);
                if( DA.IsValid() == false ) DA = A;
                if( FA.IsValid() == false ) FA = A;
                continue;
            }
            if( found == false ) {
                O++;
                continue;
            }
            if( has_arg == true ) O++;
            if( DA == O || O.Current() ==  NULL ) {
                if( Verbose ) {
                    if( IsError == false ) fprintf(stderr,"\n");
                    fprintf(stderr,"%s: option '%s' requires an argument\n", (char*)GetProgramName(),p_str->GetBuffer());
                    IsError = true;
                }
                result = false;
                continue;
            }
            O++;
        } else {
            int           id = O.CurrentID();
            CSmallString* p_tstr = O.Remove(false);   // remove object from list but do not destroy it
            A.InsertAfter(p_tstr,id,true);
            if( DA.IsValid() == false ) DA = A;
            if( FA.IsValid() == false ) FA = A;
        }
    }

    CO.SetToBegin();

    if( (AllowPrgArgs == false) && (GetNumberOfProgArgs() > 0) ) {
        if( Verbose ) {
            if( IsError == false ) fprintf(stderr,"\n");
            fprintf(stderr,"%s: arguments are not allowed (%d detected)\n", (char*)GetProgramName(),GetNumberOfProgArgs());
            IsError = true;
        }
        result = false;
    }

    Initialized = result;
    if( Initialized == false ) {
        Restart();
        if( ShowUsage ) PrintUsage(stderr);
        if( ShowMiniUsage ) PrintMiniUsage(stderr);
    }

    return(Initialized);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool  CSmallGetOpt::IsItShortOption(char sopt,bool& has_arg)
{
    return( FindShortOption(sopt,has_arg) != NULL );
}

//------------------------------------------------------------------------------

CSmallGetOpt::COption*  CSmallGetOpt::FindShortOption(char sopt,bool& has_arg)
{
    CSimpleIterator<COption>   I(OptList);
    COption*                   p_opt;

    has_arg = false;

    while( (p_opt = I.Current()) != NULL ) {
        if( p_opt->Short == sopt ) {
            has_arg = p_opt->HasArg;
            return(p_opt);
        }
        I++;
    }

    return(NULL);
}

//------------------------------------------------------------------------------

bool CSmallGetOpt::IsOptionSet(int optid)
{
    CSimpleIterator<COption>   I(OptList);
    COption*                   p_opt;

    while( (p_opt = I.Current()) != NULL ) {
        if( p_opt->OptID == optid ) {
            return(p_opt->Set);
        }
        I++;
    }

    return(false);
}

//------------------------------------------------------------------------------

void CSmallGetOpt::SetOption(int optid)
{
    CSimpleIterator<COption>   I(OptList);
    COption*                   p_opt;

    while( (p_opt = I.Current()) != NULL ) {
        if( p_opt->OptID == optid ) {
            p_opt->Set = true;
        }
        I++;
    }
}

//------------------------------------------------------------------------------

bool  CSmallGetOpt::IsItLongOption(const CSmallString& lopt,bool& has_arg)
{
    return( FindLongOption(lopt,has_arg) != NULL );
}

//------------------------------------------------------------------------------

CSmallGetOpt::COption*  CSmallGetOpt::FindLongOption(const CSmallString& lopt,bool& has_arg)
{
    CSimpleIterator<COption>   I(OptList);
    COption*                   p_opt;

    has_arg = false;

    while( (p_opt = I.Current()) != NULL ) {
        if( p_opt->Long == lopt ) {
            has_arg = p_opt->HasArg;
            return(p_opt);
        }
        I++;
    }

    return(NULL);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

const CSmallString& CSmallGetOpt::GetCurrOpt(void)
{
    return(CurrentOpt);
}

//------------------------------------------------------------------------------

const CSmallString& CSmallGetOpt::GetOptArg(void)
{
    static CSmallString null;
    if( HasOptArg() == false ) return(null);
    return(OptArgument);
}

//------------------------------------------------------------------------------

bool CSmallGetOpt::HasOptArg(void)
{
    return(HasArgument);
}

//------------------------------------------------------------------------------

int CSmallGetOpt::GetNumberOfProgArgs(void)
{
    CSimpleIterator<CSmallString>  A(CmdArgs);
    int                            narg = 0;
    A = FA;

    while( A.Current() != NULL ) {
        narg++;
        A++;
    }

    return(narg);
}

//------------------------------------------------------------------------------

const CSmallString& CSmallGetOpt::GetProgArg(int index)
{
    static CSmallString null_str;

    CSimpleIterator<CSmallString>  A(CmdArgs);
    int                            narg = 0;
    A = FA;

    while( A.Current() != NULL ) {
        if( narg == index ) return(*A.Current());
        narg++;
        A++;
    }

    return(null_str);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSmallGetOpt::COption::COption(void)
{
    Short = '\0';
    HasArg = false;
    OptID = GO_BAD_OPTION;
    Set = false;
}

//------------------------------------------------------------------------------

CSmallGetOpt::COption::COption(const char _short,const CSmallString& _long,const bool _hasarg,const int _id)
{
    Short = _short;
    Long = _long;
    HasArg = _hasarg;
    OptID = _id;
    Set = false;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
