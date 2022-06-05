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

* Troubleshoots
- ['undefined reference to `rom_temp_to_power'](https://github.com/espressif/esp-idf/issues/7946)<br>
uncomment 'rom_temp_to_power = 0x40001ab4;' in file 
~/esp/esp-idf/components/esp_rom/esp32c3/ld/esp32c3.rom.ld
