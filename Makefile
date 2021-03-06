WWIDTH = 1600
WHEIGHT = 900

CFLAGS += -g3 `pkg-config --cflags gtk+-3.0`
CFLAGS += -DWWIDTH=$(WWIDTH) -DWHEIGHT=$(WHEIGHT)
LDFLAGS += `pkg-config --libs gtk+-3.0`
LDFLAGS += -lsqlite3

table-tennis: main.o leader_list.o stat_list.o player.o game.o stats.o
	gcc -o table-tennis main.o leader_list.o stat_list.o player.o game.o stats.o $(LDFLAGS)

player.o: player.c player.h
	gcc $(CFLAGS) -c -o player.o player.c

leader_list.o: leader_list.c leader_list.h player.h
	gcc $(CFLAGS) -c -o leader_list.o leader_list.c

game.o: game.c game.h
	gcc $(CFLAGS) -c -o game.o game.c

stats.o: stats.c stats.h game.h player.h
	gcc $(CFLAGS) -c -o stats.o stats.c

stat_list.o: stat_list.c stat_list.h stats.h
	gcc $(CFLAGS) -c -o stat_list.o stat_list.c

main.o: main.c leader_list.h stat_list.h stats.h player.h game.h
	gcc $(CFLAGS) -c -o main.o main.c

clean:
	rm -f main.o leader_list.o stat_list.o player.o game.o stats.o table-tennis
