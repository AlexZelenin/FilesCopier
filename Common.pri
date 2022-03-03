ROOT_DIR = $$PWD


DESTDIR = $$ROOT_DIR/../FCBin



INCLUDEPATH += $$ROOT_DIR/CopyManager
DEPENDPATH += $$ROOT_DIR/CopyManager


copydata.commands = $(COPY_DIR) $$shell_path($${PWD}/Tests/testdirs) $$shell_path($${DESTDIR}/testdirs)
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
