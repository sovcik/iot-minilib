#include "Button.h"

#define NODEBUG_PRINT
#include <debug_print.h>

Button::Button(const char* id, bool enabled){
    this->id = new char[strlen(id)+1];
    strcpy(this->id,id);
    this->label = NULL;
    setStatus(enabled);
    DEBUG_PRINT("[Button:new] button created id=%s enabled=%d\n",id, this->enabled);
    pushCount = 0;
}

Button::~Button() {
    if (id) delete []id;
    if (label) delete []label;
}

unsigned long Button::getPushDuration() const {
     return longTimer.getRunningTime();
}

int Button::getPushCount() const {
    DEBUG_PRINT("[Button:pushCount] id=%s pushCount=%d\n",id, this->pushCount);
    return this->pushCount;
}

bool Button::wasPushed(bool clear){
    if (pushCount>0){
        pushCount--;
        if (clear)
            pushCount = 0;
        DEBUG_PRINT("[Button:wasPushed] id=%s pushCount=%d\n",id,pushCount);
        return true;            
    } else {
        return false;
    }
    
}

const char* Button::getId(){
    return id;
}

bool Button::isEnabled() const {
    return enabled;
}

void Button::enable(){
    this->enabled = true;
}

void Button::disable(){
    this->enabled = false;
}

void Button::setStatus(bool status){
    if (status)
        enable();
    else
        disable();
}


const char* Button::getLabel(){
    return label;
}

void Button::setLabel(const char* label){
    if (this->label)
        delete []this->label;
    this->label = new char[strlen(label)+1];
    strcpy(this->label, label);
}

void Button::loop() {
    delay(2);           // as buttons are called often, this is good place to let microcontroller do its internal stuff
    
    if (isPressed()){
        if (!btnDown){
            DEBUG_PRINT("[Button:loop] Button down id=%s enabled=%d count=%d\n",id, enabled, this->pushCount);
            btnDown = true;
            upTimer.stop();
            longTimer.restart();
        }
    } else {
        if (btnDown){
            btnDown = false;
            this->pushCount++;
            DEBUG_PRINT("[Button:loop] Button up id=%s enabled=%d count=%d\n",id, enabled, this->pushCount);
            upTimer.restart();
            longTimer.stop();
        } else {
            if (pushCount > 0 && upTimer.timeout()){         // if too much time between presses
                DEBUG_PRINT("[Button:loop] Long press timeout id=%s\n",id);
                pushCount = 0;
            }
        }
    }
    
}





