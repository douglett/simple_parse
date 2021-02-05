# 02.doug1.bas

# commands
dim CMD_LOOK 	= malloc(1)
dim CMD_NORTH 	= malloc(1)
dim CMD_SOUTH 	= malloc(1)
dim CMD_EAST 	= malloc(1)
dim CMD_WEST 	= malloc(1)

function main()
	call init()
	call room_1()
end function

function init()
	call ioredirect(0, CMD_LOOK)
	print "look"
	call ioredirect(0, CMD_NORTH)
	print "north"
	call ioredirect(0, CMD_SOUTH)
	print "south"
	call ioredirect(0, CMD_EAST)
	print "east"
	call ioredirect(0, CMD_WEST)
	print "west"
	call ioredirect(0, 0)
end function

function room_1()
	dim cmd = malloc(1)

	while 1
		# get input line
		print "You are in a dark room, what will you do?"
		input "> ", cmd

		# commands
		if arcompare(cmd, CMD_LOOK)
			print "The room is dark. There are exits north, south, east, west."
		else if arcompare(cmd, CMD_NORTH) || arcompare(cmd, CMD_SOUTH)
			print "You walk ", cmd, "."
			print "You die."
			return
		else
			print "Unknown command: [", cmd, "]"
		end if
	end while

end function