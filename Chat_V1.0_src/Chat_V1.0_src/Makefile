# Makefile:

GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
INC  = $(GTKINC)
LIBS = $(GTKLIBS)

CC = gcc
DEBUG = -DDEBUG
CFLAGS = -ansi -std=c99 -Wall -g -c 
LFLAGS = -ansi -std=c99 -Wall -g

PROJECT	= bin/client bin/server src/gui_notebook #database hashtest
TEST = bin/serverTest bin/clientTest #guiTest
#CHESS = src/chess_src/GameMenu.o src/chess_src/ChessRules.o src/chess_src/ChessPieceHierarchy.o src/chess_src/CheckWin.o src/chess_src/GameBoard.o src/chess_src/Castling.o src/chess_src/EnPassant.o src/chess_src/ChessTools.o src/chess_src/Chess_AI.o libfilter.a

all: $(PROJECT) 
test: $(TEST)

clean:
	rm -rf *.o $(PROJECT)
	rm -rf src/*.o
	rm -rf src/hashtest
	rm -rf *.binary
	rm -rf libhashtable.a
	


test:
	@echo "To run the examples, first start the server in one terminal,"
	@echo "then start the client in another."
	@echo
	@echo "For example:"
	@echo "crystalcove# server 10000"
	@echo "crystalcove# client crystalcove 10000"
	@echo
	@echo "Different servers may be used as well:"
	@echo "crystalcove# server2 10000"
	@echo "zuma# client crystalcove 10000"
tar: bin doc src INSTALL.txt README.txt Copyright.txt
	gtar cvzf Chat_V1.0_src.tar.gz bin doc src INSTALL.txt README.txt Copyright.txt
src/hashtest.o: src/hash.h src/hash.c
	gcc $(CFLAGS) $(DEBUG) src/hash.c -o src/hashtest.o

src/hashtest: src/hashtest.o
	gcc $(LFLAGS) src/hashtest.o -o src/hashtest.binary

src/client.o: src/client.c libhashtable.a 
	gcc $(CFLAGS) src/client.c $(INC) -o src/client.o

bin/client: src/client.o libhashtable.a 
	gcc src/client.o $(LIBS) -ansi -std=c99 -g -L. -lhashtable -o bin/client

src/server.o: src/server.c src/server.h src/database.h src/hash.h libhashtable.a
	gcc $(CFLAGS) src/server.c  $(INC) -o src/server.o

bin/server: src/server.o libhashtable.a 
	gcc src/server.o  $(LIBS) -ansi -std=c99 -g -L. -lhashtable -o bin/server

src/gui.o: src/gui.c src/gui.h
	gcc src/gui.c $(CFLAGS) $(INC) -o src/gui.o

src/gui: src/gui.o
	gcc src/gui.o $(LFLAGS) $(LIBS) -o src/gui

src/guiTest.o: src/gui.c src/gui.h
	gcc src/gui.c $(CFLAGS) $(INC) $(DEBUG) -o src/guiTest.o

src/guiTest: src/guiTest.o
	gcc src/guiTest.o $(LFLAGS) $(LIBS) $(DEBUG) -o src/guiTest 

src/gui_notebook.o: src/gui_notebook.c
	gcc -c src/gui_notebook.c $(INC) -g -std=c99 -ansi -Wall -o src/gui_notebook.o

src/gui_notebook: src/gui_notebook.o
	gcc src/gui_notebook.o $(LIBS) -g -std=c99 -Wall -o src/gui_notebook

src/database.o: src/database.c src/database.h src/hash.h
	gcc $(CFLAGS) src/database.c -o src/database.o

src/hash.o: src/hash.h src/hash.c
	gcc $(CFLAGS) src/hash.c -o src/hash.o

libhashtable.a: src/hash.o src/database.o src/gui.o src/gui_notebook.o
	ar rc libhashtable.a src/hash.o src/database.o src/gui.o src/gui_notebook.o
	ranlib libhashtable.a

#Test files

src/clientTest.o: src/client.c libhashtableTest.a 
	gcc $(CFLAGS) src/client.c $(DEBUG) $(INC) -o src/clientTest.o

bin/clientTest: src/clientTest.o libhashtableTest.a 
	gcc src/clientTest.o $(LIBS) -ansi -std=c99 -g $(DEBUG) -L. -lhashtableTest -o bin/clientTest

src/serverTest.o: src/server.c src/server.h src/database.h src/hash.h libhashtableTest.a
	gcc $(CFLAGS) src/server.c $(DEBUG) $(INC) -o src/serverTest.o

bin/serverTest: src/serverTest.o libhashtableTest.a 
	gcc src/serverTest.o  $(LIBS) -ansi -std=c99 -g $(DEBUG) -L. -lhashtableTest -o src/serverTest.binary

src/databaseTest.o: src/database.c src/database.h src/hash.h
	gcc $(CFLAGS) src/database.c $(DEBUG) -o src/databaseTest.o

src/hashTest.o: src/hash.h src/hash.c
	gcc $(CFLAGS) src/hash.c $(DEBUG) -o src/hashTest.o

libhashtableTest.a: src/hashTest.o src/databaseTest.o src/guiTest.o src/gui_notebook.o
	ar rc libhashtableTest.a src/hashTest.o src/databaseTest.o src/guiTest.o src/gui_notebook.o
	ranlib libhashtableTest.a

#database.o: database.h database.c hash.h
#	gcc $(CFLAGS) database.c -o database.o

#database: database.o
#	gcc $(LFLAGS) database.o -o database.binary

#To Compile database/hash and clienttest for the time being"
#gcc database.c hash.c -ansi -std=c99 -Wall -o dbtest.binary
#gcc clienttest.c -g -ansi -std=c99 -Wall -o clienttest.binary

#src/chess_src/CheckWin.o: src/chess_src/CheckWin.c src/chess_src/CheckWin.h src/chess_src/ChessRules.h src/chess_src/GameBoard.h src/chess_src/ChessPieceHierarchy.h src/chess_src/ChessTools.h
#	$(CC) $(CFLAGS) src/chess_src/CheckWin.c -o src/chess_src/CheckWin.o
#src/chess_src/Castling.o: src/chess_src/Castling.c src/chess_src/Castling.h src/chess_src/ChessPieceHierarchy.h src/chess_src/ChessRules.h src/chess_src/CheckWin.h
#	$(CC) $(CFLAGS) src/chess_src/Castling.c -o src/chess_src/Castling.o
#src/chess_src/EnPassant.o: src/chess_src/EnPassant.c src/chess_src/EnPassant.h src/chess_src/ChessPieceHierarchy.h src/chess_src/ChessRules.h src/chess_src/ChessTools.h
#	$(CC) $(CFLAGS) src/chess_src/EnPassant.c -o src/chess_src/EnPassant.o
#src/chess_src/GameMenu.o: src/chess_src/GameMenu.h src/chess_src/GameMenu.c src/chess_src/ChessPieceHierarchy.h src/chess_src/GameBoard.h src/chess_src/CheckWin.h
#	$(CC) $(CFLAGS) src/chess_src/GameMenu.c -o src/chess_src/GameMenu.o
#src/chess_src/GameBoard.o: src/chess_src/GameBoard.h src/chess_src/GameBoard.c src/chess_src/CheckWin.h src/chess_src/ChessRules.h src/chess_src/ChessPieceHierarchy.h src/chess_src/ChessTools.h
#	$(CC) $(CFLAGS) src/chess_src/GameBoard.c -o src/chess_src/GameBoard.o
#src/chess_src/ChessRules.o: src/chess_src/ChessRules.c src/chess_src/ChessRules.h src/chess_src/ChessPieceHierarchy.h src/chess_src/Castling.h src/chess_src/ChessTools.h src/chess_src/EnPassant.h
#	$(CC) $(CFLAGS) src/chess_src/ChessRules.c -o src/chess_src/ChessRules.o
#src/chess_src/ChessTools.o: src/chess_src/ChessTools.c src/chess_src/ChessTools.h src/chess_src/ChessPieceHierarchy.h
#	$(CC) $(CFLAGS) src/chess_src/ChessTools.c -c -o src/chess_src/ChessTools.o
#src/chess_src/ChessPieceHierarchy.o: src/chess_src/ChessPieceHierarchy.c src/chess_src/ChessPieceHierarchy.h
#	$(CC) $(CFLAGS) src/chess_src/ChessPieceHierarchy.c -o src/chess_src/ChessPieceHierarchy.o
#src/chess_src/Chess_AI.o: src/chess_src/Chess_AI.c src/chess_src/GameBoard.h src/chess_src/ChessPieceHierarchy.h src/chess_src/Chess_AI.h
#	$(CC) $(CFLAGS) src/chess_src/Chess_AI.c -o src/chess_src/Chess_AI.o
#libfilter.a: src/chess_src/GameMenu.o src/chess_src/GameBoard.o src/chess_src/ChessRules.o src/chess_src/CheckWin.o src/chess_src/ChessTools.o src/chess_src/Castling.o src/chess_src/EnPassant.o src/chess_src/Chess_AI.o
#	ar rc libfilter.a src/chess_src/GameMenu.o src/chess_src/GameBoard.o src/chess_src/ChessRules.o src/chess_src/ChessTools.o src/chess_src/CheckWin.o src/chess_src/ChessTools.o src/chess_src/Castling.o src/chess_src/EnPassant.o src/chess_src/Chess_AI.o
#	ranlib libfilter.a


# EOF
