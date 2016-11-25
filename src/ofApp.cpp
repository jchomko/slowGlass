#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	


    ofBackground(0);
	ofSetVerticalSync(true);
    //ofLogToFile("myLogFile.txt", true);
    ofSetFrameRate(120);
	//Video Recording setup
	ofToggleFullscreen();
 //    FC2Version fc2Version;
 //    ostringstream version;
	// version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
 //    cout << version.str() << endl;

 //    ostringstream timeStamp;
 //    timeStamp <<"Application build date: " << __DATE__ << " " << __TIME__;
 //    cout << timeStamp.str() << endl << endl;

 //    BusManager busMgr;
 //    unsigned int numCameras;
 //    error = busMgr.GetNumOfCameras(&numCameras);
 //    if (error != PGRERROR_OK)
 //    {
 //        PrintError( error );

 //    }

 //    PGRGuid guid;
 //    cout << "Number of cameras detected: " << numCameras << endl;

 //    for (unsigned int i=0; i < numCameras; i++)
 //    {

 //        error = busMgr.GetCameraFromIndex(i, &guid);
 //        if (error != PGRERROR_OK)
 //        {
 //            PrintError( error );

 //        }

 //       // RunSingleCamera( guid );
 //    }
	//  error = cam.Connect(&guid);
 //    if (error != PGRERROR_OK)
 //    {
 //        PrintError( error );

 //    }

 //    CameraInfo camInfo;
 //    error = cam.GetCameraInfo(&camInfo);
 //    if (error != PGRERROR_OK)
 //    {
 //        PrintError( error );
 //    }

 //    Property camProp;
 //    PropertyInfo camPropInfo;

 //    camProp.type = FRAME_RATE;
 //    error = cam.GetProperty( &camProp );
 //    if (error != PGRERROR_OK){
 //        PrintError( error );
 //    }
 //    cout <<  "Frame rate is : " <<  camProp.absValue << endl;

	// error = cam.StartCapture();
 //    if (error != PGRERROR_OK)
 //    {
 //        PrintError( error );
 //    }

 //    error = cam.StopCapture();
 //    error = cam.StartCapture();

 //    grayImage.allocate(808,608);
 //    colorImage.allocate(808,608);

 //    switchImage.allocate(808,608,OF_IMAGE_COLOR_ALPHA);

 //    const char* cmd = "ffmpeg -re -f rawvideo -pix_fmt rgba  -s 808x608 -i - "" -threads 0 -preset fast -y  -map 0 -r 30 -f segment -segment_time 20 -reference_stream auto -segment_format mp4  -pix_fmt yuv420p -vcodec h264 -strftime 1 recordings/%s.mp4";        //flv for successfull m3u8 streams!                                                                                
 //    ffmpeg = popen(cmd, "w");
    
    //pclose(ffmpeg);
    //playback setup
    
    delay = 60;
    closestVidPath = "";
    //dir.open("/home/nuc/of_v0.8.3_linux64_release/apps/slowglass/slowGlassPlayer/bin/recordings"); //recordings;
    //matchFound =false;
    //dir.listDir();

    lastVidPath = "";


    whichMovie = true;

    //load both videos 
    // loadNextVideo();
    // playNextVideo();

    // loadNextVideo();
    // playNextVideo();
    
    getIndex = true;

    videoLoadTimer = 0; 

    firstPlay = true;
    
    playLatch = false;
    videoSwitchTimer = 0;
    // movieA.stop();
    // movieB.stop();
    
    movieA.loadAsync("../recordings/test.flv");
    movieA.play();
    movieA.setPaused(TRUE);


    movieB.loadAsync("../recordings/test.flv");
    movieB.play();
    movieB.setPaused(TRUE);

   
    
   
    checkAgainForVideo = false;
    showData = true;
    search.start();
    record.start();


     // playA.start();
    
     // playB.start();



}

void ofApp::releaseCamera()
{
    // error = cam.StopCapture();
    // if (error != PGRERROR_OK)
    // {
    //     PrintError( error );

    // }

    // // Disconnect the camera
    // error = cam.Disconnect();
    // if (error != PGRERROR_OK)
    // {
    //     PrintError( error );

    // }

}


//--------------------------------------------------------------
void ofApp::update(){
    
    //Video recording
	// if( checkAgainForVideo && ofGetElapsedTimeMillis() > noMatchDelay ){
	//    	loadNextVideo();
    //        ofLog() << "no match, checking again " << endl;

    //    }

    // if(ofGetElapsedTimeMillis() > videoSwitchTimer ){
    //     playNextVideo();
    //     //playLatch = false;
    //     videoSwitchTimer = ofGetElapsedTimeMillis()+VID_LENGTH*1000;
    // }

    //Get frame from camera
	// error = cam.RetrieveBuffer( &rawImage );

 //    if (error != PGRERROR_OK)
 //    {
 //        PrintError( error );
 //    }

 //    grayImage.resize(808,608);
    
 //    grayImage.setFromPixels(rawImage.GetData(), 808, 608);
    
 //    grayImage.mirror(false, true);
    
 //    switchImage.setFromPixels(grayImage.getPixels(), 808,608,OF_IMAGE_GRAYSCALE);

 //    switchImage.setImageType(OF_IMAGE_COLOR_ALPHA);

 //    fwrite(switchImage.getPixels(), sizeof(int)*808*608, 1, ffmpeg);


     //reversed because it switches after loading
     //if( whichMovie ){

    // whichMovie = search.getWhichMovie();

    if(playLatch && movieA.getIsMovieDone() ){ // movieA.getCurrentFrame() >= 996 
    
        playNextVideo();
        playLatch = false;
        videoLoadTimer = ofGetElapsedTimeMillis()+(VID_LENGTH-(VID_LENGTH/4))*1000;
        videoDoneTimer = ofGetElapsedTimeMillis();

    }

    if(playLatch && movieB.getIsMovieDone() ){ // movieB.getCurrentFrame() >= 996
        
        playNextVideo();
        playLatch = false;
        videoLoadTimer = ofGetElapsedTimeMillis()+(VID_LENGTH-(VID_LENGTH/4))*1000;
        videoDoneTimer = ofGetElapsedTimeMillis();
        
    }

    if(ofGetElapsedTimeMillis() > videoLoadTimer){
        
        ofLog() << "loading next video " << endl;
        loadNextVideo();
    
    }


  


     movieA.update();

    // //    }else{

     movieB.update();

    updateSearch();

}


