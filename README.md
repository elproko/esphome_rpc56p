# esphome_rpc56p
ESPHome component for reading rpc56p (controller) heatpump's sensors. It uses ESP12 module and RS-485.
RPC56p controller uses RS485 communication which is not MODBUS compatible.
Therefore this component uses underlying UART component and additionally DIRECTION pin for RS485 communication and not the MODBUS component directly.
Please note DIRECTION pin is not put into component's configuration yet, its hardcoded as D4.

Feel free to make a PR if you want to contribute e.g. adding DIR pin to the configuration or any other improvements.

