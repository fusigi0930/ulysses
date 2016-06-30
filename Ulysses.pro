TEMPLATE = subdirs

SUBDIRS += \
    engine \
    netio \
	xmlrun \
	unittest \
    comio \
    factory_engine \
    xmlcfg \
    luacore \
    resultcore

engine.depends = \
	netio \
	xmlrun \
	comio \
    xmlcfg \
    luacore \
    resultcore

unittest.depends = \
	engine \
	netio \
	xmlrun \
	comio \
    xmlcfg \
    luacore \
    resultcore

factory_engine.depends = \
	engine \
	netio \
	xmlrun \
    xmlcfg \
    comio \
    resultcore
