# stdlib.bas

# system functions
system function malloc(size)
system function sizeof(pointer)
system function realloc(pointer, size)
system function free(pointer)
system function ioredirect(iotype, pointer)

# arrays
function concat(dest, source)
	dim dsize = sizeof(dest)
	dim ssize = sizeof(source)
	dim i = 0
	call realloc(dest, dsize + ssize)
	while i < ssize
		dest[dsize + i] = ssize[i]
		i = i + 1
	end while
end function