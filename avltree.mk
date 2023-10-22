cmdobjects = avltree.o tree.o queue.o
winobjects = winavltree.o drawtree.o tree.o queue.o
CC = gcc.exe
CFLAGS += -c -Wall -g 

avltree: $(cmdobjects) 
	$(CC) $^ -o $@

winavltree: $(winobjects) treeres.res 
	$(CC) $^ -o $@ -mwindows         

%.o:%.c                              
	$(CC) $^ $(CFLAGS) -o $@         

treeres.res:treeres.rc               
	windres -o $@ -O coff $^

clean:
	rm -f *.o *.exe treeres.res