
all : jp jp-win

jp: 
		gcc -O3 main.c -o jp

jp-win :
	 x86_64-w64-mingw32-gcc -O3  main.c  -o jp-w

clean: 
	 rm  jp jp-w.exe