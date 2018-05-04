//
// Created by lz on 2017/10/24.
//
#include "../Common.h"

#ifndef ADVMPTESTEASY_INTERP_H
#define ADVMPTESTEASY_INTERP_H

static inline s4 s4FromSwitchData(const void* switchData) {
    return *(s4*) switchData;
}
s4 dvmInterpHandlePackedSwitch(const u2* switchData, s4 testVal);
s4 dvmInterpHandleSparseSwitch(const u2* switchData, s4 testVal);



#endif //ADVMPTESTEASY_INTERP_H
