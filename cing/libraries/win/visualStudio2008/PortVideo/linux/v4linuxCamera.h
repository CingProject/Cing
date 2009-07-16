/*  portVideo, a cross platform camera framework
    Copyright (C) 2005 Martin Kaltenbrunner <mkalten@iua.upf.es>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef V4LINUXCAMERA_H
#define V4LINUXCAMERA_H

#include "../common/cameraEngine.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/videodev.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <linux/videodev.h>
#include "ccvt.h"

class v4linuxCamera : public cameraEngine
{
public:
	v4linuxCamera(void);
	~v4linuxCamera(void);

	bool findCamera();
	bool initCamera(int width, int height, bool colour);
	bool startCamera();
	unsigned char* getFrame();
	bool stopCamera();
	bool stillRunning();
	bool resetCamera();
	bool closeCamera();
	void showSettingsDialog();
	
private:
	struct video_capability v4l_caps;
	struct video_channel v4l_chan;
	struct video_mbuf v4l_buf;
	struct video_mmap v4l_mmap;
	struct video_window v4l_win;
	struct video_picture v4l_pict;
	unsigned char *camBuffer;
};

#endif
