# `imagekeys` -- Letter typing game for children

Andrew A. Cashner

## Description

This is a simple game to make it fun for kids to find letters on the keyboard.
It flashes rainbow colors in a loop until you press a key, then it displays
an image corresponding to that key.
You can set up your own images so that typing `a` can call up an apple, or an 
aardvark, or a giant letter A.

## Usage

`imagekeys [-hv -d DIRECTORY]`

* Default directory is `./img/`.
* E.g., `image_keys -d ~/Pictures/imagekeys/`

## Setup 

### Install SDL2

On a Linux system, use your package manager (`aptitude`, `dnf`) to install 
the SDL2 library as root. E.g., `sudo dnf install SDL2 SDL2-devel`
Check your operating system information to make sure which packages to install.

### Compile `imagekeys`

Download `imagekeys.c`.
Compile with GCC and link the SDL2 library: `gcc -o imagekeys -lSDL2 imagekeys.c`

### Setup your images

The program will show a bitmap image for each key pressed.
By default, it will look for these in `./img/` and will expect the filenames to be
alphabetical, like so: `img/a.bmp`, `img/b.bmp`.
You can either download the `img` directory here and use the open-licensed images,
or you can be creative and make your own.

* Create images however you like and save them in `.bmp` format. You might use `imagemagick` to convert from other formats, e.g., `convert unicorn.jpg u.bmp`.
* Give these alphabetic filenames and save them in the directory of your choice.
* Specify this directory using the command-line `-d` option, e.g., `imagekeys -d ~/Pictures/imagekeys/`. Make sure this ends with a backslash.

## License

This software is in the public domain.
You may do what you like with it.
I offer no warranty.


