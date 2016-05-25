#include "luacore.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


CLuaCore::CLuaCore() : CRootInterp()
{
	m_LuaState=NULL;
}

CLuaCore::~CLuaCore() {
	close();
}

bool CLuaCore::open(QString szFile) {
	CRootInterp::open(szFile);

	if (NULL != m_LuaState) {
		lua_close(m_LuaState);
		m_LuaState=NULL;
	}

	m_LuaState=luaL_newstate();
	luaL_openlibs(m_LuaState);

	return true;
}

void CLuaCore::close() {
	// terminate lua and thread
	if (NULL != m_LuaState) {
		lua_close(m_LuaState);
		m_LuaState;
	}

	CRootInterp::close();
}


int CLuaCore::run() {

	return 0;
}

int CLuaCore::run_as_string(QString szScript) {
	if (NULL == m_LuaState) {
		m_LuaState=luaL_newstate();
		luaL_openlibs(m_LuaState);
	}

	luaL_dostring(m_LuaState, QSZ(szScript));
	return 0;
}

QString CLuaCore::getResult() {
	return m_szLuaResult;
}
