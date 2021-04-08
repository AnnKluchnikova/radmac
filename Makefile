# TODO Сделать запуск утилиты по команде, а не через "./radmac"
# TODO Прописать установку библиотеки ncurses
# TODO прописать установку библиотеки json-c

radar: radmac.c
	gcc -Wall -g radmac.c rand-mac.c -o radmac -lncurses $(shell pkg-config --libs --cflags json-c)
#	gcc -Wall -g radmac.c -o radmac $(shell pkg-config --libs --cflags json-c)

#win: test-win.c
#	gcc -Wall -g test-win.c -o win -lncurses
#	gcc -Wall -g test-win.c rand-mac.c -o win -lncurses $(shell pkg-config --libs --cflags json-c)

win: front_window.c
	gcc -Wall -g front_window.c -o win -lpanel -lncurses

rand: rand-mac.c
	gcc -Wall -g rand-mac.c -o rand $(shell pkg-config --libs --cflags json-c)

test: test-rad.c
	gcc -Wall -g test-rad.c json-parser.c -o test $(shell pkg-config --libs --cflags json-c)