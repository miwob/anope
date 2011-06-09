#ifndef THREADENGINE_H
#define THREADENGINE_H

#ifdef _WIN32
typedef HANDLE ThreadHandle;
typedef CRITICAL_SECTION MutexHandle;
typedef HANDLE CondHandle;
#else
# include <pthread.h>
typedef pthread_t ThreadHandle;
typedef pthread_mutex_t MutexHandle;
typedef pthread_cond_t CondHandle;
#endif

class ThreadEngine;
class Thread;

extern CoreExport ThreadEngine threadEngine;

class CoreExport ThreadEngine
{
 public:
	/* Vector of threads */
	std::vector<Thread *> threads;

	/** Threadengines constructor
	 */
	ThreadEngine();

	/** Threadengines destructor
	 */
	~ThreadEngine();

	/** Start a new thread
	 * @param thread A pointer to a newley allocated thread
	 */
	void Start(Thread *thread);

	/** Check for finished threads
	 */
	void Process();
};

class CoreExport Thread : public Extensible
{
 private:
	/* Set to true to tell the thread to finish and we are waiting for it */
	bool exit;

 public:
	/* Handle for this thread */
	ThreadHandle Handle;

	/** Threads constructor
	 */
	Thread();

	/** Threads destructor
	 */
	virtual ~Thread();

	/** Join to the thread, sets the exit state to true
	 */
	void Join();

	/** Sets the exit state as true informing the thread we want it to shut down
	 */
	void SetExitState();

	/** Exit the thread. Note that the thread still must be joined to free resources!
	 */
	void Exit();

	/** Returns the exit state of the thread
	 * @return true if we want to exit
	 */
	bool GetExitState() const;

	/** Called to run the thread, should be overloaded
	 */
	virtual void Run();
};

class CoreExport Mutex
{
 protected:
	/* A mutex, used to keep threads in sync */
	MutexHandle mutex;

 public:
	/** Constructor
	 */
	Mutex();

	/** Destructor
	 */
	~Mutex();

	/** Attempt to lock the mutex, will hang until a lock can be achieved
	 */
	void Lock();

	/** Unlock the mutex, it must be locked first
	 */
	void Unlock();

	/** Attempt to lock the mutex, will return true on success and false on fail
	 * Does not block
	 * @return true or false
	 */
	bool TryLock();
};

class CoreExport Condition : public Mutex
{
 private:
	/* A condition */
	CondHandle cond;

 public:
	/** Constructor
	 */
	Condition();

	/** Destructor
	 */
	~Condition();

	/** Called to wakeup the waiter
	 */
	void Wakeup();

	/** Called to wait for a Wakeup() call
	 */
	void Wait();
};

#endif // THREADENGINE_H