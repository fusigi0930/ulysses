TEMPLATE = subdirs

SUBDIRS += \
    engine \
    netio \
	xmlrun \
	unittest

engine.depends = netio \
				 xmlrun

unittest.depends = engine
