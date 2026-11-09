# Twenty-four cells that start at one spot with a very small displacement budget.
# -extended_recovery widens the search for stalled cells until they are legal.
source "helpers.tcl"
read_lef precise_pg.lef
read_def recovery_rows.def
detailed_placement -max_displacement 1 -extended_recovery
check_placement -verbose

set def_file [make_result_file recovery_rows1.def]
write_def $def_file
diff_files recovery_rows1.defok $def_file
