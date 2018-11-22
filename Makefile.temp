all: wmi acpi_dump

wmi:
	gcc utils/wmidump.c -std=gnu99 -o wmidump
	chmod +x wmidump

acpi_dump:
	cat /sys/firmware/acpi/tables/DSDT >  dsdt.dat
	iasl -d dsdt.dat
	sed '/_WDG/,/Name/!d' dsdt.dsl  | sed '/{/,/}/!d' | grep -v } | grep -v { > wmi.dump
	./wmidump < wmi.dump > teste.txt

