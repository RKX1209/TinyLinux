#! /bin/sh

copy_f(){
    for e in $1/*; do
	path=$e
	if [ -d "$path" ]; then
	    #echo "D:$path"
	    copy_f "$path"
	elif [ -f "$path" ]; then
	    echo "F:$path"
	    mv "$path" /tmp/Abyon/arch
	fi
    done
}
mkdir /tmp/Abyon
cp -r ./arch /tmp/Abyon
cp -r ./boot /tmp/Abyon
cp -r ./include /tmp/Abyon
cp -r ./kernel /tmp/Abyon
cp -r ./mm /tmp/Abyon
cp -r ./fs /tmp/Abyon
cp -r ./lib /tmp/Abyon
cp -r ./drivers /tmp/Abyon
cp -r ./ldscript /tmp/Abyon
cp -r ./security /tmp/Abyon
cp Makefile /tmp/Abyon
cp -r /tmp/Abyon/arch/i386/* /tmp/Abyon/arch/
#copy_f /tmp/Abyon/arch
