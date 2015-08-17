TEMPLATE = subdirs

SUBDIRS += \
    engine \
    netio \
	xmlrun \
	unittest \
    comio

engine.depends = \
	netio \
	xmlrun \
	comio

unittest.depends = \
	engine \
	netio \
	xmlrun \
	comio
