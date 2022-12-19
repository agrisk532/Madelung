///////////////////////////////////////////////////////////////////////

#include "CalculateElStatEnergy.h"
#include "Madelung.h"
#include "MadelungDoc.h"
#include "StringArray.h"
#include <math.h>

extern StringArray g_TextOutput;
const double PI = 3.141592653589793;

UINT CalculateElStatEnergy(LPVOID pParam)
{
	CString txt;

	ElStatEnergyThreadInfo* pP = (ElStatEnergyThreadInfo*) pParam;
	ASSERT(pP->m_pDoc->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));
// percent done notify window (CMainFrame)
	HWND wndNotify = pP->m_hwndNotifyPercentDone;
// text window thread window
	HWND wndText = pP->m_hwndTextWindow;
// pointer to UnitCell
	UnitCell* pUC = pP->pDoc->m_pSpaceGroup->m_pUnitCell;
// number of atoms in unit cell
	int atomsInUnitCell = pUC->m_atomsInUnitCell;

// set the calculation event
	SetEvent(pP->m_hEventCalculationInProgress);

//	this function calculates the V'(r) part of the electrostatic potential
//  for one 'r'. Other 'r' handled in separate threads.
//	reciprocal lattice summation
//
// how many shells in reciprocal lattice to include in summation
	int iShellsInReciprocalLattice = 3;
// pick the index of atom in UnitCell for which to run the calculation
	int index = pP->atomIndex; // (index 'i' in the Heyes paper)
// volume of 4PI/vol(unitcell * 4PI
	double c = 4.*PI/pUC->GetVolume();
	double Vp = 0;	// V'(r) part of the potential
// we calculate the reciprocal lattice vectors in shells
// shell == |ix|+|iy|+|iz|. Shell == 0 is omitted
	for(int ishell=1;ishell<=iShellsInReciprocalLattice;ishell++)
	{
		for(int ix=-ishell;ix<=ishell;ix++)
		{
// kill thread if event signalled
			if(WaitForSingleObject(pP->m_hEventKillWorkThread,0) == WAIT_OBJECT_0)
			{
				SetEvent(pP->m_hEventWorkThreadKilled);
				ResetEvent(pP->m_hEventCalculationInProgress);
				break;
			}
// proceed with calculation
			for(int iy=-ishell;iy<=ishell;iy++)
				for(int iz=-ishell;iz<=ishell;iz++)
				{
					if(ix==0 && iy==0 && iz==0) continue;	// skip |H|==0 item
// skip terms not belonging to 'ishell'
					if(abs(ix)+abs(iy)+abs(iz) != ishell) continue;
// loop over ALL atoms in UnitCell (Wyckoff positions expanded)
					for(int atom=0;atom<atomsInUnitCell;atom++)
					{
						double rij[3];
// r(i) - r(j)
						rij[0] = pUC->m_unitCellAtomCartX[index] - pUC->m_unitCellAtomCartX[atom];
						rij[1] = pUC->m_unitCellAtomCartY[index] - pUC->m_unitCellAtomCartY[atom];
						rij[2] = pUC->m_unitCellAtomCartZ[index] - pUC->m_unitCellAtomCartZ[atom];
						double r=0;
						for(int i=0;i<3;i++)
							r += rij[i]*rij[i];
						r = sqrt(r);
// calculate the reciprocal lattice vector
						double H[3];
						H[0] =  2.*PI*(ix*pUC->m_recLatticeVectorA[0]+
									iy*pUC->m_recLatticeVectorB[0]+
									iz*pUC->m_recLatticeVectorC[0]);
						H[1] =  2.*PI*(ix*pUC->m_recLatticeVectorA[1]+
								 	iy*pUC->m_recLatticeVectorB[1]+
						 			iz*pUC->m_recLatticeVectorC[1]);
						H[2] =  2.*PI*(ix*pUC->m_recLatticeVectorA[2]+
									iy*pUC->m_recLatticeVectorB[2]+
									iz*pUC->m_recLatticeVectorC[2]);
						double h2=0;	// h2=4*PI*PI*H*H
						for(int i=0;i<3;i++)
							h2 += H[i]*H[i];
						h = sqrt(h2);	// h = 2PI*|H|
// add term to the V'(r)
						Vp += c*pUC->m_unitCellAtomCartCharge[atom]*
								cos(H[0]*rij[0]+H[1]*rij[1]+H[2]*rij[2])*
								fi(h,pP->eta,r,pP->ChargeDistribution)/h2;
					}
				}
	}









	
//////////////////////////////////////////////////////////////////////////////////
	for (int I = 0; I < 40; I++)
	{
// kill thread if event signalled
		if(WaitForSingleObject(pP->m_hEventKillWorkThread,0) == WAIT_OBJECT_0)
		{
			SetEvent(pP->m_hEventWorkThreadKilled);
			ResetEvent(pP->m_hEventCalculationInProgress);
			break;
		}

		for(int j=0;j<20000000;j++);
			txt.Format("iteration %d",I);
			g_TextOutput.m_pStrings.push_back(txt);
			int size = g_TextOutput.m_pStrings.size();
			::PostMessage(wndNotify,WM_USER_CALC_IN_PROGRESS,I+1,0);
// sends the total number of lines in the StringArray
			::PostMessage(wndText,WM_USER_TEXT_WINDOW_ADD_LINE,size,0);
	}



	::PostMessage(wndNotify,WM_USER_DESTROY_PROGRESS_BAR,0,0);
	::PostMessage(wndNotify,WM_USER_TOGGLE_STATUS_BAR,0,0);
	::PostMessage(wndNotify,WM_USER_MODIFY_MAINFRAME_MENU,0,0);
	ResetEvent(pP->m_hEventCalculationInProgress);
	return 0;
}


