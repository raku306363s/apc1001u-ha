#include "apc1001u.h"
#include "esphome/core/log.h"

namespace esphome {
namespace apc1001u {

static const char *const TAG = "apc1001u";

// APC1001U defaults to passive mode — host must request each measurement.
// Command: 0x42 0x4D 0xE2 0x00 0x00 0x01 0x71  (request one 64-byte frame)
static const uint8_t REQUEST_CMD[7] = {0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71};

// 64-byte response frame layout (all uint16 values are big-endian):
//  [0-1]  Header      0x42 0x4D
//  [2-3]  Flags
//  [4-5]  PM1.0 µg/m³
//  [6-7]  PM2.5 µg/m³
//  [8-9]  PM10  µg/m³
//  [10-15] PM in-air values (not published)
//  [16-27] Particle counts (not published)
//  [28-29] TVOC ppb
//  [30-31] eCO2 ppm
//  [32-33] Reserved
//  [34-35] T-comp  × 0.1 °C
//  [36-37] RH-comp × 0.1 %
//  [38-41] T-raw / RH-raw (not published)
//  [42-57] Gas raw uint32s (not published)
//  [58]    AQI (1–5 UBA scale)
//  [59]    Reserved
//  [60]    Firmware version
//  [61]    Error code
//  [62-63] Checksum = sum of bytes [0..61]

void APC1001UComponent::setup() {
  ESP_LOGI(TAG, "APC1001U: sending first measurement request");
  this->write_array(REQUEST_CMD, 7);
  last_request_ms_ = millis();
}

void APC1001UComponent::loop() {
  // Request a new measurement every 5 seconds
  uint32_t now = millis();
  if (now - last_request_ms_ >= 5000) {
    ESP_LOGD(TAG, "Requesting measurement...");
    this->write_array(REQUEST_CMD, 7);
    last_request_ms_ = now;
  }

  // Discard any leading bytes that aren't 0x42
  while (this->available() > 0 && this->peek() != 0x42) {
    uint8_t discard;
    this->read_byte(&discard);
    ESP_LOGV(TAG, "Discarding non-header byte");
  }

  // Wait until a full 64-byte frame is buffered
  if (this->available() < 64) return;

  uint8_t buf[64];
  if (!this->read_array(buf, 64)) return;

  // Validate header
  if (buf[0] != 0x42 || buf[1] != 0x4D) {
    ESP_LOGW(TAG, "Bad header: 0x%02X 0x%02X", buf[0], buf[1]);
    return;
  }

  // Validate checksum
  uint16_t calc = 0;
  for (int i = 0; i < 62; i++) calc += buf[i];
  uint16_t expected = ((uint16_t)buf[62] << 8) | buf[63];
  if (calc != expected) {
    ESP_LOGW(TAG, "Checksum mismatch: calc=0x%04X expected=0x%04X", calc, expected);
    return;
  }

  if (buf[61] != 0x00)
    ESP_LOGW(TAG, "Sensor error code: 0x%02X", buf[61]);

  auto w = [&](int off) -> uint16_t {
    return ((uint16_t)buf[off] << 8) | buf[off + 1];
  };

  if (pm1_0_)       pm1_0_->publish_state(w(4));
  if (pm2_5_)       pm2_5_->publish_state(w(6));
  if (pm10_)        pm10_->publish_state(w(8));
  if (tvoc_)        tvoc_->publish_state(w(28));
  if (eco2_)        eco2_->publish_state(w(30));
  if (temperature_) temperature_->publish_state(w(34) * 0.1f);
  if (humidity_)    humidity_->publish_state(w(36) * 0.1f);
  if (aqi_)         aqi_->publish_state(buf[58]);

  ESP_LOGD(TAG,
    "PM1=%.0f PM2.5=%.0f PM10=%.0f TVOC=%.0f eCO2=%.0f T=%.1f RH=%.1f AQI=%d",
    (float)w(4), (float)w(6), (float)w(8),
    (float)w(28), (float)w(30),
    w(34) * 0.1f, w(36) * 0.1f, buf[58]);
}

}  // namespace apc1001u
}  // namespace esphome
