@echo off

del test.pyd
gcc test.c core\src\data_set.c -o test.dll -shared -D DEBUG -I core\src -I C:\Users\hiroshi\AppData\Local\Programs\Python\Python312\include -L C:\Users\hiroshi\AppData\Local\Programs\Python\Python312\libs -lpython3 -lpython312
ren test.dll test.pyd