///////////////////////////////////////////////////////////////////////////
//	this function calculates the V'(r) part of the electrostatic potential
//  for one 'r'. Other 'r' handled in separate threads.
//	reciprocal lattice summation
//
void Vp(LPVOID pParam)
{
	ElStatEnergyThreadInfo* pP = (ElStatEnergyThreadInfo*)pParam;
	ASSERT(pP->m_pDoc->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));

	CMadelungDoc* pDoc = pP->pDoc;
	UnitCell* pUC = pDoc->m_pSpaceGroup->m_pUnitCell;
// number of atoms in unit cell
	int atomsInUnitCell = pUC->m_atomsInUnitCell;
// how many shells in reciprocal lattice to include in summation
	int iShellsInReciprocalLattice = 3;
// pick the index of atom in UnitCell for which to run the calculation
	int index = pP->atomIndex; // (index 'i' in the Heyes paper)
// volume of 4PI/vol(unitcell * 4PI
	double c = 4PI/pUC->GetVolume();
	double Vp = 0;	// V'(r) part of the potential
// we calculate the reciprocal lattice vectors in shells
// shell == |ix|+|iy|+|iz|. Shell == 0 is omitted
	for(int ishell=1;ishell<=iShellsInReciprocalLattice;ishell++)
	{
		for(int ix=-ishell;ix<=ishell;ix++)
			for(int iy=-ishell;iy<=ishell;iy++)
				for(int iz=-ishell;iz<=ishell;iz++)
				{
					if(ix==0 && iy==0 && iz==0) continue;	// skip |H|==0 item
// skip terms not belonging to 'ishell'
					if(abs(ix)+abs(iy)+abs(iz) != ishell) continue;
// loop over ALL atoms in UnitCell (Wyckoff positions expanded)
					for(int atom=0;atom<atomsInUnitCell;atom++)
					{
						double rij[3];
// r(i) - r(j)
						rij[0] = pUC->m_unitCellAtomCartX[index] - pUC->m_unitCellAtomCartX[atom];
						rij[1] = pUC->m_unitCellAtomCartY[index] - pUC->m_unitCellAtomCartY[atom];
						rij[2] = pUC->m_unitCellAtomCartZ[index] - pUC->m_unitCellAtomCartZ[atom];
						double r=0;
						for(int i=0;i<3;i++)
							r += rij[i]*rij[i];
						r = sqrt(r);
// calculate the reciprocal lattice vector
						double H[3];
						H[0] =  2.*PI*(ix*pUC->m_recLatticeVectorA[0]+
									iy*pUC->m_recLatticeVectorB[0]+
									iz*pUC->m_recLatticeVectorC[0]);
						H[1] =  2.*PI*(ix*pUC->m_recLatticeVectorA[1]+
								 	iy*pUC->m_recLatticeVectorB[1]+
						 			iz*pUC->m_recLatticeVectorC[1]);
						H[2] =  2.*PI*(ix*pUC->m_recLatticeVectorA[2]+
									iy*pUC->m_recLatticeVectorB[2]+
									iz*pUC->m_recLatticeVectorC[2]);
						double h2=0;	// h2=4*PI*PI*H*H
						for(int i=0;i<3;i++)
							h2 += H[i]*H[i];
						h = sqrt(h2);	// h = 2PI*|H|
// add term to the V'(r)
						Vp += c*pUC->m_unitCellAtomCartCharge[atom]*
								cos(H[0]*rij[0]+H[1]*rij[1]+H[2]*rij[2])*
								f(h,pP->eta,r,pP->ChargeDistribution)/h2;
					}
				}
	}
}
