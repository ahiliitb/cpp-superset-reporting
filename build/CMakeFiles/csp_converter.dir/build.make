# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/build

# Include any dependencies generated for this target.
include CMakeFiles/csp_converter.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/csp_converter.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/csp_converter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/csp_converter.dir/flags.make

CMakeFiles/csp_converter.dir/src/csp_converter.cpp.o: CMakeFiles/csp_converter.dir/flags.make
CMakeFiles/csp_converter.dir/src/csp_converter.cpp.o: /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/src/csp_converter.cpp
CMakeFiles/csp_converter.dir/src/csp_converter.cpp.o: CMakeFiles/csp_converter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/csp_converter.dir/src/csp_converter.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/csp_converter.dir/src/csp_converter.cpp.o -MF CMakeFiles/csp_converter.dir/src/csp_converter.cpp.o.d -o CMakeFiles/csp_converter.dir/src/csp_converter.cpp.o -c /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/src/csp_converter.cpp

CMakeFiles/csp_converter.dir/src/csp_converter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/csp_converter.dir/src/csp_converter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/src/csp_converter.cpp > CMakeFiles/csp_converter.dir/src/csp_converter.cpp.i

CMakeFiles/csp_converter.dir/src/csp_converter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/csp_converter.dir/src/csp_converter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/src/csp_converter.cpp -o CMakeFiles/csp_converter.dir/src/csp_converter.cpp.s

# Object files for target csp_converter
csp_converter_OBJECTS = \
"CMakeFiles/csp_converter.dir/src/csp_converter.cpp.o"

# External object files for target csp_converter
csp_converter_EXTERNAL_OBJECTS =

csp_converter: CMakeFiles/csp_converter.dir/src/csp_converter.cpp.o
csp_converter: CMakeFiles/csp_converter.dir/build.make
csp_converter: CMakeFiles/csp_converter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable csp_converter"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/csp_converter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/csp_converter.dir/build: csp_converter
.PHONY : CMakeFiles/csp_converter.dir/build

CMakeFiles/csp_converter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/csp_converter.dir/cmake_clean.cmake
.PHONY : CMakeFiles/csp_converter.dir/clean

CMakeFiles/csp_converter.dir/depend:
	cd /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/build /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/build /home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/build/CMakeFiles/csp_converter.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/csp_converter.dir/depend
