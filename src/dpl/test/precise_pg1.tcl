# M2 power wires denser than a wide cell with an M2 obstruction comb:
# without -precise_pg the diamond legalizer cannot place the wide cells.
source "helpers.tcl"
read_lef precise_pg.lef
read_def precise_pg_place.def
catch { detailed_placement -use_diamond_legalizer } msg
puts $msg
