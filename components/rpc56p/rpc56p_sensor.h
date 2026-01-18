#pragma once

//#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace rpc56p_sensor {

#define max_line_length 80

class RPC56pSensor : public sensor::Sensor, public PollingComponent, public uart::UARTDevice {

private:
float in_temp = 1.0;
float out_temp = 1.0;
float top_temp = 1.0;
float bottom_temp = 1.0;
float water_temp = 1.0;
int relays = 1;
int alarms = 1;
int old_relays = 1;
int old_alarms = 1;
int read_ok = 0;

GPIOPin *dir_pin_{nullptr};


 public:

  Sensor *in_temp_sensor;
  Sensor *out_temp_sensor;
  Sensor *top_temp_sensor;
  Sensor *bottom_temp_sensor;
  Sensor *water_temp_sensor;

  Sensor *alarms_sensor;
  Sensor *relays_sensor;

  void setup() override;
  void update() override;
  void loop() override;
  void dump_config() override;

  void set_in_temp_sensor(sensor::Sensor *sensor) { this->in_temp_sensor = sensor; }
  void set_out_temp_sensor(sensor::Sensor *sensor) { this->out_temp_sensor = sensor; }
  void set_top_temp_sensor(sensor::Sensor *sensor) { this->top_temp_sensor = sensor; }
  void set_bottom_temp_sensor(sensor::Sensor *sensor) { this->bottom_temp_sensor = sensor; }
  void set_water_temp_sensor(sensor::Sensor *sensor) { this->water_temp_sensor = sensor; }
  void set_alarms_sensor(sensor::Sensor *sensor) { this->alarms_sensor = sensor; }
  void set_relays_sensor(sensor::Sensor *sensor) { this->relays_sensor = sensor; }

  void set_dir_pin(GPIOPin *pin) { this->dir_pin_ = pin; }

};

}  // namespace rpc56p_sensor
}  // namespace esphome
