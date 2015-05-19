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

#include <SimpleOptions.hpp>
#include <string.h>
#include <Terminal.hpp>
#include <VerboseStr.hpp>

//------------------------------------------------------------------------------

#define SO_NAME_LENGTH      30
#define SO_NEXT_LENGTH      31

//------------------------------------------------------------------------------

using namespace std;

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSimpleOptions::CSimpleOptions(void)
{
    ConversionError = false;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSimpleOptions::~CSimpleOptions(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CSimpleOptions::ParseCmdLine(const std::string& cmdline)
{
    std::vector<std::string>    args;
    std::istringstream          iss(cmdline);
    std::string                 arg;
    
    // split(args,cmdline,is_any_of("\t "),boost::token_compress_on);
    for(;;){
        iss >> arg;
        if( ! iss ) break;
        args.push_back(arg);
    }

    char** argv;
    argv = (char**)malloc(args.size()*sizeof(char*));
    if( argv == NULL ) return(SO_OPTS_ERROR);
    for(size_t i=0; i < args.size(); i++){
        argv[i] = (char*)malloc(args[i].size()*sizeof(char)+1);
        if( argv[i] == NULL ){
            // rollback
            for(size_t j=0; j < i; j++){
                free(argv[j]);
            }
            free(argv);
            return(SO_OPTS_ERROR);
        }
        strcpy(argv[i],args[i].c_str());
    }

    int rcode = ParseCmdLine(args.size(),argv);

    // release
    for(size_t i=0; i < args.size(); i++){
        free(argv[i]);
    }
    free(argv);

    return(rcode);
}

//------------------------------------------------------------------------------

int CSimpleOptions::ParseCmdLine(int argc,char* argv[])
{
    // start from begining
    ConversionError = false;
    Restart();
    ClearOptionList();

    CTerminal terminal;
    terminal.SetOutputStream(stderr);
    terminal.SetColors(CTerminal::EC_RED);

    // set command line
    try {
        SetCmdLine(argc,argv);
    } catch(...) {
        if( IsVerbose() == true ) fprintf(stderr,"\n");
        terminal.SetDefault();
        return(SO_OPTS_ERROR);
    }

    // set default params
    SetDefault();

    // runtime check
    if( RunTimeCheck() == false ) {
        if( IsVerbose() == true ) fprintf(stderr,"\n");
        terminal.SetDefault();
        return(SO_OPTS_ERROR);
    }

    // register options
    if( RegisterOptions() == false ) {
        if( IsVerbose() == true ) fprintf(stderr,"\n");
        terminal.SetDefault();
        return(SO_OPTS_ERROR);
    }

    // register arguments
    if( RegisterArguments() == false ) {
        if( IsVerbose() == true ) fprintf(stderr,"\n");
        terminal.SetDefault();
        return(SO_OPTS_ERROR);
    }

    // initialize options
    if( InitializeOptions() == false ) {
        if( IsVerbose() == true ) fprintf(stderr,"\n");
        terminal.SetDefault();
        return(SO_OPTS_ERROR);
    }

    // go through list and initialize options
    if( SetOptions() == false ) {
        if( IsShowUsage() == true ) PrintUsage(stderr);
        if( IsShowMiniUsage() == true ) PrintMiniUsage(stderr);
        if( IsVerbose() == true ) fprintf(stderr,"\n");
        terminal.SetDefault();
        return(SO_OPTS_ERROR);
    }

    int result;
    // check options
    if( (result = CheckOptions()) != SO_CONTINUE ) {
        if( result == SO_OPTS_ERROR ) {
            if( IsShowUsage() == true ) PrintUsage(stderr);
            if( IsShowMiniUsage() == true ) PrintMiniUsage(stderr);
            if( IsVerbose() == true ) fprintf(stderr,"\n");
        }
        terminal.SetDefault();
        return(result);
    }

    // final operations with options
    if( (result = FinalizeOptions()) != SO_CONTINUE ) {
        if( result == SO_OPTS_ERROR ) {
            if( IsShowUsage() == true ) PrintUsage(stderr);
            if( IsShowMiniUsage() == true ) PrintMiniUsage(stderr);
            if( IsVerbose() == true ) fprintf(stderr,"\n");
        }
        terminal.SetDefault();
        return(result);
    }

    // go through list and initialize arguments
    if( SetArguments() == false ) {
        if( IsShowUsage() == true ) PrintUsage(stderr);
        if( IsShowMiniUsage() == true ) PrintMiniUsage(stderr);
        if( IsVerbose() == true ) fprintf(stderr,"\n");
        terminal.SetDefault();
        return(SO_OPTS_ERROR);
    }

    // check number of arguments
    if( GetProgArgsShortDesc() == NULL ) {
        if( GetNumberOfProgArgs() != GetNumOfArgsInList() ) {
            if( IsVerbose() ) {
                if( IsError == false ) fprintf(stderr,"\n");
                fprintf(stderr,"%s: incorrect number of arguments, %d provided but %d required\n", (char*)GetProgramName(),GetNumberOfProgArgs(),GetNumOfArgsInList());
                IsError = true;
                if( IsShowUsage() == true ) PrintUsage(stderr);
                if( IsShowMiniUsage() == true ) PrintMiniUsage(stderr);
                fprintf(stderr,"\n");
            }
            terminal.SetDefault();
            return(SO_OPTS_ERROR);
        }
    }

    // check arguments
    if( (result = CheckArguments()) != SO_CONTINUE ) {
        if( result == SO_OPTS_ERROR ) {
            if( IsShowUsage() == true ) PrintUsage(stderr);
            if( IsShowMiniUsage() == true ) PrintMiniUsage(stderr);
            if( IsVerbose() == true ) fprintf(stderr,"\n");
        }
        terminal.SetDefault();
        return(result);
    }

    terminal.SetDefault();
    return(SO_CONTINUE);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSimpleOptions::PrintUsage(FILE* fout)
{
    char* p_help = getenv("HIPOLY_HELP");

    if( p_help == NULL ) {
        PrintUsageInTerminal(fout);
        return;
    }

    if( strcmp(p_help,"wiki") == 0 ) {
        PrintUsageInWiki(fout);
        return;
    }
}

//------------------------------------------------------------------------------

void CSimpleOptions::PrintUsageInTerminal(FILE* fout)
{
    if( fout == NULL ) {
        fout = stdout;  // if output stream is not specified use standard output
    }
    CTerminalStr vout;
    vout.Attach(fout);
    PrintUsageInTerminal(vout);
}

//------------------------------------------------------------------------------

void CSimpleOptions::PrintUsageInTerminal(std::ostream& vout)
{
    vout << endl;

    // count number of options
    int cmand = GetNumOfMandOptsInList();
    int copt = GetNumOfOptsInList();

    // print header --------------------------------
    vout << "<b>Usage: <red>" << GetProgramName() << "</red>";
    if( cmand == 0 ) {
        if( copt > 0 ) {
            vout << " <blue>[OPTIONS]</blue>";
        }
    } else {
        vout << " <blue>OPTIONS</blue>";
    }
    vout << "</b>";

    int            i;
    bool           op_mand;
    CSmallString   op_short,op_long,op_param,op_desc;

    int cmarg = 0;
    int carg = 0;

    if( GetProgArgsShortDesc() == NULL ) {
        // count number of arguments
        cmarg = GetNumOfMandArgsInList();
        carg = GetNumOfArgsInList();

        if( copt > 0 && carg > 0 ) {
            vout << " [--]";
        }

        i = 0; // arguments are addressed from zero
        vout << "<b><cyan>";
        while(ProcessOption(i,GET_ARG_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
            if( op_mand == true ) {
                vout << " " << op_param;
            } else {
                vout << " [" << op_param << "]";
            }
            i++;
        }
        vout << "</cyan></b>";
    } else {
        if( copt > 0 ) {
            vout << " [--]";
        }
        vout << " <b><cyan>" << GetProgArgsShortDesc() << "</cyan></b>";
    }
    vout << endl << endl;

    // print description ---------------------------
    const char* p_progdescr = GetProgramDesc();
    if( p_progdescr != NULL ) {
        vout << GetProgramDesc() << endl;
    }
    vout << endl;

    // print arguments -----------------------------
    if( GetProgArgsLongDesc() == NULL ) {
        if( carg > 0 ) {
            vout << "<b>Arguments:</b>" << endl;
        }
        i = 0; // arguments are addressed from zero
        while(ProcessOption(i,GET_ARG_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
            if( i > 0 ) vout << endl;
            if(op_mand == true) {
                vout << "<b><yellow>* </yellow></b>";
            } else {
                vout << "  ";
            }
            vout << "<b><cyan>" << op_param << "</cyan></b>" << endl;
            vout << "<block 10>" << op_desc << "</block>" << endl;
            i++;
        }
        if( copt > 0 && carg > 0 ) vout << endl;
    } else {
        vout << GetProgArgsLongDesc() << endl;
        if( copt > 0 ) vout << endl;
    }

    // print options -----------------------------
    if( copt > 0 ) {
        vout << "<b>Options:</b>" << endl;
    }
    i = 1;
    while(ProcessOption(i,GET_OPT_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
        if( i > 1 ){
            vout << endl;
        }
        if(op_mand == true) {
            vout << "<b><yellow>* </yellow></b>";
        } else {
            vout << "  ";
        }
        if( op_short == NULL ) {
            if( op_param == NULL ) {
                vout << "<b><blue>      --" << op_long << "</blue></b>" << endl;
            } else {
                vout << "<b><blue>      --"<< op_long << "</blue></b> <blue>" << op_param << "</blue>" << endl;
            }
        }
        if( op_long == NULL ) {
            if( op_param == NULL ) {
                vout << "<b><blue>  -" << op_short << "</blue></b>" << endl;
            } else {
                vout << "<b><blue>  -"<< op_short << "</blue></b> <blue>" << op_param  << "</blue>" <<  endl;
            }
        }
        if( (op_short != NULL) && (op_long != NULL) ) {
            if( op_param == NULL ) {
                vout << "<b><blue>  -" << op_short << ", --" << op_long << "</blue></b>" << endl;
            } else {
                vout << "<b><blue>  -" << op_short << ", --" << op_long  << "</blue></b> <blue>" << op_param << "</blue>" << endl;
            }
        }
        // print option description
        if(op_desc != NULL){
            vout << "<block 10>" << op_desc << "</block>" << endl;
        }
        // print option default value
        if(op_param != NULL) {
            DefValue.str("");
            ProcessOption(i,PRINT_DEFAULT_OPT_VALUE,op_mand,op_short,op_long,op_param,op_desc);
            if( ! DefValue.str().empty() ){
                vout << "<block 10><purple>(Default: " << DefValue.str() << ")</purple></block>" << endl;
            }
        }
        i++;
    }

    // write comment about mandatory options
    if( cmand > 0 || cmarg > 0 ) {
        vout << endl;
        vout << "<b><yellow>*</yellow></b> mandatory arguments and options." << endl;
    }
}

//------------------------------------------------------------------------------

void CSimpleOptions::PrintUsageInWiki(FILE* fout)
{
    if( fout == NULL ) {
        fout = stdout;  // if output stream is not specified use standard output
    }
    if( fout == stdout ){
        PrintUsageInWiki(cout);
    }
    if( fout == stderr ){
        PrintUsageInWiki(cerr);
    }
}

//------------------------------------------------------------------------------

void CSimpleOptions::PrintUsageInWiki(std::ostream& vout)
{
    // count number of options
    int cmand = GetNumOfMandOptsInList();
    int copt = GetNumOfOptsInList();

    // print header --------------------------------
    vout << "'''Name'''" << endl << endl;
    vout << GetProgramName() << endl << endl;

    vout << "'''Synopsis'''" << endl << endl;
    vout << "<span style=\"color: maroon;\">" << GetProgramName() << "</span>";

    if( cmand == 0 ) {
        if( copt > 0 ) {
            vout << " <span style=\"color: blue;\">[OPTIONS]</span>";
        }
    } else {
        vout << " <span style=\"color: blue;\">OPTIONS</span>";
    }

    int            i;
    bool           op_mand;
    CSmallString   op_short,op_long,op_param,op_desc;

    //int cmarg = 0;
    int carg = 0;

    if( GetProgArgsShortDesc() == NULL ) {
        // count number of arguments
        //cmarg = GetNumOfMandArgsInList();
        carg = GetNumOfArgsInList();

        if( copt > 0 && carg > 0 ) {
            vout << " [--]";
        }

        i = 0; // arguments are addressed from zero
        while(ProcessOption(i,GET_ARG_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
            if( op_mand == true ) {
                vout << " <span style=\"color: purple;\">" << op_param << "</span>";
            } else {
                vout << " <span style=\"color: purple;\">[" << op_param << "]</span>";
            }
            i++;
        }
    } else {
        if( copt > 0 ) {
            vout << " [--]";
        }
        vout << " <span style=\"color: purple;\">" << GetProgArgsShortDesc() << "</span>";
    }
    vout << endl << endl;

    vout << "'''Description'''" << endl << endl;
    vout << GetProgramDesc() << endl << endl;

    // print arguments -----------------------------
    if( GetProgArgsLongDesc() == NULL ) {
        if( carg > 0 ) {
            vout << "'''Arguments'''" << endl << endl;
            vout << "{| style=\"margin-left: 2em; width: 75%;\"\n";
        }
        i = 0; // arguments are addressed from zero
        while(ProcessOption(i,GET_ARG_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
            if( i % 2 == 0 ){
                vout << "|- style=\"vertical-align: top; background-color: #e6e6e6;\"" << endl;
            } else {
                vout << "|- style=\"vertical-align: top;\"" << endl;
            }
            vout << "| <span style=\"color: purple;\">" << op_param << "</span><br/>" << endl;
            vout << "<span style=\"margin-left: 30px;\">" << op_desc << "</span>" << endl;
            i++;
        }
        if( carg > 0 ) vout << "|}" << endl << endl;
    } else {
        vout << GetProgArgsLongDesc() << endl << endl;
    }

    // print options -----------------------------
    if( copt > 0 ) {
            vout << "'''Options'''" << endl << endl;
            vout << "{| style=\"margin-left: 2em; width: 95%;\"" << endl;
    }
    i = 1;
    while(ProcessOption(i,GET_OPT_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
        if( i % 2 == 1 ){
            vout << "|- style=\"vertical-align: top; background-color: #e6e6e6;\"" << endl;
        } else {
            vout << "|- style=\"vertical-align: top;\"" << endl;
        }
        if( op_short == NULL ) {
            if( op_param == NULL ) {
                vout << "|<span style=\"color: blue;\">--" << op_long << "</span><br/>";
            } else {
                vout << "|<span style=\"color: blue;\">--" << op_long << " " << op_param << "</span><br/>";
            }
        }
        if( op_long == NULL ) {
            if( op_param == NULL ) {
                vout << "|<span style=\"color: blue;\">-" << op_short << "</span><br/>";
            } else {
                vout << "|<span style=\"color: blue;\">-" << op_short << " " << op_param << "</span><br/>";
            }
        }
        if( (op_short != NULL) && (op_long != NULL) ) {
            if( op_param == NULL ) {
                vout << "|<span style=\"color: blue;\">-" << op_short << ", --" << op_long << "</span><br/>";
            } else {
                vout << "|<span style=\"color: blue;\">-" << op_short << ", --" << op_long << " " << op_param << "</span><br/>";
            }
        }
        // print option description
        if(op_desc != NULL) vout << "<span style=\"margin-left: 30px;\">" << op_desc << "</span>";
        // print option default value
        if(op_param != NULL) {
            DefValue.str("");
            ProcessOption(i,PRINT_DEFAULT_OPT_VALUE,op_mand,op_short,op_long,op_param,op_desc);
            vout << "<br/>";
            vout << "<span style=\"color:purple; margin-left: 30px;\">(Default: " << DefValue.str() << ")</span>";
        }
        vout << endl;
        i++;
    }
    if( copt > 0 ) vout << "|}" << endl << endl;
}

//------------------------------------------------------------------------------

void CSimpleOptions::PrintMiniUsage(FILE* fout)
{
    if( fout == NULL ) {
        fout = stdout;    // if output stream is not specified use standard output
    }
    CTerminalStr vout;
    vout.Attach(fout);
    PrintMiniUsage(vout);
}

//------------------------------------------------------------------------------

void CSimpleOptions::PrintMiniUsage(std::ostream& vout)
{
    vout << "<blue>'" << GetProgramName() << " --help' for more information.</blue>" << endl;
}

//------------------------------------------------------------------------------

void CSimpleOptions::PrintVersion(FILE* fout)
{
    if( fout == NULL ) {
        fout = stdout;  // if output stream is not specified use standard output
    }
    CTerminalStr vout;
    vout.Attach(fout);
    PrintVersion(vout);
}

//------------------------------------------------------------------------------

void CSimpleOptions::PrintVersion(std::ostream& vout)
{
    vout << endl;
    vout << "<blue>" << GetProgramName() << ": version " << GetProgramVers() << "</blue>" << endl;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CSimpleOptions::CheckOptions(void)
{
    return(SO_CONTINUE);
}

//------------------------------------------------------------------------------

int CSimpleOptions::FinalizeOptions(void)
{
    return(SO_CONTINUE);
}

//------------------------------------------------------------------------------

int CSimpleOptions::CheckArguments(void)
{
    return(SO_CONTINUE);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

const char* CSimpleOptions::GetProgramName(void) const
{
    return( CSmallGetOpt::GetProgramName() );
}

//------------------------------------------------------------------------------

const char* CSimpleOptions::GetProgramDesc(void) const
{
    return(NULL);
}

//------------------------------------------------------------------------------

const char* CSimpleOptions::GetProgramVers(void) const
{
    return("0.0.0.0");
}

//------------------------------------------------------------------------------

const char* CSimpleOptions::GetProgArgsShortDesc(void) const
{
    return(NULL);
}

//------------------------------------------------------------------------------

const char* CSimpleOptions::GetProgArgsLongDesc(void) const
{
    return(NULL);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSimpleOptions::ProcessOption(int item,
                                   EActions action,
                                   bool& op_mand,
                                   CSmallString& op_short,
                                   CSmallString& op_long,
                                   CSmallString& op_param,
                                   CSmallString& op_desc)
{
    return(false);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CSimpleOptions::SetDefault(void)
{
    int            i;
    bool            op_mand;
    CSmallString     op_short,op_long,op_param,op_desc;

    i = 1;
    while(ProcessOption(i,SET_OPT_DEFAULT,op_mand,op_short,op_long,op_param,op_desc)) {
        i++;
    }

    i = 1;
    while(ProcessOption(i,SET_ARG_DEFAULT,op_mand,op_short,op_long,op_param,op_desc)) {
        i++;
    }
}

//------------------------------------------------------------------------------

bool CSimpleOptions::RunTimeCheck(void)
{
    bool           result = true;

    int            i;
    bool           op_mand;
    CSmallString   op_short,op_long,op_param,op_desc;

    // options
    i = 1;
    while(ProcessOption(i,GET_OPT_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
        if( (op_long == NULL) && (op_short == NULL) ) {
            fprintf(stderr," >>> RUNTIME ERROR [RunTimeCheck]: Short and long option names are NULL (%d)!\n",i);
            result = false;
        }
        if( (op_param == NULL) && (op_mand == true) ) {
            fprintf(stderr," >>> RUNTIME ERROR [RunTimeCheck]: Option with op_param=NULL (bool?) cannot be mandatory (%d)!\n",i);
            result = false;
        }
        i++;
    }

    i = 1;
    bool arg_man = true;
    while(ProcessOption(i,GET_ARG_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
        if( op_param == NULL ) {
            fprintf(stderr," >>> RUNTIME ERROR [RunTimeCheck]: Argument name is NULL (%d)!\n",i);
            result = false;
        }
        arg_man &= op_mand;
        i++;
    }

    if( arg_man == false ) {
        fprintf(stderr," >>> RUNTIME ERROR [RunTimeCheck]: All arguments have to be mandatory!\n");
        result = false;
    }

    return(result);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CSimpleOptions::GetNumOfMandOptsInList(void)
{
    int            i = 1;
    int            count = 0;
    bool            op_mand;
    CSmallString     op_short,op_long,op_param,op_desc;

    while(ProcessOption(i,GET_OPT_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
        if( op_mand == true ) count++;
        i++;
    }

    return(count);
}

//------------------------------------------------------------------------------

int CSimpleOptions::GetNumOfOptsInList(void)
{
    int            i = 1;
    int            count = 0;
    bool           op_mand;
    CSmallString   op_short,op_long,op_param,op_desc;

    while(ProcessOption(i,GET_OPT_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
        count++;
        i++;
    }

    return(count);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::RegisterOptions(void)
{
    int            i = 1;
    bool            op_mand;
    CSmallString     op_short,op_long,op_param,op_desc;

    while(ProcessOption(i,REGISTER_OPT,op_mand,op_short,op_long,op_param,op_desc)) {
        i++;
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::SetOptions(void)
{
    int            optid;
    bool           op_mand;
    CSmallString   op_short,op_long,op_param,op_desc;

    while( (optid = GetOpt()) >= 0 ) {
        ProcessOption(optid,SET_OPT,op_mand,op_short,op_long,op_param,op_desc);
    }

    if( ConversionError == true ) return(false);

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CSimpleOptions::GetNumOfMandArgsInList(void)
{
    int            i = 0;
    int            count = 0;
    bool           op_mand;
    CSmallString   op_short,op_long,op_param,op_desc;

    while(ProcessOption(i,GET_ARG_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
        if( op_mand == true ) count++;
        i++;
    }

    return(count);
}

//------------------------------------------------------------------------------

int CSimpleOptions::GetNumOfArgsInList(void)
{
    int            i = 0;
    int            count = 0;
    bool           op_mand;
    CSmallString   op_short,op_long,op_param,op_desc;

    while(ProcessOption(i,GET_ARG_DESC,op_mand,op_short,op_long,op_param,op_desc)) {
        count++;
        i++;
    }

    return(count);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::RegisterArguments(void)
{
    int            i = 0;
    int            count = 0;
    bool           op_mand;
    CSmallString   op_short,op_long,op_param,op_desc;

    while(ProcessOption(i,REGISTER_ARG,op_mand,op_short,op_long,op_param,op_desc)) {
        count++;
        i++;
    }

    if( count > 0 ) SetAllowProgArgs(true);

    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::SetArguments(void)
{
    bool           op_mand;
    CSmallString   op_short,op_long,op_param,op_desc;

    for(int id=0; id < GetNumberOfProgArgs(); id++) {
        ProcessOption(id,SET_ARG,op_mand,op_short,op_long,op_param,op_desc);
    }

    if( ConversionError == true ) return(false);

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSimpleOptions::ConvertOption(const CSmallString& src_arg,bool& dest_opt)
{
    dest_opt = true;
    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::ConvertOption(const CSmallString& src_arg,int& dest_opt)
{
    char* end = NULL;
    dest_opt = strtol(src_arg,&end,10);

    if( *end != '\0' ) {
        if( IsVerbose() == true) {
            if( IsError == false ) fprintf(stderr,"\n");
            fprintf(stderr,
                    "%s: cannot convert option argument to integer, option: %s, arg: %s\n",
                    (const char*)GetProgramName(),(const char*)GetCurrOpt(),(const char*)src_arg);
            IsError = true;
        }
        ConversionError = true;
        return(false);
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::ConvertOption(const CSmallString& src_arg,float& dest_opt)
{
    char* end = NULL;
    dest_opt = strtof(src_arg,&end);

    if( *end != '\0' ) {
        if( IsVerbose() == true) {
            if( IsError == false ) fprintf(stderr,"\n");
            fprintf(stderr,
                    "%s: cannot convert option argument to float, option: %s, arg: %s\n",
                    (const char*)GetProgramName(),(const char*)GetCurrOpt(),(const char*)src_arg);
            IsError = true;
        }
        ConversionError = true;
        return(false);
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::ConvertOption(const CSmallString& src_arg,double& dest_opt)
{
    char* end = NULL;
    dest_opt = strtod(src_arg,&end);

    if( *end != '\0' ) {
        if( IsVerbose() == true) {
            if( IsError == false ) fprintf(stderr,"\n");
            fprintf(stderr,
                    "%s: cannot convert option argument to double, option: %s, arg: %s\n",
                    (const char*)GetProgramName(),(const char*)GetCurrOpt(),(const char*)src_arg);
            IsError = true;
        }
        ConversionError = true;
        return(false);
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::ConvertOption(const CSmallString& src_arg,std::string& dest_opt)
{
    dest_opt = src_arg;
    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::ConvertOption(const CSmallString& src_arg,CSmallString& dest_opt)
{
    dest_opt = src_arg;
    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSimpleOptions::PrintDefaultOptValue(bool& dest_opt)
{
    if( dest_opt == true ){
        DefValue << "true";
    } else {
        DefValue << "false";
    }
    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::PrintDefaultOptValue(int& dest_opt)
{
    DefValue << dest_opt;
    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::PrintDefaultOptValue(float& dest_opt)
{
    DefValue << dest_opt;
    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::PrintDefaultOptValue(double& dest_opt)
{
    DefValue << dest_opt;
    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::PrintDefaultOptValue(CSmallString& dest_opt)
{
    DefValue << dest_opt;
    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::PrintDefaultOptValue(std::string& dest_opt)
{
    DefValue << dest_opt;
    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CSimpleOptions::ConvertArgument(const CSmallString& src_arg,int& dest_arg)
{
    char* end = NULL;
    dest_arg = strtol(src_arg,&end,10);

    if( *end != '\0' ) {
        if( IsVerbose() == true) {
            if( IsError == false ) fprintf(stderr,"\n");
            fprintf(stderr,
                    "%s: cannot convert argument to integer, arg: %s\n",
                    (const char*)GetProgramName(),(const char*)src_arg);
            IsError = true;
        }
        ConversionError = true;
        return(false);
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::ConvertArgument(const CSmallString& src_arg,float& dest_arg)
{
    char* end = NULL;
    dest_arg = strtof(src_arg,&end);

    if( *end != '\0' ) {
        if( IsVerbose() == true) {
            if( IsError == false ) fprintf(stderr,"\n");
            fprintf(stderr,
                    "%s: cannot convert argument to float, arg: %s\n",
                    (const char*)GetProgramName(),(const char*)src_arg);
            IsError = true;
        }
        ConversionError = true;
        return(false);
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::ConvertArgument(const CSmallString& src_arg,double& dest_arg)
{
    char* end = NULL;
    dest_arg = strtod(src_arg,&end);

    if( *end != '\0' ) {
        if( IsVerbose() == true) {
            if( IsError == false ) fprintf(stderr,"\n");
            fprintf(stderr,
                    "%s: cannot convert argument to double, arg: %s\n",
                    (const char*)GetProgramName(),(const char*)src_arg);
            IsError = true;
        }
        ConversionError = true;
        return(false);
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CSimpleOptions::ConvertArgument(const CSmallString& src_arg,CSmallString& dest_arg)
{
    dest_arg = src_arg;
    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


