#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
talkingClockEspeak.py -- demo Talking clock in Lithuanian language. 
Script generates time representation into string and execute espeak command in a subshell
AUTHOR: Mondhs Bus
LICENSE: GPLv2
based on http://johntellsall.blogspot.com/2011/09/i-love-python-espeak-speech-synthesizer.html	
Lithuaninan lessons: http://www.slic.org.au/Language/less_23.swf
'''

import os, time
from array import *

voice='lt'
#voice='mb-lt1'

resolveNumberDivMap = {2: "dvidešimt", 3: "trisdešimt", 4: "keturesdešimt", 
5: "penkiasdešimt"}

resolveNumberModMap = {0:"0", 1: "viena", 2: "dvi", 3: "trys", 4: "keturios", 
5: "penkios", 6: "šešios", 7: "septynios", 8: "aštuonios", 9:"devynios"}


def resolveHourName( hour ):
	rtn = "valandos"
	hourDiv, hourMod= divmod(hour, 10)
	if hourMod == 0:# 0, 10, 20, 30, 40, 50
		rtn = "valandų"
	elif hourDiv == 1:#10-19
		rtn = "valandų"
	elif hourMod == 1:#1, 21, 
		rtn = "valanda"
	return rtn

def resolveMinuteName( minutes ):
	rtn = "minutės"
	minutesDiv, minutesMod= divmod(minutes, 10)
	if minutesMod == 0:# 0, 10, 20, 30, 40, 50
		rtn = "minučių"
	elif minutesDiv == 1:#10-19
		rtn = "minučių"
	elif minutesMod == 1:#1, 21, 31, 41, 51
		rtn = "minutė"
	return rtn

def resolveNumber( number ):
	rtn = "";
	numberDiv, numberMod= divmod(number, 10)
	if number == 0:
		rtn = "0"
	elif numberDiv == 0:#0-9
		rtn = resolveNumberModMap[numberMod]
	elif numberDiv == 1:#10-19
		rtn = "" + str(number)
	elif numberMod == 0:#20,30,40,50
		rtn = resolveNumberDivMap[numberDiv]	
	else:
		rtn = "" + resolveNumberDivMap[numberDiv] + resolveNumberModMap[numberMod]
	return rtn





currentHours = time.localtime()[3]
currentMinutes = time.localtime()[4]

timeForSpeech = "{0} {1} {2} {3}".format(
	resolveNumber(currentHours), 
	resolveHourName(currentHours),
	resolveNumber(currentMinutes), 
	resolveMinuteName(currentMinutes))

#for m in range(0, 60):
#	timeForSpeech = timeForSpeech = "{0} {1} {2} {3}".format(
#		resolveNumber(m), 
#		resolveHourName(m),
#		resolveNumber(0), 
#		resolveMinuteName(0))
#	print timeForSpeech
	#os.system('espeak -v {0} "{1} " 2>/dev/null >/dev/null'.format(voice, timeForSpeech))

print timeForSpeech
os.system('espeak -v {0} "{1} " 2>/dev/null >/dev/null'.format(voice, timeForSpeech))
