#include "luacore.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <typeinfo>

static const struct luaL_Reg override_func[] = {
	{ "print", CLuaCore::luaPrint },
	{ NULL, NULL},
};

void CLuaCore::registerLuaOverrideFunc() {
	if (NULL == m_LuaState)
		return;

	for (int i=0; NULL != override_func[i].func; i++) {
		lua_pushcfunction(m_LuaState, override_func[i].func);
		lua_setglobal(m_LuaState, override_func[i].name);
		//lua_push(m_LuaState, 1);
	}
}

int CLuaCore::luaPrint(lua_State *L) {
	CLuaCore *luacore=NULL;
	CLuaCore **p;
	QString outString;
	lua_getglobal(L, "LuaCore");
	DMSG("top: %d", lua_gettop(L));
	for (int i=0; i<=lua_gettop(L); i++) {
		switch (lua_type(L, i)) {
			case LUA_TUSERDATA:
				p=reinterpret_cast<CLuaCore**>(lua_touserdata(L, i));
				if (NULL != p && typeid(CLuaCore) == typeid(**p))
					luacore=*p;
				break;
			case LUA_TSTRING:
				outString.append(lua_tostring(L, i));
				break;
			case LUA_TNUMBER:
				outString.append(QString().sprintf("%d",lua_tonumber(L,i)));
				break;
			default:
				outString.append(lua_tostring(L, i));
				break;
		}
	}

	if (luacore) {
		luacore->setResult(outString);
	}
	return 0;
}
