CXX=g++
CXX_FLAGS=-g -O2 -std=c++11 -Wall -Wno-format-zero-length -fPIC -DDISABLE_PACKET_COUNTER_HACK 

INCLUDE_DIRS=/usr/include /opt/cactus/include/   /opt/cactus/include/uhal/uhal/include/  /opt/cactus/include/uhal/grammars/include/ /opt/cactus/include/uhal/log/include/
LIB_DIRS=/usr/lib64 /opt/cactus/lib   
LIBS=cactus_uhal_uhal boost_filesystem cactus_uhal_grammars cactus_uhal_log cactus_uhal_uhal boost_unit_test_framework boost_system boost_thread boost_chrono boost_regex boost_program_options boost_timer c_nonshared
#pugixml 

INCLUDE_FLAGS=$(foreach d, $(INCLUDE_DIRS), -I$d) 
LD_FLAGS=$(foreach d, $(LIB_DIRS), -L$d) $(foreach f, $(LIBS), -l$f)


all : ttc mgt reg pc

mgt : mgttool.exe
mgttool.exe : mgtargs.o mgtroutines.o

ttc : ttctool.exe
ttctool.exe : ttcargs.o ttcroutines.o

reg : regtool.exe
regtool.exe : regargs.o

pc : print_capture.exe print_capture_long.exe

%.o : %.cc
	$(CXX) $(CXX_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

%.exe : %.cc
	$(CXX) $(CXX_FLAGS) $(INCLUDE_FLAGS)  $^ -o $@ $(LD_FLAGS)

clean :
	rm *.o *.exe
