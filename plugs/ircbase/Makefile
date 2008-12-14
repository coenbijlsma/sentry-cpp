all: clean
		@echo "Building plugin libircbase.so.1.0.0"
		g++ -I./include/ -I./plugs/ircbase/include/ -fPIC -g -c -Wall ./plugs/ircbase/IRCBase.cpp
		g++ -shared -Wl,-soname,libircbase.so.1 -o ./libircbase.so.1.0.0 ./IRCBase.o -lc
		@mv ./libircbase.so.1.0.0 ./plugs/

clean:
		@if [ -f ./IRCBase.o ]; then rm ./IRCBase.o; fi
		@if [ -f ./plugs/libircbase.so.1.0.0 ]; then rm ./plugs/libircbase.so.1.0.0; fi

.PHONY: all clean