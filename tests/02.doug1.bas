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
	prints CMD_LOOK, "look"
	prints CMD_NORTH, "north"
	prints CMD_SOUTH, "south"
	prints CMD_EAST, "east"
	prints CMD_WEST, "west"
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
			print "You walk ", $cmd, "."
			print "You die."
			return
		else
			print "Unknown command: [", $cmd, "]"
		end if
	end while

	call free(cmd)
end function