
import os, sys, time
import random

from Configuration.PyReleaseValidation.WorkFlow import WorkFlow
from Configuration.PyReleaseValidation.WorkFlowRunner import WorkFlowRunner

# ================================================================================

class MatrixRunner(object):

    def __init__(self, wfIn=None, nThrMax=4):

        self.workFlows = wfIn

        self.threadList = []
        self.maxThreads = int(nThrMax) # make sure we get a number ...


    def activeThreads(self):

        nActive = 0
        for t in self.threadList:
            if t.isAlive() : nActive += 1

        return nActive

        
    def runTests(self, testList=None):

        startDir = os.getcwd()

    	report=''    	
        if self.maxThreads == 0:
            print 'resetting to default number of threads'
            self.maxThreads=4

    	print 'Running in %s thread(s)' % self.maxThreads

            
        for wf in self.workFlows:

            if testList and float(wf.numId) not in [float(x) for x in testList]: continue

            item = wf.nameId
            if os.path.islink(item) : continue # ignore symlinks
            
    	    # make sure we don't run more than the allowed number of threads:
    	    while self.activeThreads() >= self.maxThreads:
    	        time.sleep(10)
                continue
    	    
    	    print '\nPreparing to run %s %s' % (wf.numId, item)
          
    	    current = WorkFlowRunner(wf)
    	    self.threadList.append(current)
    	    current.start()
            time.sleep(random.randint(1,5)) # try to avoid race cond by sleeping random amount of time [1,5] sec 

    	# wait until all threads are finished
        while self.activeThreads() > 0:
    	    time.sleep(5)


        #wrap up !
        totpassed=[]
        totfailed=[]
        def count(collect,result):
            #pad with zeros
            for i in range(len(collect),len(result)):
                collect.append(0)
            for i,c in enumerate(result):
                collect[i]+=c
                
        for pingle in self.threadList:
            pingle.join()
            try:
                count(totpassed,pingle.npass)
                count(totfailed,pingle.nfail)
                report+=pingle.report
            except Exception, e:
                msg = "ERROR retrieving info from thread: " + str(e)
                report += msg
                
        report+=' '.join(map(str,totpassed))+' tests passed, '+' '.join(map(str,totfailed))+' failed\n'
        print report

        runall_report_name='runall-report-step123-.log'
        runall_report=open(runall_report_name,'w')
        runall_report.write(report)
        runall_report.close()
        os.chdir(startDir)
        
                                        
        return

