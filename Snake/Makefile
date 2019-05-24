CXXFLAGS += -g -Wall
PROG = snake
OBJ = control.o game.o main.o tui.o view.o
HEAD = control.h game.h keypressable.h tui.h view.h

all: $(PROG)

$(PROG): $(OBJ)
	$(CXX) $^ -o $@

clean:
	$(RM) *.o $(PROG)