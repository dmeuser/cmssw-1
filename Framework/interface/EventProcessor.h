#ifndef FWCore_Framework_EventProcessor_h
#define FWCore_Framework_EventProcessor_h

/*----------------------------------------------------------------------

EventProcessor: This defines the 'framework application' object. It is
configured in the user's main() function, and is set running.

Requirements placed upon the command line processor:

 1. Support registering of switches/options from user modules
    a. make help available
    b. make sure there are no collisions.

 2. If a switch or option is not supplied, the module looking for it
    must have a sensible default behavior. There should be no required
    switches, nor required options.

Software policing seems needed in order to provent illicit use to
configure moddules entirely with passed arguments, rather than using
the ParameterSet which the module is passed at the time of its
creation.


problems:
  specification of "pass" and other things like it - things that
  have to do with the job as a whole or with this object in particular.

  we are not careful yet about catching exceptions and printing
  useful information.

  where does the pluginmanager initialize call go?


$Id$

----------------------------------------------------------------------*/

#include <string>
#include <vector>

#include "sigc++/signal.h"
#include "boost/shared_ptr.hpp"
#include "boost/thread/thread.hpp"
#include "boost/utility.hpp"

#include "FWCore/Framework/interface/IEventProcessor.h"
#include "FWCore/ServiceRegistry/interface/ServiceLegacy.h"
#include "FWCore/ServiceRegistry/interface/ServiceToken.h"
#include "FWCore/Framework/src/WorkerRegistry.h"
#include "FWCore/Framework/src/SignallingProductRegistry.h"
#include "FWCore/Framework/interface/Actions.h"
#include "DataFormats/Provenance/interface/PassID.h"
#include "DataFormats/Provenance/interface/ReleaseVersion.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EventHelperDescription.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/Framework/src/PrincipalCache.h"

namespace edm {

  namespace event_processor
  {  
    /*
      ------------
      cause events to be processed in a separate thread
      and functions used in the online.  Several of these
      state are likely to be transitory in the offline
      because they are completly driven by the data coming
      from the input source.
    */
    enum State { sInit=0,sJobReady,sRunGiven,sRunning,sStopping,
		 sShuttingDown,sDone,sJobEnded,sError,sErrorEnded,sEnd,sInvalid };
    
    enum Msg { mSetRun=0, mSkip, mRunAsync, mRunID, mRunCount, mBeginJob,
	       mStopAsync, mShutdownAsync, mEndJob, mCountComplete,
	       mInputExhausted, mStopSignal, mShutdownSignal, mFinished,
	       mAny, mDtor, mException, mInputRewind };

    class StateSentry;
    class LuminosityBlockSentry;
    class RunSentry;
    class InputFileSentry;
    class PrePostSourceSignal;
  }
    
  class EventProcessor : public IEventProcessor, private boost::noncopyable
  {
    // ------------ friend classes and functions ----------------

  public:

    /// The input string contains the entire contents of a
    /// configuration file. Uses default constructed ServiceToken, so
    /// an EventProcessor created with this constructor will allow
    /// access to no externally-created services.
    /// This should become pretty much useless when construction of
    /// services is moved outside of the EventProcessor.
    /// explicit EventProcessor(const std::string& config);


    // The input string 'config' contains the entire contents of a  configuration file.
    // Also allows the attachement of pre-existing services specified  by 'token', and
    // the specification of services by name only (defaultServices and forcedServices).
    // 'defaultServices' are overridden by 'config'.
    // 'forcedServices' cause an exception if the same service is specified in 'config'.
    explicit EventProcessor(std::string const& config,
			    ServiceToken const& token = ServiceToken(),
			    serviceregistry::ServiceLegacy =
			      serviceregistry::kOverlapIsError,
			    std::vector<std::string> const& defaultServices =
			      std::vector<std::string>(),
			    std::vector<std::string> const& forcedServices =
			      std::vector<std::string>());

    // Same as previous constructor, but without a 'token'.  Token will be defaulted.

    explicit EventProcessor(std::string const& config,
			    std::vector<std::string> const& defaultServices,
			    std::vector<std::string> const& forcedServices =
			    std::vector<std::string>());
    

    EventProcessor(boost::shared_ptr<edm::ProcessDesc> & processDesc,
                   ServiceToken const& token,
                   serviceregistry::ServiceLegacy legacy);

    ~EventProcessor();

    /**This should be called before the first call to 'run'
       If this is not called in time, it will automatically be called
       the first time 'run' is called
       */
    void beginJob();

    /**This should be called before the EventProcessor is destroyed
       throws if any module's endJob throws an exception.
       */
    void endJob();

    /**Member functions to support asynchronous interface.
       */

    char const* currentStateName() const;
    char const* stateName(event_processor::State s) const;
    char const* msgName(event_processor::Msg m) const;
    event_processor::State getState() const;
    void runAsync();
    StatusCode statusAsync() const;

