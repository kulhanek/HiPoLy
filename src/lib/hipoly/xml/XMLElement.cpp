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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <XMLElement.hpp>
#include <SimpleIterator.hpp>
#include <ErrorSystem.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLElement::CXMLElement(CXMLNode* p_parent)
    : CXMLNode(p_parent)
{
    FirstAttribute = NULL;
    LastAttribute = NULL;
    NextToTry = NULL;
}

//------------------------------------------------------------------------------

CXMLElement::~CXMLElement(void)
{
    RemoveAllAttributes();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLElement::SetName(const CSmallString& name)
{
    Name = name;
}

//------------------------------------------------------------------------------

const CSmallString&  CXMLElement::GetName(void) const
{
    return(Name);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CXMLElement::HasAttributes(void) const
{
    return( FirstAttribute != NULL );
}

//------------------------------------------------------------------------------

int CXMLElement::NumOfAttributes(void) const
{
    CXMLAttribute* p_attr = FirstAttribute;
    int            count = 0;
    // find attribute
    while( p_attr != NULL ) {
        count++;
        p_attr = p_attr->GetNextSiblingAttribute();
    }

    return(count);
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,char value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value = value;
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,int value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value.IntToStr(value);
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,unsigned int value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value.UIntToStr(value);
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,long int value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value.UIntToStr(value);
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,long unsigned int value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value.UIntToStr(value);
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,double value,const char* precision)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value.DoubleToStr(value,precision);
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const bool value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value.BoolToStr(value);
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const float value,const char* precision)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value.FloatToStr(value,precision);
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const CSmallString& value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value = value;
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const char* value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value = value;
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const std::string& value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value = value.c_str();
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const CSmallTime& value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);

    if( value.IsClear() == false ) {
        p_attr->Value.IntToStr(value.Time);
    } else {
        p_attr->Value = "-1";
    }
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const CSmallDate& value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);

    if( value.IsClear() == false ) {
        time_t t = mktime((tm*)&value.Date);
        p_attr->Value.IntToStr(t);
    } else {
        p_attr->Value = "-1";
    }
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const CSmallTimeAndDate& value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);

    if( value.IsClear() == false ) {
        time_t t = mktime((tm*)&value.TimeDate);
        p_attr->Value.IntToStr(t);
    } else {
        p_attr->Value = "-1";
    }
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const CUUID& value)
{
    return( SetAttribute(name,value.GetStringForm()) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#ifdef HAVE_QT5
void CXMLElement::SetAttribute(const CSmallString& name,const QString& value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value = value;
}

//------------------------------------------------------------------------------

void CXMLElement::SetAttribute(const CSmallString& name,const QByteArray& value)
{
    CXMLAttribute* p_attr = FindAttribute(name,true);
    p_attr->Value = NULL;

    // allocate string
    p_attr->Value.SetLength(value.size()*2);
    if( value.size() == 0 ) return; // array is empty, string is NULL

    for(int i=0; i < value.size(); i++) {
        unsigned char z = value[i];
        unsigned char a;
        a  = (z >> 4);
        if( a >= 10 ) a += 'A'-10;
        else a += '0';
        p_attr->Value[i*2] = a;
        a  = z & 0x0F;
        if( a >= 10 ) a += 'A'-10;
        else a += '0';
        p_attr->Value[i*2+1] = a;
    }
}
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CXMLElement::GetAttribute(const CSmallString& name,int& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    if( p_attr->Value == NULL ) return(false);
    value = p_attr->Value.ToInt();
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,char& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    if( p_attr->Value == NULL ) return(false);
    value = p_attr->Value[0];
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,unsigned int& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    if( p_attr->Value == NULL ) return(false);
    value = p_attr->Value.ToUInt();
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,bool& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    if( p_attr->Value.IsInt() == false ) {
        value = p_attr->Value.ToBool();
    } else {
        int i = p_attr->Value.ToInt();
        if( i > 0 ) {
            value = true;
        } else {
            value = false;
        }
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,float& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    if( p_attr->Value == NULL ) return(false);
    value = p_attr->Value.ToFloat();
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,double& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    if( p_attr->Value == NULL ) return(false);
    value = p_attr->Value.ToDouble();
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,CSmallString& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    value = p_attr->Value;
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,std::string& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    value = p_attr->Value;
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,CFileName& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    value = p_attr->Value;
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,CSmallTime& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    if( p_attr->Value == NULL ) return(false);
    if( p_attr->Value == "-1" ) {
        value.Clear();
        return(true);
    }
    value.Time = p_attr->Value.ToUInt();
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,CSmallDate& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    if( p_attr->Value == NULL ) return(false);
    if( p_attr->Value == "-1" ) {
        value.Clear();
        return(true);
    }
    time_t t = p_attr->Value.ToUInt();
    tm* mt = localtime(&t);
    memcpy(&value.Date,mt,sizeof(tm));
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,CSmallTimeAndDate& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    if( p_attr->Value == NULL ) return(false);
    if( p_attr->Value == "-1" ) {
        value.Clear();
        return(true);
    }
    time_t t = p_attr->Value.ToUInt();
    tm* mt = localtime(&t);
    memcpy(&value.TimeDate,mt,sizeof(tm));
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,CUUID& value)
{
    CSmallString suuid;
    if( GetAttribute(name,suuid) == false ) return(false);
    return( value.SetFromStringForm(suuid) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#ifdef HAVE_QT5
bool CXMLElement::GetAttribute(const CSmallString& name,QString& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);

    value = p_attr->Value;
    return(true);
}

//------------------------------------------------------------------------------

bool CXMLElement::GetAttribute(const CSmallString& name,QByteArray& value)
{
    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr == NULL ) return(false);
    if( p_attr->Value.GetLength() % 2 != 0 ) {
        ES_ERROR("incorrect array definition");
        return(false);
    }
    value.resize(p_attr->Value.GetLength()/2);
    if( p_attr->Value.GetLength()/2 == 0 ) return(true);

    for(unsigned int i=0; i < p_attr->Value.GetLength()/2; i++) {
        char a = p_attr->Value[i*2];
        char b = p_attr->Value[i*2+1];
        unsigned char result;
        a = toupper(a);
        b = toupper(b);
        if( isdigit(a) == 0 ) {
            result = (a-'A'+10)*16;
        } else {
            result = (a-'0')*16;
        }
        // decode b
        if( isdigit(b) == 0 ) {
            result += (b-'A'+10);
        } else {
            result += (b - '0');
        }
        value[i] = result;
    }

    return(true);
}
#endif

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool CXMLElement::RemoveAttribute(const CSmallString& name)
{
    CXMLAttribute* p_attr = FirstAttribute;
    CXMLAttribute* p_prev = NULL;

    // find attribute
    while( p_attr != NULL ) {
        if( p_attr->Name == name ) break;
        p_prev = p_attr;
        p_attr = p_attr->GetNextSiblingAttribute();
    }

    if( p_attr == NULL ) return(false);    // attribute was not found

    // remove it from list
    if( p_prev == NULL ) {
        FirstAttribute = p_attr->NextSiblingAttribute;
    } else {
        p_prev->NextSiblingAttribute = p_attr->NextSiblingAttribute;
    }
    if( LastAttribute == p_attr ){
        LastAttribute = p_prev;
    }
    if( NextToTry == p_attr ){
        NextToTry = NULL;
    }

    p_attr->ParentElement = NULL;           // break chain
    p_attr->NextSiblingAttribute = NULL;
    delete p_attr;

    return(true);
}

//------------------------------------------------------------------------------

void CXMLElement::RemoveAllAttributes(void)
{
    CXMLAttribute* p_attr = FirstAttribute;

    // delete attributes
    while( p_attr != NULL ) {
        p_attr->ParentElement = NULL;
        CXMLAttribute* p_delattr = p_attr;
        p_attr = p_attr->GetNextSiblingAttribute();
        delete p_delattr;
    }

    // all attributes are destroyed
    FirstAttribute = NULL;
    LastAttribute = NULL;
    NextToTry = NULL;
}

//------------------------------------------------------------------------------

void CXMLElement::RemoveAll(void)
{
    RemoveAllAttributes();
    RemoveAllChildNodes();
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLAttribute* CXMLElement::GetFirstAttribute(void) const
{
    return(FirstAttribute);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CXMLAttribute* CXMLElement::FindAttribute(const CSmallString& name,bool create)
{
    if( create && (FastSetAttribute == true) ){
        CXMLAttribute* p_attr = CreateAttribute();
        p_attr->Name = name;
        return(p_attr);
    }

    CXMLAttribute* p_attr = FindAttribute(name);
    if( p_attr ) return(p_attr);

    // should we create new attribute?
    if( create == false ) return(NULL);

    p_attr = CreateAttribute();
    p_attr->Name = name;

    return(p_attr);
}

//------------------------------------------------------------------------------

CXMLAttribute* CXMLElement::FindAttribute(const CSmallString& name)
{
    // speculative search
    if( NextToTry ){
        if( NextToTry->Name == name ){
            CXMLAttribute* p_attr = NextToTry;
            NextToTry = NextToTry->NextSiblingAttribute;
            return(p_attr);
        }
    }

    CXMLAttribute* p_attr = FirstAttribute;

    // find attribute
    while( p_attr != NULL ) {
        if( p_attr->Name == name ) return(p_attr);
        p_attr = p_attr->GetNextSiblingAttribute();
    }

    return(NULL);
}

//------------------------------------------------------------------------------

CXMLAttribute* CXMLElement::CreateAttribute(void)
{
    CXMLAttribute* p_attr = new CXMLAttribute(this);

    // add it to the list of attributes
    if( LastAttribute == NULL ) {
        FirstAttribute = p_attr;
    } else {
        LastAttribute->NextSiblingAttribute = p_attr;
    }
    LastAttribute = p_attr;

    return(p_attr);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CXMLElement::CopyData(CXMLNode* p_src)
{
    CXMLElement* p_rsrc = dynamic_cast<CXMLElement*>(p_src);
    if( p_rsrc == NULL ) {
        LOGIC_ERROR("incompatible source node");
    }

    Name = p_rsrc->Name;

    // copy attributes
    CXMLAttribute* p_att = p_rsrc->FirstAttribute;

    while( p_att != NULL ) {
        SetAttribute(p_att->Name,p_att->Value);
        p_att = p_att->NextSiblingAttribute;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


