# A liberty file read after the design is in the database gets the same check as link time: a cell
# of the design without a liberty cell at one of the corners is an error. Before, the check ran only
# when the design was read after the liberty files, and report_check_types -max_slew /
# -max_capacitance dereferenced the missing corner's port (a segmentation fault).
source "helpers.tcl"
read_lef Nangate45/Nangate45.lef
read_lef Nangate45/fakeram45_64x7.lef
read_def liberty_scene_missing_cell.def

define_corners slow fast
read_liberty -corner fast Nangate45/Nangate45_fast.lib
read_liberty -corner slow Nangate45/Nangate45_slow.lib
# the memory has a liberty file at the slow corner only
read_liberty -corner slow Nangate45/fakeram45_64x7.lib
create_clock -name clk -period 10 clk

catch { report_check_types -max_slew -violators } msg
puts $msg
catch { report_check_types -max_capacitance -violators } msg
puts $msg

