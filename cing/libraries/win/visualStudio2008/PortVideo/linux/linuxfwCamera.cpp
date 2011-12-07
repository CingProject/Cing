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

#include "linuxfwCamera.h"

linuxfwCamera::linuxfwCamera(void)
{
	cameraID = -1;
	buffer = NULL;
	cameraName = new char[255];
}

linuxfwCamera::~linuxfwCamera(void)
{
	if (buffer!=NULL) delete []buffer;
	delete []cameraName;
}

bool linuxfwCamera::initCamera(int width, int height, bool colour) {
	
	if (cameraID < 0) return false;
	this->width = width;
	this->height = height;
	this->colour = colour;
	bytes = (colour?3:1);
	
	quadlet_t format;
	if (dc1394_query_supported_modes(handle, cameraID,FORMAT_VGA_NONCOMPRESSED,&format) !=DC1394_SUCCESS) {
		fprintf( stderr,"unable to query format\n");
	}
	
	image_mode_ = 0;
	if (colour) {	
		int yuv411 = 29;
		int rgb = 27;
		if	(format & (0x1<<yuv411)) image_mode_=MODE_640x480_YUV411;
		else if (format & (0x1<<rgb)) 	 image_mode_=MODE_640x480_RGB;
	} else {
		int mono = 26;
		int yuv411 = 29;
		if (format & (0x1<<mono))        image_mode_=MODE_640x480_MONO;
		else if	(format & (0x1<<yuv411)) image_mode_=MODE_640x480_YUV411;
	}
	if(!image_mode_) return false;
	quadlet_t frames;
	if (dc1394_query_supported_framerates(handle, cameraID,FORMAT_VGA_NONCOMPRESSED, image_mode_, &frames) !=DC1394_SUCCESS) {
		fprintf( stderr,"unable query frame rate\n");
	}
	
	fps = 60;
	while (fps>=15) {
		frame_rate_ = 0;
		int test_rate = 0;
		
		if (fps == 60)
		test_rate = FRAMERATE_60;
		else if (fps == 30)
		test_rate = FRAMERATE_30;
		else if (fps == 15)
		test_rate = FRAMERATE_15;    
		
		if ((frames & (0x1<<(31-(test_rate-FRAMERATE_MIN)))))
			frame_rate_=test_rate;
		
		if(!frame_rate_) {
			fps=fps/2;
		} else break;
	}
	if(!frame_rate_) return false;
	printf("framerate\n");  
	
	char videoDevice[24] = "";
	sprintf(videoDevice,"/dev/video1394/%d",dc1394_get_camera_port(handle));
	
	// setup capture
	if (dc1394_dma_setup_capture(handle,camera_nodes[cameraID],
				0, /* channel */ 
				FORMAT_VGA_NONCOMPRESSED,
				image_mode_,
				SPEED_400,
				frame_rate_,
				4,
				0,
				videoDevice,
				&camera)==DC1394_SUCCESS) {
		use_dma=true;
	} else if (dc1394_setup_capture(handle,camera_nodes[cameraID],
				0, /* channel */ 
				FORMAT_VGA_NONCOMPRESSED,
				image_mode_,
				SPEED_400,
				frame_rate_,
				&camera)==DC1394_SUCCESS) {
		use_dma=false;
	} else {
		fprintf( stderr,"unable to setup camera -\n"
			"make sure that the video mode,framerate and format are supported by your camera\n");
		dc1394_release_camera(handle,&camera);
		dc1394_destroy_handle(handle);
		dc1394_free_camera_nodes(camera_nodes);
		return false;
	}
	
	
	dc1394_camerainfo info;
	if (dc1394_get_camera_info(handle, cameraID, &info) !=DC1394_SUCCESS ) {
		//printf("could not read camera info\n");
		sprintf(cameraName,"unknown camera");
	} else {
		sprintf(cameraName,"%s, %s", info.vendor, info.model);
	}
	
	/*if (dc1394_set_video_framerate(handle,cameraID,frame_rate)!=DC1394_SUCCESS) {
		fprintf( stderr,"%i nope\n");
		dc1394_release_camera(handle,&camera);
		//dc1394_destroy_handle(handle);
		//sdc1394_free_camera_nodes(camera_nodes);
		return false;
	}*/
	
	dc1394_free_camera_nodes(camera_nodes);
	
	// set trigger mode
	/*if( dc1394_set_trigger_mode(handle, camera.node, TRIGGER_MODE_0)
	!= DC1394_SUCCESS)
	{
	//fprintf( stderr, "unable to set camera trigger mode\n");
	}*/
	
	buffer = new unsigned char[width*height*bytes];	
	return true;
}