    // Concerning the async control functions:
    // The event processor is left with the running thread.
    // The async thread is stuck at this point and the process 
    // is likely not going to be able to continue. 
    // The reason for this timeout could be either an infinite loop
    // or I/O blocking forever.
    // The only thing to do is end the process.
    // If you call endJob, you will likely get an exception from the
    // state checks telling you that it is not valid to call this function.
    // All these function force the event processor state into an
    // error state.

    // tell the event loop to stop and wait for its completion
    StatusCode stopAsync(unsigned int timeout_secs=60*2);
    
    // tell the event loop to shutdown and wait for the completion
    StatusCode shutdownAsync(unsigned int timeout_secs=60*2);

    // wait until async event loop thread completes
    // or timeout occurs (See StatusCode for return values)
    StatusCode waitTillDoneAsync(unsigned int timeout_seconds=0);

    // Both of these calls move the EP to the ready to run state but only
    // the first actually sets the run number, the other one just stores
    // the run number set externally in order to later compare to the one
    // read from the input source for verification
    void setRunNumber(RunNumber_t runNumber);
    void declareRunNumber(RunNumber_t runNumber);

    // -------------

    // Invoke event processing.  We will process a total of
    // 'numberEventsToProcess' events. If numberEventsToProcess is -1, we will
    // process events intil the input sources are exhausted. If it is
    // given a non-negative number, processing continues until either (1)
    // this number of events has been processed, or (2) the input
    // sources are exhausted.
    // The 'repeatable' flag affects behavior if and when 'numberEventsToProcess'
    // events have been processed.  If the flag is false, the current luminosity
    // block and run will be ended normally.  Any subsequent calls will
    // begin at the next run.  If the flag is true, the current luminosity
    // block and run will not be ended, and a repeat call will begin at the next event.

    StatusCode run(int numberEventsToProcess, bool repeatable = true);

    // Invoke event processing.  Invokes run(maxEvents_, false );
    StatusCode run();

    // Process one event with the given EventID
    StatusCode run(EventID const& id);

    // Skip the specified number of events.
    // If numberToSkip is negative, we will back up.
    StatusCode skip(int numberToSkip);
 
    // Rewind to the first event
    void rewind();

    /// Return a vector allowing const access to all the
    /// ModuleDescriptions for this EventProccessor.

    /// *** N.B. *** Ownership of the ModuleDescriptions is *not*
    /// *** passed to the caller. Do not call delete on these
    /// *** pointers!

    std::vector<ModuleDescription const*>
    getAllModuleDescriptions() const;

    /// Return the number of events this EventProcessor has tried to process
    /// (inclues both successes and failures, including failures due
    /// to exceptions during processing).
    int totalEvents() const;

    /// Return the number of events processed by this EventProcessor
    /// which have been passed by one or more trigger paths.
    int totalEventsPassed() const;

    /// Return the number of events that have not passed any trigger.
    /// (N.B. totalEventsFailed() + totalEventsPassed() == totalEvents()
    int totalEventsFailed() const;

    /// Turn end_paths "off" if "active" is false;
    /// turn end_paths "on" if "active" is true.
    void enableEndPaths(bool active);

    /// Return true if end_paths are active, and false if they are
    /// inactive.
    bool endPathsEnabled() const;

    /// Return the trigger report information on paths,
    /// modules-in-path, modules-in-endpath, and modules.
    void getTriggerReport(TriggerReport& rep) const;      

    /// Clears counters used by trigger report.
    void clearCounters();

    // Really should not be public,
    //   but the EventFilter needs it for now.    
    ServiceToken getToken();

    /// signal is emitted after the Event has been created by the
    /// InputSource but before any modules have seen the Event
    ActivityRegistry::PreProcessEvent &
    preProcessEventSignal() {return preProcessEventSignal_;}

    /// signal is emitted after all modules have finished processing
    /// the Event
    ActivityRegistry::PostProcessEvent &
    postProcessEventSignal() {return postProcessEventSignal_;}

    //------------------------------------------------------------------
    //
    // Nested classes and structs below.

    struct CommonParams
    {
      CommonParams():
	processName_(),
	releaseVersion_(),
	passID_(),
	maxEventsInput_(),
	maxLumisInput_()
      { }

      CommonParams(std::string const& processName,
		   ReleaseVersion const& releaseVersion,
		   PassID const& passID,
		   int maxEvents,
		   int maxLumis):
	processName_(processName),
	releaseVersion_(releaseVersion),
	passID_(passID),
        maxEventsInput_(maxEvents),
        maxLumisInput_(maxLumis)
      { }
      
      std::string processName_;
      ReleaseVersion releaseVersion_;
      PassID passID_;
      int maxEventsInput_;
      int maxLumisInput_;
    }; // struct CommonParams

    // These classes work with the boost statemachine

    virtual StatusCode runToCompletion(bool onlineStateTransitions);

    virtual void readFile();
    virtual void closeInputFile();
    virtual void openOutputFiles();
    virtual void closeOutputFiles();

    virtual void respondToOpenInputFile();
    virtual void respondToCloseInputFile();
    virtual void respondToOpenOutputFiles();
    virtual void respondToCloseOutputFiles();

    virtual void startingNewLoop();
    virtual bool endOfLoop();
    virtual void rewindInput();
    virtual void prepareForNextLoop();
    virtual void writeCache();
    virtual bool shouldWeCloseOutput();

