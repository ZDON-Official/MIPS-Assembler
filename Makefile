EXECS = assemble
OBJS = Main.cpp assembler.cpp
#### use next two lines for Mac
#CC = g++
#CCFLAGS = -std=c++14 -stdlib=libc++ -g

#### use next two lines for mathcs* machines:
CC = g++
CCFLAGS = -std=c++17 -Wall -Wno-deprecated -Werror=return-type -g

all: $(EXECS)

assemble: $(OBJS)  
	$(CC) $(CCFLAGS) $^ -o $@

%.o: %.cpp *.h
	$(CC) $(CCFLAGS) -I $(BOOST_ROOT) -c $<

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	/bin/rm -f a.out $(OBJS) $(EXECS)
