TARGET=main.o
SOURCES=main.cpp SetOfStrings.cpp StringRelation.cpp
CXX=g++

default: $(TARGET)
	@echo " Compiling..."; $(CXX) -o $(TARGET) $(SOURCES)
	
debug:
	@echo " Compiling..."; $(CXX) -D_DEBUG -o $(TARGET) $(SOURCES)
	
clean:
	@echo " Cleaning..."; $(RM) -rf $(TARGET)
 
