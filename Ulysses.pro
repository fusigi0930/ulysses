TEMPLATE = subdirs

SUBDIRS += \
    engine \
    netio

engine.depends = netio
