#system function malloc(size)

dim str = malloc(1)
dim str2 = malloc(1)

function main()
	prints str,  "test1"
	prints str2, "test1"
	print "result: ", arcompare(str, str2)
end function

function testy(val)
	if val < 10
		return val * 2
	else
		return val * 3
	end if
end function