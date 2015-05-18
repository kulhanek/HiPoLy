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

#include <TemplNode.hpp>
#include <stdlib.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CTemplNode::CTemplNode(void)
{
    NextNode = NULL;
    PrevNode = NULL;
    Object = NULL;
    Own = false;
}

// ----------------------------------------------------------------------------

void CTemplNode::SetIndex(int index)
{
    if( Object != NULL ) ((CObject*)Object)->SetIndex(index);
}

// ----------------------------------------------------------------------------

int  CTemplNode::GetIndex(void) const
{
    if( Object != NULL ) return(((CObject*)Object)->GetIndex());
    return(-1);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================




