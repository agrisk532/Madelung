// Ewald.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Madelung.h"
#include "MadelungDoc.h"
#include "StringArray.h"
#include "UnitCell.h"
#include "Spg_dll.h"
#include "AtomProperties.h"
#include "Lattice.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef PI
#define PI 3.141592653589793
#endif

// forward declarations
double fi(double h, double eta, double u, int chargeDistribution);
double fii( double eta,double r,int chargeDistribution);
double erfc(double x);
double fself(double eta,int chargeDistribution);

// Calculates the electrostatic energy at atom 'atomNumber' in UnitCell
double CalculateElStatEnergy(const CMadelungDoc* pDoc,
// corresponding atom number in the first unitcell
							 const int atomNumber,
// atom number as in the CLattice
							 const int originalAtomNumber, 
							 double eta,
							 const int fitFunction,
							 StringArray* pSa,
							 const HWND hTextWindow)
{

	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));
// pointer to UnitCell
	UnitCell* pUC = pDoc->m_pSpaceGroup->m_pUnitCell;
// number of atoms in unit cell (ALL atoms in UnitCell. Wyckoff positions expanded)
	int atomsInUnitCell = pUC->m_atomsInUnitCell;
// pick the index of atom in UnitCell for which to run the calculation
	int index = atomNumber; // (index 'i' in the Heyes paper)

//  FOR TEST PURPOSES ONLY --------------
	eta *= pUC->m_a;
//---------------------------------------

//-----------------------------------------------------------------------------
//	this function calculates the V'(r) part of the electrostatic potential
//  for one atom. Other atoms handled in separate threads also by this function
//	Reciprocal lattice summation
//-----------------------------------------------------------------------------
// how many shells in reciprocal lattice to include in summation
	const int iShellsInReciprocalLattice = 12;
// volume of 4PI/vol(unitcell * 4PI
	double volume = pUC->GetVolume();	// in Angstrom^3
	double c = 4.*PI/volume;
	double Vp = 0.;	// V'(r) part of the potential

	int ishell = iShellsInReciprocalLattice;

		for(int ix=-ishell;ix<=ishell;ix++)
		{
			for(int iy=-ishell;iy<=ishell;iy++)
			{
				for(int iz=-ishell;iz<=ishell;iz++)
				{
					if(ix==0 && iy==0 && iz==0) continue;	// skip |H|==0 item
// skip terms not belonging to 'ishell'
//					if(abs(ix)+abs(iy)+abs(iz) != ishell) continue;
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
						for(i=0;i<3;i++)
							h2 += H[i]*H[i];
						double h = sqrt(h2);	// h = 2PI*|H|
// add term to the V'(r)
						Vp += c*pUC->m_unitCellAtomCartCharge[atom]*
								cos(H[0]*rij[0]+H[1]*rij[1]+H[2]*rij[2])*
								fi(h,eta,r,fitFunction)/h2;
					}
				}
			}
//		}
	}
//-----------------------------------------------------------------------------
//	this function calculates the V''(r) part of the electrostatic potential
//  for one atom. Other atoms handled in separate threads also by this function
//	reciprocal lattice summation
//-----------------------------------------------------------------------------
// how many shells in direct lattice to include in summation
	const int iShellsInDirectLattice = 12;
	double Vpp = 0.;	// V''(r) part of the potential
