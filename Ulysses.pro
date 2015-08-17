TEMPLATE = subdirs

SUBDIRS += \
    engine \
    netio \
	xmlrun \
	unittest \
    comio \
    factory_engine

engine.depends = \
	netio \
	xmlrun \
	comio

unittest.depends = \
	engine \
	netio \
	xmlrun \
	comio

factory_engine.depends = \
	engine \
	netio \
	xmlrun \
	comio
