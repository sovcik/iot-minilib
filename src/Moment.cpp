#include "Moment.h"

//#define NODEBUG_PRINT
#include <debug_print.h>

int Moment::fromISO(const char *isoTime){
    char buff[15];
    char buff2[6];

    DEBUG_PRINT("[mmnt:fromISO] input=%s\n",isoTime);

    sscanf(isoTime,"%4d-%02d-%02dT%02d:%02d:%02d%s", 
        &me.year, 
        &me.month,
        &me.day,
        &me.hour,
        &me.minute,
        &me.second,
        &buff[0]
        );
    
    int i = 0;
    
    if (buff[i] == '.'){
        i++;
        while(buff[i] >= '0' && buff[i] <= '9') i++;
        if (i>1){
            strncpy(&buff2[0],&buff[1],i-1);
            buff[i-1] = 0;
            sscanf(&buff2[0], "%d", &me.millis);
        }
    }

    if (buff[i] == '+' || buff[i] == '-'){
        me.tzSig=1;
        if(buff[i] == '-') me.tzSig = -1;
        sscanf(&buff[i+1],"%02d:%02d",&me.tzOffH, &me.tzOffM);
    }

    DEBUG_PRINT("[mmnt:fromISO] elements %d-%02d-%02dT%02d:%02d:%02d.%03d+%02d:%02d",
        me.year, me.month, me.day,
        me.hour, me.minute, me.second, me.millis,
        me.tzSig, me.tzOffH, me.tzOffM);

}