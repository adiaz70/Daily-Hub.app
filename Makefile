# Makefile
# MS: 3/13/21 - initial code

wxComp = `wx-config --cxxflags`
wxLink = `wx-config --libs`

obj_dir = obj
src_path = ./src/
obj_path := ./$(obj_dir)/
src := $(wildcard $(src_path)*.cpp)
obj := $(patsubst $(src_path)%.cpp,$(obj_path)%.o,$(src))

DailyHub.out: $(obj)
	$(CXX) $^ $(wxLink) -o $@

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
