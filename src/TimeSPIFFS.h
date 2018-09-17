#ifndef __TIMESPIFFS_H__
#define __TIMESPIFFS_H__

#include <FS.h>

class TimeSPIFFS {
    protected:
        File _timeFile;
        bool _started;
    
    public:
        ~TimeSPIFFS();
        void begin(const char *fileName);
        bool update();
        bool isStarted() {return _started;};
        void stop();

};


#endif