import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import bufferex_base
from esphome.components import color

CODEOWNERS = ['@SenexCrenshaw']

AUTO_LOAD = ['bufferex_base']

CONF_COLOR_OFF = 'color_off'
CONF_COLOR_ON = 'color_on'

bufferex_1bit_2color_ns = cg.esphome_ns.namespace('bufferex_1bit_2color')
bufferex_1bit_2color = bufferex_1bit_2color_ns.class_(
    'Bufferex1bit2color', bufferex_base.bufferex_base)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(bufferex_1bit_2color),
    cv.Optional(CONF_COLOR_ON): cv.use_id(color),
    cv.Optional(CONF_COLOR_OFF): cv.use_id(color),
})


def to_code(config):
    buffer = cg.new_Pvariable(config[CONF_ID])
    if CONF_COLOR_ON in config:
        color_on = yield cg.get_variable(config[CONF_COLOR_ON])
        cg.add(buffer.set_color_on(color_on))

    if CONF_COLOR_OFF in config:
        color_off = yield cg.get_variable(config[CONF_COLOR_OFF])
        cg.add(buffer.set_color_off(color_off))
