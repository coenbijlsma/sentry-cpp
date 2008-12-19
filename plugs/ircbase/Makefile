all: clean
		@echo "Building plugin libircbase.so.1.0.0"
		g++ -g3 -I./include/ -I./plugs/ircbase/include/ -fPIC -g -c -Wall \
		./tools/StringTokenizer.cpp
		g++ -g3 -I./include/ -I./plugs/ircbase/include/ -fPIC -g -c -Wall \
		./log/Logger.cpp
		g++ -g3 -I./include/ -I./plugs/ircbase/include/ -fPIC -g -c -Wall \
		./config/SentryConfigSection.cpp ./config/SentryConfig.cpp
		g++ -g3 -I./include/ -I./plugs/ircbase/include/ -fPIC -g -c -Wall \
		./plugs/ircbase/EnqueueMessageCommand.cpp
		g++ -g3 -I./include/ -I./plugs/ircbase/include/ -fPIC -g -c -Wall \
		./plugs/ircbase/IRCSocket.cpp
		g++ -g3 -I./include/ -I./plugs/ircbase/include/ -fPIC -g -c -Wall \
		./plugs/ircbase/IRCBase.cpp
		g++ -g3 -I./include/ -I./plugs/ircbase/include/ -fPIC -g -c -Wall \
		./plugs/ircbase/BufferedSocketReader.cpp
		g++ -g3 -shared -Wl,-soname,libircbase.so.1 -o ./libircbase.so.1.0.0 \
		./SentryConfigSection.o ./SentryConfig.o ./EnqueueMessageCommand.o \
		IRCSocket.o ./IRCBase.o ./Logger.o ./StringTokenizer.o \
		./BufferedSocketReader.o -lc -lpthread
		@mv ./libircbase.so.1.0.0 ./plugs/

clean:
		@if [ -f ./IRCBase.o ]; then rm ./*.o; fi
		@if [ -f ./plugs/libircbase.so.1.0.0 ]; then rm ./plugs/libircbase.so.1.0.0; fi

.PHONY: all clean