
# https://github.com/parai/as
## This repository has been dropped and all the code has been merged into my new Automotive Software repository https://github.com/parai/as
Arctic Core - the open source AUTOSAR embedded platform


For information on getting started with Arctic Core, please take a look at: 
  http://arccore.com/wiki/Quick-start_Tutorial

For more general information and help, browse the wiki at:
  http://arccore.com/wiki/

or post a question in the forum:
  https://arccore.com/forum/

Since 2013-11-23 17:17 base on arc-stable-a45528fee37b.zip AUTOSAR 3.1
	parai@foxmail.com  
	https://github.com/parai
	As I have tried a lot, I want to implement an OSEK or AutoSAR infrastructure software for automative electronics,but the fact is that I am not smart and energetic enough to do the whole thing. In the day, I have to do a job to feed me. I just spent some time for the goal I want to realise on night or at the weekend. No doubt, it turns out no good.I feel tired, espeically when I want to make the project OpenOSEK (https://github.com/parai/OpenOSEK) to be able to run on Windows, bugs here and there, and I still cann't solve it even I have debuged it for a month, but OpenOSEK runs all OK on the board STM32F107VC_EVAL.
	So I start to think, slow down, technology explosion, if I do all of it by myslef, OK, no doubt, I will be out of date as I am not as clever as linus. But it's OK, I am working hard, and now I know most of the OSEK or AutoSAR, I can work on the base of arccore and make arccore more easily to be used without the expensive tools from arccore.	
	So let's go...

	[2014-03-01]
	Now, Gui Tool for OS,COM,NvM is ready.
	Simulation runnable on gtk3.0.
	DCM Gui Tool is on the way.
	
	[2014-04-05]
	Most of the Module is OK, but I think the ArcCore is not that good, start to make it better.
	No care of drivers related to hardware, I will use GTK to simulate most of the drivers.
	Start from OS firstly..., all According to AUTOSAR 4.0
	By reading ArcCore OS, I think it is not that perfect, So what to do

		>> 2014-4-16 Add a script intepreter for arvfb, good for me.
		   2014-5-17, something no good happened to me, this script is stopped half done.
		   That is the script has no Event and Alarm attribute which are really I hoped.
		
		>> 2014-5-17, I learn something more about python, it is so powerful, that is somehow 
		the reason why I left the script half done. Yes, get the point from ply<python-lex-yacc>,
		I think I can use the python reflection attribute to build a more powerful arvfb.
		  SIP will do the support for interaction with C/C++ code.
		
		>> 2014-6-2, recently, I spent less time on github, I feel guilty as I waste time on playing.
		But really, I became more and more lazy, I feel I couldn't continue this project. 
		I think a lot, I still feel confused about what the true meaning of the things I do.
		I am still wondering about.
		So I add FreeRTOS to this project, as I feel ArcCore OS is stupid, and I have no ability to develop a good one. And really FreeRTOS is so good, it even runs perfect on MinGW and MSVC.So maybe I can build a OSEK OS abstract layer base on FreeRTOS.
		
	
	

	
