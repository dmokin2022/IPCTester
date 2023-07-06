#!/bin/sh
mkdir /mnt/CameraMount
# Монтируем сетевой каталог, для запуска/отладки
mount -t nfs -o nolock 10.30.7.13:/home/dmitriy/Projects/IPCTester/CameraMount /mnt/CameraMount
# Заносим настройку в fstab, чтобы сохранялась при перезагрузке
#echo "10.30.7.13:/home/dmitriy/Projects/IPCTester/CameraMount /mnt/CameraMount nfs rw,nolock,defaults 0 0" >> /etc/fstab
/mnt/CameraMount/IPCTesterCam
