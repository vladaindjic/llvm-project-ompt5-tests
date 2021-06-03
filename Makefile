CC-FLAGS= -g -fopenmp
# compiler
CC=gcc $(CC-FLAGS)

# llvm openmp 5 runtime installation directory
OLIB=/home/vi3/llvm-build/gcc-8.3.1/install/lib
OMPT_LIBS=-L$(OLIB) -Wl,-rpath=$(OLIB) -lomp

all: serialized-regions-in-task-compile serialized-regions-in-task-run

serialized-regions-in-task-compile:
	$(CC) -o serialized-regions-in-task serialized-regions-in-task.c $(OMPT_LIBS)

serialized-regions-in-task-run:
	./serialized-regions-in-task

clean:
	rm serialized-regions-in-task
