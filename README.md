# osOS
Tugas Besar IF2230 Sistem Operasi <br>
Pembuatan Sistem Operasi ~seadanya~ sederhana <br>
Semester II 2021/2022

## Program Requirements

### Recommended Tools
- [Virtual Box](https://www.virtualbox.org/wiki/Downloads)

### Requirements
- [Netwide Assembler](https://www.nasm.us/) untuk kompilasi program assembly
- [Bruce C Compiler](https://linux.die.net/man/1/bcc) untuk kompilasi C 16 bit (GCC sudah tidak mendukung 16 bit yang murni)
- [ld86](https://linux.die.net/man/1/ld86) untuk melakukan linking object code
- [Bochs](http://bochs.sourceforge.net/) sebagai emulator untuk menjalankan sistem operasi

If you have Ubuntu 20.04, you can just type
```
sudo apt update
sudo apt install nasm bcc bin86 bochs bochs-x make
```

## Cara Menggunakan Program
- Clone repo ini
```
git clone https://github.com/Sister19/tugas-besar-os-osos.git osOS
cd osOS/milestone-3
```
- Init shell dan TC
```
cd shell_init
gcc shell_init.c tc_lib -o ../out/shell_init

cd ../tc_gen
gcc tc_gen.c tc_lib -o ../out/tc_gen
```
- Run
```
make build-run
c (After bocsh compiled)
```
- On another terminal, type
```
cd out
./shell_init ; ./tc_gen A|B|C
```

## Author
osOS

| NIM      | Nama                       |
|----------|----------------------------|
| 13520059 | Suryanto                   | 
| 13520113 | Brianaldo Phandiarta       | 
| 13520161 | M Syahrul Surya Putra      |