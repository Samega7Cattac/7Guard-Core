OUTPUT_FILE = 7Guard
HEADER_FILES = otp_s7c.hpp
OBJ = main.o otp_s7c.o
CXXFLAGS = -mrdrnd -pthread

$(info Identifing OS...)
ifdef OS
$(info $(shell echo %OS%))
RM = del /q
FixPath = $(subst /,\,$1)
else
ifeq ($(shell uname -o), GNU/Linux)
$(info $(shell uname -o))
RM = rm -f
FixPath = $1
else
$(error OS not supported.)
endif
endif

all: $(OBJ)
	if [ ! -d "./build" ]; then mkdir build; fi
	$(CXX) -o $(call FixPath, ./build/$(OUTPUT_FILE)) $^ $(CXXFLAGS)

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

debug: CXXFLAGS += -DDEBUG -g
debug: all

docs:
	doxygen Doxyfile

clean:
	$(RM) $(call FixPath, ./build/$(OUTPUT_FILE))
	$(RM) $(call FixPath, ./*.o)
