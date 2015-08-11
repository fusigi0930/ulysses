TEMPLATE = subdirs

SUBDIRS += \
    engine \
    netio \
    xmlrun

engine.depends = netio
