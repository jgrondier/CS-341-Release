# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jgrondier/Prog/CS-341/Project/icg17

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jgrondier/Prog/CS-341/Project/icg17

# Include any dependencies generated for this target.
include hw2_trackball/CMakeFiles/hw2_trackball.dir/depend.make

# Include the progress variables for this target.
include hw2_trackball/CMakeFiles/hw2_trackball.dir/progress.make

# Include the compile flags for this target's objects.
include hw2_trackball/CMakeFiles/hw2_trackball.dir/flags.make

hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o: hw2_trackball/CMakeFiles/hw2_trackball.dir/flags.make
hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o: hw2_trackball/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgrondier/Prog/CS-341/Project/icg17/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o"
	cd /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hw2_trackball.dir/main.cpp.o -c /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball/main.cpp

hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hw2_trackball.dir/main.cpp.i"
	cd /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball/main.cpp > CMakeFiles/hw2_trackball.dir/main.cpp.i

hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hw2_trackball.dir/main.cpp.s"
	cd /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball/main.cpp -o CMakeFiles/hw2_trackball.dir/main.cpp.s

hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o.requires:

.PHONY : hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o.requires

hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o.provides: hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o.requires
	$(MAKE) -f hw2_trackball/CMakeFiles/hw2_trackball.dir/build.make hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o.provides.build
.PHONY : hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o.provides

hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o.provides.build: hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o


# Object files for target hw2_trackball
hw2_trackball_OBJECTS = \
"CMakeFiles/hw2_trackball.dir/main.cpp.o"

# External object files for target hw2_trackball
hw2_trackball_EXTERNAL_OBJECTS =

hw2_trackball/hw2_trackball: hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o
hw2_trackball/hw2_trackball: hw2_trackball/CMakeFiles/hw2_trackball.dir/build.make
hw2_trackball/hw2_trackball: /usr/lib64/libGLU.so
hw2_trackball/hw2_trackball: /usr/lib64/libGL.so
hw2_trackball/hw2_trackball: /usr/lib64/libglfw.so
hw2_trackball/hw2_trackball: /usr/lib64/libGLEW.so
hw2_trackball/hw2_trackball: hw2_trackball/CMakeFiles/hw2_trackball.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jgrondier/Prog/CS-341/Project/icg17/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hw2_trackball"
	cd /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw2_trackball.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
hw2_trackball/CMakeFiles/hw2_trackball.dir/build: hw2_trackball/hw2_trackball

.PHONY : hw2_trackball/CMakeFiles/hw2_trackball.dir/build

hw2_trackball/CMakeFiles/hw2_trackball.dir/requires: hw2_trackball/CMakeFiles/hw2_trackball.dir/main.cpp.o.requires

.PHONY : hw2_trackball/CMakeFiles/hw2_trackball.dir/requires

hw2_trackball/CMakeFiles/hw2_trackball.dir/clean:
	cd /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball && $(CMAKE_COMMAND) -P CMakeFiles/hw2_trackball.dir/cmake_clean.cmake
.PHONY : hw2_trackball/CMakeFiles/hw2_trackball.dir/clean

hw2_trackball/CMakeFiles/hw2_trackball.dir/depend:
	cd /home/jgrondier/Prog/CS-341/Project/icg17 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jgrondier/Prog/CS-341/Project/icg17 /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball /home/jgrondier/Prog/CS-341/Project/icg17 /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball /home/jgrondier/Prog/CS-341/Project/icg17/hw2_trackball/CMakeFiles/hw2_trackball.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hw2_trackball/CMakeFiles/hw2_trackball.dir/depend
