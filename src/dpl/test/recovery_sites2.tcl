# Same design without the option: unchanged upstream behaviour, the cells of
# the site that no row offers cannot be placed.
source "helpers.tcl"
read_lef recovery_sites.lef
read_def recovery_sites.def
catch { detailed_placement } msg
puts $msg
catch { check_placement -verbose } msg
puts $msg
