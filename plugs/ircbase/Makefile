EXTERNALFILES= $(shell cat ircbase.files )
FILES= $(shell find ./plugs/ircbase/ -maxdepth 1 -type f -name "*.cpp" )
PLUGSDIR=./plugs
SONAME=libircbase.so.1
LIBNAME= libircbase.so.1.0.0
CPP= g++
CPPFLAGS= -g3 -I./include -I./plugs/ircbase/include/ -fPIC -g -c -Wall
LFLAGS= -g3 -shared -Wl,-soname,$(SONAME) -o ./$(LIBNAME)
LIBTHREAD= /usr/lib/libthread.so.1.0.0
LIBTHREADURL= http://svn.coenbijlsma.nl/libthread/

all: libthreadexists $(EXTERNALFILES) $(FILES) objects
		@mv ./$(LIBNAME) $(PLUGSDIR)/

$(EXTERNALFILES):
		$(CPP) $(CPPFLAGS) $@

$(FILES):
		$(CPP) $(CPPFLAGS) $@

objects:
		$(CPP) $(LFLAGS) $(shell ls *.o) -lc -lthread

clean:
		@if [ -f ./IRCBase.o ]; then rm ./*.o; fi

distclean: clean
		@if [ -f ./$(PLUGSDIR)/$(LIBNAME) ]; then rm ./$(PLUGSDIR)/$(LIBNAME); fi

libthreadexists:
		@if [ -f $(LIBTHREAD) ]; then \
			echo "libthread exists, continuing..."; \
		else \
			echo "Cannot continue because libthread is missing."; \
			echo "You can obtain it from " $(LIBTHREADURL); \
			exit 1; \
		fi

.PHONY: all clean distclean $(EXTERNALFILES) $(FILES) objects