// we calculate the direct lattice vectors in shells
		ishell = iShellsInDirectLattice;

		for( ix=-ishell;ix<=ishell;ix++)
		{
// proceed with calculation
			for(int iy=-ishell;iy<=ishell;iy++)
			{
				for(int iz=-ishell;iz<=ishell;iz++)
				{
// loop over ALL atoms in UnitCell (Wyckoff positions expanded)
					for(int atom=0;atom<atomsInUnitCell;atom++)
					{
// skip |n|==0 and r i==j item (n' in the paper eq.18)
						if(ix==0 && iy==0 && iz==0 && atom == index)
							continue;
						double rij[3];
// r(i) - r(j) + n
					rij[0] = pUC->m_unitCellAtomCartX[index] - pUC->m_unitCellAtomCartX[atom] +
							 ix*pUC->m_unitCellVectorA_Cart[0] +
							 iy*pUC->m_unitCellVectorB_Cart[0] +
							 iz*pUC->m_unitCellVectorC_Cart[0];
					rij[1] = pUC->m_unitCellAtomCartY[index] - pUC->m_unitCellAtomCartY[atom] +
							 ix*pUC->m_unitCellVectorA_Cart[1] +
							 iy*pUC->m_unitCellVectorB_Cart[1] +
							 iz*pUC->m_unitCellVectorC_Cart[1];
					rij[2] = pUC->m_unitCellAtomCartZ[index] - pUC->m_unitCellAtomCartZ[atom] +
							 ix*pUC->m_unitCellVectorA_Cart[2] +
							 iy*pUC->m_unitCellVectorB_Cart[2] +
							 iz*pUC->m_unitCellVectorC_Cart[2];

						double r=0.;
						for(int i=0;i<3;i++)
							r += rij[i]*rij[i];
						r = sqrt(r);
// add term to the V''(r)
						Vpp += pUC->m_unitCellAtomCartCharge[atom]*
							fii(eta,r,fitFunction);
					}
				}
			}
//		}
	}
//-------------------------------
//	subtract the self energy term
	double selfEnergy = pUC->m_unitCellAtomCartCharge[index]*fself(eta,fitFunction);
	double calculatedEnergy = Vp + Vpp - selfEnergy;
// store result
	pUC->m_unitCellAtomCartPotential[atomNumber] = calculatedEnergy;
	CString str;
	str.Format("Potential on atom %d (%s), number %d in the first unitcell, is %9.5f V'(%9.5f) V''(%9.5f) Vis(%9.5f)",
		originalAtomNumber,pDoc->m_pAtomProperties->m_Type[pUC->m_unitCellAtomCartType[atomNumber]],
		atomNumber,calculatedEnergy,Vp,Vpp,selfEnergy);
	pSa->AddString(str);
	TRACE(str);
	::PostMessage(hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)pSa->Size(),0);
	return calculatedEnergy;
}


