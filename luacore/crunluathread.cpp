#include "crunluathread.h"
#include <typeinfo>
#include "debug.h"

CRunLuaThread::CRunLuaThread(lua_State *L)
{
	m_LuaState=L;
}

CRunLuaThread::~CRunLuaThread() {

}

void CRunLuaThread::setFunc(char *szFunc) {
	m_szLuaFunc=szFunc;
}

void CRunLuaThread::run() {
	if (NULL == m_LuaState) return;

	if (m_szLuaFunc.isEmpty()) return;

	lua_getglobal(m_LuaState, QSZ(m_szLuaFunc));
	DMSG("start call func %s", QSZ(m_szLuaFunc));
	lua_call(m_LuaState, 0, 0);
	DMSG("end of call func %s", QSZ(m_szLuaFunc));

}
