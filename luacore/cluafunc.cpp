#include "luacore.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "crunluathread.h"

#include <typeinfo>

#define EVENT_KEY		0
#define EVENT_MOUSE		1
#define EVENT_CUSTOM	2

#define VARNAME_EVENT_MODE		"__var_eventmode__"

static const struct luaL_Reg additional_func[] = {
	{ "run_thread", CLuaCore::CLuaRunThread },
	{ "send_event", CLuaCore::CLuaSendEvent },
	{ "set_event_mode", CLuaCore::CLuaSetEventMode },
	{ NULL, NULL},
};

void CLuaCore::registerFunc() {
	if (NULL == m_LuaState)
		return;

	for (int i=0; NULL != additional_func[i].func; i++) {
		lua_pushcfunction(m_LuaState, additional_func[i].func);
		lua_setglobal(m_LuaState, additional_func[i].name);
	}
}

int CLuaCore::CLuaRunThread(lua_State *L) {
	DMSG("run function");
	if (NULL == L) {
		return 0;
	}

	int nArgNum=lua_gettop(L);
	if (0 >= nArgNum) {
		DMSG("no argument!");
		return 0;
	}

	char *szFunc=const_cast<char*>(lua_tostring(L, -1));
	if (NULL == szFunc || 0 == strlen(szFunc)) {
		DMSG("function name is null");
		return 0;
	}

	DMSG("register lua function name: %s", szFunc);

	QString szBuf=const_cast<char*>(szFunc);

	lua_getglobal(L, GLOBAL_LUACORE);
	CLuaCore *core=NULL;
	for (int i=0; i<=lua_gettop(L); i++) {
		if (LUA_TUSERDATA == lua_type(L, i)) {
			CLuaCore **p=reinterpret_cast<CLuaCore**>(lua_touserdata(L, i));
			if (NULL != p && typeid(CLuaCore) == typeid(**p))
				core=*p;
			break;
		}
	}

	std::map<QString, QThread*>::iterator pThread=core->m_mapFuncThread.find(szBuf);
	if (pThread == core->m_mapFuncThread.end()) {
		CRunLuaThread *pNewThread=new CRunLuaThread(L);
		pNewThread->setFunc(szFunc);
		core->m_mapFuncThread[szBuf] = pNewThread;
		pNewThread->start();
	}

	return 0;
}

int CLuaCore::CLuaSendEvent(lua_State *L) {
	if (NULL == L) {
		return 0;
	}

	int nArgNum=lua_gettop(L);
	if (1 != nArgNum) {
		DMSG("argument number error!");
		return 0;
	}



	return 0;
}

int CLuaCore::CLuaSetEventMode(lua_State *L) {
	if (NULL == L) {
		return 0;
	}

	int nArgNum=lua_gettop(L);
	if (1 != nArgNum) {
		DMSG("argument number error!");
		return 0;
	}

	int nEventMode=lua_tointeger(L, -1);
	DMSG("event mode: %d", nEventMode);

	lua_pushinteger(L, nEventMode);
	lua_setglobal(L, VARNAME_EVENT_MODE);

	return 0;
}