void ofApp::updateSearch(){

    if(search.hasMatch()){

        
            ofLog() << "match found!: " << search.getFilePath() << endl;


        if(whichMovie){
        
                movieA.loadAsync(search.getFilePath());
                movieA.play();
                movieA.setPaused(TRUE);
                

                 //playA.loadFile(search.getFilePath());

                //ofLog() << "total num frames " << movieA.getTotalNumFrames() << endl;
                //movieA.setLoopState(OF_LOOP_NONE);
                //movieA.play();

            }else{
            
                 movieB.loadAsync(search.getFilePath());
                 movieB.play();
                 movieB.setPaused(TRUE);
                

                // playB.loadFile(search.getFilePath());

                //ofLog() << "total num frames " << movieB.getTotalNumFrames() << endl;
                //movieB.setLoopState(OF_LOOP_NONE);
                //movieB.play();
            
            }

        ofLog() << "is movie A paused : " << movieA.isPaused() << " or is movie B paused? : " << movieB.isPaused() << endl; 

        // //if neither player is playing, play the one that just loaded
        if(movieA.isPaused() && movieB.isPaused() ){ //}

            playNextVideo();
            ofLog() << "playing video because neither are playing" << endl;
           
        }

        
        videoLoadTimer = ofGetElapsedTimeMillis()+(VID_LENGTH-(VID_LENGTH/4))*1000;

        playLatch = true;

        //ofLog() << "deleting " << lastVidPath << endl;
        //ofSystem("rm -f "+ lastVidPath);
        matchFound =false;

        newVideoTimer = ofGetElapsedTimeMillis();

    }

}

void ofApp::loadNextVideo(){
    
    search.getNextFile();

    videoLoadTimer = ofGetElapsedTimeMillis()+(VID_LENGTH-(VID_LENGTH/4))*1000;
    
    //noMatchDelay = ofGetElapsedTimeMillis() + VID_LENGTH/4*1000;
    //checkAgainForVideo = true;
    
    ofLog() << "checking in 18 " << endl;

}

void ofApp::playNextVideo(){

    //flipped cause reversed
    if(whichMovie == true){
        //movieA.loadMovie(closestVidPath);
        //ofLog() << "total num frames " << movieA.getTotalNumFrames() << endl;
        //movieA.setLoopState(OF_LOOP_NONE);
         movieA.setPaused(FALSE);
        // movieA.play();
      
        // playA.play();
    
    }else{
        //movieB.loadMovie(closestVidPath);
        //ofLog() << "total num frames " << movieB.getTotalNumFrames() << endl;
        //movieB.setLoopState(OF_LOOP_NONE);
         movieB.setPaused(FALSE);
        // movieB.play();
      
         //playB.play();

    }

    ofLog() << "playing video : " << whichMovie << endl;

    whichMovie = !whichMovie;

    ofLog() << "next video : " << whichMovie << endl;
   
   

}
//--------------------------------------------------------------
void ofApp::draw(){

	int videoProgressA = 0;
	int videoProgressB = 0;

	
    ofSetColor(255,255,255, 255);

	if(whichMovie == false){
        //ofSetColor(255,200,200,255);
    	//playA.draw();
    	movieA.draw(0,0);
        videoProgressA = movieA.getCurrentFrame();
    
    }else{
        //ofSetColor(200,200,255, 255);
        movieB.draw(0,0);
    	//playB.draw();
    	videoProgressB = movieB.getCurrentFrame();
	
    }

    ofSetColor(255,255,255);
    if(showData){

    	ofSetColor(255,255,255);
    	ofDrawBitmapString( s_closestVidOffset + " : " + ofToString(closestVidPath) + "\n  video position A: " + ofToString(videoProgressA) + "\n  video position B: " + ofToString(videoProgressB) +  "\n  app framerate " + ofToString(ofGetFrameRate()), 70,620);
    	
    	ofSetColor(0);
    	ofDrawBitmapString( s_closestVidOffset + " : " + ofToString(closestVidPath) + "\n  video position A: " + ofToString(videoProgressA) + "\n  video position B: " + ofToString(videoProgressB), 70,720);
    	
        if(ofGetElapsedTimeMillis() - newVideoTimer < 400){
    		
    		ofFill();

    		ofSetColor(255,0,0);
    		ofDrawEllipse(50,50,30,30);

    	}

        if(ofGetElapsedTimeMillis() - videoDoneTimer < 400){
            
            ofFill();

            ofSetColor(0,255,0);
            ofDrawEllipse(100,50,30,30);

        }

    }

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
  	case 'f':
  		ofToggleFullscreen();
  		break;
    case 'v':
        showData = !showData;
        break;
  	default:
  	break;
  	

  }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


void ofApp::PrintError (Error error){
    
    error.PrintErrorTrace();

}

void ofApp::exit(){


    search.stop();

    record.stop();
    
    playA.stop();

    playB.stop();

    ofLog() << "exiting" << endl;
    
    //	releaseCamera();
    
    // pclose(ffmpeg);
    
    ofLog() << "exited" << endl;

}


