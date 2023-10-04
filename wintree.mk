objects = wintree.o tree.o queue.o
CC = gcc
CFLAGS += -c -Wall -g 

wintree.exe: $(objects) treeres.res 
	$(CC) $^ -o $@ -mwindows         
%.o:%.c                              
	$(CC) $^ $(CFLAGS) -o $@         
treeres.res:treeres.rc               
	windres -o $@ -O coff $^

clean:
	rm -f *.o wintree.exe treeres.res 	


#### $(objects)=wintree.o tree.o queue.o
#### $(CC) = gcc
#### $@ = all the object files  
#### $^ = all the files prerequisited
#### $< = the first prerequisit file 
