import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

apc1001u_ns = cg.esphome_ns.namespace("apc1001u")
APC1001UComponent = apc1001u_ns.class_(
    "APC1001UComponent", cg.Component, uart.UARTDevice
)

CONF_PM1_0       = "pm1_0"
CONF_PM2_5       = "pm2_5"
CONF_PM10        = "pm10"
CONF_TVOC        = "tvoc"
CONF_ECO2        = "eco2"
CONF_TEMPERATURE = "temperature"
CONF_HUMIDITY    = "humidity"
CONF_AQI         = "aqi"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(APC1001UComponent),
            cv.Optional(CONF_PM1_0): sensor.sensor_schema(
                unit_of_measurement="µg/m³",
                accuracy_decimals=0,
                device_class="pm1",
                state_class="measurement",
                icon="mdi:blur",
            ),
            cv.Optional(CONF_PM2_5): sensor.sensor_schema(
                unit_of_measurement="µg/m³",
                accuracy_decimals=0,
                device_class="pm25",
                state_class="measurement",
                icon="mdi:blur",
            ),
            cv.Optional(CONF_PM10): sensor.sensor_schema(
                unit_of_measurement="µg/m³",
                accuracy_decimals=0,
                device_class="pm10",
                state_class="measurement",
                icon="mdi:blur",
            ),
            cv.Optional(CONF_TVOC): sensor.sensor_schema(
                unit_of_measurement="ppb",
                accuracy_decimals=0,
                state_class="measurement",
                icon="mdi:molecule",
            ),
            cv.Optional(CONF_ECO2): sensor.sensor_schema(
                unit_of_measurement="ppm",
                accuracy_decimals=0,
                device_class="carbon_dioxide",
                state_class="measurement",
                icon="mdi:molecule-co2",
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement="°C",
                accuracy_decimals=1,
                device_class="temperature",
                state_class="measurement",
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                unit_of_measurement="%",
                accuracy_decimals=1,
                device_class="humidity",
                state_class="measurement",
            ),
            cv.Optional(CONF_AQI): sensor.sensor_schema(
                accuracy_decimals=0,
                state_class="measurement",
                icon="mdi:air-filter",
            ),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_PM1_0 in config:
        sens = await sensor.new_sensor(config[CONF_PM1_0])
        cg.add(var.set_pm1_0_sensor(sens))
    if CONF_PM2_5 in config:
        sens = await sensor.new_sensor(config[CONF_PM2_5])
        cg.add(var.set_pm2_5_sensor(sens))
    if CONF_PM10 in config:
        sens = await sensor.new_sensor(config[CONF_PM10])
        cg.add(var.set_pm10_sensor(sens))
    if CONF_TVOC in config:
        sens = await sensor.new_sensor(config[CONF_TVOC])
        cg.add(var.set_tvoc_sensor(sens))
    if CONF_ECO2 in config:
        sens = await sensor.new_sensor(config[CONF_ECO2])
        cg.add(var.set_eco2_sensor(sens))
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))
    if CONF_AQI in config:
        sens = await sensor.new_sensor(config[CONF_AQI])
        cg.add(var.set_aqi_sensor(sens))
