require('coroutine')
require('os')

local function wait(timeMs)
	last = os.clock()
	
	while((os.clock()-last) < (timeMs/1000.0))
	do
	end
end

function Task1(...)
	while true
	do
		print "Task1 is runnig!\n"
		wait(1000)
	end
end 
local function main()
	print "Hello World!\n"
	task1_handle = coroutine.create(Task1)
	coroutine.resume(task1_handle)
	while true
	do
		print "main task is runnig!\n"
		wait(1000)
	end
end
main()
