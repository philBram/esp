# esp
## Labor 2 GHP
pinout mapping

![esp pinout mapping](https://microcontrollerslab.com/wp-content/uploads/2019/02/ESP32-pinout-mapping.png)

## Commands for Aufgabenteil_C

curl -X GET -w "\n" [ip]:80/led_toggle

curl -X POST -w "\n" -d [0-8191] [ip]:80/set_duty
