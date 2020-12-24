dim global1
dim global2 = 2
dim a

function main()  # my function
	dim a
	dim b = 1 + 1
	a = 3 + a + 10 - 2*4
	print "ass hole   blah", "mewmew", 101, a, b
	call testfn(1, 1, b+2)
end function

function testfn(a, b, c)
	print "running test func"
end function