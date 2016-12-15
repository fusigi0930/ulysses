#include "luacore.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "crunluathread.h"

#include <typeinfo>
#include <vector>
#include <windows.h>

#define EVENT_KEY		0
#define EVENT_MOUSE		1
#define EVENT_CUSTOM	2

#define EVENT_ACTION_DOWN		0
#define EVENT_ACTION_UP			1
#define EVENT_ACTION_CLICK		2
#define EVENT_ACTION_RDOWN		3
#define EVENT_ACTION_RUP		4
#define EVENT_ACTION_RCLICK		5
#define EVENT_ACTION_MOVE		6

#define MODE_EVENT_WM	1
#define MODE_EVENT_FUNC	0

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
	else {
		pThread->second->start();
	}

	return 0;
}

void CLuaCore::sendInputWmEvent(std::vector<int> *vt) {
	// argument 0: event type
	// argument 1: event value
	// argument 2: event action
	// argument 3: event duration or repeat times
	DMSG("sent input event");
	switch ((*vt)[0]) {
		default: break;
		case EVENT_KEY:
		switch((*vt)[2]) {
			default: break;
			case EVENT_ACTION_DOWN:
				::SendMessage(::GetForegroundWindow(), WM_KEYDOWN, static_cast<WPARAM>((*vt)[1]), 1);
				QThread::msleep((*vt)[3]);
				break;
			case EVENT_ACTION_UP:
				::SendMessage(::GetForegroundWindow(), WM_KEYUP, static_cast<WPARAM>((*vt)[1]), 1);
				QThread::msleep((*vt)[3]);
				break;
			case EVENT_ACTION_CLICK:
				for(int i=0; i<(*vt)[3]; i++) {
					::SendMessage(::GetForegroundWindow(), WM_KEYDOWN, static_cast<WPARAM>((*vt)[1]), 1);
					QThread::msleep(70);
					::SendMessage(::GetForegroundWindow(), WM_KEYUP, static_cast<WPARAM>((*vt)[1]), 1);
					QThread::msleep(70);
				}
				break;
			}

			break;
		case EVENT_MOUSE:
			switch ((*vt)[3]) {
				default: break;
				case EVENT_ACTION_DOWN:
					::SendMessage(NULL, WM_LBUTTONDOWN, 0, static_cast<LPARAM>((*vt)[1] | (*vt)[2] << 16));
					QThread::msleep((*vt)[4]);
					break;
				case EVENT_ACTION_UP:
					::SendMessage(NULL, WM_LBUTTONUP, 0, static_cast<LPARAM>((*vt)[1] | (*vt)[2] << 16));
					QThread::msleep((*vt)[4]);
					break;
				case EVENT_ACTION_CLICK:
					for(int i=0; i<(*vt)[4]; i++) {
						::SendMessage(NULL, WM_LBUTTONDOWN, 0, static_cast<LPARAM>((*vt)[1] | (*vt)[2] << 16));
						QThread::msleep(70);
						::SendMessage(NULL, WM_LBUTTONUP, 0, static_cast<LPARAM>((*vt)[1] | (*vt)[2] << 16));
						QThread::msleep(70);
					}
					break;
				case EVENT_ACTION_RDOWN:
					::SendMessage(NULL, WM_RBUTTONDOWN, 0, static_cast<LPARAM>((*vt)[1] | (*vt)[2] << 16));
					QThread::msleep((*vt)[4]);
					break;
				case EVENT_ACTION_RUP:
					::SendMessage(NULL, WM_RBUTTONDOWN, 0, static_cast<LPARAM>((*vt)[1] | (*vt)[2] << 16));
					QThread::msleep((*vt)[4]);
					break;
				case EVENT_ACTION_RCLICK:
					for(int i=0; i<(*vt)[4]; i++) {
						::SendMessage(NULL, WM_RBUTTONDOWN, 0, static_cast<LPARAM>((*vt)[1] | (*vt)[2] << 16));
						QThread::msleep(70);
						::SendMessage(NULL, WM_RBUTTONUP, 0, static_cast<LPARAM>((*vt)[1] | (*vt)[2] << 16));
						QThread::msleep(70);
					}
					break;
				case EVENT_ACTION_MOVE:
					::SendMessage(::GetForegroundWindow(), WM_MOUSEMOVE, 0, static_cast<LPARAM>((*vt)[1] | (*vt)[2] << 16));
					QThread::msleep((*vt)[4]);
					break;
			}
			break;
	}
}

