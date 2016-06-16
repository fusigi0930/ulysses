#include "netio.h"
#include "debug.h"

#include <QRegExp>

#include <QDateTime>
#include <QTime>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#include <unistd.h>

#define TEMP_BUF_SIZE 1024

////////////////////////////////////////////////////
/// \brief CTelnetIO
///

CTelnetIO::CTelnetIO() : CNetcatIO() {

}

CTelnetIO::~CTelnetIO() {

}

