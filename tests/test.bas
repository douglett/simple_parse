dim g1 = 1

function main()
	print "test: ", g1
	call t2()
	print "test 3: ", g1
end function

function t2()
	dim g1 = 2
	print "test 2: ", g1
end function