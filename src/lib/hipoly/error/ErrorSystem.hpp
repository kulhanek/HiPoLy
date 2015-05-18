#ifndef ErrorSystemH
#define ErrorSystemH
//==============================================================================
//    Copyright 2001,2004 Petr Kulhanek
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
#include <XMLDocument.hpp>
#include <SimpleMutex.hpp>
#include <EventManager.hpp>
#include <Terminal.hpp>
#include <SmallTime.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

// -----------------------------------------------------------------------------

DECLARE_EVENT(HIPOLY_PACKAGE,ErrorSystem_Changed);

// -----------------------------------------------------------------------------

#define ES_ERROR(error) \
            ErrorSystem.AddError(error,__func__,error_file_path(__FILE__),__LINE__)

#define ES_ERROR_FROM_EXCEPTION(error,exception) \
            {\
                CSmallString text; \
                text << error << " (exception: " << e.what() << ")"; \
                ErrorSystem.AddError(text,__func__,error_file_path(__FILE__),__LINE__); \
            }

#define ES_TRACE_ERROR(error)\
            ErrorSystem.AddTraceError(error,__func__,error_file_path(__FILE__),__LINE__)

#define ES_WARNING(warning)\
            ErrorSystem.AddWarning(warning,__func__,error_file_path(__FILE__),__LINE__)

/*
This macro throws an exception to report an invalid argument.
*/
#define INVALID_ARGUMENT(error) \
            { \
                ES_ERROR(error); \
                std::stringstream str; \
                str << std::string(error) << " in " << std::string(__func__) << " "; \
                str << std::string(error_file_path(__FILE__)) << ":" << __LINE__; \
                throw std::invalid_argument( str.str() ); \
            }

/*
This macro throws an exception to report errors in the internal
logic of the program. These are theoretically preventable.
*/
#define LOGIC_ERROR(error) \
            { \
                ES_ERROR(error); \
                std::stringstream str; \
                str << std::string(error) << " in " << std::string(__func__) << " "; \
                str << std::string(error_file_path(__FILE__)) << ":" << __LINE__; \
                throw std::logic_error( str.str() ); \
            }

/*
This macro throws an exception to report errors
that can only be determined during runtime.
*/
#define RUNTIME_ERROR(error) \
            { \
                ES_ERROR(error); \
                std::stringstream str; \
                str << std::string(error) << " in " << std::string(__func__) << " "; \
                str << std::string(error_file_path(__FILE__)) << ":" << __LINE__; \
                throw std::runtime_error( str.str() ); \
            }

// -----------------------------------------------------------------------------

#define TRY_OBJECT(object) \
    try { \
        int result = 0; \
        switch(object.Init(argc,argv)) { \
            case SO_CONTINUE: \
                result = 0; \
                if(object.Run() == false) result = 1; \
                break; \
            case SO_EXIT: \
                return(0); \
            case SO_OPTS_ERROR: \
                return(1); \
            case SO_USER_ERROR: \
            default: \
                result = 2; \
                break; \
        } \
        object.Finalize(); \
        return(result); \
    } catch(std::exception& e) {\
        CTerminalStr tout; \
        tout << std::endl; \
        tout << "<red>>>> UNHANDLED EXCEPTION: " << e.what() << "</red>" << std::endl; \
        tout << std::endl; \
        ErrorSystem.PrintErrors(tout); \
        tout << std::endl; \
        return(255); \
    }

/*! use this macro as main function for classes conforming
    Init,Run, and Finalize methods - for details see TRY_OBJECT
*/

#define MAIN_ENTRY(classname) \
    int main(int argc,char* argv[]){ \
        classname object; \
        TRY_OBJECT(object); \
    }

#define MAIN_ENTRY_OBJECT(object) \
    int main(int argc,char* argv[]){ \
        TRY_OBJECT(object); \
    }

// -----------------------------------------------------------------------------

/*! \brief error messages
 *
 *  It records error messages and manages them.
 */

class HIPOLY_PACKAGE CErrorSystem : public CEventManager {
public:
    /// constructor
    CErrorSystem(void);

// -----------------------------------------------------------------------------
    /// append error message to error queue
    void AddError(const CSmallString& error,
                  const CSmallString& fce,
                  const CSmallString& file,int line);

    /// append trace error message to error queue
    void AddTraceError(const CSmallString& error,
                       const CSmallString& fce,
                       const CSmallString& file,int line);

    /// append warning message to error queue
    void AddWarning(const CSmallString& warning,
                    const CSmallString& fce,
                    const CSmallString& file,int line);

// -----------------------------------------------------------------------------
    /// return last error on the error stack
    const CSmallString GetLastError(void);

    /// return last trace error on the error stack
    const CSmallString GetLastTraceError(void);

    /// return last warning on the error stack
    const CSmallString GetLastWarning(void);

// -----------------------------------------------------------------------------
    /// is there an error?
    bool IsError(void);

    /// remove all errors
    void RemoveAllErrors(void);

    /// get root element containing error messages
    CXMLElement* GetRootElement(void);

    /// copy all errors from given element
    void LoadErrors(CXMLElement* p_root);

    /// copy all errors to given element
    void SaveErrors(CXMLElement* p_root);

    /// inject as remote errors to the current error stack
    void AddAsRemoteErrors(CXMLElement* p_root);

// -----------------------------------------------------------------------------
    /// append error into log file and optionaly remove all errors
    void AppendToErrLog(const CSmallString& logname,bool remove = false);

    /// print error messages to sout - formatted
    void PrintErrors(std::ostream& sout,bool header=true);

    /// print error messages to fout, if fout == NULL, then stderr is used
    void PrintErrors(FILE* fout=NULL,bool header=true);

    /// print last error message to fout, if fout == NULL, then stderr is used
    void PrintLastError(const CSmallString& prgname,FILE* fout=NULL,bool incverbose=true);

// section of private data ----------------------------------------------------
private:
    CXMLDocument    Errors;
    CXMLElement*    MainElement;
    CSimpleMutex    ErrorMutex;

    // error printing ----------------------------
    bool            TerminalInfoAvailable;
    int             NumberOfColumns;

    /// create main element - throw an exception if no memory
    void CreateMainElement(void);

    /// print header
    void PrintHeaderInternal(std::ostream& sout);

    /// without mutex !!!
    void PrintErrorsInternal(std::ostream& sout);
};

// -----------------------------------------------------------------------------

extern HIPOLY_PACKAGE CErrorSystem ErrorSystem;

// -----------------------------------------------------------------------------

HIPOLY_PACKAGE const char* error_file_path(const char* name);

// -----------------------------------------------------------------------------

#endif
