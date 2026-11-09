# Same design, diamond legalizer with -precise_pg: the wide cells land
# where a power wire runs through their obstruction channel.
source "helpers.tcl"
read_lef precise_pg.lef
read_def precise_pg_place.def
detailed_placement -use_diamond_legalizer -precise_pg
check_placement -verbose

set def_file [make_result_file precise_pg2.def]
write_def $def_file
diff_file precise_pg2.defok $def_file
