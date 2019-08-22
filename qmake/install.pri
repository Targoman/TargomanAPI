!contains(CONFIG, no_install) {
    INCLUDE_PREFIX = $$[QT_INSTALL_HEADERS]/
    LIB_PREFIX     = $$[QT_INSTALL_LIBS]
    EXAMPLES_PREFIX= $$[QT_INSTALL_EXAMPLES]

    unix:!isEmpty(PREFIX){
            INCLUDE_PREFIX = $$PREFIX/include/
            contains(QT_ARCH, x86_64){
                LIB_PREFIX     = $$PREFIX/lib64
            } else {
                LIB_PREFIX     = $$PREFIX/lib
            }
    }

    PRJDIR = $${dirname(PWD)}
    for(header, DIST_HEADERS) {
      relPath = $${relative_path($$header, $$PRJDIR)}
      path = $${INCLUDE_PREFIX}/$${dirname(relPath)}
      eval(headers_$${path}.files += $$relPath)
      eval(headers_$${path}.path = $$path)
      eval(INSTALLS *= headers_$${path})
    }

    target = $$TARGET
    target.path = $$LIB_PREFIX

    INSTALLS += target
}





