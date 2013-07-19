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


#ifndef LINUXFWCAMERA_H
#define LINUXFWCAMERA_H

#include "../common/cameraEngine.h"
#include <string.h>
#include <libraw1394/raw1394.h>
#include <libdc1394/dc1394_control.h>
#include <math.h>

class linuxfwCamera : public cameraEngine
{
public:
	linuxfwCamera(void);
	~linuxfwCamera(void);

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
	dc1394_cameracapture camera;
	int numNodes;
	int numCameras;
	raw1394handle_t handle;
	nodeid_t * camera_nodes;
	//dc1394_feature_set *features;
	//dc1394_camerainfo *cameraInfo;
	bool use_dma;
	
	int image_mode_;
	int frame_rate_;
	
};

#endif
