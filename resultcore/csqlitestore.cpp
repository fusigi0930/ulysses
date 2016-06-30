#include "csqlitestore.h"
#include "debug.h"

CSQLiteStore::CSQLiteStore() : CBaseStore(),
	m_szFile()
{

}

CSQLiteStore::~CSQLiteStore() {

}

bool CSQLiteStore::open(char *szFile) {
	if (NULL == szFile) {
		return false;
	}

	m_szFile=szFile;
	return true;
}

unsigned long long CSQLiteStore::add(const QVariant &item) {
	return 0;
}

bool CSQLiteStore::update(const QVariant &item) {
	return true;
}

void CSQLiteStore::remove(const QVariant &item) {

}

bool CSQLiteStore::query(std::list<QVariant> &result, char *fmt, ...) {
	return true;
}

void CSQLiteStore::close() {

}
