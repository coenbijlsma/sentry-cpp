PLUGINS=$(shell find ./plugs/ -mindepth 1 -maxdepth 1 -type d -name '[a-zA-Z0-9]*' )

help:
		@echo "Following options are available:"
		@echo "- all		Builds the sentry core and the available plug-ins;"
		@echo "- core		Builds the sentry core;"
		@echo "- plugins	Builds the available plug-ins;"
		@echo "- testplugin	Builds the test plug-in;"
		@echo "- ircbase	Builds the irc-base plug-in;"
		@echo "- clean		Cleans up the mess;"
		@echo "- help		Shows this message."
		@echo
		
all: core plugins

core:
		@echo "Building Sentry core"
		g++ -g3 -I./include/ -combine ./config/SentryConfigSection.cpp \
			./config/SentryConfig.cpp ./exceptions/NoSuchLibraryException.cpp \
			./exceptions/NoSuchSymbolException.cpp ./exceptions/ConfigReloadException.cpp \
			./log/Logger.cpp ./plugs/PluginLoader.cpp ./tools/StringTokenizer.cpp \
			SentryHookPoint.cpp Sentry.cpp main.cpp -ldl -o sentry

testplugin:
		@echo "Building plugin libtestplugin.so.1.0.0"
		g++ -I./include/ -fPIC -g -c -Wall ./plugs/TestPlugin.cpp
		g++ -shared -Wl,-soname,libtestplugin.so.1 -o libtestplugin.so.1.0.0 TestPlugin.o -lc
		@if [ -f ./plugs/libtestplugin.so.1.0.0 ]; then rm ./plugs/libtestplugin.so.1.0.0; fi
		@if [ -d ./plugs ]; then mv libtestplugin.so.1.0.0 ./plugs; fi

$(PLUGINS):
		@make -f $@/Makefile

plugins: $(PLUGINS)

clean: 
		@if [ -f ./TestPlugin.o ]; then rm ./TestPlugin.o; fi
		@if [ -f ./plugs/libtestplugin.so.1.0.0 ]; then rm ./plugs/libtestplugin.so.1.0.0; fi

distclean: clean
		@if [ -f ./sentry ]; then rm ./sentry; fi

.PHONY: help all core plugins testplugin clean distclean $(PLUGINS)
