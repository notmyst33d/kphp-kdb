# Kitten DB + Kitten PHP
Древние движки ВК из 2014 года

## Как собрать
Система должна иметь libz, libre2, libpcre, openssl и scons для сборки.

Всё что нужно для того чтобы собрать всё:
```
$ scons
```

## Как запустить
```
$ ./random-engine -vvvv -p 11211 ./random.bin &
$ ./main -q -H 10203
```

Результат:  
![](docs/result.jpg)
