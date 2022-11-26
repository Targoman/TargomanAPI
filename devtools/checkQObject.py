# check_code v1.0
# run from root of repository
from subprocess import check_output

_qt_autogen_macro_list = (\
'signals:', \
'Q_CLASSINFO', \
'Q_PLUGIN_METADATA', \
'Q_INTERFACES', \
'Q_PROPERTY', \
'Q_PRIVATE_PROPERTY', \
'Q_REVISION', \
'Q_OVERRIDE', \
'Q_ENUM', \
'Q_FLAGS', \
'Q_SCRIPTABLE', \
'Q_INVOKABLE', \
'Q_SIGNAL', \
'Q_SLOT' \
)
_qt_autogen_macro_regexp = '({0})'.format('|'.join(_qt_autogen_macro_list))

def _FnamesToBaseFileNames(fnames_list):
    result = []
    for fn in fnames_list:
        start = fn.rfind('/')
        end = fn.rfind('.')
        if end > start:
            result.append(fn[start + 1 : end])
    return result

def CheckQObject():
    def get_files(pattern):
        files = check_output('git grep -E "{0}"'.format(pattern)).split('\n')
        files = [s[:s.find(':')] for s in files]
        return set([s for s in files if len(s) > 0])
    qobject_files = _FnamesToBaseFileNames(get_files('Q_OBJECT'))
    macro_files = _FnamesToBaseFileNames(get_files(_qt_autogen_macro_regexp))
    include_moc_files = _FnamesToBaseFileNames(get_files('#include \\"moc_.*?\.cpp\\"'))
    for fn in qobject_files:
        if fn not in include_moc_files:
            print 'looks like {0} misses include for moc_* file'.format(fn)
        if fn not in macro_files:
            print 'probably {0} has redundant Q_OBJECT macro'.format(fn)

CheckQObject()
