//////////////////////////////////////////////////
#if !defined(__EL_STAT_ENERGY_THREAD_INFO)
#define __EL_STAT_ENERGY_THREAD_INFO

#include <afxwin.h>
#include <afxcmn.h>
#include <afxext.h>

class CMadelungDoc;
class StringArray;

/////////////////////////////////////////////////////////////////////////////
//	structure for work thread initialization
//
struct ElStatEnergyThreadInfo
{
// document object
	CMadelungDoc* m_pDoc;
// main frame window
	HWND m_hwndNotifyPercentDone;
// text window thread window
	HWND m_hwndTextWindow;
// work thread events
//	HANDLE m_hEventKill;
//	HANDLE m_hEventDead;
//
	int atomIndex;					// atom for which to run the calculation
	double eta;						// eta parameter (as in Heyes paper)
	int chargeDistribution;			// charge distribution function
	StringArray* m_pStringArray;	// string array for text output
	double m_potEnergy;				// calculated potential energy on atom 'atomIndex'
};

#endif