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

#include "portVideoSDL.h"

// the thread function which contantly retrieves the latest frame
int getFrameFromCamera(void *obj) {

		portVideoSDL *engine = (portVideoSDL *)obj;
		
		unsigned char *cameraBuffer = NULL;
		unsigned char *cameraWriteBuffer = NULL;
		
		while(engine->running_) {
			if(!engine->pause_) {
				cameraBuffer = engine->camera_->getFrame();
				if (cameraBuffer!=NULL) {
				
					cameraWriteBuffer = engine->ringBuffer->getNextBufferToWrite();
					if (cameraWriteBuffer!=NULL) {
						memcpy(cameraWriteBuffer,cameraBuffer,engine->ringBuffer->size());
						engine->framenumber_++;
						engine->ringBuffer->writeFinished();
					}
					SDL_Delay(10);
				} /*else {
					if (!engine->camera_->stillRunning()) {
						printf("camera disconnected\n");
						engine->running_=false;
					} else SDL_Delay(1);
				}*/
			} else SDL_Delay(10);
		}
		return(0);
}

void portVideoSDL::saveBuffer(unsigned char* buffer,int size) {
	char fileName[32];
	sprintf(fileName,"frame%ld_%dx%d.raw",framenumber_, width_, height_);
	FILE*  imagefile=fopen(fileName, "w");
	fwrite((const char *)buffer, 1,  size, imagefile);
	fclose(imagefile);
}


// the principal program sequence
void portVideoSDL::run() {

	if( !setupCamera() )
		return;
	
	if( !setupWindow() ) {
		teardownCamera();
		return;
	}
	
	allocateBuffers();
	initFrameProcessors();

	bool success = camera_->startCamera();

	if( success ){

		SDL_EnableKeyRepeat(200, 10);

		printf( "display:  n - none\n" );
		printf( "          s - source\n" );
		printf( "          t - target\n" );		
		printf( "          f - fps\n" );
		printf( "settings: c - camera\n" );
		printf( "commands: b - save BMP frame\n" );
		printf( "          r - save RAW frame\n" );
		printf( "          p - pause\n" );
		printf( "          ESC - quit\n" );
		printf( "\n" );

		for (frame = processorList.begin(); frame!=processorList.end(); frame++)
			(*frame)->printOptions();
	
		cameraThread = SDL_CreateThread(getFrameFromCamera,this);
		
		mainLoop();
		
		SDL_KillThread(cameraThread);
		camera_->stopCamera();

	}else{
		printf("could not start camera\n");
	}	

	teardownCamera();
	teardownWindow();
	
	freeBuffers();
	
}

// does what its name suggests
void portVideoSDL::mainLoop()
{
	unsigned char *cameraReadBuffer = NULL;

	while(running_) {
		
		process_events();

		// do nothing if paused
		if (pause_){
			SDL_Delay(5);
			continue;
		}

		// loop until we get access to a frame
		cameraReadBuffer = ringBuffer->getNextBufferToRead();
		while (cameraReadBuffer==NULL) {
			SDL_Delay(1);
			cameraReadBuffer = ringBuffer->getNextBufferToRead();
			if(!running_) goto emergencyexit; // escape on quit
		}
		
		// try again if we can get a more recent fram
		do {
			memcpy(sourceBuffer_,cameraReadBuffer,ringBuffer->size());
			ringBuffer->readFinished();
			
			cameraReadBuffer = ringBuffer->getNextBufferToRead();
		} while( cameraReadBuffer != NULL );
		
		// do the actual image processing job
		for (frame = processorList.begin(); frame!=processorList.end(); frame++)
			(*frame)->process(sourceBuffer_,destBuffer_);
		
		// update display
		switch( displayMode_ ) {
			case NO_DISPLAY:
				break;
			case SOURCE_DISPLAY: {
				SDL_BlitSurface(sourceImage_, NULL, window_, NULL);
				SDL_Flip(window_);
				//SDL_UpdateRect( window_, 0, 0, width_, height_ );
				break;
			}			
			case DEST_DISPLAY: {
				SDL_BlitSurface(destImage_, NULL, window_, NULL);
				SDL_Flip(window_);
				//SDL_UpdateRect( window_, 0, 0, width_, height_ );
				break;
			}
		}
		
		if (fpson_) fpsCount();
	}
	emergencyexit: 
	
	// do the actual image processing job
	for (frame = processorList.begin(); frame!=processorList.end(); frame++)
		(*frame)->finish();

	return;
}

