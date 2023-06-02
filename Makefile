all: semaphore
	./semaphore

semaphore: semaphore.c
	clang -o $@ -lpthread -lrt $^
