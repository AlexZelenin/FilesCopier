ROOT_DIR = $$PWD


DESTDIR = $$ROOT_DIR/../FCBin

INCLUDEPATH += $$ROOT_DIR/CopyManager
DEPENDPATH += $$ROOT_DIR/CopyManager

copydata.commands = $(COPY_DIR) $$PWD/Tests/testdirs $$ROOT_DIR/../FCBin
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
