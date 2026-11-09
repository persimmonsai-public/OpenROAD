# Same design without the option: unchanged upstream behaviour.
source "helpers.tcl"
read_lef precise_pg.lef
read_def recovery_rows.def
catch { detailed_placement -max_displacement 1 } msg
puts $msg
catch { check_placement -verbose } msg
puts $msg
