#ifndef XMLElementH
#define XMLElementH
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

#include <HiPoLyMainHeader.hpp>
#include <XMLNode.hpp>
#include <XMLAttribute.hpp>
#include <SimpleList.hpp>
#include <SmallDate.hpp>
#include <SmallTime.hpp>
#include <SmallTimeAndDate.hpp>
#include <FileName.hpp>
#include <UUID.hpp>
#include <string>

// -----------------------------------------------------------------------------

/// \brief define XML element

class HIPOLY_PACKAGE CXMLElement : public CXMLNode {
public:
// constructor and destructor --------------------------------------------------
    CXMLElement(CXMLNode* p_parent);
    virtual ~CXMLElement(void);

// name of element -------------------------------------------------------------
    /// set name of element
    void SetName(const CSmallString&);

    /// get name of element
    const CSmallString& GetName(void) const;

// operations with attributes --------------------------------------------------

    /// has attributes
    bool HasAttributes(void) const;

    /// return number of attributes
    int  NumOfAttributes(void) const;

    void SetAttribute(const CSmallString& name,char value);
    void SetAttribute(const CSmallString& name,bool value);
    void SetAttribute(const CSmallString& name,int value);
    void SetAttribute(const CSmallString& name,unsigned int value);
    void SetAttribute(const CSmallString& name,long int value);
    void SetAttribute(const CSmallString& name,long unsigned int value);
    void SetAttribute(const CSmallString& name,float value,
                                   const char* precision="%9.7le");
    void SetAttribute(const CSmallString& name,double value,
                                   const char* precision="%17.15le");
    void SetAttribute(const CSmallString& name,const CSmallString& value);
    void SetAttribute(const CSmallString& name,const std::string& value);
    void SetAttribute(const CSmallString& name,const char* value);
    void SetAttribute(const CSmallString& name,const CSmallTime& value);
    void SetAttribute(const CSmallString& name,const CSmallDate& value);
    void SetAttribute(const CSmallString& name,const CSmallTimeAndDate& value);
    void SetAttribute(const CSmallString& name,const CUUID& value);
#ifdef HAVE_QT5
    void SetAttribute(const CSmallString& name,const QString& value);
    void SetAttribute(const CSmallString& name,const QByteArray& value);
#endif

    bool GetAttribute(const CSmallString& name,char& value);
    bool GetAttribute(const CSmallString& name,int& value);
    bool GetAttribute(const CSmallString& name,unsigned int& value);
    bool GetAttribute(const CSmallString& name,bool& value);
    bool GetAttribute(const CSmallString& name,float& value);
    bool GetAttribute(const CSmallString& name,double& value);
    bool GetAttribute(const CSmallString& name,CSmallString& value);
    bool GetAttribute(const CSmallString& name,std::string& value);
    bool GetAttribute(const CSmallString& name,CFileName& value);
    bool GetAttribute(const CSmallString& name,CSmallTime& value);
    bool GetAttribute(const CSmallString& name,CSmallDate& value);
    bool GetAttribute(const CSmallString& name,CSmallTimeAndDate& value);
    bool GetAttribute(const CSmallString& name,CUUID& value);
#ifdef HAVE_QT5
    bool GetAttribute(const CSmallString& name,QString& value);
    bool GetAttribute(const CSmallString& name,QByteArray& value);
#endif
    /// for those types that can be converted to int, typically enums
    template <typename T1>
    inline bool GetAttribute(const CSmallString& name,T1& value);

    /// remove specified attribute
    bool RemoveAttribute(const CSmallString& name);

    /// remove all attributes
    void RemoveAllAttributes(void);

    /// remove all attributes and child nodes
    void RemoveAll(void);

    /// get first attribute
    CXMLAttribute* GetFirstAttribute(void) const;

    /// find attribute with name, create it if it does not exist
    CXMLAttribute* FindAttribute(const CSmallString& name,bool create);

    /// find attribute with name - return NULL if attribute does not exist
    CXMLAttribute* FindAttribute(const CSmallString& name);

// section of private data -----------------------------------------------------
private:
    CSmallString    Name;               // name of element
    CXMLAttribute*  FirstAttribute;     // attributes
    CXMLAttribute*  LastAttribute;      // attributes
    CXMLAttribute*  NextToTry;          // for speculative FindAttribute

    /// create new attribute
    CXMLAttribute* CreateAttribute(void);

    friend class CXMLAttribute;
    friend class CXMLParser;

protected:
    /// copy data of node
    virtual void CopyData(CXMLNode* p_src);
};

// -----------------------------------------------------------------------------

template <typename T1>
inline bool CXMLElement::GetAttribute(const CSmallString& name,T1& value)
{
    int loc_value;
    if( GetAttribute(name,loc_value) == true ){
        value = static_cast<T1>(loc_value);
        return(true);
    }
    return(false);
}

// -----------------------------------------------------------------------------

#endif

