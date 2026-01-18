#include "esphome/core/log.h"
#include "rpc56p_sensor.h"
#include <cstring>
#include <cstdlib>

namespace esphome {
namespace rpc56p_sensor {

static const char *TAG = "rpc56p_sensor.sensor";

int readline(int readch, char *buffer, int len)  {
    static int pos = 0;
    int rpos;

    if (readch > 0) {
      switch (readch) {
        case ':': // reset pointer
          pos = 0;
          buffer[pos++] = readch;
          buffer[pos] = 0;
          break;
        case '\n': // Ignore new-lines
          break;
        case '\r': // Return on CR
          rpos = pos;
          pos = 0;  // Reset position index ready for next time
          return rpos;
        default:
          if (pos < len-1) {
            buffer[pos++] = readch;
            buffer[pos] = 0;
          }
      }
    }
    // No end of line has been found, so return -1.
    return -1;
}

unsigned long str2hex(const char* str){
  unsigned long ret = 0;
  int slen = strlen(str);
  int pot = 1;
  int a;
  int w = 0;

  for (int x = slen - 1; x >= 0; x--){
      a = (int)str[x];
      if (a >= 48 && a < 58) w = a - 48;
      if (a >= 65 && a < 71) w = a - 55;
      if (a >= 97 && a < 103) w = a - 87;
      ret = ret + w * pot;
      pot = pot << 4;
  }
  return ret;
}


void RPC56pSensor::setup() // override
{
  // Setup direction pin for RS485 transceiver
  if (this->dir_pin_ != nullptr) {
    this->dir_pin_->setup();
    this->dir_pin_->digital_write(false); // read direction for 74sn176
  }
}

void RPC56pSensor::update() {
  if (read_ok==1) {
    if (fabs(in_temp)<99) in_temp_sensor->publish_state(in_temp);
    if (fabs(out_temp)<99) out_temp_sensor->publish_state(out_temp);
    if (fabs(top_temp)<99) top_temp_sensor->publish_state(top_temp);
    if (fabs(bottom_temp)<99) bottom_temp_sensor->publish_state(bottom_temp);
    if (fabs(water_temp)<99) water_temp_sensor->publish_state(water_temp);
    alarms_sensor->publish_state(alarms);
    relays_sensor->publish_state(relays);
  }
}

void RPC56pSensor::loop() {
  static char buffer[max_line_length];
  int reads_done = 0;

  while (available() && reads_done < 20) {
    int n = readline(read(), buffer, max_line_length);
    if (n > 5) {
      // Parse command code from positions 3-5
      char cmd_str[3];
      cmd_str[0] = buffer[3];
      cmd_str[1] = buffer[4];
      cmd_str[2] = '\0';
      int C = atoi(cmd_str);

      // Parse value from position 5 onwards
      int wart = atoi(&buffer[5]);

      switch (C) {
        case 1: // relays: 0b=spr+cwu+dz, 0a=cwu+dz, 08=cwu
          relays = str2hex(&buffer[5]);
          reads_done++;
          break;
        case 10: // cmdgettemppok ":0110" - czujnik pokojowy
          in_temp = (float)wart / 10;
          reads_done++;
          break;
        case 11: // cmdgettemppog ":0111" - pogodowka
          out_temp = (float)wart / 10;
          reads_done++;
          break;
        case 12: // cmdgettempdz ":0112" - Dolne żródło
          bottom_temp = (float)wart / 10;
          reads_done++;
          break;
        case 13: // cmdgettempcwu ":0113" - CWU
          water_temp = (float)wart / 10;
          reads_done++;
          break;
        case 14: // cmdgettempco ":0114" - CO
          top_temp = (float)wart / 10;
          reads_done++;
          break;
        case 22: // cmdreadinput ":0122" - wejścia zezwoleń i alarmów
          alarms = 7 - str2hex(&buffer[5]);
          reads_done++;
          read_ok = 1;
          break;
      }
    }
    reads_done++;
  }

  if (relays != old_relays) {
    relays_sensor->publish_state(relays);
  }
  if (alarms != old_alarms) {
    alarms_sensor->publish_state(alarms);
  }

  old_alarms = alarms;
  old_relays = relays;
}

void RPC56pSensor::dump_config(){
    ESP_LOGCONFIG(TAG, "RPC56p sensor");
}

}  // namespace rpc56p_sensor
}  // namespace esphome