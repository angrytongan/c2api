CC=gcc

CFLAGS=-Wall -pedantic -g -D_THREAD_SAFE -pthread -DDEBUG
LDFLAGS=-L$(HIDAPI_LIBDIR) -lhidapi

C2API_SOURCES=c2api.cpp csafe.cpp csafe_util.cpp debug.cpp utils.cpp

C2API_OBJS=$(C2API_SOURCES:.cpp=.o)

.cpp.o:
	$(CC) -I$(HIDAPI_INCDIR) $(CFLAGS) -c $< -o $@

c2: $(C2API_OBJS) c2.o
	$(CC) -o $@ $^ $(LDFLAGS)

all: c2

unittests: ut_utils ut_csafe

ut_utils: utils.cpp
	$(CC) $(CFLAGS) -o $@ -DUNITTEST_UTILS utils.cpp

ut_csafe: csafe.cpp csafe.h debug.cpp debug.h utils.cpp utils.h
	$(CC) $(CFLAGS) -o $@ -DUNITTEST_CSAFE csafe.cpp debug.cpp utils.cpp

dep:
	makedepend -I$(HIDAPI_INCDIR) $(C2API_SOURCES)

# preprocess macros out of csafe.cpp
pre: csafe.cpp
	$(CC) -E -I$(HIDAPI_INCDIR) $(CFLAGS) -E csafe.cpp -o csafe-pre.cpp

clean: dep
	/bin/rm -rf *.dSYM
	/bin/rm -f c2
	/bin/rm -f ut_*
	/bin/rm -f *.o
	/bin/rm -f Makefile.bak
	/bin/rm -f csafe-pre.cpp
# DO NOT DELETE

csafe.o: /usr/include/string.h /usr/include/_types.h
csafe.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
csafe.o: /usr/include/sys/_symbol_aliasing.h
csafe.o: /usr/include/sys/_posix_availability.h /usr/include/machine/_types.h
csafe.o: /usr/include/i386/_types.h
csafe.o: /usr/include/sys/_pthread/_pthread_types.h
csafe.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
csafe.o: /usr/include/sys/_types/_size_t.h /usr/include/sys/_types/_null.h
csafe.o: /usr/include/sys/_types/_rsize_t.h
csafe.o: /usr/include/sys/_types/_errno_t.h
csafe.o: /usr/include/sys/_types/_ssize_t.h /usr/include/strings.h
csafe.o: /usr/include/secure/_string.h /usr/include/secure/_common.h csafe.h
csafe.o: debug.h utils.h
csafe_util.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
csafe_util.o: /usr/include/sys/_symbol_aliasing.h
csafe_util.o: /usr/include/sys/_posix_availability.h
csafe_util.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
csafe_util.o: /usr/include/_types.h /usr/include/sys/_types.h
csafe_util.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
csafe_util.o: /usr/include/sys/_pthread/_pthread_types.h
csafe_util.o: /usr/include/sys/_types/_va_list.h
csafe_util.o: /usr/include/sys/_types/_size_t.h
csafe_util.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
csafe_util.o: /usr/include/sys/_types/_off_t.h
csafe_util.o: /usr/include/sys/_types/_ssize_t.h /usr/include/secure/_stdio.h
csafe_util.o: /usr/include/secure/_common.h /usr/include/string.h
csafe_util.o: /usr/include/sys/_types/_rsize_t.h
csafe_util.o: /usr/include/sys/_types/_errno_t.h /usr/include/strings.h
csafe_util.o: /usr/include/secure/_string.h csafe.h utils.h
utils.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
utils.o: /usr/include/sys/_symbol_aliasing.h
utils.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
utils.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
utils.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
utils.o: /usr/include/i386/_types.h
utils.o: /usr/include/sys/_pthread/_pthread_types.h
utils.o: /usr/include/sys/_types/_va_list.h /usr/include/sys/_types/_size_t.h
utils.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
utils.o: /usr/include/sys/_types/_off_t.h /usr/include/sys/_types/_ssize_t.h
utils.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
