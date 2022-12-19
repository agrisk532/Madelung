//////////////////////////////////////////////////////////////////////////////
//
// Deferred Processing Queue object to handle the job submission and execution
// on a separate thread

#include "stdafx.h"
#include "DPQ1.h"
#include "Mcl4Mfc.h"
#include "MadelungDoc.h"
#include "StringArray.h"
#include "Madelung.h"
#include "MainFrm.h"
#include "Lattice.h"
#include "AtomProperties.h"
#include "Spg_dll.h"
#include <math.h>

// Ewald summation
extern double CalculateElStatEnergy(const CMadelungDoc* pDoc,
							 const int atomNumber,
							 const int originalAtomNumber,
							 double eta,
							 const int fitFunction,
							 StringArray* pSa,
							 const HWND hTextWindow);

extern StringArray g_TextOutput;	// holds text output
extern CMadelungDoc* g_pDoc;		// global pointer to CMadelungDoc. Not thread-safe
// calculates el.stat. potential on selected atom i from all unselected atoms in pLattice
// x[] holds optimized charges of all selected atoms
extern double ElStatPotentialOnAtom(int i,CLattice* pLattice,double x[]);
// optimizator
extern "C" __declspec(dllimport) double solvopt(unsigned short,double*,double (*f)(double x[]),
												void (*g)(),double*,double (*func)(),
												double (*gradc)());
// function to optimize in optimizator
extern "C" double f_opt(double x[]);

//-----------------------------------------------------------------------

BOOL CJobList::QueueJob( CJob *pJob) {
        return PutOnTailOfList(pJob);
}

BOOL CJobList::DequeueJob( CJob **pJob, CMclEvent *pInterrupt, DWORD dwTimeout) {
        return GetFromHeadOfList( *pJob, dwTimeout, pInterrupt);
}

BOOL CJobList::RemoveJob( CJob *pJob) {
        // decrease the semaphore count...
        // if the wait fails, there are no jobs in the queue...
        if (CMclWaitTimeout(m_csNotEmpty.Wait(0))) {
            return FALSE;
        }

        // acquire the critical section lock...
        CMclAutoLock autoLock(m_cCritSec);

        // search the list for a matching job...
        CMclLinkedListNode *pNode = m_MasterNode.m_pNext;
        while (pNode != &m_MasterNode) {
            // since pNode is not the master node, we know that
            // it points to a data node, so this cast is safe...
            if ((static_cast<CMclLinkedListDataNode *>(pNode))->m_data == pJob)
                break;
        }

        // if we found a match, remove it...
        if (pNode != &m_MasterNode) {
            // remove the node...
            pNode->m_pPrev->m_pNext = pNode->m_pNext;
            pNode->m_pNext->m_pPrev = pNode->m_pPrev;

            // add the list node to the free list...
            AddToFreeList(pNode);

            // return TRUE when we remove a node...
            return TRUE;
        }
        else {
            // return FALSE when there is no matching
            // node to remove, we need to bump the semaphore
            // count back up...
            m_csNotEmpty.Release(1);
            return FALSE;
        }
}


unsigned CDPQueue2::ThreadHandlerProc(void) {
			CMainFrame* pF = (CMainFrame*) AfxGetApp()->m_pMainWnd;
			HWND wnd = pF->m_hWnd;
			CJob *pJob;
			int jobsProcessed = 0;
            while (TRUE) {
				if(m_cJobList.DequeueJob( &pJob, &m_ceControl, m_dwTimeout)) {
				   pJob->Execute();
				   jobsProcessed++;
// POST Messages but not SEND, to avoid deadlock
// update the progress bar
					WPARAM jp = (WPARAM) 100/m_numberOfJobs*jobsProcessed;
					::SendMessage(wnd,WM_USER_CALC_IN_PROGRESS,jp,0);

				   CMclAutoLock lock(m_CritSec);
				   m_bJobPosted = FALSE;
				}
				else {
					CMclAutoLock lock(m_CritSec);
					if(!m_bJobPosted) {
						m_bThreadExists = FALSE;
						break;
					}
				}
			}

	CString str;
	str.Format("Worker Thread exiting");
	g_TextOutput.AddString(str);
	::PostMessage(this->m_hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)g_TextOutput.Size(),0);
// POST Messages but not SEND, to avoid deadlock
// remove progress bar
	::PostMessage(wnd,WM_USER_DESTROY_PROGRESS_BAR,0,0);
