BUILT_SOURCE = $(srcdir)/swig_wrap.cpp
SWIG_SOURCES = swig.i

pkgpython_PYTHON = pynetmod.py
pkgpyexec_LTLIBRARIES = _pynetmod.la
_pynetmod_la_SOURCES = $(srcdir)/swig_wrap.cpp $(SWIG_SOURCES)
_pynetmod_la_CPPFLAGS = $(SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir)/src
_pynetmod_la_LDFLAGS = -module
_pynetmod_la_LIBADD = ../src/libnetmod.la

$(srcdir)/swig_wrap.cpp : $(SWIG_SOURCES)
	$(SWIG) $(SWIG_PYTHON_OPT) -I$(top_srcdir)/src/ -o $@ $<
