#include "xmlcfg.h"
#include "debug.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

CXmlConfig::CXmlConfig() : CBaseItem<QXmlStreamReader, QXmlStreamWriter, SConfig>()
{
}

CXmlConfig::~CXmlConfig() {

}
