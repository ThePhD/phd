# Copyright � 2018 ThePhD

if(_common_core_included)
	return()
endif(_common_core_included)
set(_common_core_included true)

# # Prepend function (is there an equivalent in CMake somewhere...?)
function(prepend var prefix)
	set(l "")
	foreach(f ${ARGN})
		list(APPEND l "${prefix}${f}")
	endforeach(f)
	SET(${var} "${l}" PARENT_SCOPE)
ENDFUNCTION(prepend)
