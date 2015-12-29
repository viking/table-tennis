WWIDTH = 1600
WHEIGHT = 900

CFLAGS += -g3 `pkg-config --cflags gtk+-3.0`
CFLAGS += -DWWIDTH=$(WWIDTH) -DWHEIGHT=$(WHEIGHT)
LDFLAGS += `pkg-config --libs gtk+-3.0`
LDFLAGS += -lsqlite3

table-tennis: main.o leader_list.o player.o
	gcc -o table-tennis main.o leader_list.o player.o $(LDFLAGS)

player.o: player.c player.h
	gcc $(CFLAGS) -c -o player.o player.c

leader_list.o: leader_list.c leader_list.h player.h
	gcc $(CFLAGS) -c -o leader_list.o leader_list.c

main.o: main.c leader_list.h player.h
	gcc $(CFLAGS) -c -o main.o main.c

clean:
	rm main.o leader_list.o player.o table-tennis
