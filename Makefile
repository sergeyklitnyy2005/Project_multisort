CC = gcc
CFLAGS = -Wall -pthread -O2

OBJS = main.o file_sorter.o chunk_reader.o thread_safe_queue.o sort_worker.o sorted_chunk_manager.o merger.o

all: sorter

sorter: $(OBJS)
	$(CC) $(CFLAGS) -o sorter $(OBJS)

main.o: main.c file_sorter.h
file_sorter.o: file_sorter.c file_sorter.h chunk_reader.h sort_worker.h sorted_chunk_manager.h merger.h
chunk_reader.o: chunk_reader.c chunk_reader.h
thread_safe_queue.o: thread_safe_queue.c thread_safe_queue.h
sort_worker.o: sort_worker.c sort_worker.h thread_safe_queue.h sorted_chunk_manager.h
sorted_chunk_manager.o: sorted_chunk_manager.c sorted_chunk_manager.h
merger.o: merger.c merger.h

clean:
	rm -f *.o sorter generate_input
