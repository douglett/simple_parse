system function malloc(size)
system function realloc(pointer, size)
system function free(pointer)
system function ioredirect(iotype, pointer)

dim global1
dim global2 = 2
dim a = malloc(100)

function main()  # my function
	dim a
	dim b = 1 + 1
	a = 3 + a + 10 - 2*4
	print "ass hole   blah", "mewmew", 101, a, b
	b = testfn(1, 2, 3)
	call testfn(1, 1, b+2)
end function

function testfn(a, b, c)
	a = b[20*2]
	print "running test func"
end function