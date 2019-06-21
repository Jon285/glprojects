CC = clang++
FLAGS =  -g -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lassimp
SOURCE = main.cc shader.cc stb.cc

model: main.cc shader.cc stb.cc
	${CC} ${FLAGS} ${SOURCE} -o main