void CLuaCore::sendInputFuncEvent(std::vector<int> *vt) {
	// argument 0: event type
	// argument 1: event value
	// argument 2: event action
	// argument 3: event duration or repeat times

	INPUT in;
	DMSG("sent input event");
	switch ((*vt)[0]) {
		default: break;
		case EVENT_KEY:
			in.type=INPUT_KEYBOARD;
			in.ki.wVk=(*vt)[1];
			in.ki.wScan=0;
			in.ki.time=0;
			in.ki.dwExtraInfo=NULL;

			switch((*vt)[2]) {
				default: break;
				case EVENT_ACTION_DOWN:
					in.ki.dwFlags=0;
					::SendInput(1, &in, sizeof(in));
					QThread::msleep((*vt)[3]);
					break;
				case EVENT_ACTION_UP:
					in.ki.dwFlags=KEYEVENTF_KEYUP;
					::SendInput(1, &in, sizeof(in));
					QThread::msleep((*vt)[3]);
					break;
				case EVENT_ACTION_CLICK:
					for(int i=0; i<(*vt)[3]; i++) {
						in.ki.dwFlags=0;
						::SendInput(1, &in, sizeof(in));
						QThread::msleep(70);
						in.ki.dwFlags=KEYEVENTF_KEYUP;
						::SendInput(1, &in, sizeof(in));
						QThread::msleep(70);
					}
					break;
			}

			break;
		case EVENT_MOUSE:
			in.type=INPUT_MOUSE;
			in.mi.dx=(*vt)[1]*(65535/::GetSystemMetrics(SM_CXSCREEN));
			in.mi.dy=(*vt)[2]*(65535/::GetSystemMetrics(SM_CYSCREEN));
			in.mi.mouseData=0;
			in.mi.time=0;
			in.mi.dwExtraInfo=NULL;

			switch ((*vt)[3]) {
				default: break;
				case EVENT_ACTION_DOWN:
					in.mi.dwFlags=MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN;
					::SendInput(1, &in, sizeof(in));
					QThread::msleep((*vt)[4]);
					break;
				case EVENT_ACTION_UP:
					in.mi.dwFlags=MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP;
					::SendInput(1, &in, sizeof(in));
					QThread::msleep((*vt)[4]);
					break;
				case EVENT_ACTION_CLICK:
					for(int i=0; i<(*vt)[4]; i++) {
						in.mi.dwFlags=MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN;
						::SendInput(1, &in, sizeof(in));
						QThread::msleep(70);
						in.mi.dwFlags=MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP;
						::SendInput(1, &in, sizeof(in));
						QThread::msleep(70);
					}
					break;
				case EVENT_ACTION_RDOWN:
					in.mi.dwFlags=MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN;
					::SendInput(1, &in, sizeof(in));
					QThread::msleep((*vt)[4]);
					break;
				case EVENT_ACTION_RUP:
					in.mi.dwFlags=MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP;
					::SendInput(1, &in, sizeof(in));
					QThread::msleep((*vt)[4]);
					break;
				case EVENT_ACTION_RCLICK:
					for(int i=0; i<(*vt)[4]; i++) {
						in.mi.dwFlags=MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN;
						::SendInput(1, &in, sizeof(in));
						QThread::msleep(70);
						in.mi.dwFlags=MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP;
						::SendInput(1, &in, sizeof(in));
						QThread::msleep(70);
					}
					break;
				case EVENT_ACTION_MOVE:
					in.mi.dwFlags=MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE;
					::SendInput(1, &in, sizeof(in));
					QThread::msleep((*vt)[4]);
					break;
			}

			break;
	}
}

int CLuaCore::CLuaSendEvent(lua_State *L) {
	if (NULL == L) {
		return 0;
	}

	int nArgNum=lua_gettop(L);
	if (4 > nArgNum) {
		DMSG("argument number error!");
		return 0;
	}

	std::vector<int> vtArgu;

	for (int i=1; i<=nArgNum; i++) {
		int nBuf=lua_tointeger(L, i);
		DMSG("argument %d: %d", i, nBuf);
		vtArgu.push_back(nBuf);
	}

	lua_getglobal(L, VARNAME_EVENT_MODE);
	int nEventMode=lua_tointeger(L, -1);

	DMSG("event mode: %d", nEventMode);

	switch (nEventMode) {
		default: break;
		case MODE_EVENT_FUNC:
			sendInputFuncEvent(&vtArgu);
			break;
		case MODE_EVENT_WM:
			sendInputWmEvent(&vtArgu);
			break;
	}

	return 0;
}

int CLuaCore::CLuaSetEventMode(lua_State *L) {
	DMSG("set mode!!");
	if (NULL == L) {
		return 0;
	}

	int nArgNum=lua_gettop(L);
	//if (1 > nArgNum) {
	//	DMSG("argument number error!");
	//	return 0;
	//}

	int nEventMode=lua_tointeger(L, -1);
	DMSG("event mode: %d", nEventMode);

	lua_pushinteger(L, nEventMode);
	lua_setglobal(L, VARNAME_EVENT_MODE);

	return 0;
}
