all: main.c export.c unexport.c set_dir.c set_value.c util.h
	g++ -o L2Program main.c export.c unexport.c set_dir.c set_value.c util.h
