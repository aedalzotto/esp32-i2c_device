# esp32-i2c_device-cpp

This is a high-level C++ API for I2C programming with ESP32. It is intended to be used inside device libraries.

## Adding to your project

You need to clone this project and it's dependency to your project's components folder.
Inside you project folder:
```
mkdir -p components && cd components
git clone https://github.com/aedalzotto/esp32-i2c_device-cpp.git
```
If you prefer you can use submodule instead of cloning:
```
mkdir -p components && cd components
git submodule add https://github.com/aedalzotto/esp32-i2c_device-cpp.git
```

There is no need for additional configuration.
