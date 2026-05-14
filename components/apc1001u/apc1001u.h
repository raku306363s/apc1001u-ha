#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace apc1001u {

class APC1001UComponent : public Component, public uart::UARTDevice {
 public:
  void set_pm1_0_sensor(sensor::Sensor *s)       { pm1_0_       = s; }
  void set_pm2_5_sensor(sensor::Sensor *s)       { pm2_5_       = s; }
  void set_pm10_sensor(sensor::Sensor *s)        { pm10_        = s; }
  void set_tvoc_sensor(sensor::Sensor *s)        { tvoc_        = s; }
  void set_eco2_sensor(sensor::Sensor *s)        { eco2_        = s; }
  void set_temperature_sensor(sensor::Sensor *s) { temperature_ = s; }
  void set_humidity_sensor(sensor::Sensor *s)    { humidity_    = s; }
  void set_aqi_sensor(sensor::Sensor *s)         { aqi_         = s; }

  void setup() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  sensor::Sensor *pm1_0_      {nullptr};
  sensor::Sensor *pm2_5_      {nullptr};
  sensor::Sensor *pm10_       {nullptr};
  sensor::Sensor *tvoc_       {nullptr};
  sensor::Sensor *eco2_       {nullptr};
  sensor::Sensor *temperature_{nullptr};
  sensor::Sensor *humidity_   {nullptr};
  sensor::Sensor *aqi_        {nullptr};

  uint32_t last_request_ms_{0};
};

}  // namespace apc1001u
}  // namespace esphome
