all: build/semaphore
	./build/semaphore 0111 4

build/semaphore: semaphore.c export.c unexport.c set_dir.c set_value.c
	clang -lpthread -lrt -g $^ -o build/semaphore 

clean:
	rm build/semaphore
