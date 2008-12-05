
all:
	g++ -I./include/ -combine ./config/SentryConfigSection.cpp ./config/SentryConfig.cpp ./exceptions/NoSuchLibraryException.cpp ./exceptions/NoSuchSymbolException.cpp \
		./log/Logger.cpp ./plugs/PluginLoader.cpp ./plugs/TestPlugin.cpp ./tools/StringTokenizer.cpp SentryHookPoint.cpp Sentry.cpp main.cpp -ldl -o sentry
