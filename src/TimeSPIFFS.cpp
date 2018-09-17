#include "TimeSPIFFS.h"

#include <TimeLib.h>

// uncomment/comment following line to stop/start debug print 
#define NODEBUG_PRINT
#include <debug_print.h>

TimeSPIFFS::~TimeSPIFFS(){
    stop();
}

void TimeSPIFFS::begin(const char *fileName){
    const char* mode;
    DEBUG_PRINT("[timeSPF:begin] starting SPIFFS time\n");

    time_t savedTime = now();

    if (SPIFFS.exists(fileName))
        mode = "r+";
    else 
        mode = "w+";

    _timeFile = SPIFFS.open(fileName, mode);
    if (!_timeFile) {
        DEBUG_PRINT("[timeSPF:begin] failed opening time file\n");
        return;
    }

    _started = true;

    int c = _timeFile.read((uint8_t*)&savedTime, sizeof(savedTime));
    if (c == sizeof(savedTime)) {
        setTime(savedTime);
        DEBUG_PRINT("[timeSPF:begin] time set to=%lu\n",now());
    } else {
        DEBUG_PRINT("[timeSPF:begin] time file corrupted. Saving current time=%lu\n",now());
        update();
    }

}

void TimeSPIFFS::stop(){
    _timeFile.close();
    _started = false;
}

bool TimeSPIFFS::update() {
    if (!_started) return false;

    time_t savedTime = now();
    _timeFile.seek(0, SeekSet);
    bool ret = _timeFile.write((uint8_t*)&savedTime, sizeof(savedTime)) == sizeof(savedTime);
    DEBUG_PRINT("[timeSPF:update] saved time=%lu\n", savedTime);

    return ret;
}