//////////////////////////////////////////////////////////////////////////////////
//	this function calculates the f(h,eta,nfunc) part of the potential expansion
//	in the term V'(r)
//	h = 2*PI*|H|
//	eta - parameter from outside
//	chargeDistribtion = 1...9 (choice of the shape of charge distribution function)
//	u - distance between atoms
//
double fi(double h, double eta, double u, int chargeDistribution)
{
	double alpha = eta*h;
	double e;

	switch(chargeDistribution)
	{
	case 1:
		if(u<=eta)
			return 3./(eta*eta*eta)*(sin(alpha)-alpha*cos(alpha))/(h*h*h);
		else
			return 0.;
	case 2:
		if((eta-u)*u<=eta)
			return 3./(PI*eta*eta*eta*eta)*4.*PI*(2.-alpha*sin(alpha)-2.*cos(alpha))/(h*h*h*h);
		else
			return 0;
	case 3:
		if((eta-u)*(eta-u)*u<=eta)
			return 15./(2.*PI*eta*eta*eta*eta*eta)*8.*PI*(alpha*cos(alpha)-3.*sin(alpha)+2.*alpha)/
						(h*h*h*h*h);
		else
			return 0;
	case 4:
		e = eta-u;
		if(e*e*e*u<=eta)
			return 15./(PI*eta*eta*eta*eta*eta*eta)*24.*PI*(alpha*alpha+alpha*sin(alpha)+
						4.*cos(alpha)-4.)/(h*h*h*h*h*h);
		else
			return 0;
	case 5:
		return exp(-alpha);
	case 6:
		return exp(-alpha*alpha/4.);
	case 7:
		e = 1./(eta*eta)+h*h;
		return 1./(eta*eta*eta*eta)/(e*e);
	case 8:
		e = 1./(eta*eta)+h*h;
		return 1./(3.*eta*eta*eta*eta)*(3./(eta*eta)-h*h)/(e*e*e);
	case 9:
		e = 1./(eta*eta)+h*h;
		return 1./(eta*eta*eta*eta*eta*eta)*(1./(eta*eta)-h*h)/(e*e*e*e);
	default:
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	this function calculates the f(r,eta,nfunc) part of the potential expansion
//	in the term V''(r)
//	eta - parameter from outside
//	nfunc = 1...9 (choice of the shape of charge distribution function)
//	r - distance between atoms
//
double fii( double eta,double r,int chargeDistribution)
{
	switch(chargeDistribution)
	{
	case 1:
		if(r<=eta)
			return 1./r+0.5*r*r/(eta*eta*eta)-1.5/eta;
		else
			return 0.;
	case 2:
		if(r<=eta)
			return 1./r+2.*r*r/(eta*eta*eta)-r*r*r/(eta*eta*eta*eta)-2./eta;
		else
			return 0.;
	case 3:
		if(r<=eta)
			return 1./r+5.*r*r/(eta*eta*eta)-5.*r*r*r/(eta*eta*eta*eta)+
					1.5*r*r*r*r/(eta*eta*eta*eta*eta)-2.5/eta;
		else
			return 0.;
	case 4:
		if(r<=eta)
			return 1./r+10.*r*r/(eta*eta*eta)-15.*r*r*r/(eta*eta*eta*eta)+
					9.*r*r*r*r/(eta*eta*eta*eta*eta)-2.*r*r*r*r*r/(eta*eta*eta*eta*eta*eta)-
					3./eta;
		else
			return 0.;
	case 5:
		return 1./r-2./(PI*r)*atan(r/eta);
	case 6:
		return erfc(r/eta)/r;

	case 7:
		return exp(-r/eta)*(1./r+0.5/eta);
	case 8:
		return exp(-r/eta)*(1./r+r/(eta*eta*6.)+2./(3*eta));
	case 9:
		return exp(-r/eta)*(1./r+r*r/(24.*eta*eta*eta)+r/(4.*eta*eta)+3./(4.*eta));
	default:
		return 0.;
	}
}

///////////////////////////////////////////////////////////////////////////
//	computes the erfc(x) function
//
double erfc(double x)
{
    double t, u, y;

    t = 3.97886080735226 / (fabs(x) + 3.97886080735226);
    u = t - 0.5;
    y = (((((((((0.00127109764952614092 * u + 1.19314022838340944e-4) * u - 
        0.003963850973605135) * u - 8.70779635317295828e-4) * u + 
        0.00773672528313526668) * u + 0.00383335126264887303) * u - 
        0.0127223813782122755) * u - 0.0133823644533460069) * u + 
        0.0161315329733252248) * u + 0.0390976845588484035) * u + 
        0.00249367200053503304;
    y = ((((((((((((y * u - 0.0838864557023001992) * u - 
        0.119463959964325415) * u + 0.0166207924969367356) * u + 
        0.357524274449531043) * u + 0.805276408752910567) * u + 
        1.18902982909273333) * u + 1.37040217682338167) * u + 
        1.31314653831023098) * u + 1.07925515155856677) * u + 
        0.774368199119538609) * u + 0.490165080585318424) * u + 
        0.275374741597376782) * t * exp(-x * x);
    return x < 0 ? 2 - y : y;
}

/////////////////////////////////////////////////////////////////////////////
//
double fself(double eta,int chargeDistribution)
{
	switch(chargeDistribution)
	{
	case 1:
		return 1.5/eta;
	case 2:
		return 2./eta;
	case 3:
		return 2.5/eta;
	case 4:
		return 3./eta;
	case 5:
		return 2./(PI*eta);
	case 6:
		return 2./(sqrt(PI)*eta);
	case 7:
		return 1./(2.*eta);
	case 8:
		return 1./(3.*eta);
	case 9:
		return 1./(4.*eta);
	default:
		return 0.;
	}
}