bool linuxfwCamera::findCamera() {
	
	// get the number of ports
	raw1394handle_t phandle = raw1394_new_handle();
	
	if (phandle==NULL) {
		printf( "no firewire ports found\n");
		return false;
	}
	
	struct raw1394_portinfo pinf;
	int ports = raw1394_get_port_info(phandle, &pinf, 0);
	dc1394_destroy_handle(phandle);
	
	//printf("firewire ports: %d\n",ports);
	if (ports<1) {
		printf("no firewire ports found\n");
		return false;
	}
	// open ohci and asign handle to it
	for (int i=0;i<ports;i++)  {
		//printf("trying port %d\n",i);
		handle = dc1394_create_handle(i);
	
		// get the camera nodes and describe them as we find them
		numNodes = raw1394_get_nodecount(handle);
		camera_nodes = dc1394_get_camera_nodes(handle,&numCameras,0);
		//fflush(stdout);
	
		//printf("found %d cameras\n",numCameras);
		if (numCameras<1) {
			dc1394_destroy_handle(handle);
			continue;
		} else {
			cameraID = 0;
			return true;
		}
	}
	
	fprintf( stderr, "no firewire camera found\n");
	return false;
}

bool linuxfwCamera::startCamera() {
	
	if (dc1394_start_iso_transmission(handle,camera.node) !=DC1394_SUCCESS)  {
		fprintf( stderr, "unable to start camera iso transmission\n");
		dc1394_dma_release_camera(handle,&camera);
		dc1394_destroy_handle(handle);
		return false;
	}
	return true;
}

unsigned char* linuxfwCamera::getFrame()  {

	if (handle==NULL) return NULL;
	
	if (use_dma) {
		if (dc1394_dma_single_capture(&camera)!=DC1394_SUCCESS) 
		{
			fprintf( stderr, "unable to capture a frame\n");
			dc1394_dma_release_camera(handle,&camera);
			dc1394_destroy_handle(handle);
			return NULL;
		} dc1394_dma_done_with_buffer(&camera);
	} else {
		if (dc1394_single_capture(handle,&camera)!=DC1394_SUCCESS) 
		{
			fprintf( stderr, "unable to capture a frame\n");
			dc1394_dma_release_camera(handle,&camera);
			dc1394_destroy_handle(handle);
			return NULL;
		}
	}
	
	switch (colour) {
		case false: {
			if (image_mode_==MODE_640x480_YUV411) {
				unsigned char *src = (unsigned char*)camera.capture_buffer;
				unsigned char *dest = buffer;
				for(int y=0;y<height;y++) {
					for(int x=0;x<width/4;++x) {
						src++;
						*dest++ = *src++;
						*dest++ = *src++;
						src++;
						*dest++ = *src++;
						*dest++ = *src++;
					}
				}
			} else {
				if (buffer!=NULL)
					memcpy(buffer,(unsigned char*)camera.capture_buffer,width*height*bytes);
			}
			break;
		}
		case true: {
			if (image_mode_==MODE_640x480_YUV411) {
				int R,G,B;
				int Y[4];
				int U,V;
				
				unsigned char *src = (unsigned char*)camera.capture_buffer;
				unsigned char *dest = buffer;
				for(int y=0;y<height;y++) {
					for(int x=0;x<width/4;++x) {
						U    = *src++;
						Y[0] = *src++;
						Y[1] = *src++;
						V    = *src++;
						Y[2] = *src++;
						Y[3] = *src++;
	
						// U and V are +-0.5
						U -= 128;
						V -= 128;
						
						// conversion
						for (int i=0;i<4;i++) {
							R = (int)(Y[i] + 1.370705f * V); 			//R
							G = (int)(Y[i] - 0.698001f * V - 0.337633f * U);	//G
							B = (int)(Y[i] + 1.732446f * U); 			//B
							
							if (R < 0) R=0;
							if (G < 0) G=0;
							if (B < 0) B=0;
							if (R > 255) R=255;
							if (G > 255) G=255;
							if (B > 255) B=255;
		
							*dest++ = B;
							*dest++ = G;
							*dest++ = R;
						}
					}
				}			
			} else {
				if (buffer!=NULL)
					memcpy(buffer,(unsigned char*)camera.capture_buffer,width*height*bytes);
			}
			break;
		}
		
	}
	
	return buffer;
}

bool linuxfwCamera::stopCamera() {
	
	if (dc1394_stop_iso_transmission(handle,camera.node)!=DC1394_SUCCESS) 
	{
	//printf("couldn't stop the camera\n");
	return false;
	}
	
	//delete []buffer;
	//buffer = NULL;
	return true;
}

bool linuxfwCamera::stillRunning() {
	if (handle==NULL) return false;
	else return true;
}

bool linuxfwCamera::resetCamera() {
	return (stopCamera() && startCamera());
}


bool linuxfwCamera::closeCamera() {
	
	if (use_dma) {
		dc1394_dma_unlisten(handle, &camera);
		dc1394_dma_release_camera(handle, &camera);
	}
	else { 
		dc1394_release_camera(handle,&camera);
	}
	dc1394_camera_off(handle, cameraID);
	dc1394_destroy_handle(handle);
	return true;
}


void linuxfwCamera::showSettingsDialog() {
	printf("not yet implemented\n");
}



