
build:
	g++ -I./include/exceptions -I./include/plugs -combine ./exceptions/NoSuchLibraryException.cpp ./exceptions/NoSuchSymbolException.cpp ./plugs/PluginLoader.cpp ./main.cpp -ldl
