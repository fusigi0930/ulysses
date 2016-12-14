#include "luacore.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <typeinfo>
#include <QFileInfo>

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
	registerConst();
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

void CLuaCore::adjustLuaPath() {
	// compare path environment
	lua_getglobal(m_LuaState, "package");
	lua_getfield(m_LuaState, -1, "path");
	QString szEnvPath=lua_tostring(m_LuaState, -1);
	DMSG("LUA PATH: %s", QSZ(szEnvPath));
	QFileInfo fi(m_szFile);

	if (!szEnvPath.contains(fi.absolutePath(), Qt::CaseInsensitive)) {
		QString szBuf;
		szBuf.sprintf("%s;%s/?.lua;", QSZ(szEnvPath), QSZ(fi.absolutePath()));
		lua_pop(m_LuaState, 1);
		lua_pushstring(m_LuaState, QSZ(szBuf));
		lua_setfield(m_LuaState, -2, "path");
		lua_pop(m_LuaState, 1);
	}
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

	adjustLuaPath();

	if (luaL_loadfile(m_LuaState, QSZ(m_szFile))) {
		DMSG("load %s failed!", QSZ(m_szFile));
		return -3;
	}

	if (lua_pcall(m_LuaState, 0, LUA_MULTRET, 0)) {
		DMSG("run lua script failed");
		return -4;
	}

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