void portVideoSDL::fpsCount() {

	frames_++;

	time_t currentTime;
	time(&currentTime);
	long diffTime = (long)( currentTime - lastTime_ );
	
	if (diffTime >= 2) {
		int fps = (int)( frames_ / diffTime );
		static char caption[24] = "";
		sprintf(caption,"%s - %d FPS",appName,fps);
		SDL_WM_SetCaption( caption, NULL );
		frames_ = 0;
		
		lastTime_ = (long)currentTime;
    }
}

bool portVideoSDL::setupWindow() {
	// initialize SDL
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		printf("SDL could not be initialized: %s\n", SDL_GetError());
		//SDL_Quit();
		return false;
	}

	#ifndef __APPLE__
	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
	#endif
	
	window_ = SDL_SetVideoMode(width_, height_, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if ( window_ == NULL ) {
		printf("Could not open window: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	static char caption[24] = "";
	sprintf(caption,"%s - ?? FPS",appName);
	SDL_WM_SetCaption(caption, NULL);
	return true;
}

void portVideoSDL::teardownWindow()
{
	SDL_Quit();
}

void portVideoSDL::process_events()
{
    SDL_Event event;

    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
			case SDL_KEYDOWN:
				//printf("%d\n",event.key.keysym.sym);
				if( event.key.keysym.sym == SDLK_n ){
					displayMode_ = NO_DISPLAY;
					// turn the display black
					for (int i=0;i<width_*height_*bytesPerDestPixel_;i++)
						destBuffer_[i]=0;
					SDL_BlitSurface(destImage_, NULL, window_, NULL);
					SDL_UpdateRect( window_, 0, 0, width_, height_ );
				} else if( event.key.keysym.sym == SDLK_s ){
					displayMode_ = SOURCE_DISPLAY;
				} else if( event.key.keysym.sym == SDLK_t ){
					displayMode_ = DEST_DISPLAY;
				} else if( event.key.keysym.sym == SDLK_c ){
					camera_->showSettingsDialog(); 
				} else if( event.key.keysym.sym == SDLK_f ){
					if (fpson_) {
						fpson_=false;
						SDL_WM_SetCaption(appName, NULL );
					}
					else {
						// reset the fps counter
						frames_ = 0;
						time_t currentTime;
						time(&currentTime);
						lastTime_ = (long)currentTime;
						fpson_=true;
						static char caption[24] = "";
						sprintf(caption,"%s - ?? FPS",appName);
						SDL_WM_SetCaption(caption, NULL);
					} 
				} else if( event.key.keysym.sym == SDLK_p ){
					if (pause_) {
						pause_=false;
						// reset the fps counter
						frames_ = 0;
						time_t currentTime;
						time(&currentTime);
						lastTime_ = (long)currentTime;
						static char caption[24] = "";
						if (fpson_) sprintf(caption,"%s - ?? FPS",appName);
						else sprintf(caption,"%s",appName);
						SDL_WM_SetCaption(caption, NULL);
						
					} else {
						pause_=true;
						static char caption[24] = "";
						sprintf(caption,"%s - paused",appName);
						SDL_WM_SetCaption(caption, NULL);
						// turn the display black
						for (int i=0;i<width_*height_*bytesPerDestPixel_;i++)
							destBuffer_[i]=0;
						SDL_BlitSurface(destImage_, NULL, window_, NULL);
						SDL_UpdateRect( window_, 0, 0, width_, height_ );
					}
				} else if( event.key.keysym.sym == SDLK_b ){
					char fileName[32];
					sprintf(fileName,"frame%li.bmp",framenumber_);
					SDL_SaveBMP(sourceImage_, fileName);
				} else if( event.key.keysym.sym == SDLK_r ){
					saveBuffer(sourceBuffer_, width_ * height_ * bytesPerSourcePixel_);
				} else if( event.key.keysym.sym == SDLK_ESCAPE ){
					running_=false;
				} else {
					for (frame = processorList.begin(); frame!=processorList.end(); frame++)
						(*frame)->toggleFlag(event.key.keysym.sym);
				}
				break;
			case SDL_QUIT:
				running_ = false;
				break;
        }
    }
}

