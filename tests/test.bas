system function malloc(size)
system function arcompare(s1, s2)

dim str = malloc(1)
dim str2 = malloc(1)
dim arr[]

function main()
	dim arr2[]
	prints str,  "test1"
	prints str2, "test1"
	arr[1] = 1
	print "result: ", arcompare(str, str2)
end function

function testy(val)
	if val < 10
		return val * 2
	else
		return val * 3
	end if
end function