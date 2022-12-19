//////////////////////////////////////////////////////////////////////////////
//	Header file for the
// Deferred Processing Queue object to handle the job submission and execution
// on a separate thread

#if !defined(__DPQ2_H)
#define __DPQ2_H

#include "Mcl4Mfc.h"

class StringArray;
class CMadelungDoc;

class CJob {
    // this class has only pure virtual functions
    // and must be derived from to use with the
    // CJobList object...
public:
    // execute method will be called by the
    // deferred processing queue to process
    // this job...
    virtual void Execute(void) = 0;

    // cancel method will be called by the
    // deferred processing queue to process
    // this job...
    virtual void Cancel(void) = 0;
};

class CJobList : private CMclLinkedList<CJob *> {
public:
    BOOL QueueJob( CJob *pJob);
    BOOL DequeueJob( CJob **pJob, CMclEvent *pInterrupt = NULL, DWORD dwTimeout = INFINITE);
    BOOL RemoveJob( CJob *pJob);
};

class CDPQueue2 : public CMclThreadHandler {
private:
    CJobList m_cJobList; // list of pending jobs
    CMcl4MfcWorkerThreadAutoPtr m_apWorkerThread; // worker thread auto-pointer
	CMclEvent m_ceControl;	// auto-reset event to control thread
	BOOLEAN m_bThreadExists;	// TRUE when worker thread exists
	BOOLEAN m_bJobPosted;	// TRUE between a job is being queued and processed
	CMclCritSec m_CritSec;	// synchronize access to m_bThreadExists and m_bJobPosted
	DWORD m_dwTimeout;		// Timeout value passed in constructor

	HWND m_hTextWindow;		// handle of the text output window
	DWORD m_numberOfJobs;	// holds the number of submitted jobs. Used only for progress bar.

private:
	unsigned ThreadHandlerProc(void);

public:
    CDPQueue2(DWORD dwTimeout = 1000);
	~CDPQueue2();
    BOOL Stop(void);
    BOOL QueueJob( CJob *pJob);
	BOOL CancelJob(CJob *pJob);
    void CancelAllJobs(void);
	void SetTextWindowHandle(HWND hTextWindow);
	void SetNumberOfJobs(int jobs) {m_numberOfJobs = jobs;}
};

//	Ewald summation handler
//
class CEwaldJob : public CJob {

private:

	CMadelungDoc* m_pDoc;			// pointer to the document object
// atom number for which to execute the job (in the first unitcell)
	int	m_atomNumber;
// atom number for which to execute the job (as in CLattice)
	int m_originalAtomNumber;
	double m_eta;					// eta parameter
	int m_fitFunction;				// fit function parameter
	StringArray* m_pSa;				// StringArray* for text output (thread-safe)
	HWND m_hTextWindow;			// HANDLE to TextWindow
	double m_calculatedEnergy;		// returns the calculated energy

public:

	// constructor...
    CEwaldJob(CMadelungDoc* pDoc,int atomNumber,int originalAtomNumber, double eta,
				int fitFunction,StringArray* pSa,HWND hwndTextWindow);
	virtual ~CEwaldJob();
    void Execute(void);
    void Cancel(void);
};

///////////////////////////////////////////////////////////
//	Optimization Job
//
class COptJob : public CJob {
private:
	int m_paramsToOptimize;	// number of optimization parameters
	double* m_optVariables;	// ponter to the dimension of optimization variables
	StringArray* m_pSa;		// for text output
	HWND m_hTextWindow;		// text output window
	double m_OptError;	// holds optimization error value
public:
	COptJob(int params,double* variables,StringArray* sa,HWND textWindow);
	virtual ~COptJob();
	void Execute(void);
	void Cancel(void);
};


#endif