bool portVideoSDL::setupCamera() {

	camera_ = cameraTool::findCamera();	
	if (camera_ == NULL) return false;
	
	bool colour = false;
	if (sourceDepth_==24) colour = true;
	bool success = camera_->initCamera(width_, height_, colour);
	
	if(success) {
		width_ = camera_->getWidth();
		height_ = camera_->getHeight();
		fps_ = camera_->getFps();
					
		printf("camera: %s\n",camera_->getName());
		printf("format: %dx%d, %dfps\n\n",width_,height_,fps_);
		return true;
	} else {
		printf("could not initialize camera\n");
		camera_->closeCamera();
		delete camera_;
		return false;
	}
}

void portVideoSDL::teardownCamera()
{
	camera_->closeCamera();
	delete camera_;
}

void portVideoSDL::allocateBuffers()
{
	bytesPerSourcePixel_ = sourceDepth_/8;	
	bytesPerDestPixel_ = destDepth_/8;
	sourceBuffer_ = new unsigned char[width_*height_*bytesPerSourcePixel_];
	destBuffer_ = new unsigned char[width_*height_*bytesPerDestPixel_];
	cameraBuffer_ = NULL;
		
	sourceImage_ = SDL_CreateRGBSurfaceFrom(sourceBuffer_, width_, height_, sourceDepth_, width_*bytesPerSourcePixel_, 0 , 0, 0, 0);
	if (sourceDepth_==8)
		SDL_SetPalette(sourceImage_, SDL_LOGPAL|SDL_PHYSPAL, palette_, 0, 256 );

	destImage_ = SDL_CreateRGBSurfaceFrom(destBuffer_, width_, height_, destDepth_, width_*bytesPerDestPixel_, 0 , 0, 0, 0);
	if (destDepth_==8)
		SDL_SetPalette(destImage_, SDL_LOGPAL|SDL_PHYSPAL, palette_, 0, 256 );
		
	SDL_DisplayFormat(sourceImage_);
	SDL_DisplayFormat(destImage_);
	
	ringBuffer = new RingBuffer(width_*height_*bytesPerSourcePixel_);
}

void portVideoSDL::freeBuffers()
{
	SDL_FreeSurface(sourceImage_);
	SDL_FreeSurface(destImage_);
	delete [] sourceBuffer_;
	delete [] destBuffer_;
	
	delete ringBuffer;
}

void portVideoSDL::addFrameProcessor(FrameProcessor *fp) {

	processorList.push_back(fp);
}


void portVideoSDL::removeFrameProcessor(FrameProcessor *fp) {
	frame = std::find( processorList.begin(), processorList.end(), fp );
	if( frame != processorList.end() )
		processorList.erase( frame );

}

void portVideoSDL::setDisplayMode(DisplayMode mode) {
	displayMode_ = mode;
}

void portVideoSDL::initFrameProcessors() {
	for (frame = processorList.begin(); frame!=processorList.end(); ) {
		bool success = (*frame)->init(width_ , height_, bytesPerSourcePixel_, bytesPerDestPixel_);
		if(!success) {	
			processorList.erase( frame );
			printf("removed frame processor\n");
		} else frame++;
	}
}

portVideoSDL::portVideoSDL(char* name, bool srcColour, bool destColour)
	: frames_( 0 )
	, width_( WIDTH )
	, height_( HEIGHT )
	, displayMode_( SOURCE_DISPLAY )
{
	time(&lastTime_);
	running_ = true;
	fpson_ = true;
	pause_ = false;
	
	framenumber_=0;
	
	appName = name;
	sourceDepth_ = (srcColour?24:8);
	destDepth_   = (destColour?24:8);
		
	for(int i=0;i<256;i++){
		palette_[i].r=i;
		palette_[i].g=i;
		palette_[i].b=i;
	}
}

portVideoSDL::portVideoSDL()
	: frames_( 0 )
	, width_( WIDTH )
	, height_( HEIGHT )
	, displayMode_( SOURCE_DISPLAY )
{
	time(&lastTime_);
	running_ = true;
	fpson_ = true;
	pause_ = false;
	
	framenumber_=0;
	
	appName = "PortVideo";
	sourceDepth_ = 8;
	destDepth_   = 8;
		
	for(int i=0;i<256;i++){
		palette_[i].r=i;
		palette_[i].g=i;
		palette_[i].b=i;
	}
}


