all:
	g++ -g -I./include -I./thirdparty/pugixml-1.14/src \
	src/main.cc src/log_parser.cc src/base_log_parser.cc src/database.cc \
	thirdparty/pugixml-1.14/src/pugixml.cpp \
	-o log_parser -lpqxx -lpq -lz