all:
		@echo "Building plugin libircbase.so.1.0.0"
		g++ -I./include/ -fPIC -g -c -Wall IRCBase.cpp
		g++ -shared -Wl,-soname,libircbase.so.1 -o libircbase.so.1.0.0 IRCBase.o -lc
		@if [ -f ../libircbase.so.1.0.0 ]; then rm ../libircbase.so.1.0.0; fi
		@mv libircbase.so.1.0.0 ..

.PHONY: all