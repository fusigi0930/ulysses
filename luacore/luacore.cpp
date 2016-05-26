#include "luacore.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <typeinfo>

CLuaCore::CLuaCore() : CRootInterp()
{
	m_LuaState=NULL;
}

CLuaCore::~CLuaCore() {
	close();
}

void CLuaCore::registerLuaCore() {
	if (NULL == m_LuaState) {
		m_LuaState=luaL_newstate();
	}

	luaL_openlibs(m_LuaState);
	registerLuaOverrideFunc();
	DMSG("top: %d", lua_gettop(m_LuaState));


	m_luaUserData=reinterpret_cast<CLuaCore**>(lua_newuserdata(m_LuaState, sizeof(CLuaCore*)));
	*m_luaUserData=this;
	lua_setglobal(m_LuaState, "LuaCore");
#if 0
	CLuaCore *luacore=NULL;
	for (int i=1; i<=lua_gettop(m_LuaState); i++) {
		switch (lua_type(m_LuaState, i)) {
			case LUA_TUSERDATA:
				CLuaCore **p=reinterpret_cast<CLuaCore**>(lua_touserdata(m_LuaState, i));
				if (NULL != p && typeid(CLuaCore) == typeid(**p))
					luacore=*p;
				break;

		}
	}
#endif

}

bool CLuaCore::open(QString szFile) {
	CRootInterp::open(szFile);

	if (NULL != m_LuaState) {
		lua_close(m_LuaState);
		m_LuaState=NULL;
	}

	registerLuaCore();

	return true;
}

void CLuaCore::close() {
	// terminate lua and thread
	if (NULL != m_LuaState) {
		lua_close(m_LuaState);
		m_LuaState=NULL;
	}

	CRootInterp::close();
}


int CLuaCore::run() {

	return 0;
}

int CLuaCore::run_as_string(QString szScript) {
	if (NULL == m_LuaState) {
		registerLuaCore();
	}

	luaL_dostring(m_LuaState, QSZ(szScript));
	return 0;
}

QString CLuaCore::getResult() {
	return m_szLuaResult;
}

void CLuaCore::setResult(QString str) {
	m_szLuaResult=str;
}
