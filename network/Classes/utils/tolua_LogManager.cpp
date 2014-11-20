/*
** Lua binding: LogManager
** Generated automatically by tolua++-1.0.92 on Mon Nov  3 19:43:51 2014.
*/
#include "utils/tolua_LogManager.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"LogManager");
}

/* method: getInstance of class  LogManager */
#ifndef TOLUA_DISABLE_tolua_LogManager_LogManager_getInstance00
static int tolua_LogManager_LogManager_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LogManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   LogManager* tolua_ret = (LogManager*)  LogManager::getInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"LogManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendErrorLog of class  LogManager */
#ifndef TOLUA_DISABLE_tolua_LogManager_LogManager_sendErrorLog00
static int tolua_LogManager_LogManager_sendErrorLog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LogManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LogManager* self = (LogManager*)  tolua_tousertype(tolua_S,1,0);
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendErrorLog'", NULL);
#endif
  {
   self->sendErrorLog(msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendErrorLog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLogUrl of class  LogManager */
#ifndef TOLUA_DISABLE_tolua_LogManager_LogManager_setLogUrl00
static int tolua_LogManager_LogManager_setLogUrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LogManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LogManager* self = (LogManager*)  tolua_tousertype(tolua_S,1,0);
  const char* url = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLogUrl'", NULL);
#endif
  {
   self->setLogUrl(url);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLogUrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLogUrl of class  LogManager */
#ifndef TOLUA_DISABLE_tolua_LogManager_LogManager_getLogUrl00
static int tolua_LogManager_LogManager_getLogUrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const LogManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const LogManager* self = (const LogManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLogUrl'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getLogUrl();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLogUrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_LogManager_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"LogManager","LogManager","",NULL);
  tolua_beginmodule(tolua_S,"LogManager");
   tolua_function(tolua_S,"getInstance",tolua_LogManager_LogManager_getInstance00);
   tolua_function(tolua_S,"sendErrorLog",tolua_LogManager_LogManager_sendErrorLog00);
   tolua_function(tolua_S,"setLogUrl",tolua_LogManager_LogManager_setLogUrl00);
   tolua_function(tolua_S,"getLogUrl",tolua_LogManager_LogManager_getLogUrl00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_LogManager (lua_State* tolua_S) {
 return tolua_LogManager_open(tolua_S);
};
#endif

