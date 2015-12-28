table-tennis: main.c
	gcc `pkg-config --cflags gtk+-3.0` -o table-tennis main.c `pkg-config --libs gtk+-3.0`
