# 02.doug1.bas

# commands
dim CMD_LOOK = malloc(1)
dim CMD_NORTH = malloc(1)

function main()
	call init()
	call room_1()
end function

function init()
	call ioredirect(0, CMD_LOOK)
	print "look"
	call ioredirect(0, CMD_NORTH)
	print "north"
	call ioredirect(0, 0)
end function

function room_1()
	dim cmd = malloc(1)

	print "You are in a dark room, what will you do?"
	input "> ", cmd

	if arcompare(cmd, CMD_LOOK)
		print "The room is dark. There are exits north, south, east, west."
	else if arcompare(cmd, CMD_NORTH)
		print "You die."
	else
		print "Unknown command: [", cmd, "]"
	end if

end function