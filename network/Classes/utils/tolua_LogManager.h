//
//  tolua_LogManager.h
//  CGame
//
//  Created by admin on 14-11-3.
//
//

#ifndef CGAME_TOLUA_LOGMANAGER_H
#define CGAME_TOLUA_LOGMANAGER_H
#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

extern "C" {
#include "tolua_fix.h"
}

#include "utils/LogManager.h"


/* Exported function */
TOLUA_API int  tolua_LogManager_open (lua_State* tolua_S);

#endif
