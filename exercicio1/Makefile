SOURCE=main.c calc_primo.c
OBJ=primo
FLAGS=-pg

all: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} ${FLAGS}

O0: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -O0 ${FLAGS}

O1: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -O1 ${FLAGS}

O2: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -O2 ${FLAGS}

O3: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -O3 ${FLAGS}

Os: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -Os ${FLAGS}

mtune: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -mtune=native ${FLAGS}

run:
	./${OBJ}

time:
	time ./${OBJ}

clean: 
	rm ${OBJ}



