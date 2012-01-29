NAME = watchdog
OBJFILES=$(NAME).o logger.o regexApi.o main.o callback.o iniParser.o utility.o

WORKDIR = $(shell pwd)

CXX = g++
CFLAGS = -Wall

INC = -I$(WORKDIR)
LIBDIR = -L$(WORKDIR)

%.o : %.cpp
        $(CXX) $(CFLAGS) $(INC) $(LIBDIR) -c $<

all: $(NAME)

$(NAME): $(OBJFILES)
        $(CXX) $(CFLAGS) $(INC) $(LIBDIR) $(OBJFILES) -o $@
