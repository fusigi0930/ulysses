#include "cluacore.h"


CLuaCore::CLuaCore() : CRootInterp()
{

}

CLuaCore::~CLuaCore() {

}

bool CLuaCore::open(QString szFile) {
	CRootInterp::open(szFile);

	return true;
}

void CLuaCore::close() {
	// terminate lua and thread


	CRootInterp::close();
}


int CLuaCore::run() {
	return 0;
}

int CLuaCore::run_as_string(QString szScript) {
	return 0;
}