// reset the indicators pane to the default value
	::PostMessage(wnd,WM_USER_TOGGLE_STATUS_BAR,0,0);
// modify menu
	::PostMessage(wnd,WM_USER_MODIFY_MAINFRAME_MENU,0,0);
// post message to MainFrame window about the status of job
	if(jobsProcessed == m_numberOfJobs) // normal exit, all jobs done
		::PostMessage(wnd,WM_USER_POTENTIALS_OK,1,0);
	else
		::PostMessage(wnd,WM_USER_POTENTIALS_OK,0,0);	// jobs canceled, not all done
    return 0;
}

CDPQueue2::CDPQueue2(DWORD dwTimeout) {
	m_bThreadExists = FALSE;
	m_bJobPosted = FALSE;
	m_dwTimeout = dwTimeout;
    return;
}

CDPQueue2::~CDPQueue2() {
	m_ceControl.Set();
    if (m_apWorkerThread.IsNull() == FALSE)
		m_apWorkerThread->Wait(INFINITE);
}


void CDPQueue2::SetTextWindowHandle(HWND hTextWindow) {
	m_hTextWindow = hTextWindow;
}


BOOL CDPQueue2::Stop(void) {
        // check for redundant stop...
        if (m_apWorkerThread.IsNull() == TRUE) {
            return FALSE;
        }

        // signal the worker thread to stop and wait
        // for it to exit...
        m_ceControl.Set();
// only to respond quicker
		m_apWorkerThread->SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
        m_apWorkerThread->Wait(INFINITE);
		this->CancelAllJobs();

        printf("DPQ1 thread exited.\n");
        return TRUE;
}

BOOL CDPQueue2::QueueJob( CJob *pJob) {
	BOOL bStatus = m_cJobList.QueueJob(pJob);
	// limit scope to hold auto-lock
	{
		CMclAutoLock lock(m_CritSec);
		m_bJobPosted = TRUE;
		if(!m_bThreadExists) {
			// we need to create a worker thread
			m_bThreadExists = TRUE;
			m_apWorkerThread = new CMcl4MfcWorkerThread(this);
			m_apWorkerThread->SetPriority(THREAD_PRIORITY_BELOW_NORMAL);
	CString str;
	str.Format("Worker Thread created");
	g_TextOutput.AddString(str);
	::PostMessage(this->m_hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)g_TextOutput.Size(),0);

		}
	}
	return bStatus;
}

BOOL CDPQueue2::CancelJob( CJob *pJob) {
        // remove a particular job from the queue...
        if (m_cJobList.RemoveJob(pJob) == TRUE) {
            pJob->Cancel();
            return TRUE;
        }
        else {
            return FALSE;
        }
}

void CDPQueue2::CancelAllJobs(void) {
        // remove all jobs from the queue...
        CJob *pJob;
        while (m_cJobList.DequeueJob( &pJob, NULL, 0)) {
            pJob->Cancel();
        }
}

//	Ewald summation handler
//
CEwaldJob::CEwaldJob(CMadelungDoc* pDoc,int atomNumber,int originalAtomNumber,double eta,
					 int fitFunction,StringArray* pSa,HWND hwndTextWindow) :
				m_pDoc(pDoc),
				m_atomNumber(atomNumber),
				m_originalAtomNumber(originalAtomNumber),
				m_eta(eta),
				m_fitFunction(fitFunction),
				m_pSa(pSa),
				m_hTextWindow(hwndTextWindow)
{
}

// pure virtual destructor...
CEwaldJob::~CEwaldJob() {
}

void CEwaldJob::Execute(void) {
// executes a job...It also stores the result in pDoc->SpaceGroup->UnitCell
// and sends to TextWindow for output
		m_calculatedEnergy = CalculateElStatEnergy(m_pDoc,m_atomNumber,m_originalAtomNumber,
								m_eta,m_fitFunction,m_pSa,m_hTextWindow);
// jobs delete themselves when complete...
        delete this;
}

void CEwaldJob::Cancel(void) {
// jobs delete themselves when canceled...
        delete this;
}

///////////// Optimization job ///////////////////
//	optimizations on atoms are run for one of the selected atoms at a time
COptJob::COptJob(int n,double* x,StringArray* sa,HWND hTextWnd) :
	m_paramsToOptimize(n),
	m_optVariables(x),
	m_pSa(sa),	// for text output
	m_hTextWindow(hTextWnd)		// text output window handle
{
}

