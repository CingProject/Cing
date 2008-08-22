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

#include "v4linuxCamera.h"

v4linuxCamera::v4linuxCamera(void)
{
	cameraID = -1;

	buffer = NULL;
	cameraName = new char[255];
}

v4linuxCamera::~v4linuxCamera(void)
{
	if (buffer!=NULL) delete []buffer;
	delete []cameraName;
}

bool v4linuxCamera::initCamera(int width, int height, bool colour) {

	if (cameraID < 0) return false;

	this->width = width;
	this->height = height;
	this->colour = colour;
	bytes = (colour?3:1);

	v4l_mmap.height = height;
	v4l_mmap.width  = width;
	v4l_mmap.format = VIDEO_PALETTE_YUV420P;
	v4l_mmap.frame = 0;

	if ((v4l_caps.type & VID_TYPE_CAPTURE) == 0) {
		fprintf(stderr,"This device does not support memory capture.\n");
		return false;
	}

	if (ioctl(cameraID, VIDIOCGWIN, &v4l_win) < 0) {
		printf("Unable to query window settings: %s\n",
		       strerror(errno));
		return false;
	}

	if( ioctl(cameraID, VIDIOCGMBUF, &v4l_buf) < 0 ) {
		printf("Error: cannot get capture buffer properties: %sºn" ,
		       strerror(errno));
		return false;
	}

	v4l_win.width  = width;
	v4l_win.height = height;
	v4l_win.clipcount = 0;

	fps = 15;
	v4l_win.flags  = v4l_win.flags |= ( fps << 16 );
	if (ioctl(cameraID, VIDIOCSWIN, &v4l_win) < 0) {
			return false;
	}

	/*while (fps>=15) {
		v4l_win.flags  = ( fps << 16 );
		if (ioctl(cameraID, VIDIOCSWIN, &v4l_win) < 0) {
			//printf("Unable to commit window settings: %s\n",
			//       strerror(errno));
			fps=fps/2;
			if (fps<15) return false;
		}
	}*/

	if (ioctl(cameraID, VIDIOCGWIN, &v4l_win) < 0) {
		printf("Unable to query window settings: %s\n",
		       strerror(errno));
		return false;
	}


	if( ioctl(cameraID, VIDIOCGPICT, &v4l_pict) < 0 ) {
		printf("Error: cannot get picture properties: %sºn" ,
		       strerror(errno));
		return false;
	}

	v4l_pict.brightness = 32768;
	v4l_pict.contrast = 32768;
	v4l_pict.hue = 32768;
	v4l_pict.colour = 32768;
	v4l_pict.palette = VIDEO_PALETTE_YUV420P;

	buffer = new unsigned char[width*height*bytes];
	return true;
}

bool v4linuxCamera::findCamera() {

	// video4linux device name:
	const char * v4l_device;
	v4l_device = getenv("V4L_DEVICE");
	if (!v4l_device) {
		v4l_device="/dev/video0";
		/*fprintf(stderr,
		"You do not have a PORTVIDEO_DEVICE environment variable set.\n"
		"trying to use /dev/video0\n");*/
	}
	int fd = open(v4l_device, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "unable to open %s: %s\n",
		v4l_device, strerror(errno));
		//printf("no v4linux camera found at: %s\n", v4l_device);
		return false;
	}

	if (ioctl(fd, VIDIOCGCAP, &v4l_caps) < 0) {
		//fprintf(stderr,"Unable to query device capabilities: %s\n", strerror(errno));
		sprintf(cameraName, "unknown camera");

	} else sprintf(cameraName, "%s", v4l_caps.name);

	cameraID = fd;
	return true;
}

bool v4linuxCamera::startCamera() {
	camBuffer = (unsigned char *) mmap(0, v4l_buf.size, PROT_READ|PROT_WRITE, MAP_SHARED, cameraID, 0);
	return true;
}

unsigned char* v4linuxCamera::getFrame()  {

	int frame = 0;
	v4l_mmap.frame = frame;

	ioctl(cameraID, VIDIOCSYNC, &frame);
	if(ioctl(cameraID, VIDIOCMCAPTURE, &v4l_mmap)<0) {
		fprintf(stderr, "error in VIDIOCMCAPTURE\n");
		return NULL;
	}

	camBuffer = camBuffer + v4l_buf.offsets[frame];

	if (camBuffer==NULL) return NULL;

	switch (colour) {
		case false:
			memcpy(buffer,camBuffer,width*height);
			break;
		case true:
//			ccvt_420p_bgr24(width, height, camBuffer, buffer);
			break;
	}

	return buffer;
}

bool v4linuxCamera::stopCamera() {
  //delete []buffer;
  //buffer = NULL;
  return true;
}

bool v4linuxCamera::stillRunning() {
 	return true;
}

bool v4linuxCamera::resetCamera()
{
  return (stopCamera() && startCamera());
}


bool v4linuxCamera::closeCamera() {

	if (cameraID >= 0) {
		close(cameraID);
		cameraID = -1;
	}


   return true;
}

void v4linuxCamera::showSettingsDialog() {
	printf("not yet implemented\n");
}


