-- SPDX-FileCopyrightText: NONE
-- SPDX-License-Identifier: Unlicense

set_project("scansize")
add_rules("mode.debug", "mode.release")

set_languages("c99")
set_warnings("all", "extra")

target("scansize")
	set_kind("binary")
	add_files("src/*.c")
