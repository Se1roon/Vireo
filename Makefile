client-run: client
	./bin/client

client:
	g++ -o ./bin/client Client/src/*.cpp Common/src/*.cpp \
		-I./Client/inc/ -I./Common/inc/ \
		-L./SFML/lib/ \
		-lsfml-network -lsfml-system -lsfml-window -lsfml-graphics \
		-std=c++20 -Wall 


server-run: server
	./bin/server

server:
	g++ -o ./bin/server Server/src/*.cpp Common/src/*.cpp \
		-I./Server/inc/ -I./Common/inc/  -I/usr/local/include/mongocxx/ -I/usr/local/include/bsoncxx/  \
		-L./SFML/lib/ -L/usr/local/lib/ \
		-lmongocxx -lbsoncxx -lssl -lcrypto -lz -lpthread -lsfml-network \
		-std=c++20 -Wall

