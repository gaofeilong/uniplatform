#!/bin/bash

cwd=`pwd`

if [[ "$1" == "-r" ]]; then
        ls | grep -v mv_file.sh | xargs rm -f
        cd ../Back/
        make clean
        cd ../Web/scistor/
        bash clear_cache.sh
        cd $cwd
        exit 0
fi

ls | grep -v "mv_file.sh" | xargs rm -f
cd ../Web/scistor/
bash clear_cache.sh
cd $cwd
cd ../Back/
# find . -name "*" | xargs touch
make clean;make
if [[ $? -ne 0 ]]; then
        exit 1
fi
cp AgentMain ServerMain ArchiveMain ClientMain TestMain $cwd
service gwserver stop
service gwagent stop
cp AgentMain ServerMain ArchiveMain /usr/local/bin
service gwserver start
service gwagent start
cp Utils/Script/scigw.sh /usr/local/bin
cp Utils/Script/remote.sh /usr/local/bin
make clean;
cd $cwd
if [[ ! -e scigw.sh ]]; then
    ln -s ../Back/Utils/Script/scigw.sh scigw.sh
fi
if [[ ! -e remote.sh ]]; then
    ln -s ../Back/Utils/Script/remote.sh remote.sh
fi

if [[ ! -e uniplatform.sh ]]; then
    ln -s ../Back/Utils/Script/uniplatform.sql uniplatform.sql
fi

if [[ ! -e scistor ]]; then
    ln -s ../Web/scistor web
fi

if [[ ! -e Back ]]; then
    ln -s ../Back back
fi
