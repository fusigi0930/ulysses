#ifndef CLUACORE_H
#define CLUACORE_H

#include "luacore_global.h"

#include "debug.h"

#include <QString>
#include <QFuture>
#include <QFutureWatcher>
#include <base_interp.h>

// use Lua 5.3.2
struct lua_State;

class LUACORESHARED_EXPORT CLuaCore : public CRootInterp
{
private:
	lua_State *m_LuaState;
public:
	CLuaCore();
	virtual ~CLuaCore();

	virtual bool open(QString szFile);
	virtual void close();

	virtual int run();

	virtual int run_as_string(QString szScript);
	virtual QString getResult();
};

#endif // CLUACORE_H
