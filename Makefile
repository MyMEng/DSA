CC       = gcc
CC_FLAGS = -Wall -m64 -std=gnu99 -march=core2 -O3 #-ggdb3
CC_PATHS = 
CC_LIBS  = 

all : matrix  

matrix : $(wildcard matrix.[ch] util.[ch] file_load.[ch])
	@${CC} ${CC_FLAGS} ${CC_PATHS} -o ${@} $(filter %.c, ${^}) ${CC_LIBS}

clean : 
	@rm -f core matrix  
