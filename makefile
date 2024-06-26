
all : jp

jp: 
		gcc -O2 main.c -o jp

clean: 
	 rm  jp 