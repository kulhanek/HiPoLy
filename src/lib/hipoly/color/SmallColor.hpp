#ifndef SmallColorH
#define SmallColorH
//==============================================================================
//    Copyright 2011 Petr Kulhanek
//    Copyright 1998,1999,2000,2001,2004,2006,2008 Petr Kulhanek, Jakub Stepan
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

#include <XMLElement.hpp>
#include <stdexcept>

//------------------------------------------------------------------------------

/// template class for RGBA color manipulation

template<class type>
class CSmallColor {
public:
    CSmallColor(void);
    CSmallColor(type r,type g,type b,type alpha=1.0);

// information methods ---------------------------------------------------------
    /// return red color component
    type R(void) const;

    /// return green color component
    type G(void) const;

    /// return blue color componenet
    type B(void) const;

    /// return alpha channel color component
    type Alpha(void) const;

    /// return RGB hex color representation
    const CSmallString GetRGBHex(void);

// setup methods ---------------------------------------------------------------
    /// set RGBA components
    void SetRGB(type r,type g,type b,type alpha=1.0);

    /// set red color component
    void SetR(type r);

    /// set green color component
    void SetG(type g);

    /// set blue color component
    void SetB(type b);

    /// set alpha channel color component
    void SetAlpha(type alpha);

// overloaded operators --------------------------------------------------------
    /// scale color by factor right
    CSmallColor<type>   operator * (type right);

    /// return raw data field
    operator const type* (void) const;

    /// return color spec for OpenGL
    type* ForGL(void);

    /// are colors identical
    bool operator == (const CSmallColor<type>& right) const;

    /// are colors not identical
    bool operator != (const CSmallColor<type>& right) const;

// input/output methods --------------------------------------------------------
    bool Load(CXMLElement* p_ele);
    bool Load(const CSmallString& name,CXMLElement* p_ele);
    void Save(CXMLElement* p_ele);
    void Save(const CSmallString& name,CXMLElement* p_ele);

// section of private data -----------------------------------------------------
private:
    type    C[4];               // data
};

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template class HIPOLY_PACKAGE CSmallColor<float>;
typedef CSmallColor<float> CColor;

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class type>
CSmallColor<type>::CSmallColor(void)
{
    C[0] = 0.0;
    C[1] = 0.0;
    C[2] = 0.0;
    C[3] = 1.0;
}

//---------------------------------------------------------------------------

template<class type>
CSmallColor<type>::CSmallColor(type r,type g,type b,type alpha)
{
    SetRGB(r,g,b,alpha);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class type>
type CSmallColor<type>::R(void) const
{
    return(C[0]);
}

//------------------------------------------------------------------------------

template<class type>
type CSmallColor<type>::G(void) const
{
    return(C[1]);
}

//------------------------------------------------------------------------------

template<class type>
type CSmallColor<type>::B(void) const
{
    return(C[2]);
}

//------------------------------------------------------------------------------

template<class type>
type CSmallColor<type>::Alpha(void) const
{
    return(C[3]);
}

//------------------------------------------------------------------------------

template<class type>
const CSmallString CSmallColor<type>::GetRGBHex(void)
{
    char text[7];
    unsigned int r = static_cast<unsigned int>(C[0]*255.0);
    unsigned int g = static_cast<unsigned int>(C[1]*255.0);
    unsigned int b = static_cast<unsigned int>(C[2]*255.0);
    sprintf(text,"%02X%02X%02X",r,g,b);
    text[6] = '\0';
    return(text);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class type>
void   CSmallColor<type>::SetR(type r)
{
    C[0] = r;
}

//------------------------------------------------------------------------------

template<class type>
void   CSmallColor<type>::SetG(type g)
{
    C[1] = g;
}

//------------------------------------------------------------------------------

template<class type>
void   CSmallColor<type>::SetB(type b)
{
    C[2] = b;
}

//------------------------------------------------------------------------------

template<class type>
void   CSmallColor<type>::SetAlpha(type alpha)
{
    C[3] = alpha;
}

//------------------------------------------------------------------------------

template<class type>
void CSmallColor<type>::SetRGB(type r,type g,type b,type alpha)
{
    C[0] = r;
    C[1] = g;
    C[2] = b;
    C[3] = alpha;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class type>
CSmallColor<type> CSmallColor<type>::operator * (type right)
{
    CSmallColor<type> color(C[0]*right,C[1]*right,C[2]*right,C[3]);
    return(color);
}

//------------------------------------------------------------------------------

template<class type>
CSmallColor<type>::operator const type * (void) const
{
    return(C);
}

//------------------------------------------------------------------------------

template<class type>
type* CSmallColor<type>::ForGL(void)
{
    return(C);
}

//------------------------------------------------------------------------------

template<class type>
bool CSmallColor<type>::operator == (const CSmallColor<type>& right) const
{
    if( C[0] != right.C[0] ) return(false);
    if( C[1] != right.C[1] ) return(false);
    if( C[2] != right.C[2] ) return(false);
    if( C[3] != right.C[3] ) return(false);
    return(true);
}

//------------------------------------------------------------------------------

template<class type>
bool CSmallColor<type>::operator != (const CSmallColor<type>& right) const
{
    if( C[0] != right.C[0] ) return(true);
    if( C[1] != right.C[1] ) return(true);
    if( C[2] != right.C[2] ) return(true);
    if( C[3] != right.C[3] ) return(true);
    return(false);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class type>
bool CSmallColor<type>::Load(CXMLElement* p_ele)
{
    if( p_ele == NULL ) throw std::invalid_argument("p_ele is NULL");

    bool result = true;
    result &= p_ele->GetAttribute("r",C[0]);
    result &= p_ele->GetAttribute("g",C[1]);
    result &= p_ele->GetAttribute("b",C[2]);
    result &= p_ele->GetAttribute("a",C[3]);

    return(result);
}

//------------------------------------------------------------------------------

template<class type>
void CSmallColor<type>::Save(CXMLElement* p_ele)
{
    if( p_ele == NULL ) throw std::invalid_argument("p_ele is NULL");

    p_ele->SetAttribute("r",C[0]);
    p_ele->SetAttribute("g",C[1]);
    p_ele->SetAttribute("b",C[2]);
    p_ele->SetAttribute("a",C[3]);
}

//------------------------------------------------------------------------------

template<class type>
bool CSmallColor<type>::Load(const CSmallString& name,CXMLElement* p_ele)
{
    if( p_ele == NULL ) throw std::invalid_argument("p_ele is NULL");

    CXMLElement* p_sele = p_ele->GetFirstChildElement(name);
    if( p_sele == NULL ) return(false);

    bool result = true;
    result &= p_sele->GetAttribute("r",C[0]);
    result &= p_sele->GetAttribute("g",C[1]);
    result &= p_sele->GetAttribute("b",C[2]);
    result &= p_sele->GetAttribute("a",C[3]);

    return(result);
}

//------------------------------------------------------------------------------

template<class type>
void CSmallColor<type>::Save(const CSmallString& name,CXMLElement* p_ele)
{
    if( p_ele == NULL ) throw std::invalid_argument("p_ele is NULL");

    CXMLElement* p_sele = p_ele->CreateChildElement(name);
    p_sele->SetAttribute("r",C[0]);
    p_sele->SetAttribute("g",C[1]);
    p_sele->SetAttribute("b",C[2]);
    p_sele->SetAttribute("a",C[3]);

}
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#endif

