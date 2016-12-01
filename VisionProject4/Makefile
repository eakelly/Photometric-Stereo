########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
C++FLAG = -g -std=c++11

MATH_LIBS = -lm

EXEC_DIR=.


.cpp.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 



#First Program (ListTest)
Cpp_OBJ1=Image.o s1.o
PROGRAM_1=s1
$(PROGRAM_1): $(Cpp_OBJ1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ1) $(INCLUDES) $(LIBS_ALL)

#Second Program (ListTest)
Cpp_OBJ2=Image.o s2.o
PROGRAM_2=s2
$(PROGRAM_2): $(Cpp_OBJ2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ2) $(INCLUDES) $(LIBS_ALL)

#Third Program (ListTest)
Cpp_OBJ3=Image.o s3.o
PROGRAM_3=s3
$(PROGRAM_3): $(Cpp_OBJ3)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ3) $(INCLUDES) $(LIBS_ALL)

#Fourth Program (ListTest)
Cpp_OBJ4=Image.o s4.o
PROGRAM_4=s4
$(PROGRAM_4): $(Cpp_OBJ4)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ4) $(INCLUDES) $(LIBS_ALL)

all: 
	make $(PROGRAM_1)
	make $(PROGRAM_2)
	make $(PROGRAM_3) 
	make $(PROGRAM_4)  
 
  


clean:
	(rm -f *.o;)

(:
