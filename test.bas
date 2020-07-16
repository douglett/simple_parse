function main  # my function
	dim a
	dim b = 1 + 1
	a = 4
	a = 3 + a + 10 - 2*4
	print "ass hole   blah", "mewmew", 101, a, b

	call testfn
end function

function testfn
	print "running test func"
end function