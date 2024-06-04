# Kitten DB + Kitten PHP
Древние движки ВК из 2014 года

## Как собрать
Билд система работает только на Linux, как впринципе и сам KDB + KPHP

Система должна иметь libz, libre2, libpcre и openssl 1.1

```
$ mkdir -p objs/TL
$ make objs/TL/constants.h
$ ./ezfile.c
$ ./kphp_program_ezfile.c
```

ezfile.c пока что не умеет собирать всё сам, пока что нужен make для сборки constants.h

## Как запустить
```
$ export LD_LIBRARY_PATH=out
$ ./out/random-engine -vvvv -p 11211 ./out/random.bin &
$ ./out/kphp_program/main -q -H 10203
```

Результат:  
![](docs/result.jpg)
