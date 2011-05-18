/* ========================================================================
*  PROJECT: DirectShow Video Processing Library (DSVL)
*  Version: 0.0.8 (05/13/2005)
*  ========================================================================
*  Author:  Thomas Pintaric, Vienna University of Technology
*  Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
*  =======================================================================
* 
*  Copyright (C) 2005  Vienna University of Technology
* 
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*  For further information please contact Thomas Pintaric under
*  <pintaric@ims.tuwien.ac.at> or write to Thomas Pintaric,
*  Vienna University of Technology, Favoritenstr. 9-11/E188/2, A-1040
*  Vienna, Austria.
* ========================================================================*/

#ifndef __PIXEL_FORMAT_TYPES__
#define __PIXEL_FORMAT_TYPES__

// common pixel format enumeration
typedef enum _PIXELFORMAT
{
	PIXELFORMAT_UNKNOWN	 = 0, 
	PIXELFORMAT_UYVY	 = 1, 
	PIXELFORMAT_YUY2	 = 2,
	PIXELFORMAT_RGB565	 = 3,
	PIXELFORMAT_RGB555	 = 4,
	PIXELFORMAT_RGB24	 = 5,
	PIXELFORMAT_RGB32	 = 6,
	PIXELFORMAT_INVALID  = 7, 
	PIXELFORMAT_QUERY	 = 8,
	PIXELFORMAT_ENUM_MAX = 9
} PIXELFORMAT;

#endif