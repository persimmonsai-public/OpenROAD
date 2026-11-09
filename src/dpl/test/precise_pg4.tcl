# check_placement: pixel rule vs -precise_pg. ok_comb has the wire in its
# channel, fn_comb is mirrored (channel moved), bad_comb's channel misses
# the wire, ok_inv has M1 pins only.
source "helpers.tcl"
read_lef precise_pg.lef
read_def precise_pg_check.def
catch { check_placement -verbose } msg
puts $msg
catch { check_placement -verbose -precise_pg } msg
puts $msg
