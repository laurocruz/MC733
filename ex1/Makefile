SOURCE=main.c calc_primo.c
OBJ=primo

all: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ}

O0: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -O0

O1: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -O1

O2: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -O2

O3: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -O3

Os: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -Os

mtune: ${SOURCE}
	gcc ${SOURCE} -o ${OBJ} -mtune=native

time:
	time ./${OBJ}

clean: 
	rm ${OBJ}



