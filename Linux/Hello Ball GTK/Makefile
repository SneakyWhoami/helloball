# this file specifically i release into the public domain
# or you can have it under CC0 if that's impossible.
# i'm just writing down for posterity what i typed to make the program run.

# missing headers?
# apt systems may appreciate `dpkg -S filename.h`
# or `apt-file search`
# yum systems may enjoy `yum whatprovides filename.h` or `dnf provides filename.h`
# or `rpm -qt`

# missing libs?
# pkg-config --list-all | grep whatever

CXX=g++
CFLAGS=$(shell pkg-config --cflags gtkmm-3.0 freetype2 javascriptcoregtk-4.0)
INCLUDES=-I/usr/include/freetype2/freetype -Isrc
LIBS=$(shell pkg-config --libs gtkmm-3.0 freetype2 javascriptcoregtk-4.0 fribidi)
TARGET=helloball
# misleading
CP=ln -s
RM=rm -f

default: $(TARGET) BalsamicSansRegular.ttf model.js

$(TARGET):
        ${CXX} -include string ${CFLAGS} ${INCLUDES} src/*.cpp -o ${TARGET} ${LIBS}

BalsamicSansRegular.ttf:
        ${CP} '../../Windows/Hello Ball WPF/Hello Ball WPF/BalsamiqSansRegular.ttf' .
# other fonts were mentioned in postbuild.sh but I didn't see them in win directory
# i assume that windows is a 1st class citizen so I copied the named files from there.
#       ${CP} '../../Windows/Hello Ball WPF/Hello Ball WPF/BalsamiqSansItalic.ttf' .
#       ${CP} '../../Windows/Hello Ball WPF/Hello Ball WPF/BalsamiqSansBold.ttf' .
#       ${CP} '../../Windows/Hello Ball WPF/Hello Ball WPF/BalsamicSansBoldItalic.ttf' .

model.js:
        ${CP} '../../Windows/Hello Ball WPF/Hello Ball WPF/model.js' .

clean:
        ${RM} ball *.js *.ttf
        
