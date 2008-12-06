
all:
		g++ -I./include/ -combine ./config/SentryConfigSection.cpp ./config/SentryConfig.cpp ./exceptions/NoSuchLibraryException.cpp ./exceptions/NoSuchSymbolException.cpp \
			./log/Logger.cpp ./plugs/PluginLoader.cpp ./tools/StringTokenizer.cpp SentryHookPoint.cpp Sentry.cpp main.cpp -ldl -o sentry

testplugin:
		g++ -I./include/ -fPIC -g -c -Wall ./plugs/TestPlugin.cpp
		g++ -shared -Wl,-soname,libtestplugin.so.1 -o libtestplugin.so.1.0.0 TestPlugin.o -lc

clean:
		@if [ -f ./TestPlugin.o ]; then rm ./TestPlugin.o; fi
		@if [ -f ./sentry ]; then rm ./sentry; fi
		@if [ -f libtestplugin.so.1.0.0 ]; then rm libtestplugin.so.1.0.0; fi

.PHONY: all testplugin clean
