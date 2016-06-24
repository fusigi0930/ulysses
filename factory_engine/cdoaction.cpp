#include "cdoaction.h"

CDoAction::CDoAction() : QObject(), CBaseAction()
{

}

CDoAction::~CDoAction() {
	close();
}

int CDoAction::run() {
	return 0;
}

void CDoAction::close() {

}
