#!/usr/bin/python

import time
import os
import sys

def appendToMail(message):
    os.system("echo \""+message+"\" >> mail.txt")
    
def sendMail():
    # print "Sending mail"
    os.system("mail -s \"DCS WARNING\" \"marco.de.mattia@cern.ch\" < \"mail.txt\"")
    os.system("mail -s \"DCS WARNING\" \"gabriele.benelli@cern.ch\" < \"mail.txt\"")
    # Tracker DOC
    os.system("mail -s \"DCS WARNING\" \"0041764875503@mail2sms.cern.ch\" < \"mail.txt\"")
    # DCS on call
    os.system("mail -s \"DCS WARNING\" \"0041764872740@mail2sms.cern.ch\" < \"mail.txt\"")
    # DQM on call
    os.system("mail -s \"DCS WARNING\" \"0041764872389@mail2sms.cern.ch\" < \"mail.txt\"")
    # Offline Tracker shift leader
    os.system("mail -s \"DCS WARNING\" \"0041764872329@mail2sms.cern.ch\" < \"mail.txt\"")


def runTheAlarm(file, type):

    currentTime = time.time()
    minModulesOn = 200
    maxModulesOn = 15148 - 200

    timeDiff = 0
    firstIOV = True

    previousIOV = 0
    for item in file.read().split("]"):
        # print item
        if( len(item.split("[")) > 1 ):
            element = item.split("[")[1]
            if element.find(", ") != -1:
                IOVtime = int(element.split(", ")[0])/1000
                # if firstIOV == True:
                #     firstIOV = False
                #     previousIOV = IOVtime
                #     print "previousIOV =", previousIOV
                # else:
                # print currentTime
                # 7200 are two hours in seconds = 2*60*60
                # if (currentTime - IOVtime) > 0:
                if (currentTime - IOVtime) < 7200:
                    # print "currentTime =", currentTime
                    print "MAIN IOVtime =", time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(IOVtime))
                    modulesOn = int(element.split(", ")[1])
                    if modulesOn > minModulesOn and modulesOn < maxModulesOn:
                        # print "warning, modules on are", modulesOn, "at", time.localtime(IOVtime)
                        if previousIOV != 0:
                            timeDiff += IOVtime - previousIOV
                        print "IOVtime =", time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(IOVtime))
                        previousIOV = IOVtime
                    else:
                        # print "modulesOn =", modulesOn
                        # 10 minutes = 600 seconds
                        if timeDiff > 600:
                            message = "WARNING: Bad IOV for "+type+" lasting for = "+str(timeDiff)+" seconds and ending at "+time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(IOVtime))
                            # print message
                            appendToMail(message)
                            # sendMail(message)
                            timeDiff = 0
                            previousIOV = 0
    if timeDiff > 600:
        print "WARNING: timeDiff =", timeDiff
        message = "WARNING: Bad IOV for "+type+" lasting for = "+str(timeDiff)+" seconds and ending at "+time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(IOVtime))
        appendToMail(message)
        # sendMail(message)

def checkHVGreaterThanLV(fileHV, fileLV):
    currentTime = time.time()

    arrayHV = fileHV.read().split("]")
    arrayLV = fileLV.read().split("]")
    if len(arrayHV) == len(arrayLV):
        totalNum = len(arrayHV)
        print "Comparing", totalNum, "IOVs"
        for i in range(totalNum):
            if( arrayLV[i].find(", [") != -1 ):
                IOVtime = int(arrayHV[i].split("[")[1].split(",")[0])/1000
                # 7200 are two hours in seconds = 2*60*60
                # if (currentTime - IOVtime) > 0:
                if (currentTime - IOVtime) < 7200:
                    numHVon = int(arrayHV[i].split("[")[1].split(",")[1])
                    numLVon = int(arrayLV[i].split("[")[1].split(",")[1])
                    if( numHVon > numLVon ):
                        message = "WARNING: for IOV "+time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(IOVtime))+" numHVon("+str(numHVon)+") > numLVon("+str(numLVon)+")"
                        print message
                        appendToMail(message)
                        # sendMail(message)
    else:
        print "Error: HV and LV json files have different IOVs!"
        sys.exit()

# Clean the message
os.system("rm /afs/cern.ch/cms/tracker/sistrcalib/DCSTrend/mail.txt")

fileHV = open("/afs/cern.ch/cms/tracker/sistrcalib/DCSTrend/oneMonth_hv.js")
runTheAlarm(fileHV, "HV")

fileLV = open("/afs/cern.ch/cms/tracker/sistrcalib/DCSTrend/oneMonth_lv.js")
runTheAlarm(fileLV, "LV")

# Reopen the files that have already been looped on the previous lines
fileHV = open("/afs/cern.ch/cms/tracker/sistrcalib/DCSTrend/oneMonth_hv.js")
fileLV = open("/afs/cern.ch/cms/tracker/sistrcalib/DCSTrend/oneMonth_lv.js")
checkHVGreaterThanLV(fileHV, fileLV)

if os.path.isfile("/afs/cern.ch/cms/tracker/sistrcalib/DCSTrend/mail.txt"):
    sendMail()
