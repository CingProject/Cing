/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

  Copyright (c) 2008 XXX

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _Release_H_
#define _Release_H_

/**
 * @internal
 * File: Contains helpers for object releasing
 */

namespace Common
{

/**
 * @internal
 * Deletes a pointer and sets it to NULL
 */
template< class T > inline void Release( T*& ptr )
{
  delete ptr;
  ptr = 0;
}

/**
 * @internal
 * Deletes a pointer to an array and sets it to NULL
 */
template< class T > inline void ReleaseArray( T*& ptr )
{
  delete [] ptr;
  ptr = 0;
}


/**
 * @internal
 * Functor helper to delete pointers within for_each like stl functions
 */
template< class T >
struct ReleaseFunctor
{
  void operator()( T*& ptr )
  {
    delete p;
    ptr = 0;
  }
};

/**
 * @internal
 * Functor helper to delete array pointers within for_each like stl functions
 */
template< class T >
struct ReleaseArrayFunctor
{
  void operator()( T ptr )
  {
    delete [] ptr;
    ptr = 0;
  }
};


} // namespace Common

#endif // _Release_H_
