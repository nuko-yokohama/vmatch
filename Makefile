# contrib/ksj/Makefile

MODULE_big = vmatch
OBJS = normalize.o similar.o vmatch.o

EXTENSION = vmatch
DATA = vmatch--1.0.sql 

REGRESS = vmatch

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/vmatch
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
