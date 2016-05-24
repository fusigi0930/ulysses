TEMPLATE = subdirs

SUBDIRS += \
    engine \
    netio \
	xmlrun \
	unittest \
    comio \
    factory_engine \
    xmlcfg \
    luacore

engine.depends = \
	netio \
	xmlrun \
	comio \
        xmlcfg \
        luacore

unittest.depends = \
	engine \
	netio \
	xmlrun \
	comio \
        xmlcfg \
        luacore

factory_engine.depends = \
	engine \
	netio \
	xmlrun \
	comio
