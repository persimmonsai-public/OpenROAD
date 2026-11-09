# Same design, negotiation legalizer, precise mode from DPL_PRECISE_PG.
source "helpers.tcl"
set ::env(DPL_PRECISE_PG) 1
read_lef precise_pg.lef
read_def precise_pg_place.def
detailed_placement
check_placement -verbose

set def_file [make_result_file precise_pg3.def]
write_def $def_file
diff_file precise_pg3.defok $def_file
