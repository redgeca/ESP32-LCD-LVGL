#include <stdint.h>
#include "vars.h"

int32_t meter_indicator_value;

int32_t get_var_meter_indicator_value() {
    return meter_indicator_value;
}

void set_var_meter_indicator_value(int32_t value) {
    meter_indicator_value = value;
}
