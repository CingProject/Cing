/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef _Cing_Singleton_H_
#define _Cing_Singleton_H_

/**
 * @internal
 * @file Singleton related utilities
 */

// Precompiled headers
#include "Cing-Precompiled.h"


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
