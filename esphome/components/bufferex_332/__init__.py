import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import bufferex_base

CODEOWNERS = ['@SenexCrenshaw']

AUTO_LOAD = ['bufferex_base']

bufferex_332_ns = cg.esphome_ns.namespace('bufferex_332')
bufferex_332 = bufferex_332_ns.class_('Bufferex332', bufferex_base.bufferex_base)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(bufferex_332),
})


def to_code(config):
    cg.new_Pvariable(config[CONF_ID])
