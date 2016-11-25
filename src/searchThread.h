#pragma once
#include "ofThread.h"


/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).

class searchThread : public ofThread
{
public:
    /// Create a recordThread and initialize the member
    /// variable in an initialization list.
    searchThread():VID_LENGTH(20), b_whichMovie(true), delay(60)
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
        
        dir.open("/home/nuc/of_v0.9.8_linux64_release/apps/slowglass/slowGlassPlayer/bin/recordings"); //recordings;
        dir.listDir();

        while(isThreadRunning())
        {
            // Attempt to lock the mutex.  If blocking is turned on,
            
            if( b_getNextFile && lock() ) {
                //load the movie in the next slot - not the one currently being used
                bool whichMovieToLoad = b_whichMovie;
                b_whichMovie = !b_whichMovie;
                long min = 999999999;
                long closest = 0;

                for(int i=0; i < dir.size(); i ++){

                    long vidTime = atoi(dir.getFile(i).getBaseName().c_str());
                    long currTime = ofGetUnixTime() - delay;
                    long diff = currTime - vidTime;
                    
                    //cout << "diff: " << diff  << " | Vid time: " << vidTime << " | current Time : " << currTime << endl;
                    
                    //Todo combine this into one if case
                    if( diff > 0){
                        if(diff < min ){

                            closest = diff;
                            min = diff;

                            if(min < VID_LENGTH){
                                videoPath = dir.getFile(i).getAbsolutePath();
                                matchFound = true;
                            }

                        }
                    }
                }

                ofLog() << "min match : " << min << endl;
                ofLog() << "video path : " << videoPath << endl;

                dir.listDir();
                b_getNextFile = false;

                if(matchFound){
                    ofLog() << "deleting " << lastVideoPath << endl;
                    ofSystem("rm -f /home/nuc/of_v0.9.8_linux64_release/apps/slowglass/slowGlassPlayer/bin/recordings/" + lastVideoPath);

                }

                lastVideoPath = videoPath;

                unlock();
            }
            
        }
    }

    
    bool hasMatch(){
        ofScopedLock lock(mutex);
        
        bool tmp = matchFound;

        if(matchFound == true){
           
            matchFound = false;
        }

        return tmp;
    }

    string getFilePath(){
        ofScopedLock lock(mutex);
        return videoPath;
    }

    bool getWhichMovie(){
        ofScopedLock lock(mutex);
        return b_whichMovie;
    }

    void getNextFile(){
        ofScopedLock lock(mutex);
        b_getNextFile = true;
        ofLog() << "hello from thread! : get next file = true " << endl;
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
    int                 delay;
    ofDirectory         dir;
    bool                matchFound;
    bool                b_getNextFile;
    bool                b_whichMovie;
    string              videoPath;
    string              lastVideoPath;

    int                 count;
    int                 VID_LENGTH;  

};
