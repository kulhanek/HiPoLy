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

#include <ErrorSystem.hpp>
#include <SmallTime.hpp>
#include <XMLElement.hpp>
#include <ctype.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <sstream>
#include <XMLIterator.hpp>
#include <TerminalStr.hpp>
#include <iomanip>

//------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------

CErrorSystem ErrorSystem;

//------------------------------------------------------------------------------

DEFINE_EVENT(ErrorSystem_Changed);

//------------------------------------------------------------------------------

const char* error_file_path(const char* name)
{
    char* newstr;
    newstr = strstr((char*)name,"/projects/");
    if( newstr != NULL ){
        int offset = newstr - name;
        return( &name[offset+9] );
    }
    newstr = strstr((char*)name,"/thirdparty/");
    if( newstr != NULL ){
        int offset = newstr - name;
        return( &name[offset+11] );
    }
    return(name);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CErrorSystem::CErrorSystem(void)
    : CEventManager()
{
    EnableEvents(false);
    MainElement = NULL;
    TerminalInfoAvailable = false;
    NumberOfColumns = 80;
}

//------------------------------------------------------------------------------

void CErrorSystem::CreateMainElement(void)
{
    if( MainElement != NULL ) return;
    MainElement = Errors.CreateChildElement("STACK");
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CErrorSystem::AddError(const CSmallString& error,
                            const CSmallString& fce,
                            const CSmallString& file,int line)
{
    try {
        CreateMainElement();
    } catch(...) {
        // we do not want to throw an exception here
        return;
    }

    CSmallTimeAndDate time;
    time.GetActualTimeAndDate();

    ErrorMutex.Lock();

        try {
            CXMLElement* p_ele = MainElement->CreateChildElement("ERROR");

            p_ele->SetAttribute("text",error);
            p_ele->SetAttribute("fce",fce);
            p_ele->SetAttribute("file",file);
            p_ele->SetAttribute("line",line);
            p_ele->SetAttribute("time",time);
        } catch(...) {
            // nothing here - just to be sure that the mutex is released
        }

    ErrorMutex.Unlock();

    ThrowEvent(ErrorSystem_Changed);
}

//------------------------------------------------------------------------------

void CErrorSystem::AddTraceError(const CSmallString& error,
                                 const CSmallString& fce,
                                 const CSmallString& file,int line)
{
    try {
        CreateMainElement();
    } catch(...) {
        // we do not want to throw an exception here
        return;
    }

    CSmallTimeAndDate time;
    time.GetActualTimeAndDate();

    ErrorMutex.Lock();

        try {
            CXMLElement* p_ele = MainElement->CreateChildElement("TRACE");

            p_ele->SetAttribute("text",error);
            p_ele->SetAttribute("fce",fce);
            p_ele->SetAttribute("file",file);
            p_ele->SetAttribute("line",line);
            p_ele->SetAttribute("time",time);
        } catch(...) {
            // nothing here - just to be sure that the mutex is released
        }

    ErrorMutex.Unlock();

    ThrowEvent(ErrorSystem_Changed);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

//bool CErrorSystem::AddRemoteError(const CSmallString& error,const CSmallString& location,
//        const CSmallTimeAndDate& time)
//{
//        CreateMainElement();

//    ErrorMutex.Lock();

//    CXMLElement* p_ele = MainElement->CreateChildElement("REMERR");

//    bool result = false;

//    if( p_ele != NULL ) {
//        result = true;

//        result &= p_ele->SetAttribute("error",error);
//        result &= p_ele->SetAttribute("location",location);
//        result &= p_ele->SetAttribute("time",time);
//    }

//    ThrowEvent(ErrorSystem_Changed);
//    ErrorMutex.Unlock();

//    return(result);
//}

// -----------------------------------------------------------------------------

void CErrorSystem::AddWarning(const CSmallString& warning,
                              const CSmallString& fce,
                              const CSmallString& file,int line)
{
    try {
        CreateMainElement();
    } catch(...) {
        // we do not want to throw an exception here
        return;
    }

    CSmallTimeAndDate time;
    time.GetActualTimeAndDate();

    ErrorMutex.Lock();

        try {
            CXMLElement* p_ele = MainElement->CreateChildElement("WARNING");

            p_ele->SetAttribute("text",warning);
            p_ele->SetAttribute("fce",fce);
            p_ele->SetAttribute("file",file);
            p_ele->SetAttribute("line",line);
            p_ele->SetAttribute("time",time);
        } catch(...) {
            // nothing here - just to be sure that the mutex is released
        }

    ErrorMutex.Unlock();

    ThrowEvent(ErrorSystem_Changed);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

const CSmallString CErrorSystem::GetLastError(void)
{
    CSmallString text;

    if( MainElement == NULL ) return(text);

    ErrorMutex.Lock();

        CXMLElement* p_lele = MainElement->GetLastChildElement("ERROR");
        if( p_lele != NULL) p_lele->GetAttribute("text",text);

    ErrorMutex.Unlock();

    return(text);
}

//------------------------------------------------------------------------------

const CSmallString CErrorSystem::GetLastTraceError(void)
{
    CSmallString text;

    if( MainElement == NULL ) return(text);

    ErrorMutex.Lock();

        CXMLElement* p_lele = MainElement->GetLastChildElement("TRACE");
        if( p_lele != NULL) p_lele->GetAttribute("text",text);

    ErrorMutex.Unlock();

    return(text);
}

//------------------------------------------------------------------------------

const CSmallString CErrorSystem::GetLastWarning(void)
{
    CSmallString text;

    if( MainElement == NULL ) return(text);

    ErrorMutex.Lock();

        CXMLElement* p_lele = MainElement->GetLastChildElement("WARNING");
        if( p_lele != NULL) p_lele->GetAttribute("text",text);

    ErrorMutex.Unlock();

    return(text);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CErrorSystem::IsError(void)
{
    if( MainElement == NULL ) return(false);

    bool error = false;
    ErrorMutex.Lock();

        CXMLElement* p_lele = MainElement->GetLastChildElement("ERROR");
        if( p_lele != NULL) error = true;

    ErrorMutex.Unlock();

    return(error);
}

//------------------------------------------------------------------------------

void CErrorSystem::RemoveAllErrors(void)
{
    if( MainElement == NULL ) return;

    ErrorMutex.Lock();
        MainElement->RemoveAllChildNodes();
    ErrorMutex.Unlock();

    ThrowEvent(ErrorSystem_Changed);
}

//------------------------------------------------------------------------------

CXMLElement* CErrorSystem::GetRootElement(void)
{
    CreateMainElement(); // might throw exception
    return(MainElement);
}

//------------------------------------------------------------------------------

void CErrorSystem::LoadErrors(CXMLElement* p_root)
{
    if( p_root == NULL ){
        INVALID_ARGUMENT("p_root is NULL");
    }

    if( MainElement == NULL ) return;

    ErrorMutex.Lock();
        try {
            MainElement->CopyChildNodesFrom(p_root);
        } catch(...) {
            // nothing here - just to be sure that the mutex is released
        }
    ErrorMutex.Unlock();
}


//------------------------------------------------------------------------------

void CErrorSystem::SaveErrors(CXMLElement* p_root)
{
    if( p_root == NULL ){
        INVALID_ARGUMENT("p_root is NULL");
    }

    if( MainElement == NULL ) return;

    ErrorMutex.Lock();
        try {
            p_root->CopyChildNodesFrom(MainElement);
        } catch(...) {
            // nothing here - just to be sure that the mutex is released
        }
    ErrorMutex.Unlock();
}

//------------------------------------------------------------------------------

void CErrorSystem::AddAsRemoteErrors(CXMLElement* p_root)
{
    if( p_root == NULL ){
        INVALID_ARGUMENT("p_root is NULL");
    }

    try {
        CreateMainElement();
    } catch(...) {
        // we do not want to throw an exception here
        return;
    }

    CXMLIterator    I(p_root);
    CXMLElement*    p_err;

    ErrorMutex.Lock();
        try{
            while( (p_err = I.GetNextChildElement("ERROR")) != NULL ) {
                CXMLElement* p_ele = MainElement->CreateChildElement("REMOTE");
                p_ele->CopyContentsFrom(p_err);
                // CopyContentsFrom - overwite element name
                p_ele->SetName("REMOTE");
            }
        } catch(...) {
                // nothing here - just to be sure that mutex is released
        }
    ErrorMutex.Unlock();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CErrorSystem::AppendToErrLog(const CSmallString& logname,bool remove)
{
    FILE* p_fout = fopen(logname,"a");
    if( p_fout == NULL ) {
        CSmallString error;
        error << "unable to open log file '" << logname << "' (" << strerror(errno) << ")";
        ES_ERROR(error);
        return;
    }

    CTerminalStr tout;
    tout.Attach(p_fout);

    bool changed = false;

    ErrorMutex.Lock();
        PrintErrorsInternal(tout);
        if( remove ) {
            if( MainElement ) MainElement->RemoveAllChildNodes();
        }
    ErrorMutex.Unlock();

    tout.Attach(NULL);

    fclose(p_fout);

    if( changed ) ThrowEvent(ErrorSystem_Changed);
}

//------------------------------------------------------------------------------

void CErrorSystem::PrintErrors(std::ostream& sout,bool header)
{
    if( header ) PrintHeaderInternal(sout);
    ErrorMutex.Lock();
        PrintErrorsInternal(sout);
    ErrorMutex.Unlock();
}

//------------------------------------------------------------------------------

void CErrorSystem::PrintErrors(FILE* fout,bool header)
{
    if( fout == NULL ){
        fout = stderr;
    }

    CTerminalStr tout;
    tout.Attach(fout);

    if( header ) PrintHeaderInternal(tout);
    ErrorMutex.Lock();
        PrintErrorsInternal(tout);
    ErrorMutex.Unlock();
}

//------------------------------------------------------------------------------

void CErrorSystem::PrintLastError(const CSmallString& prgname,FILE* fout,bool incverbose)
{
    if(fout == NULL) {
        fout = stderr;
    }

    CTerminalStr tout;
    tout.Attach(fout);

    tout << "<red>" << prgname << ": ";
    if( GetLastError() != NULL ) {
        tout << GetLastError() << "</red>" << endl;
    } else {
        tout << "internal error occured" << "</red>" << endl;
    }

    if( incverbose == true ) {
        tout << "<blue>Try to use --verbose option for more details.</blue>" << endl;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CErrorSystem::PrintHeaderInternal(std::ostream& sout)
{
    sout << endl;

    // # >>>>>>>>>>>>>>>>>>>>>>>
    sout << "# ";
    for(int i=0; i < NumberOfColumns-2; i++) {
        sout << ">";
    }
    sout << endl;

    sout << "# Error and warning stack ..." << endl;

    // # >>>>>>>>>>>>>>>>>>>>>>>
    sout << "# ";
    for(int i=0; i < NumberOfColumns-2; i++) {
        sout << ">";
    }
    sout << endl;
}

//------------------------------------------------------------------------------

void CErrorSystem::PrintErrorsInternal(std::ostream& sout)
{
    if( MainElement == NULL ) return;

    CXMLElement* p_ele = MainElement->GetFirstChildElement();

    while( p_ele != NULL ) {
        CSmallString            text,fce,file,line;
        CSmallTimeAndDate       time;

        // we do not care if these operation succeeds
        p_ele->GetAttribute("text",text);
        p_ele->GetAttribute("fce",fce);
        p_ele->GetAttribute("file",file);
        p_ele->GetAttribute("line",line);
        p_ele->GetAttribute("time",time);

        // print header in color
        if( p_ele->GetName() == "ERROR" ) {
            sout << "<red>"     << "> E  " << time.GetSDateAndTime() << " in " << fce << "</red>";
        }
        if( p_ele->GetName() == "TRACE" ) {
            sout << "<purple>"  << "> TE " << time.GetSDateAndTime() << " in " << fce << "</purple>";
        }
        if( p_ele->GetName() == "WARNING" ) {
            sout << "<blue>"    << "> W  " << time.GetSDateAndTime() << " in " << fce << "</blue>";
        }
        if( p_ele->GetName() == "REMOTE" ) {
            sout << "<green>"   << "> RE " << time.GetSDateAndTime() << " in " << fce << "</green>";
        }

        // print file name if available
        sout << endl;
        sout << "  <cyan>" << file << ":" << line << "</cyan>" << endl;

        // print text message
        sout << "  " << text << endl;

        p_ele = p_ele->GetNextSiblingElement();
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

