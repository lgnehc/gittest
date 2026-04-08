# my_project — ESP-IDF (v6.0, ESP32-S3)

## Build

```bash
idf.py set-target esp32s3   # must run once before first build
idf.py build
idf.py flash monitor        # flash + serial monitor @ 115200 baud
```

IDF_PATH: `/home/qwas/.espressif/v6.0/esp-idf`

## Architecture

- **Entry point**: `main/main.c` → `app_main()`, pinned to core 1
- **Custom component**: `components/bsp/` (`bsp.c`, `common.c`) provides `myprintf` macro, `Task_Context_Base_Size`, `gpio_out_init()`. Headers in `components/bsp/include/`.
- **Inactive file**: `main/test.c` is **not listed** in `main/CMakeLists.txt` SRCS — it will not compile. Contains extensive peripheral examples (GPIO/PWM/IRQ/timer/ADC/UART/I2C/SPI/WiFi) used as reference.

## Gotchas

- Adding a new `.c` file to `main/` requires adding it to `main/CMakeLists.txt` `SRCS` list — unlisted files are silently ignored.
- `main/CMakeLists.txt` does not declare `REQUIRES` for `bsp`; it works because IDF auto-discovers components.
- `myprintf` and `Task_Context_Base_Size` are duplicated between `components/bsp/include/common.h` and `main/test.c`.
- Source comments and log strings are in **Chinese**.
- WiFi SSID/password are hardcoded in `main/test.c` — do not commit real credentials there.
