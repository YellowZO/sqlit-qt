sqlited:
	gcc -o bin/sqlited src/sqlited_fun.c src/sqlited.c -I include -L include/sqlite3/.libs -lsqlite3
	gcc -o bin/sqlite-cli src/sqlite-cli.c -I include