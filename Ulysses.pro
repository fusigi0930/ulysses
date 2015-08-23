TEMPLATE = subdirs

SUBDIRS += \
    engine \
    netio \
	xmlrun \
	unittest \
    comio \
    factory_engine \
    xmlcfg

engine.depends = \
	netio \
	xmlrun \
	comio \
	xmlcfg

unittest.depends = \
	engine \
	netio \
	xmlrun \
	comio \
	xmlcfg

factory_engine.depends = \
	engine \
	netio \
	xmlrun \
	comio
