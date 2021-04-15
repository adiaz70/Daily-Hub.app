# Makefile
# MS: 3/13/21 - initial code
# MS: 3/19/21 - added comments for clarity
# MS: 4/14/21 - linked SQLite library

wxComp = `wx-config --cxxflags`
wxLink = `wx-config --libs`

libs = sqlite3

obj_dir = obj
src_path = ./src/
obj_path := ./$(obj_dir)/

# The variable 'src' is evaluated to contain all .cpp files in the source code directory.
src := $(wildcard $(src_path)*.cpp)

# The variable 'obj' is evaluated to contain all the .o (object) files that are compiled from the source code.
# This is done by swapping the extension from .cpp to .o, which looks at one of the rules below to
# properly compile it with the wxWidgets flags.
obj := $(patsubst $(src_path)%.cpp,$(obj_path)%.o,$(src))

# The primary rule links all of the object files together and creates the executable.
DailyHub.out: $(obj)
	$(CXX) $^ $(wxLink) -l$(libs) -o $@

# This rule provides the template for compiling a single .o file from a .cpp file.
# If the directory for object files doesn't already exist, it makes one.
$(obj_path)%.o: $(src_path)%.cpp | $(obj_dir)
	$(CXX) -c $< $(wxComp) -o $@

$(obj_dir):
	mkdir $(obj_dir)

.PHONY: clean clean_all

clean:
	rm $(obj_path)*.o
	rmdir $(obj_dir)

clean_all: clean
	rm *.out
