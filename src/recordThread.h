#pragma once
#include "ofThread.h"
#include "FlyCapture2.h"

/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).
using namespace FlyCapture2;

class recordThread : public ofThread
{
public:
    /// Create a recordThread and initialize the member
    /// variable in an initialization list.
    recordThread()
    {
    }

    /// Start the thread.
    void start()
    {
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        startThread();
    }

    /// Signal the thread to stop.  After calling this method,
    /// isThreadRunning() will return false and the while loop will stop
    /// next time it has the chance to.
    void stop()
    {
        pclose(ffmpeg);
        stopThread();
    }

    /// Our implementation of threadedFunction.
    void threadedFunction()
    {
        
        FC2Version fc2Version;
        ostringstream version;
        version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
        cout << version.str() << endl;

        ostringstream timeStamp;
        timeStamp <<"Application build date: " << __DATE__ << " " << __TIME__;
        cout << timeStamp.str() << endl << endl;

        BusManager busMgr;
        unsigned int numCameras;
        error = busMgr.GetNumOfCameras(&numCameras);
        if (error != PGRERROR_OK)
        {
            PrintError( error );

        }

        PGRGuid guid;
        cout << "Number of cameras detected: " << numCameras << endl;

        for (unsigned int i=0; i < numCameras; i++)
        {

            error = busMgr.GetCameraFromIndex(i, &guid);
            if (error != PGRERROR_OK)
            {
                PrintError( error );

            }

           // RunSingleCamera( guid );
        }
         error = cam.Connect(&guid);
        if (error != PGRERROR_OK)
        {
            PrintError( error );

        }

        CameraInfo camInfo;
        error = cam.GetCameraInfo(&camInfo);
        if (error != PGRERROR_OK)
        {
            PrintError( error );
        }

        Property camProp;
        PropertyInfo camPropInfo;

        camProp.type = FRAME_RATE;
        error = cam.GetProperty( &camProp );
        if (error != PGRERROR_OK){
            PrintError( error );
        }
        ofLog() <<  " --------------------------- Frame rate is : " <<  camProp.absValue << endl;

        error = cam.StartCapture();
        if (error != PGRERROR_OK)
        {
            PrintError( error );
        }

        error = cam.StopCapture();
        error = cam.StartCapture();

        grayImage.allocate(808,608);
        colorImage.allocate(808,608);

        switchImage.allocate(808,608,OF_IMAGE_COLOR_ALPHA);

        const char* cmd = "ffmpeg -re -f rawvideo -pix_fmt rgba  -s 808x608 -i - "" -threads 0 -preset fast -y  -map 0  -f segment -segment_time 20 -reference_stream auto -segment_format mp4  -pix_fmt yuv420p -vcodec h264 -strftime 1 recordings/%s.mp4";        //flv for successfull m3u8 streams!                                                                                
        ffmpeg = popen(cmd, "w");

        ofLog() << "video record setup done " << endl;
       
        while(isThreadRunning())
        {
            // Attempt to lock the mutex.  If blocking is turned on,
            
            if( lock() ) {

                error = cam.RetrieveBuffer( &rawImage );

                if (error != PGRERROR_OK)
                {
                    PrintError( error );
                }

                grayImage.resize(808,608);
                
                grayImage.setFromPixels(rawImage.GetData(), 808, 608);
                
                grayImage.mirror(false, true);
                
                switchImage.setFromPixels(grayImage.getPixels()); //, 808,608,OF_IMAGE_GRAYSCALE, true

                switchImage.setImageType(OF_IMAGE_COLOR_ALPHA);

                fwrite(switchImage.getPixels().getData(), sizeof(int)*808*608, 1, ffmpeg);
               
                unlock();

            }
            
        }
    }

  
void PrintError (Error error){
    
    error.PrintErrorTrace();

}

protected:
    // A flag to check and see if we should throw a test exception.
    Poco::AtomicCounter shouldThrowTestException;

    // This is a simple variable that we aim to always access from both the
    // main thread AND this threaded object.  Therefore, we need to protect it
    // with the mutex.  In the case of simple numerical variables, some
    // garuntee thread safety for small integral types, but for the sake of
    // illustration, we use an int.  This int could represent ANY complex data
    // type that needs to be protected.
    //
    // Note, if we simply want to count in a thread-safe manner without worrying
    // about mutexes, we might use Poco::AtomicCounter instead.
    Error error;
    Camera cam;
    Image rawImage;
    Image convertedImage;
    ofVideoGrabber vidGrabber;
    ofxCvGrayscaleImage     grayImage;
    ofxCvColorImage         colorImage;
    FILE * ffmpeg;
    ofImage                 switchImage;


};
