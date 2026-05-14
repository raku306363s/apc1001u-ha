# Quick Install (1 minute)

## Prerequisites
- ESP32 dev board wired to APC1001U (see README for wiring)
- Home Assistant with ESPHome Builder add-on

## 1 — Flash the ESP32 (2 minutes)

In ESPHome Builder → **New Device** → paste `aqi-sensor.yaml` → fill WiFi credentials → **Install**.

## 2 — Dashboard (30 seconds)

Paste this into your HA Terminal & SSH:

```bash
mkdir -p /config/www && \
curl -sL -o /config/www/aqi_dashboard.html \
  https://raw.githubusercontent.com/raku306363s/apc1001u-ha/main/aqi_dashboard.html && \
ha core restart
```

Then: **Settings → Dashboards → Add Dashboard** → URL: `/local/aqi_dashboard.html`

Open it → paste a Long-Lived Access Token → Connect. Done.
