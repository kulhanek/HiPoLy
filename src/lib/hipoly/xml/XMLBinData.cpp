//==============================================================================
//    Copyright 2000,2004,2008,2011 Petr Kulhanek
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

#include <string.h>
#include <XMLBinData.hpp>
#include <ErrorSystem.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLBinData::CXMLBinData(CXMLNode* p_parent)
    : CXMLElement(p_parent)
{
    Length = 0;
    Owner = false;
    Type = EXBDT_UNKNOWN;
    DataArray = NULL;

    SetAttribute("bin_data","on");
    SetAttribute("length",0);
    SetAttribute("type","unknown");
}

//------------------------------------------------------------------------------

CXMLBinData::~CXMLBinData(void)
{
    if( (Owner == true) && (DataArray != NULL) ) delete[] DataArray;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

EXMLBinDataType CXMLBinData::GetDataType(void) const
{
    return(Type);
}

//------------------------------------------------------------------------------

long unsigned int CXMLBinData::GetLength(void) const
{
    return(Length);
}

//------------------------------------------------------------------------------

bool CXMLBinData::IsOwnerOfData(void) const
{
    return(Owner);
}

//------------------------------------------------------------------------------

void* CXMLBinData::GetData(void) const
{
    return(DataArray);
}

//------------------------------------------------------------------------------

unsigned char* CXMLBinData::GetDataAsCopy(void) const
{
    unsigned char* p_data = NULL;
    if( Length == 0 ) return(p_data);

    try {
        p_data = new unsigned char[Length];
    } catch(...) {
        return(NULL);
    }

    memcpy(p_data,DataArray,Length);

    return(p_data);
}

//------------------------------------------------------------------------------

unsigned char* CXMLBinData::ReleaseData(void)
{
    Owner = false;
    return(DataArray);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLBinData::SetLength(long unsigned int length,EXMLBinDataType type)
{
    FreeData();

    Length = length;
    DataArray = new unsigned char[Length];
    Type = type;
    Owner = true;
    UpdateHeader();
}

//------------------------------------------------------------------------------

void CXMLBinData::CopyData(void* p_data,long unsigned int length,
                            EXMLBinDataType type)
{
    SetLength(length,type);
    memcpy(DataArray,p_data,Length);
}

//------------------------------------------------------------------------------

void CXMLBinData::SetData(void* p_data,long unsigned int length,
                          bool own,
                          EXMLBinDataType type)
{
    FreeData();

    Length = length;
    Type = type;
    Owner = own;
    DataArray = static_cast<unsigned char*>(p_data);

    UpdateHeader();
}

//------------------------------------------------------------------------------

void CXMLBinData::FreeData(void)
{
    if( (Owner == true) && (DataArray != NULL) ) delete[] DataArray;
    Length = 0;
    Owner = false;
    Type = EXBDT_UNKNOWN;
    DataArray = NULL;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLBinData::UpdateHeader(void)
{
    SetAttribute("length",Length);
    switch(Type) {
        case EXBDT_CHAR:
            SetAttribute("type","char");
            break;
        case EXBDT_SHORT:
            SetAttribute("type","short");
            break;
        case EXBDT_INT:
            SetAttribute("type","int");
            break;
        case EXBDT_LONG:
            SetAttribute("type","long");
            break;
        case EXBDT_FLOAT:
            SetAttribute("type","float");
            break;
        case EXBDT_DOUBLE:
            SetAttribute("type","double");
            break;
        case EXBDT_LONG_DOUBLE:
            SetAttribute("type","long double");
            break;
        case EXBDT_UNKNOWN:
        default:
            SetAttribute("type","unknown");
            break;
    };
}

//------------------------------------------------------------------------------

void CXMLBinData::RestoreType(void)
{
    CSmallString stype;
    Type = EXBDT_UNKNOWN;
    GetAttribute("type",stype);

    if( stype == "char" ) Type = EXBDT_CHAR;
    if( stype == "short" ) Type = EXBDT_SHORT;
    if( stype == "int" ) Type = EXBDT_INT;
    if( stype == "long" ) Type = EXBDT_LONG;
    if( stype == "float" ) Type = EXBDT_FLOAT;
    if( stype == "double" ) Type = EXBDT_DOUBLE;
    if( stype == "long double" ) Type = EXBDT_LONG_DOUBLE;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLBinData::CopyData(CXMLNode* p_src)
{
    CXMLBinData* p_rsrc = dynamic_cast<CXMLBinData*>(p_src);
    if( p_rsrc == NULL ) {
        LOGIC_ERROR("incompatible source node");
    }

    // copy attribute
    CXMLElement::CopyData(p_src);

    // copy binary data
    Length = p_rsrc->Length;
    Type = p_rsrc->Type;
    Owner = p_rsrc->Owner;

    if( Owner == true ) {
        DataArray = new unsigned char[Length];
        memcpy(DataArray,p_rsrc->DataArray,Length);
    } else {
        DataArray = p_rsrc->DataArray;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


