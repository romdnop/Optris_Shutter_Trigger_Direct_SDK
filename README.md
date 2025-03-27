# About

A small program that saves IR images from Xi410 camera by Optris.

# Useful links

https://sdk.optris.com/libirimager2/html/Overview.html

# Prerequisites

- [CMake](https://cmake.org/)
- MSVC compiler installed



# How to build

```
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```


# Usage

1) Run xi410_tcp_daemon

```
xi410_tcp_daemon.exe generic.xml
```

2) Execute xi410_tcp_trigger
```
.\xi410_tcp_trigger.exe  -e C:\Users\User\Desktop\xi410_direct_sdk_project\tools\test -n example.csv -p
```

Running xi410_tcp_trigger will result in *.csv file being created.

You can then use csv_to_png.py script from "tools/". For more details see [README.md](tools/README.md)

