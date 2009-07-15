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

#include "dslibCamera.h"

dslibCamera::dslibCamera()
{

	//xml_config = "<?xml version='1.0' encoding='UTF-8'?> <dsvl_input> <camera show_format_dialog='false'><pixel_format><RGB24/></pixel_format></camera></dsvl_input>";

	CoInitialize(NULL);
	dsvl_vs = new DSVL_VideoSource();

	//buffer = NULL;
	pbuffer = NULL;

	//cameraName = new char[255];
}


dslibCamera::~dslibCamera()
{
	//if (buffer!=NULL) delete []buffer;
	if (pbuffer!=NULL) delete []pbuffer;
	//delete []cameraName;
}

bool dslibCamera::findCamera() {
	return true;
}

bool dslibCamera::initCamera(int width, int height, int fps, bool colour) {

	this->width = width;
	this->height = height;
	this->fps = fps;
	this->colour = colour;
	bytes = (colour?3:1);
	//buffer = new unsigned char[width*height*bytes];

	LONG w,h;
	double f;

	
	// Build the xml config file
	xmlConfig << "<?xml version='1.0' encoding='UTF-8'?> <dsvl_input> <camera show_format_dialog = \"true\" frame_width=\"320\" frame_height=\"240\" frame_rate=\"15.0\">";
	//xmlConfig << "frame_width = \""		<< width	<< "\" ";
	//xmlConfig << "frame_height = \""	<< height << "\" ";
	//xmlConfig << "frame_rate = \""		<< fps		<< "\" ";
	//xmlConfig << ">";
	xmlConfig << "<pixel_format><RGB24/></pixel_format>";
	xmlConfig << "</camera></dsvl_input>";

	char* test = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \
								<dsvl_input>\
								<camera show_format_dialog=\"false\" frame_width=\"320\" frame_height=\"240\" frame_rate=\"30.0\">\
								<pixel_format>\
								<RGB24/>\
								</pixel_format>\
								</camera>\
								</dsvl_input>\"";


	std::ostringstream	test2;
											test2 << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
											test2 << "<dsvl_input>";
											test2 << "<camera show_format_dialog=\"false\" frame_width=\"" << width << "\" frame_height=\"" << height << "\" frame_rate=\"" << fps << "\">";
											test2 << "<pixel_format>";
											test2 << "<RGB24/>";
											test2 << "</pixel_format>";
											test2 << "</camera>";
											test2 << "</dsvl_input>";
								
	bool equal = test2.str() == test;
	//if(FAILED(dsvl_vs->BuildGraphFromXMLString( (char*)xmlConfig.str().c_str() ))) return false;
	if(FAILED(dsvl_vs->BuildGraphFromXMLString( test ))) return false;
	//if(FAILED(dsvl_vs->BuildGraphFromXMLFile( "DSVL_config.xml" ))) return false;
	if(FAILED(dsvl_vs->GetCurrentMediaFormat(&w,&h,&f,NULL))) return false;

	this->width = w;
	this->height = h;
	this->fps = (int)f;

	cameraName = "WDM Camera";

	pbuffer = new unsigned char[this->width*this->height*bytes];
	return true;
}


bool dslibCamera::startCamera()
{ 
	if(FAILED(dsvl_vs->EnableMemoryBuffer())) return(false);
	if(FAILED(dsvl_vs->Run())) return(false);

	return true;
}

unsigned char* dslibCamera::getFrame()
{
		
		DWORD wait_result = dsvl_vs->WaitForNextSample(/*INFINITE*/1000/fps);
		if(wait_result == WAIT_OBJECT_0)
		{

			unsigned char r,g,b;
			int dwidth = 2*width;

			dsvl_vs->CheckoutMemoryBuffer(&g_mbHandle, &buffer);
			g_Timestamp = dsvl_vs->GetCurrentTimestamp();
			switch (colour) {
				case false: {
					unsigned char *src = (unsigned char*)buffer;
					unsigned char *dest = pbuffer;
					dest+=width*height-width;

					for(int y=0;y<height;y++) {
						for(int x=0;x<width;x++) {
							//quicker
							/**dest++ = *src++;
							src++;
							src++;*/

							//apparently incorrect
							/*r = *src++;
							g = *src++;
							b = *src++;
							*dest++ = (r+b+g)/3;*/

							//better
							/*r = *src++;
							g = *src++;
							b = *src++;
							*dest++ = (unsigned char)(0.30 * r + 0.59 * g + 0.11 * b);*/

							//fastest and best
							r = *src++;
							g = *src++;
							b = *src++;
							*dest++ = hibyte(r * 77 + g * 151 + b * 28);
						}
						dest-=dwidth;
					}
					break;
				}
				case true: {
					memcpy(pbuffer,buffer,width*height*bytes);
					break;
				}
			}
			dsvl_vs->CheckinMemoryBuffer(g_mbHandle);
			return pbuffer;
		}

		return NULL;
}

bool dslibCamera::stopCamera()
{
	dsvl_vs->Stop();
	return true;
}
bool dslibCamera::stillRunning() {
	return true;
}

bool dslibCamera::resetCamera()
{
  return (stopCamera() && startCamera());

}

bool dslibCamera::closeCamera()
{
	if(FAILED(dsvl_vs->DisableMemoryBuffer())) return(false);
	delete dsvl_vs;
	return true;
}

void dslibCamera::showSettingsDialog() {

	dsvl_vs->ShowFilterProperties();
}
