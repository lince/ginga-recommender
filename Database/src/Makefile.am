
# set the include path found by configure
INCLUDES = $(all_includes)

LIBS =

libgingarecommenderdatabasedir = /usr/local/lib/ginga

libgingarecommenderdatabase_so_LDADD = -lgingarecommenderutil -lgingarecommendersystemresource -lpthread

libgingarecommenderdatabase_so_LDFLAGS = -Wl,-soname,libgingarecommenderdatabase.so \
    --shared -ldl -I/usr/local/include/ginga/

libgingarecommenderdatabase_PROGRAMS = libgingarecommenderdatabase.so
libgingarecommenderdatabase_so_SOURCES = Database.cpp sqlite3.c
