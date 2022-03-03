TEMPLATE = subdirs


CONFIG += ordered

SUBDIRS +=  \
    CopyManager \
    FileCopierUI


FileCopierUI.depends = CopyManager

OTHER_FILES = Common.pri
