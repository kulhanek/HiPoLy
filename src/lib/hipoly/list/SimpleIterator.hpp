#ifndef SimpleIteratorH
#define SimpleIteratorH
//==============================================================================
//    Copyright 1998,1999,2000,2001,2004,2005 Petr Kulhanek
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
#include <SimpleList.hpp>
#include <BaseIterator.hpp>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class MemberObject>
class CSimpleIterator : public CBaseIterator< CSimpleList<MemberObject>, MemberObject, CSimpleNode> {
public:
    CSimpleIterator(void);
    CSimpleIterator(CSimpleList<MemberObject> &l);
    CSimpleIterator(CSimpleList<MemberObject> *l);
};

//------------------------------------------------------------------------------

//this is for iteration in constant methods

template<class MemberObject>
class CSimpleIteratorC : public CBaseIterator<const CSimpleList<MemberObject>,MemberObject,CSimpleNode> {
public:
    CSimpleIteratorC(void);
    CSimpleIteratorC(const CSimpleList<MemberObject> &l);
    CSimpleIteratorC(const CSimpleList<MemberObject> *l);
};

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class MemberObject>
CSimpleIterator<MemberObject>::CSimpleIterator(void)
    : CBaseIterator<CSimpleList<MemberObject>,MemberObject,CSimpleNode>()
{

}

//------------------------------------------------------------------------------

template<class MemberObject>
CSimpleIterator<MemberObject>::CSimpleIterator(CSimpleList<MemberObject> &l)
    : CBaseIterator<CSimpleList<MemberObject>,MemberObject,CSimpleNode>(l)
{

}

// ----------------------------------------------------------------------------

template<class MemberObject>
CSimpleIterator<MemberObject>::CSimpleIterator(CSimpleList<MemberObject> *l)
    : CBaseIterator<CSimpleList<MemberObject>,MemberObject,CSimpleNode>(l)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

template<class MemberObject>
CSimpleIteratorC<MemberObject>::CSimpleIteratorC(void)
    : CBaseIterator<const CSimpleList<MemberObject>,MemberObject,CSimpleNode>()
{

}

// ----------------------------------------------------------------------------

template<class MemberObject>
CSimpleIteratorC<MemberObject>::CSimpleIteratorC(const CSimpleList<MemberObject> &l)
    : CBaseIterator<const CSimpleList<MemberObject>,MemberObject,CSimpleNode>(l)
{

}

// ----------------------------------------------------------------------------

template<class MemberObject>
CSimpleIteratorC<MemberObject>::CSimpleIteratorC(const CSimpleList<MemberObject> *l)
    : CBaseIterator<const CSimpleList<MemberObject>,MemberObject,CSimpleNode>(l)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

#endif
