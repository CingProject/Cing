/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

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

#ifndef _Cing_Singleton_H_
#define _Cing_Singleton_H_

/**
 * @internal
 * @file Singleton related utilities
 */

#include "CommonPrereqs.h"
#ifndef NULL
	#define NULL 0
#endif

namespace Cing
{

/**
 * @internal
 * @brief Base class to create static singletons. This means that its creation/destruction cycle
 * is not controlled. If you need to control the creation/destruction of the singleton use
 * the Singleton class.
 */
template < class T >
class SingletonStatic
{
public:

  virtual ~SingletonStatic() { m_bIsValid = false; }

  /**
   * @brief Returns a reference to the singleton instance
   * @return a reference to the singleton instance
   */
  inline static T &getSingleton() { return m_singleton; }

  /**
   * @brief Returns a pointer to the singleton instance
   * @return a reference to the singleton instance
   */
	inline static T *getSingletonPtr() { return m_bIsValid? &m_singleton: NULL; }

private:
  static T		m_singleton; ///< Singleton
	static bool	m_bIsValid;
};

// Singleton definition
template < class T >
T SingletonStatic<T>::m_singleton;

template < class T >
bool SingletonStatic<T>::m_bIsValid = true;


/**
 * @internal
 * @brief Base class to create controlled singletons. In this class the creation and destruction of
 * the singleton are controlled. The creation occurs with the first getSingleton call, and the
 * destruction when the method destroySingleton is called
 */template < class T >
class Singleton
{
public:

    virtual ~Singleton() { }

  /**
   * @brief Returns a reference to the singleton instance
   * The first call will create the singleton
   * @return a reference to the singleton instance
   */
  inline static T &getSingleton() {
    if ( !m_singleton ){
      m_singleton = new T();
    }
    return *m_singleton;
  }

  /**
   * @brief Returns a pointer to the singleton instance
   * The first call will create the singleton
   * @return a pointer to the singleton instance
   */
  inline static T *getSingletonPtr() {
    if ( !m_singleton ){
      m_singleton = new T();
    }
    return m_singleton;
  }

  /// To destroy the singleton
  void static destroySingleton() { if(m_singleton) {delete m_singleton; m_singleton = 0; } }

private:
  static T *m_singleton; ///< Singleton
};

//Singleton
template < class T >
T *Singleton<T>::m_singleton = 0;

} // namespace Cing

#endif // _Singleton_H_