// pure virtual destructor...
COptJob::~COptJob() {
//	delete [] m_optVariables;	// allocated in CMadelungDoc::OnOptimize()
}

////////////////////////////////////////////////////////////////////////////////
// OptJob executable
//
void COptJob::Execute(void)
{
// open a file for text output
	FILE* fp;
	fp = fopen("OptimOut.txt","w");

	CLattice* pLattice = g_pDoc->m_pLattice;
	int size = pLattice->GetSize();
	UnitCell* pUC = g_pDoc->m_pSpaceGroup->m_pUnitCell;
	double pot;
//	Calculates the electrostatic potential on all selected atoms from all unselected atoms.
//	m_optVariables at this point contain initial charges of atoms,
//  given in SpaceGroup property page

// print out initial charges and potentials
	for(int i=0;i<size;i++)
	{
		fprintf(fp,"Initial charge and potential on atom %d (%s) ",i,
		g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[i]]);
		fprintf(fp," %9.5lf  %9.5lf\n",pLattice->m_charge[i],
			pUC->m_unitCellAtomCartPotential[pLattice->m_atomInUnitCell[i]]);
	}

// NOT USING SYMMETRY

	if(!g_pDoc->m_bUseSymmetry)
	{
		int selAtoms = g_pDoc->GetNumberOfSelectedAtoms();
		for(i=0;i<selAtoms;i++)
		{
			int atom = g_pDoc->m_optimized_charges->m_selected_atoms[i];
			pot = ElStatPotentialOnAtom(atom,pLattice,m_optVariables);
			ASSERT(fabs(pot) < 100.);
			CString str;
			str.Format(" Initial potential on atom %d (%s) is %9.5lf",atom,
			g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[atom]],pot);
			m_pSa->AddString(str);
			::PostMessage(m_hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)m_pSa->Size(),0);
			TRACE0(str+"\n");

			fprintf(fp,"Initial potential on atom %d (%s) is %9.5lf\n",atom,
			g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[atom]],pot);
		}
		int reply = AfxMessageBox("Continue?",MB_OKCANCEL);
		if(reply == IDCANCEL)
		{
			fclose(fp);
			_execl("del","OptimOut.txt");
			return;
		}

// executes a job.
		double solvopt_options[13];
// use default options built in the solvopt code, if we specify all options as zeros.
// we don't use gradient routine for optimization
// f_opt calculates el.stat potential error given charges in m_optVariables
		for(i=0;i<13;i++) solvopt_options[i] = 0.;
		m_OptError = solvopt(m_paramsToOptimize,m_optVariables,f_opt,NULL,
							solvopt_options,NULL,NULL);

// trace out charges
		fprintf(fp,"\nOptimization error = %8.5lf\n",m_OptError);
//	TRACE2("Function Evaluations = %d+%d\n",solvopt_options[9],solvopt_options[10]);
//	TRACE1("Iterations = %d\n",solvopt_options[8]);
// Output optimized el.stat potential on selected atoms in the TextWindow
//	n == number of optimizable parameters - charges of unselected atoms in the CLattice object
//	for(i=0;i<size;i++)
//	{
//		if(!pLattice->m_selected[i])
//			continue;
		for(i=0;i<selAtoms;i++)
		{
			int atom = g_pDoc->m_optimized_charges->m_selected_atoms[i];
			pot = ElStatPotentialOnAtom(atom,pLattice,m_optVariables);
			ASSERT(fabs(pot) < 100.);
			CString str;
			str.Format(" Final potential on atom %d (%s) is %9.5lf",atom,
			g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[atom]],pot);
			m_pSa->AddString(str);
			::PostMessage(m_hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)m_pSa->Size(),0);
			TRACE0(str+"\n");
			fprintf(fp,"Final potential on atom %d (%s) is %9.5lf\n",atom,
			g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[atom]],pot);
		}
// sends message for the main thread that this job has ended.
// main thread will count those messages and if the last job has been executed,
// will average point charges and calculate electrostatic potential on all selected atoms
			AfxGetApp()->m_pMainWnd->PostMessage(WM_USER_OPTJOB_DONE,(unsigned int)fp,0);
//	}
// store optimized charges in the CLattice object
//	int counter = 0;
//	for(i=0;i<size;i++)
//	{
//		if(i == m_selected_atom)
//			continue;
// atom not selected. We update it's charge with optimized value
//		pLattice->m_charge[i] = m_optVariables[counter++];
//	}
// output lattice charges in the debug window
//	for(i=0;i<size;i++)
//		TRACE3("Charge on %d (%s) is %8.5lf\n",i,
//		g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[i]],pLattice->m_charge[i]);
// jobs delete themselves when complete...
// destructor deletes also 'm_optVariables' allocated in CMadelungDoc::OnOptimize()
		delete this;
	}

