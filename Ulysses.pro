TEMPLATE = subdirs

SUBDIRS += \
    engine \
    netio \
	xmlrun \
	unittest \
    comio

engine.depends = netio \
				 xmlrun

unittest.depends = engine
