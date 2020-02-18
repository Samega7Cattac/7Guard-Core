OUTPUT_FILE = 7Guard
OBJS = main.o otp_s7c.o
CXXFLAGS = -mrdrnd -pthread

$(info Identifing OS...)
ifdef OS
$(info $(shell echo %OS%))
RM = del /q
else
ifeq ($(shell uname -o), GNU/Linux)
$(info $(shell uname -o))
RM = rm -f
else
$(error OS not supported.)
endif
endif

all: $(OBJS)
	$(CXX) -o $(OUTPUT_FILE) $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

debug: CXXFLAGS += -DDEBUG -g -fsanitize=address
debug: all

docs:
	doxygen Doxyfile

clean:
	$(RM) ./*.o ./build/$(OUTPUT_FILE)
