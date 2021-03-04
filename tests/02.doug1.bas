# 02.doug1.bas

# commands
dim CMD_LOOK 	= malloc(1)
dim CMD_NORTH 	= malloc(1)
dim CMD_SOUTH 	= malloc(1)
dim CMD_EAST 	= malloc(1)
dim CMD_WEST 	= malloc(1)
dim CMD_QUIT 	= malloc(1)

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
	prints CMD_QUIT, "quit"
end function

function room_1()
	dim cmd = malloc(1)

	while 1
		# get input line
		print "You are in a dark room, what will you do?"
		input "> ", cmd

		# commands
		if arcompare(cmd, CMD_QUIT)
			print "You quit."
			call free(cmd)
			return
		else if arcompare(cmd, CMD_LOOK)
			print "The room is dark. There are exits north, south, east, west."
		else if arcompare(cmd, CMD_NORTH) || arcompare(cmd, CMD_SOUTH) || arcompare(cmd, CMD_EAST) || arcompare(cmd, CMD_WEST)  
			print "You walk ", $cmd, "."
			if arcompare(cmd, CMD_NORTH) 
				print "There was a spike pit! You fall in and are impaled."
			else if arcompare(cmd, CMD_SOUTH)
				print "You are crushed by a falling boulder!"
			else if arcompare(cmd, CMD_EAST)
				print "You see a spooky ghost! Oh no, you suffer a fatal heart attack!"
			else if arcompare(cmd, CMD_WEST)
				print "You see the exit ahead! As you run towards it, you trip over a rock and break your neck."
			end if
			print "You die."
			call free(cmd)
			return
		else
			print "Unknown command: [", $cmd, "]"
		end if
	end while

	call free(cmd)
end function