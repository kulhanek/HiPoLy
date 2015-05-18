#ifndef TemplIteratorH
#define TemplIteratorH
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

#include <TemplList.hpp>
#include <TemplNode.hpp>
#include <BaseIterator.hpp>

//------------------------------------------------------------------------------

template<class MemberObject>
class CTemplIterator : public CBaseIterator<CTemplList<MemberObject>,
        MemberObject,CTemplNode> {
public:
    CTemplIterator(void);
    CTemplIterator(CTemplList<MemberObject> &l);
    CTemplIterator(CTemplList<MemberObject> *l);
};

//------------------------------------------------------------------------------

template<class MemberObject>
CTemplIterator<MemberObject>::CTemplIterator(void)
    : CBaseIterator<CTemplList<MemberObject>,MemberObject,CTemplNode>()
{

}

//------------------------------------------------------------------------------

template<class MemberObject>
CTemplIterator<MemberObject>::CTemplIterator(CTemplList<MemberObject> &l)
    : CBaseIterator<CTemplList<MemberObject>,MemberObject,CTemplNode>(l)
{

}

// ----------------------------------------------------------------------------

template<class MemberObject>
CTemplIterator<MemberObject>::CTemplIterator(CTemplList<MemberObject> *l)
    : CBaseIterator<CTemplList<MemberObject>,MemberObject,CTemplNode>(l)
{

}

//------------------------------------------------------------------------------

#endif