    virtual void doErrorStuff();

    virtual void smBeginRun(int run);
    virtual void smEndRun(int run);

    virtual void beginLumi(int run, int lumi);
    virtual void endLumi(int run, int lumi);

    virtual int readAndCacheRun();
    virtual int readAndCacheLumi();
    virtual void writeRun(int run);
    virtual void deleteRunFromCache(int run);
    virtual void writeLumi(int run, int lumi);
    virtual void deleteLumiFromCache(int run, int lumi);

    virtual void readEvent();
    virtual void processEvent();
    virtual bool shouldWeStop();

  private:
    //------------------------------------------------------------------
    //
    // Now private functions.
    // init() is used by only by constructors
    void init(boost::shared_ptr<edm::ProcessDesc> & processDesc,
              ServiceToken const& token,
              serviceregistry::ServiceLegacy);
  
    StatusCode processEvents(int & numberEventsToProcess);
    StatusCode processLumis(int & numberEventsToProcess, bool repeatable);
    StatusCode processRuns(int & numberEventsToProcess, bool repeatable);
    StatusCode processInputFiles(int numberEventsToProcess, bool repeatable,
		     event_processor::Msg m);
    StatusCode doneAsync(event_processor::Msg m);
    EventHelperDescription runOnce(boost::shared_ptr<RunPrincipal>& rp,
                                   boost::shared_ptr<LuminosityBlockPrincipal>& lbp);
    
    boost::shared_ptr<FileBlock> beginInputFile();
    boost::shared_ptr<RunPrincipal> beginRun();
    boost::shared_ptr<LuminosityBlockPrincipal> beginLuminosityBlock(boost::shared_ptr<RunPrincipal> rp);
    std::auto_ptr<EventPrincipal> doOneEvent(boost::shared_ptr<LuminosityBlockPrincipal> lbp);
    std::auto_ptr<EventPrincipal> doOneEvent(EventID const& id);
    void procOneEvent(EventPrincipal *pep);
    void endLuminosityBlock(LuminosityBlockPrincipal *lbp);
    void endRun(RunPrincipal *rp);

    StatusCode waitForAsyncCompletion(unsigned int timeout_seconds);

    void connectSigs(EventProcessor * ep);

    struct DoPluginInit
    {
      DoPluginInit();
    };

    void changeState(event_processor::Msg);
    void errorState();
    void setupSignal();

    void preSourceSignal();
    void postSourceSignal();

    static void asyncRun(EventProcessor *);

    //------------------------------------------------------------------
    //
    // Data members below.
    // Are all these data members really needed? Some of them are used
    // only during construction, and never again. If they aren't
    // really needed, we should remove them.    

    ActivityRegistry::PreProcessEvent             preProcessEventSignal_;
    ActivityRegistry::PostProcessEvent            postProcessEventSignal_;
    DoPluginInit                                  plug_init_;
    ParameterSet			          maxEventsPset_;
    ParameterSet			          maxLumisPset_;
    boost::shared_ptr<ActivityRegistry>           actReg_;
    WorkerRegistry                                wreg_;
    SignallingProductRegistry                     preg_;
    ServiceToken                                  serviceToken_;
    boost::shared_ptr<InputSource>                input_;
    std::auto_ptr<Schedule>                       schedule_;
    std::auto_ptr<eventsetup::EventSetupProvider> esp_;    
    ActionTable                                   act_table_;

    volatile event_processor::State               state_;
    boost::shared_ptr<boost::thread>              event_loop_;

    boost::mutex                                  state_lock_;
    boost::mutex                                  stop_lock_;
    boost::condition                              stopper_;
    boost::condition                              starter_;
    volatile int                                  stop_count_;
    volatile Status                               last_rc_;
    std::string                                   last_error_text_;
    volatile bool                                 id_set_;
    volatile pthread_t                            event_loop_id_;
    int                                           my_sig_num_;
    boost::shared_ptr<FileBlock>                  fb_;
    boost::shared_ptr<RunPrincipal>               rp_;
    boost::shared_ptr<LuminosityBlockPrincipal>   lbp_;
    boost::shared_ptr<EDLooper>                   looper_;

    PrincipalCache                                principalCache_;
    std::auto_ptr<EventPrincipal>               sm_evp_;
    bool                                          shouldWeStop_;
    bool                                          stateMachineWasInErrorState_;
    std::string                                   fileMode_;
    bool                                          handleEmptyRuns_;
    bool                                          handleEmptyLumis_;
    bool                                          sourceActive_;

    friend class EDLooperHelper;
    friend class event_processor::StateSentry;
    friend class event_processor::LuminosityBlockSentry;
    friend class event_processor::RunSentry;
    friend class event_processor::InputFileSentry;
    friend class event_processor::PrePostSourceSignal;
  }; // class EventProcessor

  //--------------------------------------------------------------------
  // ----- implementation details below ------

  inline
  EventProcessor::StatusCode
  EventProcessor::run() {
    return run(-1, false);
  }

}
#endif
