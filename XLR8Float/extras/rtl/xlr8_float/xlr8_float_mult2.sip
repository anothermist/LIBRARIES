set_global_assignment -entity "xlr8_float_mult2" -library "lib_xlr8_float_mult2" -name IP_TOOL_NAME "altera_fp_functions"
set_global_assignment -entity "xlr8_float_mult2" -library "lib_xlr8_float_mult2" -name IP_TOOL_VERSION "15.1"
set_global_assignment -entity "xlr8_float_mult2" -library "lib_xlr8_float_mult2" -name IP_TOOL_ENV "mwpim"
set_global_assignment -library "lib_xlr8_float_mult2" -name SPD_FILE [file join $::quartus(sip_path) "xlr8_float_mult2.spd"]

set_global_assignment -library "lib_xlr8_float_mult2" -name MISC_FILE [file join $::quartus(sip_path) "xlr8_float_mult2_sim/xlr8_float_mult2.vo"]
