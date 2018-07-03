#ifndef SimpleOptionH
#define SimpleOptionH
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <typeinfo>
#include <SmallString.hpp>
#include <SmallGetOpt.hpp>
#include <TerminalStr.hpp>

//------------------------------------------------------------------------------

#define SO_USER_ERROR  11
#define SO_OPTS_ERROR  10
#define SO_EXIT         0
#define SO_CONTINUE     1

//------------------------------------------------------------------------------

/*
 LIMITATION:
   - supported types for options:   bool, int, float, double, string, CSmallString
   - supported types for arguments: int, float, double, string, CSmallString
   - all arguments have to be mandatory
   - option with op_desc==NULL (bool) cannot be mandatory
*/

//------------------------------------------------------------------------------

/*
 CALLING SEQUENCE:
   - setup should be in the derived class constructor
   - ParseCmdLine
        - CheckOptions
        - FinalizeOptions
        - CheckArguments
*/

//------------------------------------------------------------------------------

/*! \brief program options handler
 *
 */

class HIPOLY_PACKAGE CSimpleOptions : public CSmallGetOpt {
public:
    CSimpleOptions(void);
    virtual ~CSimpleOptions(void);

    /// set options from cmd line
    int  ParseCmdLine(int argc,char* argv[]);
    int  ParseCmdLine(const std::string& cmdline);

    /// print program usage
    virtual void PrintUsage(FILE* fout=NULL);

    /// print program mini usage
    virtual void PrintMiniUsage(FILE* fout=NULL);

    /// print program mini usage
    virtual void PrintMiniUsage(std::ostream& vout);

    /// print program version
    virtual void PrintVersion(FILE* fout=NULL);

    /// print program version
    virtual void PrintVersion(std::ostream& vout);

//------------------------------------------------------------------------------
    /// return program name
    virtual const char* GetProgramName(void) const;

    /// return program description
    virtual const char* GetProgramDesc(void) const;

    /// return program version
    virtual const char* GetProgramVers(void) const;

    /// program argument short description
    virtual const char* GetProgArgsShortDesc(void) const;

    /// program argument long description
    virtual const char* GetProgArgsLongDesc(void) const;

// section of protected data ---------------------------------------------------
protected:
    virtual int CheckOptions(void);            // check options
    virtual int FinalizeOptions(void);         // finalize options
    virtual int CheckArguments(void);          // check arguments

    // formatted output for wiki output
    int                         CommandLevel;
    std::stringstream           Command;
    bool                        BlockMode;
    int                         LeftOffset;
    bool                        FirstSpace;

    void String2Wiki(const CSmallString& text,std::ostream& vout);
    void ResetFormattedStream(void);
    bool PrintFormattedChar(int c,std::ostream& vout);

    enum EActions {
        SET_OPT_DEFAULT,
        SET_ARG_DEFAULT,
        GET_OPT_DESC,
        GET_ARG_DESC,
        REGISTER_OPT,
        REGISTER_ARG,
        SET_OPT,
        SET_ARG,
        PRINT_DEFAULT_OPT_VALUE
    };

// section of private data ----------------------------------------------------
    virtual bool ProcessOption(int item,
                               EActions action,
                               bool& op_mand,
                               CSmallString& op_short,
                               CSmallString& op_long,
                               CSmallString& op_param,
                               CSmallString& op_desc);

    /// set default values of all options and arguments
    void SetDefault(void);

    /// runtime check of option and argument lists
    bool RunTimeCheck(void);

    /// return number of mandatory options
    int  GetNumOfMandOptsInList(void);

    /// return number of options
    int  GetNumOfOptsInList(void);

    /// return number of mandatory arguments
    int  GetNumOfMandArgsInList(void);

    /// return number of arguments
    int  GetNumOfArgsInList(void);

    /// return short option string
    bool RegisterOptions(void);

    /// register arguments
    bool RegisterArguments(void);

    /// set options
    bool SetOptions(void);

    /// set arguments
    bool SetArguments(void);

    /// print program usage
    void PrintUsageInTerminal(FILE* fout=NULL);

    /// print program usage
    void PrintUsageInTerminal(std::ostream& vout);

    /// print program usage in wiki format
    void PrintUsageInWiki(FILE* fout=NULL);

    /// print program usage in wiki format
    void PrintUsageInWiki(std::ostream& vout);

// convert options ------------------------------------------------------------
    // this SIMPLIFY macro expansion, for type detection we use overloading
    bool ConvertOption(const CSmallString& src_arg,bool& dest_opt);
    bool ConvertOption(const CSmallString& src_arg,int& dest_opt);
    bool ConvertOption(const CSmallString& src_arg,float& dest_opt);
    bool ConvertOption(const CSmallString& src_arg,double& dest_opt);
    bool ConvertOption(const CSmallString& src_arg,std::string& dest_opt);
    bool ConvertOption(const CSmallString& src_arg,CSmallString& dest_opt);

