#include "luacore.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <typeinfo>

//////////////////////////////////////////////
/// CInterpThread
///

CInterpThread::CInterpThread() : QThread() {
	m_interp=NULL;
}

CInterpThread::~CInterpThread() {

}

void CInterpThread::setIO(CRootInterp *interp) {
	m_interp=interp;
}

void CInterpThread::run() {
	if (NULL == m_interp)
		return;

	m_interp->run();
}

/////////////////////////////////////////////////////
/// \brief CLuaCore::CLuaCore
///

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
	registerFunc();
	DMSG("top: %d", lua_gettop(m_LuaState));


	m_luaUserData=reinterpret_cast<CLuaCore**>(lua_newuserdata(m_LuaState, sizeof(CLuaCore*)));
	*m_luaUserData=this;
	lua_setglobal(m_LuaState, GLOBAL_LUACORE);
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

	if (NULL == m_LuaState) {
		registerLuaCore();
	}

	CInterpThread *thread=new CInterpThread;
	thread->setIO(this);
	setThread(thread);
	thread->start();

	return true;
}

void CLuaCore::close() {
	// terminate lua and thread
	for (std::map<QString,QThread*>::iterator pThread=m_mapFuncThread.begin(); pThread != m_mapFuncThread.end(); pThread++) {
		if (pThread->second->isRunning()) pThread->second->terminate();
		delete pThread->second;
	}
	m_mapFuncThread.clear();

	if (NULL != m_LuaState) {
		lua_close(m_LuaState);
		m_LuaState=NULL;
	}
	if (getThread()) {
		if (getThread()->isRunning()) getThread()->terminate();
		delete getThread();
		setThread(NULL);
	}
	CRootInterp::close();
}


int CLuaCore::run() {
	if (NULL == m_LuaState) {
		DMSG("lua is not inited!");
		return -1;
	}
	if (m_szFile.isEmpty()) {
		DMSG("file name is empty");
		return -2;
	}

	luaL_dofile(m_LuaState, QSZ(m_szFile));

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
	DMSG("get response result: %s", QSZ(m_szLuaResult));
	return m_szLuaResult;
}

void CLuaCore::setResult(QString str) {
	DMSG("set response result: %s", QSZ(str));
	m_szLuaResult=str;
}