// USING SYMMETRY

	else
	{
		CString str;
		str.Format(" USING SYMMETRY IN OPTIMIZATION");
		m_pSa->AddString(str);
		::PostMessage(m_hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)m_pSa->Size(),0);
		fprintf(fp," USING SYMMETRY IN OPTIMIZATION\n");

		int selAtoms = g_pDoc->GetNumberOfSelectedSymmIndependentAtoms();
		for(i=0;i<selAtoms;i++)
		{
			int atom = g_pDoc->m_optimized_charges->m_selected_atoms[i];
// 'm_optVariables' contain here ALL symmetry independent atoms (selected and unselected)
			pot = ElStatPotentialOnAtom(atom,pLattice,m_optVariables);
			ASSERT(fabs(pot) < 100.);
			str.Format(" Initial potential on atom %d (%s) is %9.5lf",atom,
			g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[atom]],pot);
			m_pSa->AddString(str);
			::PostMessage(m_hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)m_pSa->Size(),0);
			TRACE0(str+"\n");

			fprintf(fp," Initial potential on atom %d (%s) is %9.5lf\n",atom,
			g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[atom]],pot);
		}
		int reply = AfxMessageBox("Continue?",MB_OKCANCEL);
		if(reply == IDCANCEL)
		{
			fclose(fp);
			_execl("del","OptimOut.txt");
			return;
		}

// executes a job.
		double solvopt_options[13];
// use default options built in the solvopt code, if we specify all options as zeros.
// we don't use gradient routine for optimization
// f_opt calculates el.stat potential error given charges in m_optVariables
		for(i=0;i<13;i++) solvopt_options[i] = 0.;
		m_OptError = solvopt(m_paramsToOptimize,m_optVariables,f_opt,NULL,
							solvopt_options,NULL,NULL);
// trace out charges
		TRACE1("\nOptimization error = %8.5lf\n",m_OptError);
//	TRACE2("Function Evaluations = %d+%d\n",solvopt_options[9],solvopt_options[10]);
//	TRACE1("Iterations = %d\n",solvopt_options[8]);
// Output optimized el.stat potential on selected atoms in the TextWindow
//	n == number of optimizable parameters - charges of unselected atoms in the CLattice object
//	for(i=0;i<size;i++)
//	{
//		if(!pLattice->m_selected[i])
//			continue;
		for(i=0;i<selAtoms;i++)
		{
			int atom = g_pDoc->m_optimized_charges->m_selected_atoms[i];
			pot = ElStatPotentialOnAtom(atom,pLattice,m_optVariables);
			ASSERT(fabs(pot) < 100.);
			CString str;
			str.Format(" Final potential on atom %d (%s) is %9.5lf",atom,
			g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[atom]],pot);
			m_pSa->AddString(str);
			::PostMessage(m_hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)m_pSa->Size(),0);
			TRACE0(str+"\n");

			fprintf(fp,"Final potential on atom %d (%s) is %9.5lf\n",atom,
			g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[atom]],pot);
		}
// sends message for the main thread that this job has ended.
// main thread will count those messages and if the last job has been executed,
// will average point charges and calculate electrostatic potential on all selected atoms
			AfxGetApp()->m_pMainWnd->PostMessage(WM_USER_OPTJOB_DONE,(unsigned int)fp,0);
//	}
// store optimized charges in the CLattice object
//	int counter = 0;
//	for(i=0;i<size;i++)
//	{
//		if(i == m_selected_atom)
//			continue;
// atom not selected. We update it's charge with optimized value
//		pLattice->m_charge[i] = m_optVariables[counter++];
//	}
// output lattice charges in the debug window
//	for(i=0;i<size;i++)
//		TRACE3("Charge on %d (%s) is %8.5lf\n",i,
//		g_pDoc->m_pAtomProperties->m_Type[pLattice->m_type[i]],pLattice->m_charge[i]);
// jobs delete themselves when complete...
// destructor deletes also 'm_optVariables' allocated in CMadelungDoc::OnOptimize()
		delete this;
	}
}

void COptJob::Cancel(void) {
// jobs delete themselves when canceled...
        delete this;
}
