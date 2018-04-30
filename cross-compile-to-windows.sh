# https://stackoverflow.com/questions/10934683/how-do-i-configure-qt-for-cross-compilation-from-linux-to-windows-target

# Tässä kestää kauan

apt-get install \
    autoconf automake autopoint bash bison bzip2 flex gettext\
    git g++ gperf intltool libffi-dev libgdk-pixbuf2.0-dev \
    libtool-bin libltdl-dev libssl-dev libxml-parser-perl make \
    openssl p7zip-full patch perl pkg-config python ruby scons \
    sed unzip wget xz-utils g++-multilib libc6-dev-i386 git

if [! -d mxe]; then
    git clone https://github.com/mxe/mxe.git
fi

cd mxe && make qt5

$MXEROOT = $PWD

cd ..
export PATH=$MXEROOT/usr/bin:$PATH
export PATH=$MXEROOT/usr/i686-w64-mingw32.static/qt5/bin:$PATH

mxe/usr/i686-pc-mingw32.static/qt5/bin/qmake && make

# file in ./relese/ folder

export PATH=/home/markus/Documents/Lappbill/mxe/usr/i686-w64-mingw32.static/bin/
