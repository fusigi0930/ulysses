#ifndef CRUNLUATHREAD_H
#define CRUNLUATHREAD_H
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "luacore.h"

class CRunLuaThread : public QThread
{
private:
	lua_State *m_LuaState;
	QString m_szLuaFunc;
	CLuaCore *m_LuaCore;

public:
	CRunLuaThread(lua_State* L=NULL);
	virtual ~CRunLuaThread();

	void setFunc(char *szFunc);
	virtual void run();
};

#endif // CRUNLUATHREAD_H
