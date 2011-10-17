#/bash/bin
echo '>>>> Desinstalando'
make uninstall &&
make clean &&
make distclean
echo '>>>> Reinstalando'
./autogen.sh
make &&
make install

