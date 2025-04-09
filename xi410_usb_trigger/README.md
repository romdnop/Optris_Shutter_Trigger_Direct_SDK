# How to build

```
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```


# Usage

    xi410_usb_trigger.exe --help


# Example

    .\xi410_usb_trigger.exe -c .\generic.xml -e C:\Users\User\Desktop\xi410_direct_sdk_project\tools\test -n example.csv -p

# NOTE!

This project is similar to **xi410_tcp_trigger**, but does not require **tcp_daemon** to run. As a side-effect, it takes significantly longer to obtain a picture, as the camera being re-configured each time. Normal execution time for **xi410_usb_trugger** is more than **2s**, when it is **400ms** on average for **xi410_tcp_trigger**.