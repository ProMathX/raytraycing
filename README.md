Raytracing in C.

For building

```bash
#On Linux

gcc -o bin/raytracing src/main.c  $(sdl2-config --cflags --libs) -lm

```

(Not tested)For building on windows

```powershell

cc -o raytracing main.c 'sdl2-config --cflags --libs' -lm

```
