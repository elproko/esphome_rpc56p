import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import (
    CONF_ID,
    CONF_CURRENT,
    CONF_VOLTAGE,
    CONF_TEMPERATURE,

    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_VOLT,
    UNIT_WATT,

    UNIT_HERTZ,
    ICON_EMPTY,
    UNIT_EMPTY,

    ICON_CURRENT_AC,
    CONF_ACTIVE_POWER,

    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,

    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_VOLTAGE,
)


DEPENDENCIES = ["uart"]

rpc56p_sensor_ns = cg.esphome_ns.namespace("rpc56p_sensor")
RPC56pSensor = rpc56p_sensor_ns.class_(
    "RPC56pSensor", cg.PollingComponent, uart.UARTDevice
)

# CONFIG_SCHEMA = (
#     sensor.sensor_schema(
#         RPC56pSensor,
#         unit_of_measurement=UNIT_EMPTY,
#         icon=ICON_EMPTY,
#         accuracy_decimals=1,
#     )
#     .extend(cv.polling_component_schema("60s"))
#     .extend(uart.UART_DEVICE_SCHEMA)
# )

CONF_IN_TEMP = "in_temp_sensor"
CONF_OUT_TEMP = "out_temp_sensor"

CONF_TOP_TEMP = "top_temp_sensor"
CONF_BOTTOM_TEMP = "bottom_temp_sensor"

CONF_WATER_TEMP = "water_temp_sensor"

CONF_ALARMS = "alarms_sensor"
CONF_RELAYS = "relays_sensor"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(RPC56pSensor),
            cv.Optional(CONF_IN_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_OUT_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TOP_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_BOTTOM_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_WATER_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_ALARMS): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                icon=ICON_EMPTY,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_RELAYS): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                icon=ICON_EMPTY,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            )
        }    
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
#    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_IN_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_IN_TEMP])
        cg.add(var.set_in_temp_sensor(sens))

    if CONF_OUT_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_OUT_TEMP])
        cg.add(var.set_out_temp_sensor(sens))

    if CONF_TOP_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_TOP_TEMP])
        cg.add(var.set_top_temp_sensor(sens))

    if CONF_BOTTOM_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_BOTTOM_TEMP])
        cg.add(var.set_bottom_temp_sensor(sens))

    if CONF_WATER_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_WATER_TEMP])
        cg.add(var.set_water_temp_sensor(sens))

    if CONF_ALARMS in config:
        sens = await sensor.new_sensor(config[CONF_ALARMS])
        cg.add(var.set_alarms_sensor(sens))

    if CONF_RELAYS in config:
        sens = await sensor.new_sensor(config[CONF_RELAYS])
        cg.add(var.set_relays_sensor(sens))
