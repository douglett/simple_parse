# stdlib.bas

# system functions
system function malloc(size)
system function sizeof(pointer)
system function realloc(pointer, size)
system function free(pointer)
#system function ioredirect(iotype, pointer)

# arrays and strings
function arconcat(dest, source)
	dim dsize = sizeof(dest)
	dim ssize = sizeof(source)
	dim i = 0
	call realloc(dest, dsize + ssize)
	while i < ssize
		dest[dsize + i] = ssize[i]
		i = i + 1
	end while
end function

function arcompare(a1, a2)
	dim size = sizeof(a1)
	dim i = 0
	if size != sizeof(a2)
		return 0
	end if
	while i < size
		if a1[i] != a2[i]
			return 0
		end if
		i = i + 1
	end while
	return 1
end function