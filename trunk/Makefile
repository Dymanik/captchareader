CC=g++
DEBUG=-g
FLAGS=`pkg-config --cflags --libs opencv`

all: captchaprep captchasim captchatrain

captchasim : captchasim.cpp
	$(CC) captchasim.cpp $(FLAGS) -o captchasim

captchatrain : captchatrain.cpp
	$(CC) captchatrain.cpp $(FLAGS) -o captchatrain

captchaprep : captchaprep.cpp
	$(CC) captchaprep.cpp $(FLAGS) -o captchaprep

clean:
	rm captchasim captchatrain captchaprep
