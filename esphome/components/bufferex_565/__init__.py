import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import bufferex_base

CODEOWNERS = ['@SenexCrenshaw']

AUTO_LOAD = ['bufferex_base']

bufferex_565_ns = cg.esphome_ns.namespace('bufferex_565')
bufferex_565 = bufferex_565_ns.class_('Bufferex565', bufferex_base.bufferex_base)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(bufferex_565),
})


def to_code(config):
    cg.new_Pvariable(config[CONF_ID])
