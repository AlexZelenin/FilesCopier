TEMPLATE = subdirs


CONFIG += ordered

SUBDIRS +=  \
    CopyManager \
    FileCopierUI \
    Tests/TestReadSourceDir


FileCopierUI.depends = CopyManager

OTHER_FILES = Common.pri
