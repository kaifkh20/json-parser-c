
all : lin win

lin: 
		gcc -O3 main.c -o jp

win :
	 x86_64-w64-mingw32-gcc -O3  main.c  -o jp-w

clean-lin: 
	 rm jp
clean-win:
	rm jp-w.exe
clean: clean-lin clean-win	 