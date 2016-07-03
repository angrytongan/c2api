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

c2api.o: /usr/include/stdlib.h /usr/include/Availability.h
c2api.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
c2api.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
c2api.o: /usr/include/sys/_symbol_aliasing.h
c2api.o: /usr/include/sys/_posix_availability.h /usr/include/machine/_types.h
c2api.o: /usr/include/i386/_types.h
c2api.o: /usr/include/sys/_pthread/_pthread_types.h /usr/include/sys/wait.h
c2api.o: /usr/include/sys/_types/_pid_t.h /usr/include/sys/_types/_id_t.h
c2api.o: /usr/include/sys/signal.h /usr/include/sys/appleapiopts.h
c2api.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
c2api.o: /usr/include/machine/_mcontext.h /usr/include/i386/_mcontext.h
c2api.o: /usr/include/mach/i386/_structs.h
c2api.o: /usr/include/sys/_pthread/_pthread_attr_t.h
c2api.o: /usr/include/sys/_types/_sigaltstack.h
c2api.o: /usr/include/sys/_types/_ucontext.h
c2api.o: /usr/include/sys/_types/_sigset_t.h
c2api.o: /usr/include/sys/_types/_size_t.h /usr/include/sys/_types/_uid_t.h
c2api.o: /usr/include/sys/resource.h /usr/include/stdint.h
c2api.o: /usr/include/sys/_types/_int8_t.h /usr/include/sys/_types/_int16_t.h
c2api.o: /usr/include/sys/_types/_int32_t.h
c2api.o: /usr/include/sys/_types/_int64_t.h /usr/include/_types/_uint8_t.h
c2api.o: /usr/include/_types/_uint16_t.h /usr/include/_types/_uint32_t.h
c2api.o: /usr/include/_types/_uint64_t.h /usr/include/sys/_types/_intptr_t.h
c2api.o: /usr/include/sys/_types/_uintptr_t.h /usr/include/_types/_intmax_t.h
c2api.o: /usr/include/_types/_uintmax_t.h /usr/include/sys/_types/_timeval.h
c2api.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
c2api.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
c2api.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
c2api.o: /usr/include/sys/_types/_ct_rune_t.h
c2api.o: /usr/include/sys/_types/_rune_t.h /usr/include/sys/_types/_wchar_t.h
c2api.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
c2api.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
c2api.o: /usr/include/sys/_types/_mode_t.h /usr/include/string.h
c2api.o: /usr/include/sys/_types/_rsize_t.h
c2api.o: /usr/include/sys/_types/_errno_t.h
c2api.o: /usr/include/sys/_types/_ssize_t.h /usr/include/strings.h
c2api.o: /usr/include/secure/_string.h /usr/include/secure/_common.h
c2api.o: /usr/include/sys/time.h /usr/include/sys/_types/_fd_def.h
c2api.o: /usr/include/sys/_types/_timespec.h
c2api.o: /usr/include/sys/_types/_time_t.h
c2api.o: /usr/include/sys/_types/_suseconds_t.h
c2api.o: /usr/include/sys/_types/_fd_setsize.h
c2api.o: /usr/include/sys/_types/_fd_set.h /usr/include/sys/_types/_fd_clr.h
c2api.o: /usr/include/sys/_types/_fd_isset.h
c2api.o: /usr/include/sys/_types/_fd_zero.h
c2api.o: /usr/include/sys/_types/_fd_copy.h /usr/include/time.h
c2api.o: /usr/include/sys/_types/_clock_t.h /usr/include/sys/_select.h
c2api.o: c2api.h c2api_priv.h
c2api.o: /Users/dfogarty/usr/local/include/hidapi/hidapi.h
c2api.o: /usr/include/wchar.h /usr/include/sys/_types/_mbstate_t.h
c2api.o: /usr/include/stdio.h /usr/include/sys/_types/_va_list.h
c2api.o: /usr/include/sys/stdio.h /usr/include/sys/_types/_off_t.h
c2api.o: /usr/include/secure/_stdio.h /usr/include/_wctype.h
c2api.o: /usr/include/sys/_types/_wint_t.h /usr/include/_types/_wctype_t.h
c2api.o: /usr/include/ctype.h /usr/include/runetype.h csafe.h csafe_util.h
c2api.o: debug.h utils.h
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
