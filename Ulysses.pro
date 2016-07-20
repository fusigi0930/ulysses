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
    resultcore \
    dhcp_ip_viewer

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
    luacore \
    resultcore

dhcp_ip_viewer.depends = \
    engine \
    netio \
    xmlrun \
    xmlcfg \
    comio \
    luacore \
    resultcore
