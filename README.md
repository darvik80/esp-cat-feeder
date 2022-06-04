* Install SDK v4.4.1 
```shell
mkdir -p ~/esp
cd ~/esp
git clone -b v4.4.1 --recursive https://github.com/espressif/esp-idf.git
cd ~/esp/esp-idf
./install.sh
```

* Build Cat Feeder
```shell
cd ~/esp/esp-cat-feeder/
. $HOME/esp/esp-idf/export.sh
idf.py set-target esp32
idf.py build
idf.py flash
```