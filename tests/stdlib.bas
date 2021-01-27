# stdlib.bas

# system functions
system function malloc(size)
system function sizeof(pointer)
system function realloc(pointer, size)
system function free(pointer)
system function ioredirect(iotype, pointer)

# arrays
function concat(dest, source)
	#dim dsize = sizeof(dest)
	#dim ssize = sizeof(source)
	#call realloc(dest, dsize + ssize)
	#for i = 0 to ssize step 1
	#	dest[dsize + i] = ssize[i]
	#end for
end function