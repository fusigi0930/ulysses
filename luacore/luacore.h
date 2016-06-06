#ifndef CLUACORE_H
#define CLUACORE_H

#include "luacore_global.h"

#include "debug.h"

#include <QString>
#include <base_interp.h>
#include <QThread>

class LUACORESHARED_EXPORT CInterpThread : public QThread
{
	Q_OBJECT
private:
	CRootInterp *m_interp;
public:
	CInterpThread();
	~CInterpThread();

	void setIO(CRootInterp *io);
	virtual void run();
};

// use Lua 5.3.2
struct lua_State;

#define GLOBAL_LUACORE "__LUA_CORE__"

class LUACORESHARED_EXPORT CLuaCore : public CRootInterp
{
private:
	lua_State *m_LuaState;
	QString m_szLuaResult;

	void registerLuaCore();
	void registerLuaOverrideFunc();
	CLuaCore **m_luaUserData;
public:
	CLuaCore();
	virtual ~CLuaCore();

	virtual bool open(QString szFile);
	virtual void close();

	virtual int run();

	virtual int run_as_string(QString szScript);
	virtual void setResult(QString str);
	virtual QString getResult();

	// override original Lua function
	static int luaPrint(lua_State* L);
};

#endif // CLUACORE_H