    // print default option value
    bool PrintDefaultOptValue(bool& dest_opt);
    bool PrintDefaultOptValue(int& dest_opt);
    bool PrintDefaultOptValue(float& dest_opt);
    bool PrintDefaultOptValue(double& dest_opt);
    bool PrintDefaultOptValue(std::string& dest_opt);
    bool PrintDefaultOptValue(CSmallString& dest_opt);

// convert arguments ------------------------------------------------------------
    // this SIMPLIFY macro expansion, for type detection we use overloading
    bool ConvertArgument(const CSmallString& src_arg,int& dest_arg);
    bool ConvertArgument(const CSmallString& src_arg,float& dest_arg);
    bool ConvertArgument(const CSmallString& src_arg,double& dest_arg);
    bool ConvertArgument(const CSmallString& src_arg,std::string& dest_arg);
    bool ConvertArgument(const CSmallString& src_arg,CSmallString& dest_arg);

// protected data --------------------------------------------------------------
protected:
    bool                ConversionError;    // true if setopts or setargs raise error
    std::stringstream   DefValue;
};

//------------------------------------------------------------------------------

// program name
#define CSO_PROG_NAME_BEGIN virtual const char* GetProgramName(void)  const { return(
#define CSO_PROG_NAME_END );}

// program description
#define CSO_PROG_DESC_BEGIN virtual const char* GetProgramDesc(void) const { return(
#define CSO_PROG_DESC_END );}

// program version
#define CSO_PROG_VERS_BEGIN virtual const char* GetProgramVers(void) const { return(
#define CSO_PROG_VERS_END );}

// program arguments short description
#define CSO_PROG_ARGS_SHORT_DESC_BEGIN virtual const char* GetProgArgsShortDesc(void) const { return(
#define CSO_PROG_ARGS_SHORT_DESC_END );}

// program arguments long description
#define CSO_PROG_ARGS_LONG_DESC_BEGIN virtual const char* GetProgArgsLongDesc(void) const { return(
#define CSO_PROG_ARGS_LONG_DESC_END );}

// definition of options ------------------------------------------------------
#define CSO_LIST_BEGIN
#define CSO_OPT(type,name) \
    private: \
        type             opt_##name; \
        bool             opt_is_set_##name; \
    public: \
        type GetOpt##name(bool& set) const \
        { \
          set = opt_is_set_##name; \
          return(opt_##name); \
        } \
        type GetOpt##name(void) const \
        { \
          return(opt_##name); \
        } \
        bool IsOpt##name##Set(void) const \
        { \
          return(opt_is_set_##name); \
        } \
        void SetOpt##name(type value) \
        { \
          opt_##name = value; \
        }
#define CSO_ARG(type,name) \
    private: \
        type             arg_##name; \
        bool             arg_is_set_##name; \
    public: \
        type GetArg##name(bool& set) const \
        { \
          set = arg_is_set_##name; \
          return(arg_##name); \
        }; \
        type GetArg##name(void) const \
        { \
          return(arg_##name); \
        };
#define CSO_LIST_END

// main option handler ---------------------------------------------------------
// arguments are counted from zero
// options are counted from 1 (option number is also id used in CSmallGetOpt::COption class)
#define CSO_MAP_BEGIN virtual bool ProcessOption(int item, \
                                                 EActions action, \
                                                 bool& op_mand, \
                                                 CSmallString& op_short, \
                                                 CSmallString& op_long, \
                                                 CSmallString& op_param, \
                                                 CSmallString& op_desc){ \
    int opt_counter __attribute__ ((unused)); \
    opt_counter = 0; \
    int arg_counter __attribute__ ((unused)); \
    arg_counter = -1;

#define CSO_MAP_ARG(type,name,default_value,mand,param,desc) \
    arg_counter++; \
    if( item == arg_counter ){ \
        switch(action){ \
            case SET_ARG_DEFAULT: \
                arg_is_set_##name = false; \
                arg_##name = default_value; \
                return(true); \
            case GET_ARG_DESC: \
                op_mand = mand; \
                op_short = NULL; \
                op_long = NULL; \
                op_param = param; \
                op_desc = desc; \
                return(true); \
            case REGISTER_ARG: \
                return(true); \
            case SET_ARG: \
                arg_is_set_##name = ConvertArgument(GetProgArg(arg_counter),arg_##name); \
                return(true); \
            default: \
                break; \
            } \
        }

#define CSO_MAP_OPT(type,name,default_value,mand,short,long,param,desc) \
    opt_counter++; \
    if( item == opt_counter ){ \
        switch(action){ \
            case SET_OPT_DEFAULT: \
                opt_is_set_##name = false; \
                opt_##name = default_value; \
                return(true); \
            case GET_OPT_DESC: \
                op_mand = mand; \
                op_short = short; \
                op_long = long; \
                op_param = param; \
                op_desc = desc; \
                return(true); \
            case REGISTER_OPT:{ \
                bool haspar = typeid(type) != typeid(bool); \
                AddOption(short,long,haspar,opt_counter); \
                return(true); \
                } \
            case SET_OPT: \
                opt_is_set_##name = ConvertOption(GetOptArg(),opt_##name); \
                return(true); \
            case PRINT_DEFAULT_OPT_VALUE: \
                return(PrintDefaultOptValue(opt_##name)); \
            default: \
                return(false); \
            } \
        }

#define CSO_MAP_END return(false); }

// -----------------------------------------------------------------------------

#endif
