#pragma once

#include "ofMain.h"
#include "FlyCapture2.h"
#include "ofxOpenCv.h"
#include "searchThread.h"
#include "recordThread.h"
#include "playThreadA.h"
#include "playThreadB.h"

#define VID_LENGTH 20

using namespace FlyCapture2;


class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);	

		void PrintError(Error error);
        void releaseCamera();


		void loadNextVideo();	
		void playNextVideo();
		void updateSearch();

		ofVideoPlayer 		movieA;
		ofVideoPlayer       movieB;

		bool				whichMovie;
		ofDirectory 		dir;
		int 				mIndex;
		string 				nextMovieFilename;
		int 				delay;
		string 				closestVidPath;
		long 				newVideoTimer;
		string 				s_closestVidOffset;
		string				lastVidPath;
		bool 				matchFound;
		int					vidDirIndex;
		bool				getIndex;
		long				noMatchDelay;
		bool				checkAgainForVideo;
		long 				videoLoadTimer;

        Error error;
        Camera cam;
        Image rawImage;
        Image convertedImage;
        ofVideoGrabber vidGrabber;
        ofxCvGrayscaleImage     grayImage;
        ofxCvColorImage         colorImage;
        FILE * ffmpeg;
        ofImage                 switchImage;


        bool 				firstPlay;
        bool				playLatch;
        long 				videoSwitchTimer;
        bool				showData;
        
        long				videoDoneTimer;

        searchThread 		search;

        recordThread		record;

        playThreadA			playA;
        playThreadB			playB;




};

