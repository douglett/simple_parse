system function malloc(size)

dim str = malloc(1)

function main()
	prints str, "butt ", 4
	print "test 1: ", str
	print "test 2: ", $str
	input "why? ", str
	print "test 3: [", $str, "]"

	call testy(123)
end function

function testy(val)
	print "val: ", val
end function