# cpu-emulator
tiny x86 cpu emulator

## Installation
```
$ git clone http://github.com/tuz358/cpu-emulator.git
$ cd ./cpu-emulator
$ make
```

## Sample
```
$ ./cpu-emulator sample/fibonacci.bin
hlt called.
eax = 0x00000037 (55)
ecx = 0x00000000 (0)
edx = 0x00000015 (21)
ebx = 0x00000022 (34)
esp = 0x00000400 (1024)
ebp = 0x00000000 (0)
esi = 0x00000000 (0)
edi = 0x00000000 (0)
eip = 0x00000000 (0)
$
```

## Author
[tuz358](https://github.com/tuz358)

## License
[GPLv3](https://github.com/tuz358/slackbot/blob/master/LICENSE)
