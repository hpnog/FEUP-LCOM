cd ..
cp conf/proj /etc/system.conf.d
mkdir /home/lcom1415-t6g10-proj
cd code/
cp -vr images/ /home/lcom1415-t6g10-proj
make distclean
make
