# my first hello world program!

system function malloc(size)

function main()
	call hello1()
	call hello2()
end function

function hello1()
	print "hello world!"
end function

function hello2()
	dim name = malloc(1)
	input "What is your name? ", name
	print "Hello, ", name
end function