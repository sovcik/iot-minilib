#include "Looper.h"

LooperHandler::LooperHandler(){
    _loopers = 0;
}

LooperHandler::~LooperHandler(){
    LooperEntry *n, *e;
    e = _loopers;
    while (e->next){
        n = e->next;
        delete e;
        e = n;
    }
    if (e) delete e;
    _loopers = 0;

}

void LooperHandler::registerLooper(Looper *looper){
    LooperEntry *le = new LooperEntry();
    le->looper = looper;
    le->next = _loopers;
    _loopers = le;
}

void LooperHandler::loop(){
    LooperEntry *n = _loopers;
    while(n){
        n->looper->loop();
        n = n->next;
    }
}
