# Two sites with identical dimensions and different names: the rows use one,
# half of the cells another.  -match_sites_by_geometry lets dpl place them.
source "helpers.tcl"
read_lef recovery_sites.lef
read_def recovery_sites.def
detailed_placement -match_sites_by_geometry
check_placement -verbose

set def_file [make_result_file recovery_sites1.def]
write_def $def_file
diff_files recovery_sites1.defok $def_file
