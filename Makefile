.PHONY: all semaphore mutex clean
all: build/semaphore build/mutex

semaphore: build/semaphore
	sudo /build/semaphore 0111 4

mutex: build/mutex
	sudo /build/mutex 0111 4

build/semaphore: semaphore.c export.c unexport.c set_dir.c set_value.c
	clang -lpthread -lrt -g $^ -o build/semaphore 

build/mutex: mutex.c export.c unexport.c set_dir.c set_value.c
	clang -lpthread -lrt -g $^ -o build/mutex

clean:
	@mkdir -p build
	rm build/semaphore
