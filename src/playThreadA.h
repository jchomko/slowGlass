#pragma once
#include "ofThread.h"


/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).


class playThreadA : public ofThread
{
public:
    /// Create a recordThread and initialize the member
    /// variable in an initialization list.
    playThreadA()
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
        stopThread();
    }

    /// Our implementation of threadedFunction.
    void threadedFunction()
    {
        // video.loadMovie("../recordings/test.flv");
        // video.play();
        // video.setPaused(TRUE);
        
        videoTex.allocate(808,608,GL_RGB);

        ofLog() << "loading thread A with test  "<< endl;
       
        while(isThreadRunning())
        {
            // Attempt to lock the mutex.  If blocking is turned on,
            
            if( lock() ) {

                video.update();

                unlock();
            }
            
        }
    }

    void loadFile(string fileName){

            ofScopedLock lock(mutex);
            video.load(fileName);
            video.play();
            //video.setPaused(TRUE);
            ofLog() << "loading thread A with : " << fileName << endl;

    }

    void draw(){

        

        if(lock())
        {
            // The mutex is now locked and the "count"
            // variable is protected.  Time to read it.
            //video.draw(0,0);
            // videoTex = *video.getTexture();

            // Unlock the mutex.  This is only
            // called if lock() returned true above.
            unlock();
        }

        videoTex.draw(0,0);


    }

    void play(){
        
        ofScopedLock lock(mutex);
        video.setPaused(FALSE);
    }

    bool getIsPaused(){
        ofScopedLock lock(mutex);
        return video.isPaused();
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
    ofVideoPlayer       video;
    ofTexture           videoTex;


};