// MadelungDoc.cpp : implementation of the CMadelungDoc class
//

#include "stdafx.h"
#include "MadelungDoc.h"
#include "Madelung.h"
#include "Lattice.h"
#include "MainFrm.h"
#include "MadelungView.h"
#include "AtomProperties.h"
#include "TextWindowThread.h"
#include "spg_names.h"
#include "Spg_dll.h"
#include "DPQ1.h"
#include "SymmetryDialog.h"
#include "PointGroupIrreps.h"
#include "Molecule.h"
#include <math.h>
#include <fstream>
#include <string>

#include <vector>
#include <list>
using namespace std;

#define DISTANCE_TOLERANCE 0.00001
#define PI 3.141592653589793

extern StringArray g_TextOutput;
CMadelungDoc* g_pDoc;

// calculates el.stat. potential on selected atom i from all unselected atoms in pLattice
// x[] holds optimized charges of all selected atoms
extern double ElStatPotentialOnAtom(int i,CLattice* pLattice,double x[]);
// conversion factor from angstroms to bohrs(atomic units)
#define BOHR 0.529178

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMadelungDoc

IMPLEMENT_DYNCREATE(CMadelungDoc, CDocument)

BEGIN_MESSAGE_MAP(CMadelungDoc, CDocument)
	//{{AFX_MSG_MAP(CMadelungDoc)
	ON_COMMAND(ID_TOOLS_POTENTIAL_ON_ATOMS, OnToolsPotentialOnAtoms)
	ON_COMMAND(ID_CANCEL_CALCULATION, OnCancelCalculation)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_COMMAND(ID_OPTIMIZE, OnOptimize)
	ON_UPDATE_COMMAND_UI(ID_OPTIMIZE, OnUpdateOptimize)
	ON_COMMAND(ID_TOOLS_SYMMETRY, OnToolsSymmetry)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SYMMETRY, OnUpdateToolsSymmetry)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_COMPUTE_DECOMPOSE, OnComputeDecompose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMadelungDoc construction/destruction

CMadelungDoc::CMadelungDoc()
{
// initialize global pointer to the document object
	g_pDoc = this;

	m_pSpaceGroup = NULL;	// no default space group
	m_PotentialsCalculatedOK = false;
	m_FitFunction = 7; // exponential 'Birman function'

	m_pLattice = new CLattice();
	m_pAtomView = NULL;
	m_pAtomProperties = new AtomProperties();

	m_latticeAsSphere = true;	// shows whether the lattice is extended as sphere or brick
	m_extend_a = 1;		// cluster extension along a
	m_extend_b = 1;		// cluster extension along b
	m_extend_c = 1;		// cluster extension along c
	m_extend_sphere = 2;	// extends spherically (r=ia+jb+kc;i+j+k=m_extend_sphere)

	m_NumberOfUnitCellsForPlotAlongA = 1;	// number of unit cells for plot
	m_NumberOfUnitCellsForPlotAlongB = 1;
	m_NumberOfUnitCellsForPlotAlongC = 1;

	m_atomsInCluster = 0;
	m_atomsInExtension = 0;
	m_latticeExtended = false;
	m_numberOfUnitCells = 0;
	m_selectedAtoms = 0;

	m_IsFirstDocument = true;	// enable/disable menu item

	m_eta = 0.18;
// create new deferred processing queue
	m_queue = new CDPQueue2(10);
// pointer to the OptimizedCharges object
// (used to keep the optimized charges for every of the selected atoms
	m_optimized_charges = NULL;
// how many charge optimization jobs are done
	m_optJobsDone = 0;

	m_bCalculationInProgress = FALSE;
// 'save as' file dialog
	m_pFileDialog = new CFileDialog(FALSE);
//	Refined Symmetry elements from Symmetry.dll, contains only site symmetry elements
	m_RefinedElements = NULL;
	m_numberOfRefinedSymmetryElements = 0;
// true if a Wyckoff position symmetry elements found and can be used to extend lattice
	m_bSymmetryElementsFound = false;
// set to true if to use symmetry in lattice extensions and charge optimizations
	m_bUseSymmetry = false;
// atomsOnSymmetryElements
	m_pAtomsOnSymElem = new atomsOnSymmetryElements;
// the point group of the selected atoms cluster
	m_siteSymmetry = "";
// Point group irreps
	m_pgi = NULL;
// Stores info about the site symmetry irreps of the 's','p','d' functions.
// Used for the MOLCAS SCF module 'OCCUPIED' keyword.
	m_siteSymmetryIrreps = "";
// Molecules in unitcell
	m_currentMolecule = 0; // molecule, opened in the EditMolecule

}

CMadelungDoc::~CMadelungDoc()
{
	if(m_pLattice) delete m_pLattice;
	if(m_pAtomProperties) delete m_pAtomProperties;
	if(m_pAtomView) delete m_pAtomView;
	if(m_pSpaceGroup) delete m_pSpaceGroup;
	if(m_queue) delete m_queue;
	if(m_pFileDialog) delete m_pFileDialog;
	if(m_pAtomsOnSymElem) delete m_pAtomsOnSymElem;
	if(m_pgi) delete m_pgi;
// erase molecules in unitcell
	int size = m_molecules.size();
	if(size)
	{
		for(int i=0;i<size;i++)
			delete m_molecules[i];
		m_molecules.clear();
	}
}
////////////////////////////////////////////////////////////////////
//

BOOL CMadelungDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	if(!m_IsFirstDocument)
	{
		int reply = AfxMessageBox("All previous settings will be lost",MB_OKCANCEL);
		if(reply == IDCANCEL)
			return FALSE;
	}

	if(m_pLattice) m_pLattice->Clear();
	if(m_pAtomsOnSymElem) m_pAtomsOnSymElem->Clear();
	if(m_pgi) delete m_pgi;
// erase molecules in unitcell
	int size = m_molecules.size();
	if(size)
	{
		for(int i=0;i<size;i++)
			delete m_molecules[i];
		m_molecules.clear();
	}
// Molecules in unitcell
	m_currentMolecule = 0; // molecule, opened in the EditMolecule

// clear UnitCell data
	if(this->m_pSpaceGroup) delete this->m_pSpaceGroup;
	m_pSpaceGroup = NULL;	// no default space group

	m_pAtomView = NULL;		// no default AtomView (containing Wyckoff position coordinates)
	m_PotentialsCalculatedOK = false;
	m_FitFunction = 7;
	m_latticeAsSphere = true;
	m_extend_a = 1;
	m_extend_b = 1;
	m_extend_c = 1;
	m_extend_sphere = 1;
	m_atomsInCluster = 0;
	m_atomsInExtension = 0;
	m_latticeExtended = false;
	m_numberOfUnitCells = 0;

	m_NumberOfUnitCellsForPlotAlongA = 1;	// number of unit cells for plot
	m_NumberOfUnitCellsForPlotAlongB = 1;
	m_NumberOfUnitCellsForPlotAlongC = 1;

	m_bUseSymmetry = false;
// the point group of the selected atoms cluster
	m_siteSymmetry = "";
// Stores info about the site symmetry (geom center of selected atoms)irreps of the 's','p','d' functions.
// Used for the MOLCAS SCF module 'OCCUPIED' keyword. Used in File()
	m_siteSymmetryIrreps = "";

	if(!m_IsFirstDocument)
	{
		this->SetMenu(IDR_START);
	}
	m_IsFirstDocument = false;
	m_bCalculationInProgress = FALSE;
//	m_pElStatEnergyThread = NULL;	// no work thread at the beginning

	POSITION pos = GetFirstViewPosition();
	CMadelungView* pv = (CMadelungView*)GetNextView(pos);
	pv->Clear();
	pv->UpdateWindow();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMadelungDoc serialization

void CMadelungDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMadelungDoc diagnostics

#ifdef _DEBUG
void CMadelungDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMadelungDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMadelungDoc commands
void CMadelungDoc::CalculateUnitCellVertices()
{
//	calculate the unit cell vectors, they are used in atom coordinate calculations
	if(this->m_pSpaceGroup)
	{
// calculate UnitCell vectors
		this->m_pSpaceGroup->m_pUnitCell->CalculateVectors();
// calculate UnitCell reciprocal lattice vectors
		this->m_pSpaceGroup->m_pUnitCell->CalculateReciprocalLatticeVectors();
// calculate cartesian coordinates of ALL atoms in one unit cell (expand Wyckoff positions).
// They are stored now in this->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCartXYZ[]
		if(this->m_pSpaceGroup->CalculateUnitCellAtoms() != 0)
		{
// some atoms of the same Wyckoff position has the same coordinates
			CString str(this->m_pSpaceGroup->m_outputText);
			AfxMessageBox(str,MB_OK);
		}
// calculate a unitcell vertices (for unitcell border plots)
		this->m_pSpaceGroup->m_pUnitCell->CalculateVertices();
// if molecules have been set up, include them into the unitcell
		int size = m_molecules.size();
		if(size)
		{
			for(int i=0;i<size;i++)
				this->AddMoleculeToUnitCell(m_molecules[i]);
		}
	}
	else
	{
		AfxMessageBox("No space group selected",MB_OK);
		return;
	}
//	Now we must store coordinates in this->m_pLattice, according to the number of unit cells
// chosen for plot
	int na = this->m_NumberOfUnitCellsForPlotAlongA;
	int nb = this->m_NumberOfUnitCellsForPlotAlongB;
	int nc = this->m_NumberOfUnitCellsForPlotAlongC;
// add atoms to the lattice
	for(int i=0;i<nc;i++)
		for(int j=0;j<nb;j++)
			for(int k=0;k<na;k++)
				this->AddUnitCellToLattice(k,j,i);

	this->m_atomsInCluster = this->m_pLattice->GetSize();
	this->m_atomsInExtension = 0;
	this->m_latticeExtended = false;
	this->m_PotentialsCalculatedOK = true;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMadelungDoc::DeleteLatticeExtension()
{
	if(!m_atomsInExtension)
	{
//		AfxMessageBox("No extension to delete in CMadelungDoc",MB_OK);
		return;
	}
// there are atoms in the extension, let's delete them
	vector<double>::iterator pd;
	vector<int>::iterator pi;
	vector<bool>::iterator pb;
	vector<char>::iterator pc;

	if(this->m_pLattice)
	{
		pd = m_pLattice->m_X.begin()+m_atomsInCluster;
		m_pLattice->m_X.erase(pd,pd+m_atomsInExtension);

		pd = m_pLattice->m_Y.begin()+m_atomsInCluster;
		m_pLattice->m_Y.erase(pd,pd+m_atomsInExtension);

		pd = m_pLattice->m_Z.begin()+m_atomsInCluster;
		m_pLattice->m_Z.erase(pd,pd+m_atomsInExtension);

		pd = m_pLattice->m_charge.begin()+m_atomsInCluster;
		m_pLattice->m_charge.erase(pd,pd+m_atomsInExtension);

		pi = m_pLattice->m_type.begin()+m_atomsInCluster;
		m_pLattice->m_type.erase(pi,pi+m_atomsInExtension);

		pc = m_pLattice->m_WPosition.begin()+m_atomsInCluster;
		m_pLattice->m_WPosition.erase(pc,pc+m_atomsInExtension);

		pb = m_pLattice->m_selected.begin()+m_atomsInCluster;
		m_pLattice->m_selected.erase(pb,pb+m_atomsInExtension);

		pi = m_pLattice->m_unitCellA.begin()+m_atomsInCluster;
		m_pLattice->m_unitCellA.erase(pi,pi+m_atomsInExtension);

		pi = m_pLattice->m_unitCellB.begin()+m_atomsInCluster;
		m_pLattice->m_unitCellB.erase(pi,pi+m_atomsInExtension);

		pi = m_pLattice->m_unitCellC.begin()+m_atomsInCluster;
		m_pLattice->m_unitCellC.erase(pi,pi+m_atomsInExtension);

		pb = m_pLattice->m_SymmetricallyIndependent.begin()+m_atomsInCluster;
		m_pLattice->m_SymmetricallyIndependent.erase(pb,pb+m_atomsInExtension);

		pi = m_pLattice->m_SymmetryParentAtom.begin()+m_atomsInCluster;
		m_pLattice->m_SymmetryParentAtom.erase(pi,pi+m_atomsInExtension);

		pb = m_pLattice->m_rendered.begin()+m_atomsInCluster;
		m_pLattice->m_rendered.erase(pb,pb+m_atomsInExtension);

		this->m_atomsInExtension = 0;
		this->m_latticeExtended = false;
		return;
	}
	else
	{
		AfxMessageBox("No Lattice object created",MB_OK);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////////
//
void CMadelungDoc::ExtendLatticeAsParallelepiped(bool UseSymmetry)
{
// get the number of unit cells already in the plot
	int na = m_NumberOfUnitCellsForPlotAlongA;
	int nb = m_NumberOfUnitCellsForPlotAlongB;
	int nc = m_NumberOfUnitCellsForPlotAlongC;
// extend in the positive direction
	for(int i=-m_extend_c;i<nc+m_extend_c;i++)
		for(int j=-m_extend_b;j<nb+m_extend_b;j++)
			for(int k=-m_extend_a;k<na+m_extend_a;k++)
				this->AddUnitCellToLattice(k,j,i);
// calculate the number of atoms in the extension
	this->m_atomsInExtension = this->m_pLattice->GetSize() - this->m_atomsInCluster;
	this->m_latticeExtended = true;
// use symmetry, if possible
	if(UseSymmetry)
	{
		FILE* fp;
// just for debug output. MFC TRACE macros doesn't work well
		fp = fopen("SymTemp.txt","w");
// finds and marks symmetry independent atoms in m_pLattice
		this->FindSymmetryIndependentAtoms(fp);
// marks symmetry dependent atoms, tests refined symmetry operations
		int ins = this->SymmetrizeExtension(fp);
		fprintf(fp,"%d atoms inserted to make system symmetric\n",ins);
		this->m_atomsInExtension += ins;
// counts ALL atoms on symmetry elements and stores them in the 'm_pAtomsOnSymElem'
		this->CountAllAtomsOnSymmetryElements();
// translates m_pLattice so that geom center of selected atoms is in the coordinate origin
// and aligns symmetry elements with cartesian axes
		this->TransformExtension(fp);
// sum the symmetry independent atoms
		int sia = 0;
		int size = m_pLattice->GetSize();
		int unassignedAtoms = 0;	// count those atoms who have parent atom == -1
		int unassignedIndependent = 0;
		int symIndepAtomsInSelection = 0;
		list<int>unassigned;
		list<int>symIndepAtomsInSelect;
		for(int atom1 = 0;atom1<size;atom1++)
		{
			if(m_pLattice->m_SymmetricallyIndependent[atom1] == true)
// finds and marks all symmetry dependent atoms for 'atom1'
				sia++;
			if(m_pLattice->m_SymmetryParentAtom[atom1] == -1)
			{
				unassignedAtoms++;
				unassigned.push_back(atom1);
			}
			if(m_pLattice->m_SymmetricallyIndependent[atom1] == true && m_pLattice->m_selected[atom1] == true)
			{
				symIndepAtomsInSelect.push_back(atom1);
				symIndepAtomsInSelection++;
			}
			if(m_pLattice->m_SymmetricallyIndependent[atom1] == true && m_pLattice->m_SymmetryParentAtom[atom1] == -1)
				unassignedIndependent++;
		}
		fprintf(fp,"Total number of atoms %d\n",size);
		fprintf(fp,"Symmetry independent atoms %d\n",sia);
		fprintf(fp,"Atoms without symmetry parent atoms %d\n",unassignedAtoms);
		fprintf(fp,"Symmetry independent atoms in selection, %d: ",symIndepAtomsInSelection);
		size = symIndepAtomsInSelect.size();
		list<int>::iterator p = symIndepAtomsInSelect.begin();
		for(int i=0;i<symIndepAtomsInSelection;i++)
		{
			fprintf(fp,"%d ",*p);
			p++;
		}
		fprintf(fp,"\n");
		fprintf(fp,"Unassigned symmetry independent atoms %d\n",unassignedIndependent);

// atoms on symmetry elements, found in the CountAllAtomsOnSymmetryElements() call
		size = m_pAtomsOnSymElem->GetSize();
		m_pAtomsOnSymElem->ResetIterators();
		int se,at;
		for(i=0;i<size;i++)
		{
			m_pAtomsOnSymElem->GetNextAtom(se,at);
			fprintf(fp,"%d %d\n",se,at);
		}
		size = unassigned.size();
		p = unassigned.begin();
		fprintf(fp,"Atoms without symmetry parent\n");
		for(i=0;i<size;i++)
		{
			fprintf(fp,"%d\n",*p);
			p++;
		}

		fclose(fp);
	}
}
//////////////////////////////////////////////////////////////////////////////
//

void CMadelungDoc::ExtendLatticeAsSphere(bool UseSymmetry)
{
// get the number of unit cells already in the plot
	int na = m_NumberOfUnitCellsForPlotAlongA;
	int nb = m_NumberOfUnitCellsForPlotAlongB;
	int nc = m_NumberOfUnitCellsForPlotAlongC;

// initial cluster coordinates are (0,0,0),(na,nb,nc)
// sphere's coordinates are (na/2,nb/2,nc/2)

	for(int i=-m_extend_sphere;i<m_extend_sphere+nc;i++)
		for(int j=-m_extend_sphere;j<m_extend_sphere+nb;j++)
			for(int k=-m_extend_sphere;k<m_extend_sphere+na;k++)
				this->AddUnitCellToLattice(k,j,i);
// calculate the number of atoms in the extension
	this->m_atomsInExtension = this->m_pLattice->GetSize() - this->m_atomsInCluster;
	this->m_latticeExtended = true;
// use symmetry, if possible
	if(UseSymmetry)
	{
		FILE* fp;
// just for debug output. MFC TRACE macros doesn't work well
		fp = fopen("SymTemp.txt","w");
// finds and marks symmetry independent atoms in m_pLattice
		this->FindSymmetryIndependentAtoms(fp);
// marks symmetry dependent atoms, tests refined symmetry operations
		int ins = this->SymmetrizeExtension(fp);
		fprintf(fp,"%d atoms inserted to make system symmetric\n",ins);
		this->m_atomsInExtension += ins;
// counts ALL atoms on symmetry elements and stores them in the 'm_pAtomsOnSymElem'
		this->CountAllAtomsOnSymmetryElements();
// translates m_pLattice so that geom center of selected atoms is in the coordinate origin
// and aligns symmetry elements with cartesian axes
		this->TransformExtension(fp);
// sum the symmetry independent atoms
		int sia = 0;
		int size = m_pLattice->GetSize();
		int unassignedAtoms = 0;	// count those atoms who have parent atom == -1
		int unassignedIndependent = 0;
		int symIndepAtomsInSelection = 0;
		list<int>unassigned;
		list<int>symIndepAtomsInSelect;
		for(int atom1 = 0;atom1<size;atom1++)
		{
			if(m_pLattice->m_SymmetricallyIndependent[atom1] == true)
// finds and marks all symmetry dependent atoms for 'atom1'
				sia++;
			if(m_pLattice->m_SymmetryParentAtom[atom1] == -1)
			{
				unassignedAtoms++;
				unassigned.push_back(atom1);
			}
			if(m_pLattice->m_SymmetricallyIndependent[atom1] == true && m_pLattice->m_selected[atom1] == true)
			{
				symIndepAtomsInSelect.push_back(atom1);
				symIndepAtomsInSelection++;
			}
			if(m_pLattice->m_SymmetricallyIndependent[atom1] == true && m_pLattice->m_SymmetryParentAtom[atom1] == -1)
				unassignedIndependent++;
		}
		fprintf(fp,"Total number of atoms %d\n",size);
		fprintf(fp,"Symmetry independent atoms %d\n",sia);
		fprintf(fp,"Atoms without symmetry parent atoms %d\n",unassignedAtoms);
		fprintf(fp,"Symmetry independent atoms in selection, %d: ",symIndepAtomsInSelection);
		size = symIndepAtomsInSelect.size();
		list<int>::iterator p = symIndepAtomsInSelect.begin();
		for(int i=0;i<symIndepAtomsInSelection;i++)
		{
			fprintf(fp,"%d ",*p);
			p++;
		}
		fprintf(fp,"\n");
		fprintf(fp,"Unassigned symmetry independent atoms %d\n",unassignedIndependent);

// atoms on symmetry elements, found in the CountAllAtomsOnSymmetryElements() call
		size = m_pAtomsOnSymElem->GetSize();
		m_pAtomsOnSymElem->ResetIterators();
		int se,at;
		for(i=0;i<size;i++)
		{
			m_pAtomsOnSymElem->GetNextAtom(se,at);
			fprintf(fp,"%d %d\n",se,at);
		}
		size = unassigned.size();
		p = unassigned.begin();
		fprintf(fp,"Atoms without symmetry parent\n");
		for(i=0;i<size;i++)
		{
			fprintf(fp,"%d\n",*p);
			p++;
		}

		fclose(fp);
	}
}
////////////////////////////////////////////////////////////////////////////
//
void CMadelungDoc::SetMenu(UINT m)
{
	CMadelungApp* pApp = (CMadelungApp*)AfxGetApp();
	CMainFrame* pFr = (CMainFrame*)pApp->m_pMainWnd;
	CMenu menu;
	menu.LoadMenu(m);
	pFr->SetMenu(&menu);
	menu.Detach();
}

/////////////////////////////////////////////////////////////////////////////
//	creates the space group according to user's choice
//
void CMadelungDoc::CreateSpaceGroup(int sgNumber)
{
	m_pSpaceGroup = SpaceGroup::New(sgNumber);
}

//////////////////////////////////////////////////////////////////////////////
//	adds atoms from the UnitCell object to the CLattice object
//	ALL one UnitCell atom coordinates are assumed to be calculated and
//	stored in m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCartXYZ[];
//
void CMadelungDoc::AddUnitCellToLattice(int na,int nb,int nc)
{
	if(this->m_pSpaceGroup)
	{
// check the existence of a unitcell. If exists, return
		if(this->m_pLattice->UnitCellExists(na,nb,nc))
			return;
// calculate the number of unit cells in CLattice
		this->m_numberOfUnitCells++;
// number of ALL atoms in the UnitCell (Wyckoff positions expanded)
		int numat = this->m_pSpaceGroup->m_pUnitCell->m_atomsInUnitCell;
		for(int i=0;i<numat;i++)
		{
			this->m_pLattice->m_X.push_back(
				this->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCartX[i]+
				this->m_pSpaceGroup->m_pUnitCell->m_unitCellVectorA_Cart[0]*na+
				this->m_pSpaceGroup->m_pUnitCell->m_unitCellVectorB_Cart[0]*nb+
				this->m_pSpaceGroup->m_pUnitCell->m_unitCellVectorC_Cart[0]*nc);

			this->m_pLattice->m_Y.push_back(
					this->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCartY[i]+
					this->m_pSpaceGroup->m_pUnitCell->m_unitCellVectorA_Cart[1]*na+
					this->m_pSpaceGroup->m_pUnitCell->m_unitCellVectorB_Cart[1]*nb+
					this->m_pSpaceGroup->m_pUnitCell->m_unitCellVectorC_Cart[1]*nc);

			this->m_pLattice->m_Z.push_back(
					this->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCartZ[i]+
					this->m_pSpaceGroup->m_pUnitCell->m_unitCellVectorA_Cart[2]*na+
					this->m_pSpaceGroup->m_pUnitCell->m_unitCellVectorB_Cart[2]*nb+
					this->m_pSpaceGroup->m_pUnitCell->m_unitCellVectorC_Cart[2]*nc);

			this->m_pLattice->m_charge.push_back(
					this->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCartCharge[i]);

			int type = this->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCartType[i];
			this->m_pLattice->m_type.push_back(type);

			char wpos = this->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCartWPos[i];
			this->m_pLattice->m_WPosition.push_back(wpos);
// atom not selected initially
			this->m_pLattice->m_selected.push_back(false);
// atom not rendered initially
			this->m_pLattice->m_rendered.push_back(false);
// location of unit cell in lattice (initial unitcell is na=0,nb=0,nc=0)
			this->m_pLattice->m_unitCellA.push_back(na);
			this->m_pLattice->m_unitCellB.push_back(nb);
			this->m_pLattice->m_unitCellC.push_back(nc);
// index of atom in UnitCell object. Used in work threads for calculations
			this->m_pLattice->m_atomInUnitCell.push_back(i);
// symmetry handling parameters, treated later in symmetry routines.
// here we set parameters to dummy.
			this->m_pLattice->m_SymmetricallyIndependent.push_back(true);
			this->m_pLattice->m_SymmetryParentAtom.push_back(-1);
		}
	}
	else
	{
		AfxMessageBox("Doc::AddUnitCellToLattice. No space group selected!",MB_OK);
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////
//	This function is called after user clicks OK on the Setup->Lattice PropertySheet
//	Clears all except m_pSpaceGroup
// m_pSpaceGroup is cleared in OnNewDocument();

void CMadelungDoc::ReinitializeDocument()
{
	if(m_pLattice) m_pLattice->Clear();
// clear UnitCell data
//	if(this->m_pSpaceGroup) this->m_pSpaceGroup->m_pUnitCell->Clear();
	if(m_pAtomsOnSymElem) m_pAtomsOnSymElem->Clear();
// don't assign NULL to m_pSpaceGroup since it's assigned a real value
// in PropertyPageSpaceGroup
//	m_pSpaceGroup = NULL;	// no default space group
	if(m_pAtomView)
	{
		delete m_pAtomView;
		m_pAtomView = NULL;		// no default AtomView (containing Wyckoff position coordinates)
	}
	m_FitFunction = 7;
	m_latticeAsSphere = true;
	m_extend_a = 1;
	m_extend_b = 1;
	m_extend_c = 1;
	m_extend_sphere = 1;
	m_atomsInCluster = 0;
	m_atomsInExtension = 0;
	m_latticeExtended = false;

	m_NumberOfUnitCellsForPlotAlongA = 1;	// number of unit cells for plot
	m_NumberOfUnitCellsForPlotAlongB = 1;
	m_NumberOfUnitCellsForPlotAlongC = 1;
	m_numberOfUnitCells = 0;

	m_bUseSymmetry = false;
// the point group of the selected atoms cluster
	m_siteSymmetry = "";
// Stores info about the site symmetry (geom center of selected atoms)irreps of the 's','p','d' functions.
// Used for the MOLCAS SCF module 'OCCUPIED' keyword. Used in File()
	m_siteSymmetryIrreps = "";
// point group irreps
	if(m_pgi) delete m_pgi;
// erase molecules in unitcell
	int size = m_molecules.size();
	if(size)
	{
		for(int i=0;i<size;i++)
			delete m_molecules[i];
		m_molecules.clear();
	}
// Molecules in unitcell
	m_currentMolecule = 0; // molecule, opened in the EditMolecule

	if(!m_IsFirstDocument)
	{
		this->SetMenu(IDR_START);
	}
	m_IsFirstDocument = false;
	m_PotentialsCalculatedOK = false;
	m_bCalculationInProgress = FALSE;
//	m_pElStatEnergyThread = NULL;	// no work thread at the beginning

// clear the main view
	POSITION pos = GetFirstViewPosition();
	CMadelungView* pv = (CMadelungView*)GetNextView(pos);
	pv->Clear();
	pv->Invalidate();
//	pv->UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////////
//
void CMadelungDoc::OnToolsPotentialOnAtoms() 
{
//  Due to symmetry handling, lattice must be extended before potential calculation.
	if(m_latticeExtended)
	{
		CMainFrame* pF = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		HWND wnd = pF->m_hWnd;
// create progress bar, modify menus
		::SendMessage(wnd,WM_USER_TOGGLE_STATUS_BAR,1,0);
		::SendMessage(wnd,WM_USER_CREATE_PROGRESS_BAR,1,0);
		::SendMessage(wnd,WM_USER_MODIFY_MAINFRAME_MENU,1,0);

		this->m_bCalculationInProgress = TRUE;
// check if g_TextOutput has text in it
		if(g_TextOutput.Size() != 0)
		{
			int reply = AfxMessageBox("Purge previous results?",MB_YESNO);
			if(reply == IDYES)
				g_TextOutput.Clear();
			else
			{}
		}
// create the text output window thread (user interface thread)
// this doesn't use Mcl class library
		m_pTextWindowThread = new CTextWindowThread();
		m_pTextWindowThread->CreateThread(0);
// wait until thread has created window, otherwise work thread may start sending information
// to non-existing window
		if(WaitForSingleObject(m_pTextWindowThread->m_hEventWindowCreated,INFINITE) == WAIT_OBJECT_0)
		{
//
// DISTINGUISH BETWEEN CALCULATIONS WITH AND WITHOUT SYMMETRY
//
// NOT USING SYMMETRY
			if(!this->m_bUseSymmetry)
			{
// find all selected atoms in CLattice and store them in the list
				int latticeSize = this->m_pLattice->GetSize();
// corresponding atom numbers of the selected atoms in the first unit cell
				list<int> selectedAtomsList;
				list<int>::iterator p;
// original selected atom numbers list
				list<int> originalAtomsList;
				list<int>::iterator p_o;
// count the TOTAL selected atoms
				m_selectedAtoms = 0;
// count the unique selected atoms in UNITCELL (without lattice translations)
				int selAtomsInUnitCell = 0;
				for(int i=0;i<latticeSize;i++)
				{
// add atoms only from the first unit cell
					if(m_pLattice->m_selected[i])
					{
// code to avoid duplicate atom numbers in the list
						p = selectedAtomsList.begin();
						bool atomFound = false;
						while(p != selectedAtomsList.end())
						{
							if(this->m_pLattice->m_atomInUnitCell[i] == *p)
							{
								atomFound = true;
								break;
							}
							else
								p++;
						}
						if(!atomFound)
						{
							selectedAtomsList.push_back(this->m_pLattice->m_atomInUnitCell[i]);
							originalAtomsList.push_back(i);
// count the UNIQUELLY selected atoms in unit cell (without lattice translations)
							selAtomsInUnitCell++;
						}
// count ALL selected atoms (including lattice translated)
						m_selectedAtoms++;
					}
				}
// pointer to a job object (to be used in DPQ)
				CEwaldJob* pJob;
// handle of the text window
				HWND hTextWindow = m_pTextWindowThread->GetTextWindow();
// check queue status and start it only when not started already
				m_queue->SetTextWindowHandle(hTextWindow);
				CString str;
				str.Format("Deferred Processing Queue Running...");
				g_TextOutput.AddString(str);
				::PostMessage(hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)g_TextOutput.Size(),0);
// set the number of jobs in queue (used only for progress bar)
				m_queue->SetNumberOfJobs(selAtomsInUnitCell);
				p = selectedAtomsList.begin();
				p_o = originalAtomsList.begin();
// submit and execute jobs on another thread
				for(i=0;i<selAtomsInUnitCell;i++)
				{
// jobs deletes themselves after finishing
					pJob = new CEwaldJob(this,							// pointer to document
								// atom number in the first unitcell
									*p,
								// atom number as in the first unitcell
									*p_o,	
									this->m_eta,					// eta parameter
									this->m_FitFunction,			// fit function parameter
									&g_TextOutput,				// thread-safe text output object,
									hTextWindow); // text window handle;

					m_queue->QueueJob(pJob);
					p++;
					p_o++;
				}
			}	
			else
// USING SYMMETRY
			{
// find all selected atoms in CLattice and store them in the list
				int latticeSize = this->m_pLattice->GetSize();
// corresponding atom numbers of the selected atoms in the first unit cell
				list<int> selectedAtomsList;
				list<int>::iterator p;
// original selected atom numbers list
				list<int> originalAtomsList;
				list<int>::iterator p_o;
// count the TOTAL selected atoms
				m_selectedAtoms = 0;
// count the unique selected atoms in UNITCELL (without lattice translations)
				int selAtomsInUnitCell = 0;
				for(int i=0;i<latticeSize;i++)
				{
// add atoms only from the first unit cell
					if(m_pLattice->m_selected[i] && m_pLattice->m_SymmetricallyIndependent[i]) {
// code to avoid duplicate atom numbers in the list
						p = selectedAtomsList.begin();
						bool atomFound = false;
						while(p != selectedAtomsList.end())
						{
							if(this->m_pLattice->m_atomInUnitCell[i] == *p)
							{
								atomFound = true;
								break;
							}
							else
								p++;
						}
						if(!atomFound)
						{
							int at = this->m_pLattice->m_atomInUnitCell[i];
							selectedAtomsList.push_back(at);
							originalAtomsList.push_back(i);
// count the UNIQUELLY selected atoms in unit cell (without lattice translations)
							selAtomsInUnitCell++;
						}
// count ALL selected atoms (including lattice translated)
						m_selectedAtoms++;
					}
				}
// pointer to a job object (to be used in DPQ)
				CEwaldJob* pJob;
// handle of the text window
				HWND hTextWindow = m_pTextWindowThread->GetTextWindow();
// check queue status and start it only when not started already
				m_queue->SetTextWindowHandle(hTextWindow);
				CString str;
				str.Format("Deferred Processing Queue Running...");
				g_TextOutput.AddString(str);
				::PostMessage(hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)g_TextOutput.Size(),0);
				str.Format("USING SYMMETRY...");
				g_TextOutput.AddString(str);
				::PostMessage(hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)g_TextOutput.Size(),0);
				str.Format("%d symmetry independent atoms in the selection",selAtomsInUnitCell);
				g_TextOutput.AddString(str);
				::PostMessage(hTextWindow,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)g_TextOutput.Size(),0);
// set the number of jobs in queue (used only for progress bar)
				m_queue->SetNumberOfJobs(selAtomsInUnitCell);
				p = selectedAtomsList.begin();
				p_o = originalAtomsList.begin();
// submit and execute jobs on another thread
				for(i=0;i<selAtomsInUnitCell;i++)
				{
// jobs deletes themselves after finishing
					pJob = new CEwaldJob(this,							// pointer to document
								// atom number in the first unitcell
									*p,
								// atom number as in the first unitcell
									*p_o,	
									this->m_eta,					// eta parameter
									this->m_FitFunction,			// fit function parameter
									&g_TextOutput,				// thread-safe text output object,
									hTextWindow); // text window handle;

					m_queue->QueueJob(pJob);
					p++;
					p_o++;
				}
			}
		}
	}
	else
		AfxMessageBox("Extend lattice first",MB_OK);
}
////////////////////////////////////////////////////////////////////////////////////
//

void CMadelungDoc::OnCancelCalculation() 
{
// ask what user wants to do
	int ret = AfxMessageBox("Abort calculation?",MB_YESNO | MB_ICONQUESTION);
	int ret1;

// handle text window destruction (it's thread destroyed along with it)
	HWND htextwnd = m_pTextWindowThread->GetTextWindow();
	CString str;
	CMainFrame* pF = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	HWND wnd = pF->m_hWnd;

	switch(ret)
	{
	case IDYES:
// stop the deferred processing queue
		this->m_bCalculationInProgress = FALSE;
		this->m_queue->Stop();

		str.Format("Deferred Processing Queue Stopped...");
		g_TextOutput.AddString(str);
		::PostMessage(htextwnd,WM_USER_TEXT_WINDOW_ADD_LINE,(WPARAM)g_TextOutput.Size(),0);
// ask to purge text output
		ret1 = AfxMessageBox("Purge output?",MB_YESNO | MB_ICONQUESTION);
		if(ret1 == IDYES)
			g_TextOutput.Clear();
		else
		{}		// retain output. Next calculation output will be appended
		::SendMessage(htextwnd,WM_USER_DESTROY_TEXT_WINDOW,0,0);
// restore the menu
//		this->SetMenu(IDR_MAINFRAME);
// remove progress bar
//		::SendMessage(wnd,WM_USER_DESTROY_PROGRESS_BAR,0,0);
// reset the indicators pane to the default value
//		::SendMessage(wnd,WM_USER_TOGGLE_STATUS_BAR,0,0);
		break;
	case IDNO:
		::SendMessage(htextwnd,WM_USER_RESTORE_TEXT_WINDOW,0,0);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////
//
void CMadelungDoc::OnViewOutput() 
{
	if(g_TextOutput.Size() == 0)
		return;
	else
	{
// create the text output window thread (user interface thread)
		m_pTextWindowThread = new CTextWindowThread();
		m_pTextWindowThread->CreateThread(0);
	}
}

//////////////////////////////////////////////////////////////////////////////////
//
void CMadelungDoc::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_TextOutput.Size());
}

//////////////////////////////////////////////////////////////////////////////////////
//	this routine optimizes charges of all atoms in the cluster (selected + unselected)

void CMadelungDoc::OnOptimize()
{

// NOT USING SYMMETRY

	if(this->m_latticeExtended && this->m_selectedAtoms && !this->m_bUseSymmetry)
	{
		CMainFrame* pF = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		HWND wnd = pF->m_hWnd;
// create progress bar, modify menus
		::SendMessage(wnd,WM_USER_TOGGLE_STATUS_BAR,1,0);
		::SendMessage(wnd,WM_USER_CREATE_PROGRESS_BAR,1,0);
		::SendMessage(wnd,WM_USER_MODIFY_MAINFRAME_MENU,1,0);

		this->m_bCalculationInProgress = TRUE;
// Total number of atoms in Lattice
		int atoms = m_pLattice->GetSize();
// array of optimized charges
		if(m_optimized_charges) delete m_optimized_charges;
		m_optimized_charges = new OptimizedCharges(atoms,GetNumberOfSelectedAtoms());
// set the number of jobs in queue (used only for progress bar)
		m_queue->SetNumberOfJobs(1);
// check for corretness of the number of atoms
		ASSERT(atoms == (m_atomsInCluster + m_atomsInExtension));
// Number of parameters to optimize (all atoms)
		int paramsToOptimize = atoms;
// fill the dimension of charges with initial values
		int c1 = 0;
		int c2 = 0;
		for(int i=0;i<atoms;i++)
		{
			ASSERT(fabs(m_pLattice->m_charge[i]) < 50.);
			m_optimized_charges->m_atoms[c1] = i;
			m_optimized_charges->m_charges[c1++] = m_pLattice->m_charge[i];
// store the selected atom numbers
			if(m_pLattice->m_selected[i])
				m_optimized_charges->m_selected_atoms[c2++] = i;
		}
// optimization job (derived from CJob) to be executed in the DP Queue
		COptJob* pJob = new COptJob(paramsToOptimize,
								m_optimized_charges->m_charges,
								&g_TextOutput,
								m_pTextWindowThread->GetTextWindow());
// submit job to DP Queue
		m_queue->QueueJob(pJob);
	}

// USE SYMMETRY

	else
	if(this->m_latticeExtended && this->m_selectedAtoms && this->m_bUseSymmetry)
	{
		CMainFrame* pF = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		HWND wnd = pF->m_hWnd;
// create progress bar, modify menus
		::SendMessage(wnd,WM_USER_TOGGLE_STATUS_BAR,1,0);
		::SendMessage(wnd,WM_USER_CREATE_PROGRESS_BAR,1,0);
		::SendMessage(wnd,WM_USER_MODIFY_MAINFRAME_MENU,1,0);

		this->m_bCalculationInProgress = TRUE;
// total number of symmetrically independent atoms
		int atoms = m_pLattice->SymmetryIndependentAtoms();
// total size of m_pLattice (all atoms)
		int size = m_pLattice->GetSize();
// array of optimized charges
		if(m_optimized_charges) delete m_optimized_charges;
		m_optimized_charges = new OptimizedCharges(atoms,GetNumberOfSelectedAtoms());

// set the number of jobs in queue (used only for progress bar)
		m_queue->SetNumberOfJobs(1);
// Number of parameters to optimize (all atoms)
		int paramsToOptimize = atoms;
// fill the dimension of charges with initial values
		int c1 = 0;
		int c2 = 0;
		for(int i=0;i<size;i++)
		{
			ASSERT(fabs(m_pLattice->m_charge[i]) < 50.);
			if(m_pLattice->m_SymmetricallyIndependent[i])
			{
// store ALL the symmetrically independent atom numbers and charges
				m_optimized_charges->m_atoms[c1] = i;
				m_optimized_charges->m_charges[c1++] = m_pLattice->m_charge[i];
// store the symmetry independent, selected atom numbers
				if(m_pLattice->m_selected[i])
					m_optimized_charges->m_selected_atoms[c2++] = i;
			}
		}
// optimization job (derived from CJob) to be executed in the DP Queue
		COptJob* pJob = new COptJob(paramsToOptimize,
								m_optimized_charges->m_charges,
								&g_TextOutput,
								m_pTextWindowThread->GetTextWindow());
// submit job to DP Queue
		m_queue->QueueJob(pJob);
	}
}

//////////////////////////////////////////////////////////////////////////////////
//

void CMadelungDoc::OnUpdateOptimize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(this->m_PotentialsCalculatedOK && this->m_latticeExtended);
	
}

///////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMadelungDoc::OnOptJobDone(WPARAM wp,LPARAM lp)
{
	FILE* fp = (FILE*)wp;

	int atoms = m_pLattice->GetSize();
	if(!this->m_bUseSymmetry)
	{
		for(int i=0;i<atoms;i++)
		{
			if(fp)
			{
				fprintf(fp,"Atom %d (%s)",i,this->m_pAtomProperties->m_Type[m_pLattice->m_type[i]]);
				fprintf(fp," Charge %8.5lf\n",m_optimized_charges->m_charges[i]);
			}
			else
			{
				TRACE2("Atom %d (%s)",i,this->m_pAtomProperties->m_Type[m_pLattice->m_type[i]]);
				TRACE1(" Charge %8.5lf\n",m_optimized_charges->m_charges[i]);
			}
// store optimized charges in CLattice
			m_pLattice->m_charge[i] = m_optimized_charges->m_charges[i];
		}
// calculate el.stat potential on selected atoms from optimized charges
		for(i=0;i<m_selectedAtoms;i++)
		{
// fill the corresponding dimensions with optimized charges
			int atomNumber = m_optimized_charges->m_selected_atoms[i];
			double pot = ElStatPotentialOnAtom(atomNumber,m_pLattice,
											m_optimized_charges->m_charges);
			if(fp)
			{
				fprintf(fp,"Atom %d (%s)",atomNumber,
						this->m_pAtomProperties->m_Type[m_pLattice->m_type[atomNumber]]);
				fprintf(fp," Opt.charge potential %8.5lf\n",pot);
			}
			else
			{
				TRACE2("Atom %d (%s)",atomNumber,
						this->m_pAtomProperties->m_Type[m_pLattice->m_type[atomNumber]]);
				TRACE1(" Opt.charge potential %8.5lf\n",pot);
			}
		}
		fclose(fp);
		return TRUE;
	}

// USING SYMMETRY

	else
	{
		int size_opt = this->m_optimized_charges->GetSize();
		int selectedAtoms = this->GetNumberOfSelectedSymmIndependentAtoms();

		if(fp)
			fprintf(fp,"Symmetry independent atom charges\n");
		else
			TRACE0("Symmetry independent atom charges\n");

		for(int i=0;i<size_opt;i++)
		{
			int at = this->m_optimized_charges->m_atoms[i];
			if(fp)
			{
				fprintf(fp,"%d, Atom %d (%s)",i,at,this->m_pAtomProperties->m_Type[m_pLattice->m_type[at]]);
				fprintf(fp," Charge %8.5lf\n",m_optimized_charges->m_charges[i]);
			}
			else
			{
				TRACE3("%d, Atom %d (%s)",i,at,this->m_pAtomProperties->m_Type[m_pLattice->m_type[at]]);
				TRACE1(" Charge %8.5lf\n",m_optimized_charges->m_charges[i]);
			}
		}
// store optimized charges in CLattice
		for(int j=0;j<atoms;j++)
		{
			int index = m_pLattice->m_SymmetryParentAtom[j];
// find the charge of atom number 'index' in x[];
			bool chargeFound = false;
			for(int k=0;k<size_opt;k++)
			{
				if(index == g_pDoc->m_optimized_charges->m_atoms[k])
				{
					if(fp)
					{
						fprintf(fp,"Atom %d (%s)",j,this->m_pAtomProperties->m_Type[m_pLattice->m_type[j]]);
						fprintf(fp," Charge %8.5lf\n",m_optimized_charges->m_charges[k]);
					}
					else
					{
						TRACE2("Atom %d (%s)",j,this->m_pAtomProperties->m_Type[m_pLattice->m_type[j]]);
						TRACE1(" Charge %8.5lf\n",m_optimized_charges->m_charges[k]);
					}
// store optimized charges in CLattice
					m_pLattice->m_charge[j] = m_optimized_charges->m_charges[k];
					chargeFound = true;
					break;
				}
				else
					continue;
			}
			if(!chargeFound)
			{
				AfxMessageBox("MadelungDoc.cpp : Charge not found",MB_OK);
				exit(1);
			}
		}
		// calculate el.stat potential on selected atoms from optimized charges
		for(i=0;i<selectedAtoms;i++)
		{
// fill the corresponding dimensions with optimized charges
			int atomNumber = m_optimized_charges->m_selected_atoms[i];
			double pot = ElStatPotentialOnAtom(atomNumber,m_pLattice,
											m_optimized_charges->m_charges);
			if(fp)
			{
				fprintf(fp,"Atom %d (%s)",atomNumber,
							this->m_pAtomProperties->m_Type[m_pLattice->m_type[atomNumber]]);
				fprintf(fp," Opt.charge potential %8.5lf\n",pot);
			}
			else
			{
				TRACE2("Atom %d (%s)",atomNumber,
							this->m_pAtomProperties->m_Type[m_pLattice->m_type[atomNumber]]);
				TRACE1(" Opt.charge potential %8.5lf\n",pot);
			}
		}
		fclose(fp);
		return TRUE;
	}
}
////////////////////////////////////////////////////////////////////////////
//
BOOL CMadelungDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
	// Save the document data to a file
	// lpszPathName = path name where to save document file
	// if lpszPathName is NULL then the user will be prompted (SaveAs)
	// note: lpszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
//		CDocTemplate* pTemplate = GetDocTemplate();
//		ASSERT(pTemplate != NULL);
		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);
	   }
	}
	char filter[] = "Molcas input file (*.mcs)\0*.inp\0Gaussian input file (*.gss)\0*.gss\0HyperChem input file (*.hin)\0*.hin\0Madelung input file (*.mdg)\0*.mdg\0";
	m_pFileDialog->m_ofn.lpstrFilter = filter;
	if(m_pFileDialog->DoModal() != IDOK)
		return FALSE;

	CWaitCursor wait;
	newName = m_pFileDialog->GetPathName();
// extension of the file
	m_SaveFileType = m_pFileDialog->m_ofn.nFilterIndex;
	CString ext;
	switch(m_SaveFileType)
	{
	case 1:
		ext = ".mcs";
		break;
	case 2:
		ext = ".gss";
		break;
	case 3:
		ext = ".hin";
		break;
	case 4:
		ext = ".mdg";
		break;
	}

	newName += ext;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				e->Delete();
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CMadelungDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	std::ofstream out(lpszPathName);
	if(!out)
	{
		TRACE0("Cannot open file for writing");
		return FALSE;
	}

	TRY
	{
		CWaitCursor wait;
//		Serialize(saveArchive);     // save me
//		saveArchive.Close();
		switch(this->m_SaveFileType)
		{
		case 1:	// Molcas
			WriteMolcasFile(out);
			TRACE0("Molcas format file written\n");
			break;
		case 2:
			WriteGaussianFile(out);
			TRACE0("Gaussian format file written\n");
			break;
		case 3:
			WriteHyperChemFile(out);
			TRACE0("HyperChem format file written\n");
			break;
		case 4:
			WriteMadelungFile(out);
			TRACE0("Madelung format file written\n");
			break;
		default:
			TRACE0("Not implemented format\n");
			break;
		}
		out.close();
	}
	CATCH_ALL(e)
	{
		out.close();

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
		END_TRY
		e->Delete();
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // back to unmodified

	return TRUE;        // success
}

//////////////////////////////////////////////////////////////////////////////////
//	this function writes out the results in MOLCAS input file format
//
BOOL CMadelungDoc::WriteMolcasFile(std::ofstream& out)
{

// NOT USING SYMMETRY

	if(!this->m_bUseSymmetry)
	{
// format options
		out << " *** MOLCAS input format file ***" << endl;
		out << " &seward &end" << endl;
		out << "Title" << endl << "...complex" << endl;
// find the number of selected atoms in CLattice
		int size = m_pLattice->GetSize();
		m_selectedAtoms = GetNumberOfSelectedAtoms();
// determine the number of different atom types in CLattice
// for each type there is an output section
		CList<int,int> typeList;	// holds types of selected atoms
		for(int i=0;i<size;i++)
		{
			int tp = this->m_pLattice->m_type[i];
			if(!typeList.Find(tp)) typeList.AddTail(tp);
		}
// number of different types of atoms in the list
		int difTypes = typeList.GetCount();
		POSITION pos = typeList.GetHeadPosition();
		for(i=0;i<difTypes;i++)
		{
			out << "Basis set" << endl;
			int typeInt = typeList.GetNext(pos);
			CString type = this->m_pAtomProperties->m_Type[typeInt];
			out << LPCTSTR(type) << ".ano-l...6s5p4d2f." << endl;
			for(int j=0;j<size;j++)
			{
				if(!m_pLattice->m_selected[j]) continue;
// found a selected atom
				if(this->m_pLattice->m_type[j] != typeInt) continue;
				CString coord;
					coord.Format("%s%d  %15.8lf  %15.8lf  %15.8lf  angstrom\n",
						LPCTSTR(type),j,
						this->m_pLattice->m_X[j],
						this->m_pLattice->m_Y[j],
						this->m_pLattice->m_Z[j]);
				out << LPCTSTR(coord);
			}
			out << "End of basis" << endl;
		}	
// now add point charges
// count point charges
		int counter = 0;
		for(i=0;i<size;i++) if(!m_pLattice->m_selected[i]) counter++;
		if(counter == 0) return TRUE;
// there are some non-selected atoms
		out << "XField" << endl << counter << endl;
		for(i=0;i<size;i++)
		{
			if(m_pLattice->m_selected[i]) continue;
			CString str;
			str.Format("%15.8lf  %15.8lf  %15.8lf  %15.8lf  0.0  0.0  0.0\n",
			m_pLattice->m_X[i]/BOHR,	// cartesian coordinates of point charges
			m_pLattice->m_Y[i]/BOHR,
			m_pLattice->m_Z[i]/BOHR,
			m_pLattice->m_charge[i]);	// point charge value. Dipole components are 0.0
			out << LPCTSTR(str);
		}
		out << "Test" << endl << "RELInt" << endl << "End of input" << endl << endl;
// output the SCF section
		out << " &scf &end" << endl;
		out << "Title" << endl << "...complex" << endl;
		out << "Occupied" << endl << "End of input" << endl;
		return TRUE;
	}

// USING SYMMETRY

	else
	{
// format options
		out << " *** MOLCAS input format file ***" << endl;
		out << " &seward &end" << endl;
		out << "Title" << endl << "...complex" << endl;
		out << "symmetry" << endl;
		for(int i=0;i<this->m_numberOfRefinedSymmetryElements;i++)
		{
			SYMMETRY_ELEMENT* elem = m_RefinedElements[i];
// symmetry PLANES, output only for normals along X,Y,Z, as supported by MOLCAS
			if(elem->transform_atom == mirror_atom)
			{
				for(int j=0;j<3;j++)
				{
					if(fabs(fabs(elem->normal[j]) - 1.0) < DISTANCE_TOLERANCE)
					{
// check if a normal is along X,Y,Z axes
						for(int k=0;k<3;k++)
						{
							if(j==k) continue;
							else
							if(fabs(elem->normal[k]) > DISTANCE_TOLERANCE)
							{
								AfxMessageBox("Plane normal not along X,Y,Z axes",MB_OK);
								return FALSE;
							}
							else
								continue;
						}
						switch(j)
						{
						case 0:
							out << " X";
							break;
						case 1:
							out << " Y";
							break;
						case 2:
							out << " Z";
							break;
						default:
							out << " unknown";
							break;
						}
						break;
					}
				}
			}
// 2nd ORDER AXIS
			else
			if(elem->transform_atom == rotate_atom)
			{
				if(elem->order != 2)
				{
					AfxMessageBox("Only 2nd order axes supported in MOLCAS",MB_OK);
					return FALSE;
				}
				else
				{
					for(int j=0;j<3;j++)
					{
						if(fabs(fabs(elem->direction[j]) - 1.0) < DISTANCE_TOLERANCE)
						{
// check if a normal is along X,Y,Z axes
							for(int k=0;k<3;k++)
							{
								if(j==k) continue;
								else
								if(fabs(elem->direction[k]) > DISTANCE_TOLERANCE)
								{
									AfxMessageBox("Axis direction not along X,Y,Z axes",MB_OK);
									return FALSE;
								}
							}
							switch(j)
							{
							case 0:
								out << " YZ";
								break;
							case 1:
								out << " XZ";
								break;
							case 2:
								out << " XY";
								break;
							default:
								out << " unknown";
								break;
							}
							break;
						}
						else
							continue;
					}
				}
			}
// INVERSION CENTER
			else
			if(elem->transform_atom == invert_atom)
			{
				out << " XYZ";
			}
			else
				continue;
		}
		out << endl;

// find the number of selected atoms in CLattice
		m_selectedAtoms = GetNumberOfSelectedAtoms();
		int size = m_pLattice->GetSize();
// determine the number of different atom types in CLattice
// for each type there is an output section
		CList<int,int> typeList;	// holds types of selected atoms
		for(i=0;i<size;i++)
		{
			int tp = this->m_pLattice->m_type[i];
			if(!typeList.Find(tp)) typeList.AddTail(tp);
		}
// number of different types of atoms in the list
		int difTypes = typeList.GetCount();
		POSITION pos = typeList.GetHeadPosition();
		for(i=0;i<difTypes;i++)
		{
			out << "Basis set" << endl;
			int typeInt = typeList.GetNext(pos);
			CString type = this->m_pAtomProperties->m_Type[typeInt];
			out << LPCTSTR(type) << ".ano-l...6s5p4d2f." << endl;
			for(int j=0;j<size;j++)
			{
				if(m_pLattice->m_selected[j] && m_pLattice->m_SymmetricallyIndependent[j] &&
					j<this->m_atomsInCluster && this->m_pLattice->m_type[j] == typeInt)
				{
// found a selected atom
					CString coord;
					coord.Format("%s%d  %15.8lf  %15.8lf  %15.8lf  angstrom\n",
						LPCTSTR(type),j,
						this->m_pLattice->m_X[j],
						this->m_pLattice->m_Y[j],
						this->m_pLattice->m_Z[j]);
					out << LPCTSTR(coord);
				}
			}	
			out << "End of basis" << endl;
		}
// now add point charges
// count point charges
		int counter = 0;
		for(i=0;i<size;i++)
			if(m_pLattice->m_SymmetricallyIndependent[i] && !m_pLattice->m_selected[i])
				counter++;
		if(counter == 0) return TRUE;
// there are some non-selected atoms
		out << "XField" << endl << counter << endl;
		for(i=0;i<size;i++)
		{
			if(m_pLattice->m_SymmetricallyIndependent[i] && !m_pLattice->m_selected[i])
			{
				CString str;
				str.Format("%15.8lf  %15.8lf  %15.8lf  %15.8lf  0.0  0.0  0.0\n",
				m_pLattice->m_X[i]/BOHR,	// cartesian coordinates of point charges
				m_pLattice->m_Y[i]/BOHR,
				m_pLattice->m_Z[i]/BOHR,
				m_pLattice->m_charge[i]);	// point charge value. Dipole components are 0.0
				out << LPCTSTR(str);
			}
		}
		out << "Test" << endl << "RELInt" << endl << "End of input" << endl << endl;
// output the SCF section
		out << " &scf &end" << endl;
		out << "Title" << endl << "...complex" << endl;
		out << "Occupied" << endl;
		out << this->m_siteSymmetryIrreps << endl;
		out << "End of input" << endl;
		return TRUE;
	}
}
///////////////////////////////////////////////////////////////////////////////////
//	this function writes out the results in GAUSSIAN input file format
//
BOOL CMadelungDoc::WriteGaussianFile(std::ofstream& out)
{
	out << " *** GAUSSIAN input format file ***" << endl;
	out << " &seward &end" << endl;
	out << "Title" << endl << "KCuO complex from KCuO2 crystal structure" << endl;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	this function writes out the results in HYPERCHEM input file format
//
BOOL CMadelungDoc::WriteHyperChemFile(std::ofstream& out)
{
	out << " *** HYPERCHEM input format file ***" << endl;
	out << " &seward &end" << endl;
	out << "Title" << endl << "KCuO complex from KCuO2 crystal structure" << endl;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
//	this function writes out the results in HYPERCHEM input file format
//
BOOL CMadelungDoc::WriteMadelungFile(std::ofstream& out)
{
	string str = m_pSpaceGroup->m_Long_notation;
// remove spaces
	int pos = 0;
	while((pos = str.find(" ",pos)) != string::npos)
		str.erase(pos++,1);

	out << str.c_str() << endl;
// save the spacegroup description
	out << m_pSpaceGroup->m_pUnitCell->m_a << endl;
	out << m_pSpaceGroup->m_pUnitCell->m_b << endl;
	out << m_pSpaceGroup->m_pUnitCell->m_c << endl;
	out << m_pSpaceGroup->m_pUnitCell->m_alpha << endl;
	out << m_pSpaceGroup->m_pUnitCell->m_beta << endl;
	out << m_pSpaceGroup->m_pUnitCell->m_gamma << endl;
// WITHOUT symmetric Wyckoff positions (one atom per position)
	int numat = m_pSpaceGroup->m_pUnitCell->m_positionsInUnitCell;
	out << numat << endl;

	for(int i=0; i<numat;i++)
	{
		out << m_pSpaceGroup->m_pUnitCell->m_WyckoffPosition[i] << " ";
		out << m_pSpaceGroup->m_pUnitCell->m_unitCellAtomX[i] << " ";
		out << m_pSpaceGroup->m_pUnitCell->m_unitCellAtomY[i] << " ";
		out << m_pSpaceGroup->m_pUnitCell->m_unitCellAtomZ[i] << " ";
		out << m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCharge[i] << " ";
		out << m_pSpaceGroup->m_pUnitCell->m_unitCellAtomType[i] << endl;
	}
// write out MOLECULES, if any
	int size = this->m_molecules.size(); // number of molecules to write out
	out << size << endl;

	for(int molecule=0;molecule<size;molecule++)
	{
		out << molecule << ' ' << m_molecules[molecule]->GetName().c_str() << endl;
		double center[3];
		double orientation[3];
		double coord[3];
		double charge;
		int type;
// center of the molecule
		m_molecules[molecule]->GetCenter(center);
		for(int j=0;j<3;j++)
			out << center[j] << ' ';
		out << endl;
// orientation of the molecule
		m_molecules[molecule]->GetOrientation(orientation);
		for(j=0;j<3;j++)
			out << orientation[j] << ' ';
		out << endl;
// atoms of the molecule
		int atoms = m_molecules[molecule]->GetSize();
		out << atoms << endl;
		for(j=0;j<atoms;j++)
		{
			m_molecules[molecule]->GetAtom(j,coord,type,charge);
			out << j << ' ';
			for(int k=0;k<3;k++)
				out << coord[k] << ' ';
			out << type << ' ' << charge << endl;
		}
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//	returns the number of selected atoms in CLattice
//
int CMadelungDoc::GetNumberOfSelectedAtoms()
{
	int atoms = 0;
	int size = m_pLattice->GetSize();
	for(int i=0;i<size;i++)
		if(m_pLattice->m_selected[i]) atoms++;
	return atoms;
}

/////////////////////////////////////////////////////////////////////////////////
//	tries to use the point symmetry of the selected atoms cluster
//
void CMadelungDoc::OnToolsSymmetry() 
{
// clear all previously allocated symmetry elements
	if(m_RefinedElements && m_numberOfRefinedSymmetryElements != 0)
	{
		for(int i=0;i<m_numberOfRefinedSymmetryElements;i++)
			delete m_RefinedElements[i];
		m_RefinedElements = NULL;
		m_numberOfRefinedSymmetryElements = 0;
	}
// find all selected atoms in CLattice and store them in the list
	int latticeSize = this->m_pLattice->GetSize();
	CList<int,int> selectedAtomsList;
// count the TOTAL selected atoms
	this->m_selectedAtoms = 0;
// count ALL selected atoms
	for(int i=0;i<latticeSize;i++)
	{
		if(m_pLattice->m_selected[i]) {
			selectedAtomsList.AddTail(i);
			this->m_selectedAtoms++;
		}
	}
// find if the geometrical center of selected atoms list

	double geomCenter_UnitCell[3]; // coordinates in the units of UnitCell axes
	for(i=0;i<3;i++)
		m_geomCenter[i] = 0.;

	POSITION pos = selectedAtomsList.GetHeadPosition();
	for(i=0;i<this->m_selectedAtoms; i++)
	{
		int next = selectedAtomsList.GetNext(pos);
		m_geomCenter[0] += m_pLattice->m_X[next];
		m_geomCenter[1] += m_pLattice->m_Y[next];
		m_geomCenter[2] += m_pLattice->m_Z[next];
	}
	for(i=0;i<3;i++)
		m_geomCenter[i] /= this->m_selectedAtoms;
// now check if geomCenter[] is in a special position of the space group
	m_WPos = this->CheckForSpecialPosition(m_geomCenter,geomCenter_UnitCell);
	CString str;
	str.Format("Site (%9.4f,%9.4f,%9.4f) in unit cell coordinates, has symmetry %c",
		geomCenter_UnitCell[0],geomCenter_UnitCell[1],geomCenter_UnitCell[2],m_WPos);
//	AfxMessageBox(LPCTSTR(str),MB_OK);
	SymmetryDialog sd;
	sd.m_symmetryText = str;
	int ret = sd.DoModal();
	if(ret == IDOK)
		this->m_bUseSymmetry = true;
}

//////////////////////////////////////////////////////////////////////////////////
//
//
void CMadelungDoc::OnUpdateToolsSymmetry(CCmdUI* pCmdUI) 
{
	if(this->GetNumberOfSelectedAtoms())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

//////////////////////////////////////////////////////////////////////////////////
//	returns a pointer to the CMadelungView
//
CMadelungView* CMadelungDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	CMadelungView* pV = (CMadelungView*)GetNextView(pos);
	return pV;
}

////////////////////////////////////////////////////////////////////////////////////
//	checks if geomCenter[] is in a special Wyckoff position
//  the general position is not used since its symmetry is 1 and calculation is continued
//	without symmetry.
//  returns - Wyckoff position as a char and modulo 1 coordinates in the UnitCell, in dimension
//	          geomCenter_UnitCell[3]
//
char CMadelungDoc::CheckForSpecialPosition(double geomCenter[3],double geomCenter_UnitCell[3])
{
	double c1 = geomCenter[0];
	double c2 = geomCenter[1];
	double c3 = geomCenter[2];
	double r = 0.;
// find the length of the geomCenter[] vector
	for(int i=0;i<3;i++)
		r += geomCenter[i]*geomCenter[i];
	r =sqrt(r);
// geometrical center of the unit cell. If it is out of the 1st unit cell, it is translated
// into it.
	int ret = 
		m_pSpaceGroup->m_pUnitCell->CalcUCCoordFromCartCoord(geomCenter,geomCenter_UnitCell);
	if (ret == -1) return ' ';

	for(i=0;i<this->m_pSpaceGroup->m_numberOfWyckoffSets;i++)
	{
		char pos = 'a' + i;
// calculate the equal positions set for the geomCenter_UnitCell[]
		EqPositions* eqPos = m_pSpaceGroup->GetEqPos(geomCenter_UnitCell,pos);
// compare geomCenter[] coordinates with eqPos coordinates
		int size = eqPos->m_size;
		for(int j=0;j<size;j++)
		{
// check if the eqPos is within the first unitcell. Otherwise translate it there.
// it is necessary for the Wyckoff position checking, since coordinates are modulo 1
			int counter = 0;
			double eqPos_uc[3];
//			double tx = eqPos->m_X[j];
			eqPos_uc[0] = eqPos->m_X[j];
//			while(fabs(tx) >= 1.)
			while(fabs(eqPos_uc[0]) >= 1.)
			{
//				if(tx > 0.) eqPos_uc[0] = tx - 1.;
//				if(tx < 0.) eqPos_uc[0] = tx + 1.;
				if(eqPos_uc[0] > 0.) eqPos_uc[0] -= 1.;
				if(eqPos_uc[0] < 0.) eqPos_uc[0] += 1.;
				if(counter++ > 1000) 
				{
					AfxMessageBox("CMadelungDoc:Cannot determine position X of center",MB_OK);
					return '%';
				}
			}
			counter = 0;
			eqPos_uc[1] = eqPos->m_Y[j];
			while(fabs(eqPos_uc[1]) >= 1.)
			{
//				if(ty > 0.) eqPos_uc[1] = ty - 1.;
//				if(ty < 0.) eqPos_uc[1] = ty + 1.;
				if(eqPos_uc[1] > 0.) eqPos_uc[1] -= 1.;
				if(eqPos_uc[1] < 0.) eqPos_uc[1] += 1.;
				if(counter++ > 1000) 
				{
					AfxMessageBox("CMadelungDoc:Cannot determine position Y of center",MB_OK);
					return '%';
				}
			}
			counter = 0;
			eqPos_uc[2] = eqPos->m_Z[j];
			while(fabs(eqPos_uc[2]) >= 1.)
			{
				if(eqPos_uc[2] > 0.) eqPos_uc[2] -= 1.;
				if(eqPos_uc[2] < 0.) eqPos_uc[2] += 1.;
				if(counter++ > 1000) 
				{
					AfxMessageBox("CMadelungDoc:Cannot determine position Z of center",MB_OK);
					return '%';
				}
			}
			double diff = 0.;
			double d;
// calculate the position error				
// for 'a' axis
			if(!m_pSpaceGroup->m_WIsEditable_X)
			{
				d = geomCenter_UnitCell[0] - eqPos_uc[0];
				diff += d*d;
			}
// for 'b' axis
			if(!m_pSpaceGroup->m_WIsEditable_Y)
			{
				d = geomCenter_UnitCell[1] - eqPos_uc[1];
				diff += d*d;
			}
// for 'c' axis
			if(!m_pSpaceGroup->m_WIsEditable_Z)
			{
				d = geomCenter_UnitCell[2] - eqPos_uc[2];
				diff += d*d;
			}
// check for error
			if(diff > DISTANCE_TOLERANCE) continue;
			else
				return pos;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////
//	this function:
//	1) finds symmetry elements for a specified Wyckoff position
//  2) finds symmetry independent atoms
//
void CMadelungDoc::ApplySymmetry()
{
// find the point symmetry elements for the selected atoms cluster
//	this->FindPointGroupForSelectedAtoms();
// use only the oriented site-symmetry operations listed in the International Tables
// for the selected Wyckoff position (geom.center of the selected atoms cluster)
	this->RefineSymmetryElements();
// rotate atoms to align symmetry elements with coordinate axes, find symmetry independent atoms
	this->FinalizeSymmetrization();
}

//////////////////////////////////////////////////////////////////////////////////
//
//
void CMadelungDoc::FindPointGroupForSelectedAtoms()
{
// used to write out the selected atom coordinates for the reading in the CSymmetry object
	FILE* tmpFile;
	tmpFile = fopen("selectedTemp.txt","w+");
	fprintf(tmpFile,"%d%\n",this->m_selectedAtoms);
// find selected atoms
	int latticeSize = this->m_pLattice->GetSize();
	for(int i=0;i<latticeSize;i++)
	{
		if(m_pLattice->m_selected[i])
			fprintf(tmpFile,"%d %10.6f %10.6f %10.6f\n",m_pLattice->m_type[i],m_pLattice->m_X[i],
					m_pLattice->m_Y[i],m_pLattice->m_Z[i]);
	}
	fclose(tmpFile);
// create a CSymmetry object and determine the point symmetry of the selected atoms cluster
	CSymmetry symm;
	symm.read_coordinates("selectedTemp.txt");
	symm.find_symmetry_elements();
	symm.sort_symmetry_elements();
	symm.summarize_symmetry_elements();
//	if(symm.BadOptimization)
//    fprintf( "Refinement of some symmetry elements was terminated before convergence was reached.\n"
//            "Some symmetry elements may remain unidentified.\n" ) ;
    symm.report_symmetry_elements_verbose();
	symm.report_symmetry_elements_brief();
//	m_pointGroup = string(symm.identify_point_group());
}

//////////////////////////////////////////////////////////////////////////////////
// use only the oriented site-symmetry operations listed in the International Tables
// for the selected Wyckoff position (geom.center of the selected atoms cluster)
// Called from 'SymmetryDialog.cpp'
void CMadelungDoc::RefineSymmetryElements()
{
// retrieve the site symmetry of the center of cluster (center of something in symmetry.dll)
	m_siteSymmetry = this->m_pSpaceGroup->m_WSymmetry[this->m_pSpaceGroup->
			m_numberOfWyckoffSets - (m_WPos-'a')-1];
// open file with the symmetry elements (created in symmetry.dll)
	FILE* f = fopen("SymElem.txt","r");
	if(f == NULL)
	{
		AfxMessageBox("SymElem.txt file missing",MB_OK);
		exit(1);
	}
// distinguish between different crystal systems.
// They write out a file 'RefinedSymmetryElements.txt' with refined symmetry elements.
	int ret;
	if(this->m_pSpaceGroup->m_CrystalSystem == "Triclinic")
		ret = this->RefineTriclinic(m_siteSymmetry,f);
	else
		if(this->m_pSpaceGroup->m_CrystalSystem == "Monoclinic")
			ret = this->RefineMonoclinic(m_siteSymmetry,f);
	else
		if(this->m_pSpaceGroup->m_CrystalSystem == "Orthorhombic")
			ret = this->RefineOrthorhombic(m_siteSymmetry,f);
	else
		if(this->m_pSpaceGroup->m_CrystalSystem == "Tetragonal")
			ret = this->RefineTetragonal(m_siteSymmetry,f);
	else
		if(this->m_pSpaceGroup->m_CrystalSystem == "Hexagonal")
			ret = this->RefineHexagonal(m_siteSymmetry,f);
	else
		if(this->m_pSpaceGroup->m_CrystalSystem == "Rhombohedral")
			ret = this->RefineRhombohedral(m_siteSymmetry,f);
	else
		if(this->m_pSpaceGroup->m_CrystalSystem == "Cubic")
			ret = this->RefineCubic(m_siteSymmetry,f);
	else
	{
		AfxMessageBox("Undefined crystal system in CMadelungDoc",MB_OK);
		exit(1);
	}
	if(ret == 0)
		AfxMessageBox("Error in CMadelungDoc::RefineSymmetryElements",MB_OK);

	fclose(f);
}
///////////////////////////////////////////////////////////////////////////////////////
// rotate atoms to align symmetry elements with coordinate axes, find symmetry independent atoms
void CMadelungDoc::FinalizeSymmetrization()
{

}

////////////////////////////////////////////////////////////////////////////////////////
//	refines the TRICLINIC symmetry elements
//	returns the number of refined symmetry elements, including the '1' operation
//	returns '0' if not OK, and shows a message box
//	FILE* f contains ALL symmetry elements from Symmetry.dll
//
int CMadelungDoc::RefineTriclinic(string siteSymmetry,FILE* f)
{
// Check for basic corretness of 'siteSymmetry'
	int size = siteSymmetry.length();
// the site symmetry point group is 'C1'
	if(size == 1 && siteSymmetry[0] == '1')
	{
		return 1; // OK. Only the identity operation. No symmetry can be used
	}
// INVERSION CENTER 
// the site symmetry point group is 'Ci'
	if(size == 2 && siteSymmetry.find("-1") != string::npos)
	{
// set up new symmetry element - inversion center, type = 0 in the file
// FIND INVERSION CENTER in TRICLINIC crystal system
		int ret = this->FindSymmetryElementsInFile("Triclinic",f,0,NULL);
		if(ret != 1) return 0;
		return ret;
	}
	else
		return 0;	// error, no symmetry element '1' or '-1'
}
////////////////////////////////////////////////////////////////////////////////////////
//	refines the MONOCLINIC symmetry elements
//	returns the number of refined symmetry elements, including the '1' operation
//	returns '0' if not OK, and shows a message box
//	FILE* f contains ALL symmetry elements from Symmetry.dll
//
int CMadelungDoc::RefineMonoclinic(string siteSymmetry,FILE* f)
{
// Check for basic corretness of 'siteSymmetry'
	int size = siteSymmetry.length();
	int type, ret, dir;

	if(size > 3)
	{
		AfxMessageBox("Too many symbols in siteSymmetry symbol",MB_OK);
		return 0;
	}
// IDENTITY OPERATION
// the site symmetry point group is 'C1'
	if((size == 1) && (siteSymmetry[0] == '1'))
	{
		return 1; // only the identity operation. No symmetry can be used
	}
// INVERSION CENTER IN MONOCLINIC crystal system
// the site symmetry point group is 'Ci'
	if(size == 2 && siteSymmetry.find("-1") != string::npos)
	{
// dummy direction. For non-dummy use 1,2,3 as for directions in International Tables, page 15.
		ret = this->FindSymmetryElementsInFile("Monoclinic",f,0,NULL);
		if(ret != 1) return 0;
		else
			return 1;
	}
	if(size == 2 && siteSymmetry.find("-1") == string::npos)
	{
		AfxMessageBox("Error in symmetry symbol",MB_OK);
		return 0;
	}
// ONE SYMMETRY PLANE OR AXIS
	if((size == 1) && (siteSymmetry[0] == 'm' || siteSymmetry[0] == '2'))
	{
		if(siteSymmetry[0] == 'm')
			type = 3;
		if(siteSymmetry[0] == '2')
			type = 1;
// OK. Now test which is the unique axis ('b' or 'c') for the specified spacegroup.
		string str = this->m_pSpaceGroup->m_additionalInfo;
		if(str.find("unique axis c") != string::npos)
			dir = 3;
		else
			if(str.find("unique axis b") != string::npos)
				dir = 2;
		else
		{
			AfxMessageBox("Error in unique axis symbol",MB_OK);
			return 0;
		}
// FIND AXIS, PARALLEL TO 'C'('B') OR PLANE, PERPENDICULAR TO 'C'('B')
// Possible site symmetry point groups are 'Cs' and 'C2'
		ret = this->FindSymmetryElementsInFile("Monoclinic",f,type,dir);
		if(ret != 1) return 0;
		return 1;
	}
// SECOND ORDER AXIS AND A PLANE, PERPENDICULAR TO IT
	if(size == 3 && siteSymmetry.find("2/m") != string::npos)
	{
// The site symmetry point group is 'C2h'
// OK. Now find the unique axis ('b' or 'c')
		string str = this->m_pSpaceGroup->m_additionalInfo;
		if(str.find("unique axis c") != string::npos)
			dir = 3;
		else
			if(str.find("unique axis b") != string::npos)
				dir = 2;
		else
		{
			AfxMessageBox("Error in unique axis symbol",MB_OK);
			return 0;
		}

		int nmb = 0;
// set up new symmetry element - 2nd order axis, type = 1 in the file
		type = 1;	// axis type == 1
// 'c' direction. For non-dummy use 1,2,3 as for directions in International Tables, page 15.
		if(this->FindSymmetryElementsInFile("Monoclinic",f,type,dir) != 0) nmb++;
// reflection plane, type = 3
		type = 3;
		if(this->FindSymmetryElementsInFile("Monoclinic",f,type,dir) != 0) nmb++;
// inversion center, type = 0
		type = 0;
		if(this->FindSymmetryElementsInFile("Monoclinic",f,type,NULL) != 0) nmb++;
		
		if(nmb == 3) return 3;
		else
		{
			AfxMessageBox("Error C2h group symmetry elements",MB_OK);
			return 0;
		}
	}
	if(size == 3 && siteSymmetry.find("2/m") == string::npos)
	{
		AfxMessageBox("Error in symmetry symbol",MB_OK);
		return 0;
	}
	else
		return 0;	// error, if something else
}

////////////////////////////////////////////////////////////////////////////////////////
//	refines the ORTHORHOMBIC symmetry elements
//	returns the number of refined symmetry elements, including the '1' operation
//	returns '0' if not OK, and shows a message box
//	FILE* f contains ALL symmetry elements from Symmetry.dll
//
int CMadelungDoc::RefineOrthorhombic(string siteSymmetry,FILE* f)
{
// Check for basic corretness of 'siteSymmetry'
	int size = siteSymmetry.length();
	int type, ret, dir;
// only the IDENTITY operation. No symmetry can be used
	if((size == 1) && (siteSymmetry[0] == '1'))
	{
		return 1; // OK
	}
// check whether '.' are present
	bool dotsArePresent = (siteSymmetry.find(".") == string::npos) ? false : true;
	if(dotsArePresent)
	{
// count the number of dots
		int numberOfDots = 0;
		for(int i=0;i<size;i++)
			if(siteSymmetry[i] == '.')
				numberOfDots++;
// if numberOfDots == 1 - error
		if(numberOfDots == 1) return 0;
		if(numberOfDots == 2) 
		{
// check the number of symbols
			if(size != 3 && size != 5)
				return 0;	// error
			if(size == 3)
			{
// possible site symmetry groups are 'C2' or 'Cs'
// find the direction of symmetry element
				for(i=0;i<size;i++)
					if(siteSymmetry[i] != '.')
					{
						dir = i+1;
						break;
					}
				if(siteSymmetry[i] == '2')
					type = 1;
				else
					if(siteSymmetry[i] == 'm')
						type = 3;
				else
					return 0;	// error, symbol must be '2' or 'm'
				ret = this->FindSymmetryElementsInFile("Orthorhombic",f,type,dir);
				if(ret != 1) return 0;
				return 1;
			}
			if(size == 5)
			{
// check for corretness
				if(siteSymmetry.find("2/m") != string::npos) // OK. '2/m' found
				{
// the site symmetry point group is 'C2h(x,y,z)'
// find the direction of symmetry element
					for(int i=0;i<size;i++)
						if(siteSymmetry[i] != '.')
							dir = i+1;
					int nmb = 0;
// set up new symmetry element - 2nd order axis, type = 1 in the file
					type = 1;	// axis type == 1
// 'c' direction. For non-dummy use 1,2,3 as for directions in International Tables, page 15.
					if(this->FindSymmetryElementsInFile("Orthorhombic",f,type,dir) != 0) nmb++;
// reflection plane, type = 3
					type = 3;
					if(this->FindSymmetryElementsInFile("Orthorhombic",f,type,dir) != 0) nmb++;
// inversion center, type = 0
					type = 0;
					if(this->FindSymmetryElementsInFile("Orthorhombic",f,type,NULL) != 0) nmb++;
					if(nmb == 3) return 3;
					else
					{
						AfxMessageBox("Error C2h group symmetry elements",MB_OK);
						return 0;
					}
				}
				else
					return 0;	// error, '2/m' not found
			}
		}
	}
	else
// dots are not present
	{
		if(size == 3)
		{
// check if all symbols are '2' or 'm'
			for(int i=0;i<size;i++)
				if(siteSymmetry[i] == '.')
					return 0;	// error. If length == 3, all symbols must be '2' or 'm'
// now get symmetry elements from the file 'f'
			int nmb = 0; // number of symmetry elements returned
			if(siteSymmetry == "mmm")
			{
// the site symmetry point group is 'D2h'
// refine planes
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,3,1) !=0) nmb++;
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,3,2) !=0) nmb++;
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,3,3) !=0) nmb++;
// refine axes
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,1,1) !=0) nmb++;
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,1,2) !=0) nmb++;
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,1,3) !=0) nmb++;
// inversion center
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,0,0) !=0) nmb++;

				if(nmb != 7)
					return 0;
				else
					return nmb;
			}
			else
			if(siteSymmetry == "2mm")
			{
// the site symmetry point group is 'C2v(x)'
// axis along the 'x' direction, type = 1
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,1,1) !=0) nmb++;
// planes with normals along 'y' and 'z' directions, type = 3
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,3,2) !=0) nmb++;
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,3,3) !=0) nmb++;
				if(nmb != 3)
					return 0;	// error
				return nmb;		// OK
			}
			else
			if(siteSymmetry == "m2m")
			{
// the site symmetry point group is 'C2v(y)'
// axis along the 'y' direction, type = 1
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,1,2) !=0) nmb++;
// planes with normals along 'x' and 'z' directions, type = 3
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,3,1) !=0) nmb++;
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,3,3) !=0) nmb++;
				if(nmb != 3)
					return 0;	// error
				return nmb;		// OK
			}
			else
			if(siteSymmetry == "mm2")
			{
// the site symmetry point group is 'C2v(z)'
// axis along the 'z' direction, type = 1
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,1,3) !=0) nmb++;
// planes with normals along 'x' and 'y' directions, type = 3
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,3,1) !=0) nmb++;
				if(this->FindSymmetryElementsInFile("Orthorhombic",f,3,2) !=0) nmb++;
				if(nmb != 3)
					return 0;	// error
				return nmb;		// OK
			}
			else
			{
				AfxMessageBox("Implement symmetry element refinement in CMadelungDoc, line 2077",MB_OK);
				return 0;
			}
		}
		if(size == 2 && siteSymmetry.find("-1") != string::npos)
		{
// set up new symmetry element - inversion center, type = 0 in the file
			type = 0;	// inversion center type
// dummy direction. For non-dummy use 1,2,3 as for directions in International Tables, page 15.
// the site symmetry point group is 'Ci'
			dir = 0;
			ret = this->FindSymmetryElementsInFile("Orthorhombic",f,type,dir);
			if(ret != 1) return 0;
			return 1;
		}
		if(size == 2 && siteSymmetry.find("-1") == string::npos)
		{
			AfxMessageBox("Error in symmetry symbol",MB_OK);
			return 0;
		}
	} // dots are not present
}

////////////////////////////////////////////////////////////////////////////////////////
// finds required symmetry elements in the file and stores them in the m_RefinedElements
// on failure, return 0; otherwise the number of symmetry elements found.
//	
int CMadelungDoc::FindSymmetryElementsInFile(string crystalSystem,	// triclinic,moclinic,etc
											 FILE* f, // file from Symmetry.dll
											 int elementType, // 0-inv,1-axis,3-plane
											 int direction)	// 1-'a',2-'b',3-'c' (dir.of symmetry element)
{
	rewind(f);

	bool find_OK = false; // true if element has been found in the file 'f'.
	int numberOfElements;
	int elem_type,order,nparam;
	double maxdev,distance,normal[3],dir[3];

// for TRICLINIC system
	if(crystalSystem == "Triclinic")
	{
		if(elementType == 0)	// INVERSION CENTER
		{
// Try to pick the relevant symmetry element from the file 'f'
// read symmetry elements file
			numberOfElements = 0;
			while(fscanf(f,"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf\n",&elem_type,&order,&nparam,&maxdev,
				&distance,&normal[0],&normal[1],&normal[2],&dir[0],&dir[1],&dir[2]) != EOF)
			{
				if(elem_type != 0) continue;
				else
				{
					find_OK = true;
					numberOfElements++;
					if(numberOfElements > 1)
					{
						AfxMessageBox("Too many inversion centers",MB_OK);
						return 0;	// error condition
					}
					SYMMETRY_ELEMENT* ic = AllocSymmetryElement(invert_atom);
					ic->order = order;
					ic->nparam = nparam;
					ic->maxdev = maxdev;
					ic->distance = distance;
					for(int i=0;i<3;i++)
					{
						ic->normal[i] = normal[i];
						ic->direction[i] = dir[i];
					}
				}
			}
			if(find_OK == false)
			{
				AfxMessageBox("No symmetry element found",MB_OK);
				return 0;
			}
			else
				return numberOfElements;	// OK
		}
		else
		{	// triclinic system must contain only one inversion center
			AfxMessageBox("Erroneous symmetry element found",MB_OK);
			return 0;
		}
	}
// END OF TRICLINIC SYSTEM
// START FOR MONOCLINIC SYSTEM
	else 
	if(crystalSystem == "Monoclinic")
	{
		if(elementType == 0)	// INVERSION CENTER
		{
// Try to pick the relevant symmetry element from the file 'f'
// read symmetry elements file
			numberOfElements = 0;
			while(fscanf(f,"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf\n",&elem_type,&order,&nparam,&maxdev,
				&distance,&normal[0],&normal[1],&normal[2],&dir[0],&dir[1],&dir[2]) != EOF)
			{
				if(elem_type != 0) continue;
				else
				{
					find_OK = true;
					numberOfElements++;
					if(numberOfElements > 1)
					{
						AfxMessageBox("Too many inversion centers",MB_OK);
						return 0;	// error condition
					}
					SYMMETRY_ELEMENT* ic = AllocSymmetryElement(invert_atom);
					ic->order = order;
					ic->nparam = nparam;
					ic->maxdev = maxdev;
					ic->distance = distance;
					for(int i=0;i<3;i++)
					{
						ic->normal[i] = normal[i];
						ic->direction[i] = dir[i];
					}
				}
			}
			if(find_OK == false)
			{
				AfxMessageBox("No symmetry element found",MB_OK);
				return 0;
			}
			else
				return numberOfElements;	// OK
		}
		else
			if(elementType == 1)	// 2nd ORDER AXIS
			{
// check the direction, since it can only be 'b' or 'c'
				if(direction != 2 && direction != 3)
				{
					AfxMessageBox("Axis direction must be 'b' or 'c'",MB_OK);
					return 0;
				}

// Try to pick the relevant symmetry element from the file 'f'
				numberOfElements = 0;
				while(fscanf(f,"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf\n",&elem_type,&order,&nparam,&maxdev,
					&distance,&normal[0],&normal[1],&normal[2],&dir[0],&dir[1],&dir[2]) != EOF)
				{
					if(elem_type != 1) continue;
					else
					{
						if(order != 2) 
						{
							AfxMessageBox("Incorrect order of axis",MB_OK);
							return 0;
						}

						double axis[3];
						for(int i=0;i<3;i++)
						{
							if(direction == 2)	// 'b' axis
								axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorB_Cart[i];
							else
								if(direction == 3)	// 'c' axis
									axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorC_Cart[i];
							else
							{
								AfxMessageBox("Axis direction must be 'b' or 'c'",MB_OK);
								return 0;
							}
						}
						double vectprod[3];
						bool parallel = true;
						vectprod[0] = axis[1]*dir[2]-axis[2]*dir[1];
						vectprod[1] = axis[2]*dir[0]-axis[0]*dir[2];
						vectprod[2] = axis[0]*dir[1]-axis[1]*dir[0];
// Check if detected symmetry element is parallel to a unit cell axis ('b' or 'c')
						for(int j=0;j<3;j++)
						{
							if(fabs(vectprod[j]) > 1.e-6)
							{
								parallel = false;
								break;
							}
						}
						if(!parallel)
							continue;
// axis found and is parallel to a unit cell axis
						find_OK = true;
						numberOfElements++;
						if(numberOfElements > 1)
						{
							AfxMessageBox("Too many 2nd order axes",MB_OK);
							return 0;	// error condition
						}
						SYMMETRY_ELEMENT* ic = AllocSymmetryElement(rotate_atom);
						ic->order = order;
						ic->nparam = nparam;
						ic->maxdev = maxdev;
						ic->distance = distance;
						for(i=0;i<3;i++)
						{
							ic->normal[i] = normal[i];
							ic->direction[i] = dir[i];
						}
					}
				}
				if(find_OK == false)
				{
					AfxMessageBox("No symmetry element found",MB_OK);
					return 0;
				}
				else
					return numberOfElements;	// OK
			}
		else
			if(elementType == 3)	// SYMMETRY PLANE
			{
// check the direction, since it can only be 'b' or 'c'
				if(direction != 2 && direction != 3)
				{
					AfxMessageBox("Plane normal direction must be 'b' or 'c'",MB_OK);
					return 0;
				}

// Try to pick the relevant symmetry element from the file 'f'
				numberOfElements = 0;
				while(fscanf(f,"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf\n",&elem_type,&order,&nparam,&maxdev,
					&distance,&normal[0],&normal[1],&normal[2],&dir[0],&dir[1],&dir[2]) != EOF)
				{
					if(elem_type != 3) continue;
					else
					{
						double axis[3];
						for(int i=0;i<3;i++)
						{
							if(direction == 2)	// 'b' axis
								axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorB_Cart[i];
							else
								if(direction == 3)	// 'c' axis
									axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorC_Cart[i];
							else
							{
								AfxMessageBox("Axis direction must be 'b' or 'c'",MB_OK);
								return 0;
							}
						}
						double vectprod[3];
						bool parallel = true;
						vectprod[0] = axis[1]*normal[2]-axis[2]*normal[1];
						vectprod[1] = axis[2]*normal[0]-axis[0]*normal[2];
						vectprod[2] = axis[0]*normal[1]-axis[1]*normal[0];
// Check if detected symmetry element is parallel to a unit cell axis ('b' or 'c')
						for(int j=0;j<3;j++)
						{
							if(fabs(vectprod[j]) > 1.e-6)
							{
								parallel = false;
								break;
							}
						}
						if(!parallel)
							continue;
// axis found and is parallel to a unit cell axis
						find_OK = true;
						numberOfElements++;
						if(numberOfElements > 1)
						{
							AfxMessageBox("Too many planes",MB_OK);
							return 0;	// error condition
						}
						SYMMETRY_ELEMENT* ic = AllocSymmetryElement(mirror_atom);
						ic->order = order;
						ic->nparam = nparam;
						ic->maxdev = maxdev;
						ic->distance = distance;
						for(i=0;i<3;i++)
						{
							ic->normal[i] = normal[i];
							ic->direction[i] = dir[i];
						}
					}
				}
				if(find_OK == false)
				{
					AfxMessageBox("No symmetry element found",MB_OK);
					return 0;
				}
				else
					return numberOfElements;	// OK
			}
		else
		{	// triclinic system must contain only one inversion center
			AfxMessageBox("Erroneous symmetry element found",MB_OK);
			return 0;
		}
	}
// for ORTHORHOMBIC system //
	else
	if(crystalSystem == "Orthorhombic")
	{
		if(elementType == 0)	// INVERSION CENTER
		{
// Try to pick the relevant symmetry element from the file 'f'
// read symmetry elements file
			numberOfElements = 0;
			while(fscanf(f,"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf\n",&elem_type,&order,&nparam,&maxdev,
				&distance,&normal[0],&normal[1],&normal[2],&dir[0],&dir[1],&dir[2]) != EOF)
			{
				if(elem_type != 0) continue;
				else
				{
					find_OK = true;
					numberOfElements++;
					if(numberOfElements > 1)
					{
						AfxMessageBox("Too many inversion centers",MB_OK);
						return 0;	// error condition
					}
					SYMMETRY_ELEMENT* ic = AllocSymmetryElement(invert_atom);
					ic->order = order;
					ic->nparam = nparam;
					ic->maxdev = maxdev;
					ic->distance = distance;
					for(int i=0;i<3;i++)
					{
						ic->normal[i] = normal[i];
						ic->direction[i] = dir[i];
					}
				}
			}
			if(find_OK == false)
			{
				AfxMessageBox("No symmetry element found",MB_OK);
				return 0;
			}
			else
				return numberOfElements;	// OK
		}
		else
			if(elementType == 1)	// 2nd ORDER AXIS
			{
// check the direction, since it can only be 'b' or 'c'
				if(direction != 1 && direction != 2 && direction != 3)
				{
					AfxMessageBox("Axis direction must be 'a','b' or 'c'",MB_OK);
					return 0;
				}

// Try to pick the relevant symmetry element from the file 'f'
				numberOfElements = 0;
				while(fscanf(f,"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf\n",&elem_type,&order,&nparam,&maxdev,
					&distance,&normal[0],&normal[1],&normal[2],&dir[0],&dir[1],&dir[2]) != EOF)
				{
					if(elem_type != 1) continue;
					else
					{
// the next line deals with tetragonal systems, ignoring the (110) type axes
						if(fabs(dir[0]) > DISTANCE_TOLERANCE && fabs(dir[1]) > DISTANCE_TOLERANCE)
							continue;
// check for the axis order
						if(order != 2) 
						{
// the next line needed if calculating tetragonal lattices as orthorhombic
							if(order == 4) // for tetragonal axis, ignore it
							{
								continue;
							}
							else
							{
								AfxMessageBox("Incorrect order of axis",MB_OK);
								return 0;
							}
						}

						double axis[3];
						for(int i=0;i<3;i++)
						{
							if(direction == 1)	// 'a' axis
								axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorA_Cart[i];
							else
								if(direction == 2)	// 'b' axis
									axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorB_Cart[i];
							else
								if(direction == 3)	// 'c' axis
									axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorC_Cart[i];
							else
							{
								AfxMessageBox("Axis direction must be 'a','b' or 'c'",MB_OK);
								return 0;
							}
						}
						double vectprod[3];
						bool parallel = true;
						vectprod[0] = axis[1]*dir[2]-axis[2]*dir[1];
						vectprod[1] = axis[2]*dir[0]-axis[0]*dir[2];
						vectprod[2] = axis[0]*dir[1]-axis[1]*dir[0];
// Check if detected symmetry element is parallel to a unit cell axis ('b' or 'c')
						for(int j=0;j<3;j++)
						{
							if(fabs(vectprod[j]) > 1.e-6)
							{
								parallel = false;
								break;
							}
						}
						if(!parallel)
							continue;
// axis found and is parallel to a unit cell axis
						find_OK = true;
						numberOfElements++;
						if(numberOfElements > 1)
						{
							AfxMessageBox("Too many 2nd order axes",MB_OK);
							return 0;	// error condition
						}
						SYMMETRY_ELEMENT* ic = AllocSymmetryElement(rotate_atom);
						ic->order = order;
						ic->nparam = nparam;
						ic->maxdev = maxdev;
						ic->distance = distance;
						for(i=0;i<3;i++)
						{
							ic->normal[i] = normal[i];
							ic->direction[i] = dir[i];
						}
					}
				}
				if(find_OK == false)
				{
					AfxMessageBox("No symmetry element found",MB_OK);
					return 0;
				}
				else
					return numberOfElements;	// OK
			}
		else
			if(elementType == 3)	// SYMMETRY PLANE
			{
// check the direction, since it can only be 'b' or 'c'
				if(direction != 1 && direction != 2 && direction != 3)
				{
					AfxMessageBox("Plane normal direction must be 'b' or 'c'",MB_OK);
					return 0;
				}

// Try to pick the relevant symmetry element from the file 'f'
				numberOfElements = 0;
				while(fscanf(f,"%d %d %d %lf %lf %lf %lf %lf %lf %lf %lf\n",&elem_type,&order,&nparam,&maxdev,
					&distance,&normal[0],&normal[1],&normal[2],&dir[0],&dir[1],&dir[2]) != EOF)
				{
					if(elem_type != 3) continue;
					else
					{
						double axis[3];
						for(int i=0;i<3;i++)
						{
							if(direction == 1)	// 'a' axis
								axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorA_Cart[i];
							else
								if(direction == 2)	// 'b' axis
									axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorB_Cart[i];
							else
								if(direction == 3)	// 'c' axis
									axis[i] = m_pSpaceGroup->m_pUnitCell->m_unitCellVectorC_Cart[i];
							else
							{
								AfxMessageBox("Axis direction must be 'a','b' or 'c'",MB_OK);
								return 0;
							}
						}
						double vectprod[3];
						bool parallel = true;
						vectprod[0] = axis[1]*normal[2]-axis[2]*normal[1];
						vectprod[1] = axis[2]*normal[0]-axis[0]*normal[2];
						vectprod[2] = axis[0]*normal[1]-axis[1]*normal[0];
// Check if detected symmetry element is parallel to a unit cell axis ('b' or 'c')
						for(int j=0;j<3;j++)
						{
							if(fabs(vectprod[j]) > 1.e-6)
							{
								parallel = false;
								break;
							}
						}
						if(!parallel)
							continue;
// axis found and is parallel to a unit cell axis
						find_OK = true;
						numberOfElements++;
						if(numberOfElements > 1)
						{
							AfxMessageBox("Too many symmetry planes",MB_OK);
							return 0;	// error condition
						}
						SYMMETRY_ELEMENT* ic = AllocSymmetryElement(mirror_atom);
						ic->order = order;
						ic->nparam = nparam;
						ic->maxdev = maxdev;
						ic->distance = distance;
						for(i=0;i<3;i++)
						{
							ic->normal[i] = normal[i];
							ic->direction[i] = dir[i];
						}
					}
				}
				if(find_OK == false)
				{
					AfxMessageBox("No symmetry element found",MB_OK);
					return 0;
				}
				else
					return numberOfElements;	// OK
			}
		else
		{	// triclinic system must contain only one inversion center
			AfxMessageBox("Erroneous symmetry element found",MB_OK);
			return 0;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////
//	this function allocates a new symmetry element
//  later the 'transform' member must be reallocated for ALL atoms
//
SYMMETRY_ELEMENT* CMadelungDoc::AllocSymmetryElement(void(*p)(SYMMETRY_ELEMENT *plane,
			ATOM_ *from,ATOM_ *to))
{
	SYMMETRY_ELEMENT* ic = (SYMMETRY_ELEMENT*)calloc(1,sizeof(SYMMETRY_ELEMENT));
	if( ic == NULL )
	{
		AfxMessageBox("Out of memory allocating symmetry element in CMadelungDoc\n",MB_OK);
		return NULL;
	}
// ic->transform later must be reallocated for ALL atoms (selected + extension)
	ic->transform = (int*)calloc(m_selectedAtoms,sizeof(int));
	if( ic->transform == NULL )
	{
		AfxMessageBox("Out of memory allocating symmetry element in CMadelungDoc\n",MB_OK);
		return 0;
	}
	ic->transform_atom = p;

	m_numberOfRefinedSymmetryElements++;	// does not count '1' symmetry operation
	m_RefinedElements = 
		(SYMMETRY_ELEMENT**)realloc(m_RefinedElements,sizeof(SYMMETRY_ELEMENT*) *
			m_numberOfRefinedSymmetryElements);
	if( m_RefinedElements == NULL )
	{
		AfxMessageBox("Out of memory allocating symmetry element in CMadelungDoc\n",MB_OK);
		return 0;
	}
	m_RefinedElements[m_numberOfRefinedSymmetryElements-1] = ic;
	return ic;
}
///////////////////////////////////////////////////////////////////////////////
//
//
int CMadelungDoc::RefineTetragonal(string siteSymmetry,FILE* f)
{
	AfxMessageBox("Tetragonal symmetry refinement not implemented yet",MB_OK);
	return 0;
}
int CMadelungDoc::RefineRhombohedral(string siteSymmetry,FILE* f)
{
	AfxMessageBox("Rhombohedral symmetry refinement not implemented yet",MB_OK);
	return 0;
}
int CMadelungDoc::RefineHexagonal(string siteSymmetry,FILE* f)
{
	AfxMessageBox("Hexagonal symmetry refinement not implemented yet",MB_OK);
	return 0;
}
int CMadelungDoc::RefineCubic(string siteSymmetry,FILE* f)
{
	AfxMessageBox("Cubic symmetry refinement not implemented yet",MB_OK);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////
//	finds and marks all symmetry independent atoms
//	Supported symmetry elements:
//  MIRROR PLANE
//	ROTATION AXIS
//	INVERSION CENTER
//
void CMadelungDoc::FindSymmetryIndependentAtoms(FILE* fp)
{
	int symIndepAtoms = 0;
	int size = m_pLattice->GetSize();
//	fprintf(fp,"Total number of atoms %d\n",size);
// loop over all atoms to see which are symmetry independent.
// at the beginning all atoms MUST be marked symmetry INDEPENDENT
	int atomsOnSymmetryElements = 0;
	for(int atom1 = 0;atom1<size;atom1++)
	{
		if(m_pLattice->m_SymmetricallyIndependent[atom1] == true)
// finds and marks all symmetry dependent atoms for 'atom1'
			MarkSymmetryDependentAtoms(fp,atom1);
		else
			continue;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
//	finds and marks all symmetry dependent atoms for 'atom1'
//  returns the number of symmetrically equivalent atoms of 'atom1'
//  returns '-1' if 'atom1' on a symmetry element
//  results printed in file 'fp'
//  before using the function, set all atoms 'm_pLattice->m_SymmetricallyIndependent[] = true'
//
void CMadelungDoc::MarkSymmetryDependentAtoms(FILE* fp,int atom1)
{
	ATOM_ atom;
	ATOM_ symmetric;
	int symDepAtoms = 0;

	int size = m_pLattice->GetSize();

	atom.x[0] = m_pLattice->m_X[atom1];
	atom.x[1] = m_pLattice->m_Y[atom1];
	atom.x[2] = m_pLattice->m_Z[atom1];
	atom.type = m_pLattice->m_type[atom1];

	for(int cse=0;cse<m_numberOfRefinedSymmetryElements;cse++)
	{
		SYMMETRY_ELEMENT* elem = m_RefinedElements[cse];
//		if(elem->transform_atom == mirror_atom)		// for test purposes
//			continue;

		elem->transform_atom(elem,&atom,&symmetric);
// loop over all atoms to find out a symmetric one
		for(int atc=0;atc<size;atc++)
		{
			if(m_pLattice->m_SymmetricallyIndependent[atc] == false)
				continue;

			double xx = (symmetric.x[0] - m_pLattice->m_X[atc]);
			double yy = (symmetric.x[1] - m_pLattice->m_Y[atc]);
			double zz = (symmetric.x[2] - m_pLattice->m_Z[atc]);
			double distance = sqrt(xx*xx + yy*yy + zz*zz);
			if(distance < DISTANCE_TOLERANCE)
			{
				if(m_pLattice->m_type[atc] != m_pLattice->m_type[atom1])
					fprintf(fp,"ERROR: atom %d symm.equivalent with atom %d but types different\n",atom1,atc);
				else
				{
// atom is symmetrically independent, mark it as a symmetry parent for itself
					m_pLattice->m_SymmetryParentAtom[atom1] = atom1;
// find distance of atom to symmetry element
					double la[3];
					la[0] = m_pLattice->m_X[atc];
					la[1] = m_pLattice->m_Y[atc];
					la[2] = m_pLattice->m_Z[atc];

					if(elem->transform_atom == mirror_atom)
					{
						if(DistanceToPlane(elem,la) < DISTANCE_TOLERANCE) // 'atom1' is in plane
						{
							fprintf(fp,"PLANE:atom %d in mirror plane\n",atc);
							m_pLattice->m_SymmetricallyIndependent[atc] = true;
							m_pLattice->m_SymmetryParentAtom[atc] = atom1;
							break;
						}
						else
						{
							fprintf(fp,"PLANE:atom %d symm.equivalent with atom %d\n",atom1,atc);
							m_pLattice->m_SymmetricallyIndependent[atc] = false;
							m_pLattice->m_SymmetryParentAtom[atc] = atom1;
							symDepAtoms++;
							break;
						}
					}
					else
					if(elem->transform_atom == rotate_atom)
					{
						if(DistanceToAxis(elem,la) < DISTANCE_TOLERANCE) // 'atom1' is on axis
						{
							fprintf(fp,"AXIS:atom %d on rotation axis\n",atc);
							m_pLattice->m_SymmetricallyIndependent[atc] = true;
							m_pLattice->m_SymmetryParentAtom[atc] = atom1;
							break;
						}
						else
						{
							fprintf(fp,"AXIS:atom %d symm.equivalent with atom %d\n",atom1,atc);
							m_pLattice->m_SymmetricallyIndependent[atc] = false;
							m_pLattice->m_SymmetryParentAtom[atc] = atom1;
							symDepAtoms++;
							break;
						}
					}
					else
					if(elem->transform_atom == invert_atom)
					{
						if(DistanceToInversionCenter(elem,la) < DISTANCE_TOLERANCE) // 'atom1' is in inversion center
						{
							fprintf(fp,"INV_CENTER:atom %d in inversion center\n",atc);
							m_pLattice->m_SymmetricallyIndependent[atc] = true;
							m_pLattice->m_SymmetryParentAtom[atc] = atom1;
							break;
						}
						else
						{
							fprintf(fp,"INV_CENTER:atom %d symm.equivalent with atom %d\n",atom1,atc);
							m_pLattice->m_SymmetricallyIndependent[atc] = false;
							m_pLattice->m_SymmetryParentAtom[atc] = atom1;
							symDepAtoms++;
							break;
						}
					}
					else
					{
						AfxMessageBox("Unknown symmetry element",MB_OK);
						exit(1);
					}
				} 
			} // end (distance < TOLERANCE)
			else
				continue;
		}
	}
	fprintf(fp,"%d symmetrically dependent atoms for atom %d\n",symDepAtoms,atom1);
}
//////////////////////////////////////////////////////////////////////////////////////
//	ensures that the m_pLattice has correct point symmetry.
//  Adds atoms to m_pLattice, if necessary, to make symmetric cluster
//	returns the number of inserted atoms
//
int CMadelungDoc::SymmetrizeExtension(FILE* fp)
{
	ATOM_ atom;
	ATOM_ symmetric;
	int atomsInsertedTotal = 0;

	int size = m_pLattice->GetSize();
	int size_var = size;	// used upon adding atoms
	list<int> addedAtoms;	// keeps numbers of added atoms

	for(int atom1=0;atom1<size;atom1++)
	{
		int atomsInserted = 0; // how many atoms inserted in the m_pLattice for 'atom1'
		if(m_pLattice->m_SymmetricallyIndependent[atom1] == false)
			continue;
		else
		{
// clear added atom list
			addedAtoms.clear();
// symmetrically independent atom. Now test if it has its all symmetrically dependent atoms
			atom.x[0] = m_pLattice->m_X[atom1];
			atom.x[1] = m_pLattice->m_Y[atom1];
			atom.x[2] = m_pLattice->m_Z[atom1];

			for(int cse=0;cse<m_numberOfRefinedSymmetryElements;cse++)
			{
				bool atomFound = false;
				SYMMETRY_ELEMENT* elem = m_RefinedElements[cse];
				elem->transform_atom(elem,&atom,&symmetric);
// loop over all atoms to find out a symmetric one
				for(int atc=0;atc<size_var;atc++)
				{
					double xx = (symmetric.x[0] - m_pLattice->m_X[atc]);
					double yy = (symmetric.x[1] - m_pLattice->m_Y[atc]);
					double zz = (symmetric.x[2] - m_pLattice->m_Z[atc]);
					double distance = sqrt(xx*xx + yy*yy + zz*zz);
					if(distance < DISTANCE_TOLERANCE) // OK. Symmetric atom is already in 'm_pLattice'
					{
						if(m_pLattice->m_type[atc] != m_pLattice->m_type[atom1])
							fprintf(fp,"ERROR: atom %d symm.equivalent with atom %d but types different\n",atom1,atc);
						else
						{
							atomFound = true;
							break;
						}
					}
				}
				if(!atomFound)
				{
// insert a new atom in the m_pLattice
					this->m_pLattice->m_X.push_back(symmetric.x[0]);
					this->m_pLattice->m_Y.push_back(symmetric.x[1]);
					this->m_pLattice->m_Z.push_back(symmetric.x[2]);

					this->m_pLattice->m_charge.push_back(m_pLattice->m_charge[atom1]);
					this->m_pLattice->m_type.push_back(m_pLattice->m_type[atom1]);

					this->m_pLattice->m_WPosition.push_back(m_pLattice->m_WPosition[atom1]);
// atom not selected initially
					this->m_pLattice->m_selected.push_back(false);
// atom not rendered initially
					this->m_pLattice->m_rendered.push_back(false);
// location of unit cell in lattice (initial unitcell is na=0,nb=0,nc=0).
// We use dummy unit cell here
					this->m_pLattice->m_unitCellA.push_back(-1);
					this->m_pLattice->m_unitCellB.push_back(-1);
					this->m_pLattice->m_unitCellC.push_back(-1);
// index of atom in UnitCell object. Used in work threads for calculations
					this->m_pLattice->m_atomInUnitCell.push_back(
								m_pLattice->m_atomInUnitCell[atom1]);
// symmetry handling parameters, treated later in symmetry routines.
// here we set parameters to dummy.
					this->m_pLattice->m_SymmetricallyIndependent.push_back(false);
					this->m_pLattice->m_SymmetryParentAtom.push_back(atom1);
// check whether this atom is on a symmetry element
					double d[3];
					for(int i=0;i<3;i++)
						d[i] = symmetric.x[i];
					if(elem->transform_atom == mirror_atom)
					{
						if(DistanceToPlane(elem,d) < DISTANCE_TOLERANCE) // 'symmetric' is in plane
							fprintf(fp,"PLANE:added symmetric atom %d in mirror plane\n",size_var);
					}
					if(elem->transform_atom == rotate_atom)
					{
						if(DistanceToAxis(elem,d) < DISTANCE_TOLERANCE) // 'symmetric' is on axis
							fprintf(fp,"AXIS:added symmetric atom %d on axis\n",size_var);
					}
					if(elem->transform_atom == invert_atom)
					{
						if(DistanceToInversionCenter(elem,d) < DISTANCE_TOLERANCE) // 'symmetric' is in inv.center
						{
							fprintf(fp,"INV.CENTER:added symmetric atom %d in inversion center\n",size_var);
						}
					}
					atomsInserted++;
					size_var++;
					addedAtoms.push_back(m_pLattice->GetSize()-1);
				}	// end !atomFound
			}	// end symm.elem.
		}	// end else
		if(atomsInserted)
		{
			fprintf(fp,"OK: %d sym.dependent atoms inserted for atom %d, ",atomsInserted,atom1);
			list<int>::iterator p = addedAtoms.begin();
			for(int k=0;k<addedAtoms.size();k++)
			{
				fprintf(fp,"%d ",*p);
				p++;
			}
			fprintf(fp,"\n");
			m_pLattice->m_SymmetryParentAtom[atom1] = atom1;
		}
		atomsInsertedTotal += atomsInserted;
	}	// end 'atom1' loop
	return atomsInsertedTotal;
}
//////////////////////////////////////////////////////////////////////////////////////
//
//
void CMadelungDoc::TransformExtension(FILE* fp)
{
	fprintf(fp,"Center of symmetry at %lf %lf %lf\n",m_geomCenter[0],
		m_geomCenter[1],m_geomCenter[2]);
// shift all lattice atoms to the new coordinate origin
	int size = m_pLattice->GetSize();
	for(int i=0;i<size;i++)
	{
		m_pLattice->m_X[i] -= m_geomCenter[0];
		m_pLattice->m_Y[i] -= m_geomCenter[1];
		m_pLattice->m_Z[i] -= m_geomCenter[2];
	}
// shift 'm_unitCellVertices[8][3]' in the UnitCell
	for(i=0;i<8;i++)
		for(int j=0;j<3;j++)
			m_pSpaceGroup->m_pUnitCell->m_unitCellVertices[i][j] -= m_geomCenter[j];
// shift actors to the new coordinate origin
 	POSITION pos = GetFirstViewPosition();
	CMadelungView* pv = (CMadelungView*)GetNextView(pos);
	pv->ShiftActors(m_geomCenter);
//	pv->Clear();
//	pv->UpdateWindow();
}
//////////////////////////////////////////////////////////////////////////////////////
//	calculates the distance from a point 'la' to plane 'elem'
//
double CMadelungDoc::DistanceToPlane(SYMMETRY_ELEMENT* elem,double la[3])
{
	double pp[3];
// a point on plane
	for(int i=0;i<3;i++)
		pp[i] = elem->distance*elem->normal[i];
	double sp = 0.;
	double norm = 0.;
	for(i=0;i<3;i++)
	{
		sp += elem->normal[i]*(la[i]-pp[i]);
		norm += elem->normal[i]*elem->normal[i];
	}
	return
		fabs(sp)/sqrt(norm);
}
//////////////////////////////////////////////////////////////////////////////////////
//	calculates the distance from a point 'la' to axis 'elem'
//
double CMadelungDoc::DistanceToAxis(SYMMETRY_ELEMENT* elem,double la[3])
{
	double pa[3];
	double pp[3];
// a point on axis
	for(int i=0;i<3;i++)
		pa[i] = elem->distance*elem->normal[i];
	for(i=0;i<3;i++)
		pp[i] = la[i]-pa[i];
	double vectprod[3];
	vectprod[0] = elem->direction[1]*pp[2] - elem->direction[2]*pp[1];
	vectprod[1] = elem->direction[2]*pp[0] - elem->direction[0]*pp[2];
	vectprod[2] = elem->direction[0]*pp[1] - elem->direction[1]*pp[0];
	double vpmod = 0.;
	double norm = 0.;
	for(i=0;i<3;i++)
	{
		vpmod += vectprod[i]*vectprod[i];
		norm += elem->normal[i]*elem->normal[i];
	}
	return
		sqrt(vpmod)/sqrt(norm);
}
//////////////////////////////////////////////////////////////////////////////////////
//	calculates the distance from a point 'la' to axis 'elem'
//
double CMadelungDoc::DistanceToInversionCenter(SYMMETRY_ELEMENT* elem,double la[3])
{
	double d;
	double dist = 0.;
	for(int i=0;i<3;i++)
	{
		d = la[i] - elem->distance*elem->normal[i];
		dist += d*d;
	}
	return
		sqrt(dist);
}
//////////////////////////////////////////////////////////////////////////////////////
//
//
void CMadelungDoc::OnFileOpen()
{
	char filter[] = "Madelung input file (*.mdg)\0*.mdg\0";
	m_pFileDialog->m_ofn.lpstrFilter = filter;
	if(m_pFileDialog->DoModal() != IDOK)
		return;
	CWaitCursor wait;
	CString newName;
	newName = m_pFileDialog->GetPathName();

// extension of the file

	m_SaveFileType = m_pFileDialog->m_ofn.nFilterIndex;
	CString ext;
	switch(m_SaveFileType)
	{
	case 1:
		ext = ".mdg";
		break;
	default:
		AfxMessageBox("unknown open file type",MB_OK);
		exit(1);
	}
	newName += ext;
// try to open the file
	ifstream in(newName);
	if(!in)
	{
		CString str;
		str.Format("Cannot open %s file",newName);
		AfxMessageBox(str,MB_OK);
		return;
	}
	else
	{
// stream opened OK
		string spaceGroupInLong;
		in >> spaceGroupInLong;
// remove the old spacegroup and open a new one
		CString str = spaceGroupInLong.c_str();
		str.Remove(' ');
		str.MakeLower();
		const char* cstr;
		cstr = LPCTSTR(str);
		if(str == "")
		{
			AfxMessageBox("Specify a space group name!",MB_OK);
			return;
		}
// test if a space group has already been loaded
		if(m_pSpaceGroup)
		{
// remove old space group
			delete m_pSpaceGroup;
			m_pSpaceGroup = NULL;
		}
		spg_names* sg = new spg_names;
// cannot return the 'int' value from dll. I reused the code
		int number = -1;
		for(int i=0;i<implementedSpaceGroups;i++)	
		{
			if(!sg->sgFullName[i].compare(cstr))		// if FullName exists in the list
				number = sg->sgNumber[i];
		}
		if(number == -1)
		{
			AfxMessageBox("Cannot load space group",MB_OK);
			return;
		}
		delete sg;

		m_pSpaceGroup = SpaceGroup::New(number);
		ReinitializeDocument();	// do not pDoc->m_pSpaceGroup = NULL !!!
		m_pSpaceGroup->m_pUnitCell->Clear();
// fill space group parameters
		double tmp;
		in >> tmp;
		m_pSpaceGroup->m_pUnitCell->m_a = tmp;
		in >> m_pSpaceGroup->m_pUnitCell->m_b;
		in >> m_pSpaceGroup->m_pUnitCell->m_c;
		in >> m_pSpaceGroup->m_pUnitCell->m_alpha;
		in >> m_pSpaceGroup->m_pUnitCell->m_beta;
		in >> m_pSpaceGroup->m_pUnitCell->m_gamma;
// WITHOUT symmetric Wyckoff positions (one atom per position)
		in >> m_pSpaceGroup->m_pUnitCell->m_positionsInUnitCell;
		int numat = m_pSpaceGroup->m_pUnitCell->m_positionsInUnitCell;
		for(i=0; i<numat;i++)
		{
			char pos;
			double d1,d2,d3,d4,d5;
			in >> pos >> d1 >> d2 >> d3 >> d4 >> d5;
			m_pSpaceGroup->m_pUnitCell->m_WyckoffPosition.push_back(pos);
			m_pSpaceGroup->m_pUnitCell->m_unitCellAtomX.push_back(d1);
			m_pSpaceGroup->m_pUnitCell->m_unitCellAtomY.push_back(d2);
			m_pSpaceGroup->m_pUnitCell->m_unitCellAtomZ.push_back(d3);
			m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCharge.push_back(d4);
			m_pSpaceGroup->m_pUnitCell->m_unitCellAtomType.push_back(d5);
		}
// INPUT MOLECULES, IF ANY
// first, delete old molecules
		int size = this->m_molecules.size();
		if(size)
		{
			for(int i=0;i<size;i++)
				delete m_molecules[i];
			m_molecules.clear();
		}
// read and create new molecules
		char name[200];
		double dim[3];
		int atoms;
		int type;
		double charge;

		in >> size; // number of molecules to input
		for(i=0;i<size;i++)
		{
			m_molecules.push_back(new Molecule);
// name
			in >> number >> name;
			m_molecules[i]->SetName(string(name));
// center
			in >> dim[0] >> dim[1] >> dim[2];
			m_molecules[i]->SetCenter(dim);
// orientation
			in >> dim[0] >> dim[1] >> dim[2];
			m_molecules[i]->SetOrientation(dim);
// number of atoms in molecule
			in >> atoms;
			for(int j=0;j<atoms;j++)
			{
				in >> number >> dim[0] >> dim[1] >> dim[2] >> type >> charge;
				m_molecules[i]->AddAtom(dim,type,charge);
			}
		}
		SetMenu(IDR_MAINFRAME);
	}
}
////////////////////////////////////////////////////////////////////////////
// GetNumberOfSelectedSymmIndependentAtoms in the cluster, but not extension
//
int CMadelungDoc::GetNumberOfSelectedSymmIndependentAtoms()
{
	int atoms = 0;
//	int size = m_pLattice->GetSize();
	for(int i=0;i<m_atomsInCluster;i++)
		if(m_pLattice->m_selected[i] && m_pLattice->m_SymmetricallyIndependent[i])
			atoms++;
	return atoms;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// find the irreducible representations of the point group. Used for counting of the number of
// orbitals in each representation. Used for the SCF module of the MOLCAS program
// Orbitals are counted for atoms in EACH Wyckoff position of unitcell
// 
void CMadelungDoc::FindIrreps()
{
	if(m_siteSymmetry != "" && m_pAtomsOnSymElem != 0)
	{
		FILE* fp;
// just for debug output. MFC TRACE macros doesn't work well
		fp = fopen("SymTemp.txt","a");

		int symElem;
		int atom;
// Number of all atoms on symmetry elements
		int size = m_pAtomsOnSymElem->GetSize();
// Now we calculate the contribution to 'character(symmetry element)' for every unshifted atom
// in every type of symmetry operation. They are the same, irrespective of a point group.
//
// 'Gamma3N' keeps the reducible representation
		int* Gamma3N = new int[m_numberOfRefinedSymmetryElements+1]; // we add 'E' element here
		for(int i=0;i<m_numberOfRefinedSymmetryElements+1;i++)
			Gamma3N[i] = 0;

		for(int se=0;se<m_numberOfRefinedSymmetryElements;se++)
		{
			SYMMETRY_ELEMENT* elem = m_RefinedElements[se];
// INVERSION CENTER
			if(elem->transform_atom == invert_atom)
			{
				m_pAtomsOnSymElem->ResetIterators();
				for(int k=0;k<size;k++) // over all atoms in the 'm_pAtomsOnSymElem'
				{
					m_pAtomsOnSymElem->GetNextAtom(symElem,atom);
					if(!m_pLattice->m_selected[atom])
						continue;
					else
					if(symElem == se) // atom found
					{
// now count contributions to Gamma3N
						int ns = m_pLattice->sFunctions(atom);
						int np = m_pLattice->pFunctions(atom);
						int nd = m_pLattice->dFunctions(atom);
						if(ns)
							Gamma3N[se] += (+1)*ns;
						if(np)
							Gamma3N[se] += (-3)*np;
						if(nd)
							Gamma3N[se] += (+5)*nd;
					}
					else
						continue;
				}
			}
// REFLECTION PLANE
			else
			if(elem->transform_atom == mirror_atom)
			{
				m_pAtomsOnSymElem->ResetIterators();
				for(int k=0;k<size;k++) // over all atoms in the 'm_pAtomsOnSymElem'
				{
					m_pAtomsOnSymElem->GetNextAtom(symElem,atom);
					if(!m_pLattice->m_selected[atom])
						continue;
					else
					if(symElem == se) // atom found
					{
// now count contributions to Gamma3N
						int ns = m_pLattice->sFunctions(atom);
						int np = m_pLattice->pFunctions(atom);
						int nd = m_pLattice->dFunctions(atom);
						if(ns)
							Gamma3N[se] += (+1)*ns;
						if(np)
							Gamma3N[se] += (+1)*np;
						if(nd)
							Gamma3N[se] += (+1)*nd;
					}
					else
						continue;
				}
			}
// ROTATION AXIS, ANY ORDER, EXCEPT INFINITY
			else
			if(elem->transform_atom == rotate_atom)
			{
				m_pAtomsOnSymElem->ResetIterators();
				for(int k=0;k<size;k++) // over all atoms in the 'm_pAtomsOnSymElem'
				{
					m_pAtomsOnSymElem->GetNextAtom(symElem,atom);
					if(!m_pLattice->m_selected[atom])
						continue;
					else
					if(symElem == se) // atom found
					{
// now count contributions to Gamma3N
						int ns = m_pLattice->sFunctions(atom);
						int np = m_pLattice->pFunctions(atom);
						int nd = m_pLattice->dFunctions(atom);
						if(ns)
							Gamma3N[se] += (+1)*ns;
						if(np)
							Gamma3N[se] += (+1+2*cos(2*PI/elem->order))*np;
						if(nd)
						{
							double cosd = cos(2*PI/elem->order);
							Gamma3N[se] += (4*cosd*cosd+2*cosd-1)*nd;
						}
					}
					else
						continue;
				}
			}
			else
			{
				AfxMessageBox("Unsupported symmetry element",MB_OK);
				return;
			}
		}
// Now we add a contribution to the 'E' element
		int sizeL = m_pLattice->GetSize();
		for(int atomL=0;atomL<sizeL;atomL++)
		{
			if(m_pLattice->m_selected[atomL])
			{
				int ns = m_pLattice->sFunctions(atomL);
				int np = m_pLattice->pFunctions(atomL);
				int nd = m_pLattice->dFunctions(atomL);
				if(ns)
					Gamma3N[m_numberOfRefinedSymmetryElements] += (+1)*ns;
				if(np)
					Gamma3N[m_numberOfRefinedSymmetryElements] += (+3)*np;
				if(nd)
					Gamma3N[m_numberOfRefinedSymmetryElements] += (+5)*nd;
			}
			else
				continue;
		}
// Gamma3N has been calculated.
// Now create a point group object with irrep tables for 'm_siteSymmetry'.
		if(m_pgi)
		{
			delete m_pgi;
			m_pgi = NULL;
		}

		if(this->m_siteSymmetry == "-1")
			m_pgi = new Ci;
		else
		if(this->m_siteSymmetry == "2" || this->m_siteSymmetry == "..2")
			m_pgi = new C2('z');
		else
		if(this->m_siteSymmetry == ".2.")
			m_pgi = new C2('y');
		else
		if(this->m_siteSymmetry == "2..")
			m_pgi = new C2('x');
		else
		if(this->m_siteSymmetry == "m" || this->m_siteSymmetry == "..m")
			m_pgi = new Cs('z');
		else
		if(this->m_siteSymmetry == ".m.")
			m_pgi = new Cs('y');
		else
		if(this->m_siteSymmetry == "m..")
			m_pgi = new Cs('x');
		else
		if(this->m_siteSymmetry == "2/m" || this->m_siteSymmetry == "..2/m")
			m_pgi = new C2h('z');
		else
		if(this->m_siteSymmetry == ".2/m.")
			m_pgi = new C2h('y');
		else
		if(this->m_siteSymmetry == "2/m..")
			m_pgi = new C2h('x');
		else
		if(this->m_siteSymmetry == "222")
			m_pgi = new D2;
		else
		if(this->m_siteSymmetry == "mm2")
			m_pgi = new C2v('z');
		else
		if(this->m_siteSymmetry == "m2m")
			m_pgi = new C2v('y');
		else
		if(this->m_siteSymmetry == "2mm")
			m_pgi = new C2v('x');
		else
		if(this->m_siteSymmetry == "mmm")
			m_pgi = new D2h;
		else
		{
			AfxMessageBox("CMadelungDoc: requested PointGroupIrrep not implemented",MB_OK);
			return;
		}
// now split the 'Gamma3N' into irreducible representations
// irreps numbered as in the point group tables
//
//  CHECK THIS IF MORE THAN 1 OPERATOR IN THE SYMMETRY OPERATOR CLASS
//  SINCE SYMMETRY OPERATORS IN GAMMA3N[] ARE ALL, BUT IN m_pgi - IN CLASSES.
//
//
		int ir = m_pgi->GetNumberOfIrreps();
		int symClasses = m_pgi->GetNumberOfSymmetryClasses();
		for(int irrep=0;irrep<ir;irrep++)
		{
			int coeff = 0; // coefficient of an irrep in the 'Gamma3N'
			for(int op=0;op<symClasses;op++)
			{
				int operat = this->FindCorrespondingOperatorInGamma3N(m_pgi->GetOperatorSymbol(op));

				if(operat == -1) // error
				{
					AfxMessageBox("CMadelungDoc: Corresponding operator not found",MB_OK);
					return;
				}
				string irp = m_pgi->GetIrrepName(irrep);
				string opname = m_pgi->GetOperatorSymbol(op);
// irrep coefficient in the reducible representation expansion
// for safety we use here the symbolic names instead of integer indexes. But not necessary
				int character = m_pgi->GetCharacter(irp,opname);
				int charInGamma3N = Gamma3N[operat];
				int multip = m_pgi->GetOperatorMultiplicity(op);
				coeff += character * charInGamma3N * multip;
			}
			coeff /= m_pgi->GetNumberOfSymmetryOperators();
			m_pgi->m_Irreps[irrep]->SetCoefficient(coeff);
		}
// output the representation decomposition results
		char buffer[500]; // temporary buffer
		fprintf(fp,"%s",m_siteSymmetryIrreps.c_str());
		sprintf(buffer,"%s",m_siteSymmetryIrreps.c_str());
		m_siteSymmetryIrreps += buffer;
		fprintf(fp,"Site symmetry point group is %s\n",this->m_siteSymmetry.c_str());
		sprintf(buffer,"Site symmetry point group is %s\n",this->m_siteSymmetry.c_str());
		m_siteSymmetryIrreps += buffer;
		fprintf(fp,"Reducible representation Gamma3N[] characters = ");
		sprintf(buffer,"Reducible representation Gamma3N[] characters = ");
		m_siteSymmetryIrreps += buffer;
		for(int op=0;op<symClasses;op++)
		{
			string oper = m_pgi->GetOperatorSymbol(op);
			int operat = this->FindCorrespondingOperatorInGamma3N(oper);
			int charInGamma3N = Gamma3N[operat];
			fprintf(fp,"%d(%s) ",charInGamma3N,oper.c_str());
			sprintf(buffer,"%d(%s) ",charInGamma3N,oper.c_str());
			m_siteSymmetryIrreps += buffer;
		}
		fprintf(fp,"\nDecomposition : ");
		sprintf(buffer,"\nDecomposition : ");
		m_siteSymmetryIrreps += buffer;
		int size1 = m_pgi->GetNumberOfIrreps();
		for(int k=0;k<size1;k++)
		{
			fprintf(fp,"%d%s",m_pgi->m_Irreps[k]->GetCoefficient(),m_pgi->GetIrrepName(k).c_str());
			sprintf(buffer,"%d%s",m_pgi->m_Irreps[k]->GetCoefficient(),m_pgi->GetIrrepName(k).c_str());
			m_siteSymmetryIrreps += buffer;
			if(k<size1-1)
			{
				fprintf(fp,"+");
				sprintf(buffer,"+");
				m_siteSymmetryIrreps += buffer;
			}
		}
// output the basis function symmetries
		fprintf(fp,"\nFunction symmtries: ");
		sprintf(buffer,"\nFunction symmtries: ");
		m_siteSymmetryIrreps += buffer;
		for(k=0;k<size1;k++)
		{
			string irrep = m_pgi->GetIrrepName(k);
			string symm  = m_pgi->GetIrrepFunctionSymmetry(irrep);
			fprintf(fp,"%s(%s) ",irrep.c_str(),symm.c_str());
			sprintf(buffer,"%s(%s) ",irrep.c_str(),symm.c_str());
			m_siteSymmetryIrreps += buffer;
		}
		if(Gamma3N) delete [] Gamma3N;
		fclose(fp);
	}
	else
	{
		AfxMessageBox("CMadelungDoc: no m_pAtomsOnSymElem created",MB_OK);
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
//	finds the index of the corresponding operator in Gamma3N[], of the operator from the
//	point group class. Operator ordering in Gamma3N[] is the same as in the m_RefinedElements[]
//	PERMITTED SYMBOLS:
//	E,sh,i,C2,C2(z),C2(y),C2(x),sv(xz),sv(yz),sv(xy)
//  if an operator contains two symbols in the parantheses, symbols must be ordered alphabetically.
//  First,'x', then 'y' then 'z' !!!
//
int CMadelungDoc::FindCorrespondingOperatorInGamma3N(string op)
{
	if(op == "E")
		return m_numberOfRefinedSymmetryElements;
	else
	if(op == "sh")
//
// OPERATION 'sh'
//
	{
// find the main rotation axis
		SYMMETRY_ELEMENT* elem_1;
		SYMMETRY_ELEMENT* elem_2;
		bool axisFound = false;
		int se1,se2;
		for(se1=0;se1<m_numberOfRefinedSymmetryElements;se1++)
		{
			elem_1 = m_RefinedElements[se1];
			if(elem_1->transform_atom == rotate_atom)
			{
// returns the first rotation axis. Probably OK in the case of 'sh'
				axisFound = true;
				break;
			}
			else
				continue;
		}
		if(!axisFound)
		{
			AfxMessageBox("CMadelungDoc: no axis found for the 'sh' symmetry operator",MB_OK);
			return -1;
		}
		else
		{
// The axis has been found. Now find the plane 'sh'.
			for(se2=0;se2<m_numberOfRefinedSymmetryElements;se2++)
			{
				elem_2 = m_RefinedElements[se2];
				if(elem_2->transform_atom == mirror_atom)
				{
// check if normal direction is the same as for axis, found in the previous step
					bool planeOK = true;
					for(int k=0;k<3;k++)
					{
						if((fabs(elem_1->direction[k]) - fabs(elem_2->normal[k])) > DISTANCE_TOLERANCE)
						{
							planeOK = false;
							break;
						}
					}
					if(!planeOK)
					{
						AfxMessageBox("CMadelungDoc: no 'sh' plane found",MB_OK);
						return -1;
					}
					else
						return se2;
				}
				else
					continue;
			}
		}
	}
//
// OPERATION 'i' - inversion center
//
	else
	if(op == "i")
	{
		SYMMETRY_ELEMENT* elem;
		bool centerFound = false;
		int se;
		for(se=0;se<m_numberOfRefinedSymmetryElements;se++)
		{
			elem = m_RefinedElements[se];
			if(elem->transform_atom == invert_atom)
			{
// returns the first inversion center. Probably OK.
				centerFound = true;
				break;
			}
			else
				continue;
		}
		if(!centerFound)
		{
			AfxMessageBox("CMadelungDoc: no inversion center found",MB_OK);
			return -1;
		}
		else
			return se;
	}
//
// OPERATION 'C2' - 2-fold rotation axis
//
	else
	if(op == "C2")
	{
		SYMMETRY_ELEMENT* elem;
		bool axisFound = false;
		int se;
		for(se=0;se<m_numberOfRefinedSymmetryElements;se++)
		{
			elem = m_RefinedElements[se];
			if(elem->transform_atom == rotate_atom && elem->order == 2)
			{
// returns the first 2-fold axis. Probably OK.
				axisFound = true;
				break;
			}
			else
				continue;
		}
		if(!axisFound)
		{
			AfxMessageBox("CMadelungDoc: no C2 axis found",MB_OK);
			return -1;
		}
		else
			return se;
	}
//
// OPERATION 'C2(z)' - 2-fold rotation axis along the 'z' axis
//
	else
	if(op == "C2(z)")
	{
		SYMMETRY_ELEMENT* elem;
		bool axisFound = false;
		int se;
		for(se=0;se<m_numberOfRefinedSymmetryElements;se++)
		{
			elem = m_RefinedElements[se];
			if(elem->transform_atom == rotate_atom && elem->order == 2 &&
				fabs(elem->direction[0]) == 0. && 
				fabs(elem->direction[1]) == 0. &&
				fabs(elem->direction[2]) == 1.)
			{
// returns the first 2-fold axis along the 'z' axis. Probably OK.
				axisFound = true;
				break;
			}
			else
				continue;
		}
		if(!axisFound)
		{
			AfxMessageBox("CMadelungDoc: no C2(z) axis found",MB_OK);
			return -1;
		}
		else
			return se;
	}
//
// OPERATION 'C2(y)' - 2-fold rotation axis along the 'y' axis
//
	else
	if(op == "C2(y)")
	{
		SYMMETRY_ELEMENT* elem;
		bool axisFound = false;
		int se;
		for(se=0;se<m_numberOfRefinedSymmetryElements;se++)
		{
			elem = m_RefinedElements[se];
			if(elem->transform_atom == rotate_atom && elem->order == 2 &&
				fabs(elem->direction[0]) == 0. &&
				fabs(elem->direction[1]) == 1. &&
				fabs(elem->direction[2]) == 0.)
			{
// returns the first 2-fold axis along the 'y' axis. Probably OK.
				axisFound = true;
				break;
			}
			else
				continue;
		}
		if(!axisFound)
		{
			AfxMessageBox("CMadelungDoc: no C2(y) axis found",MB_OK);
			return -1;
		}
		else
			return se;
	}
//
// OPERATION 'C2(x)' - 2-fold rotation axis along the 'x' axis
//
	else
	if(op == "C2(x)")
	{
		SYMMETRY_ELEMENT* elem;
		bool axisFound = false;
		int se;
		for(se=0;se<m_numberOfRefinedSymmetryElements;se++)
		{
			elem = m_RefinedElements[se];
			if(elem->transform_atom == rotate_atom && elem->order == 2 &&
				fabs(elem->direction[0]) == 1. &&
				fabs(elem->direction[1]) == 0. &&
				fabs(elem->direction[2]) == 0.)
			{
// returns the first 2-fold axis along the 'x' axis. Probably OK.
				axisFound = true;
				break;
			}
			else
				continue;
		}
		if(!axisFound)
		{
			AfxMessageBox("CMadelungDoc: no C2(x) axis found",MB_OK);
			return -1;
		}
		else
			return se;
	}
//
// OPERATION 'sv(xz)' - 'xz' mirror plane 
//
	else
	if(op == "sv(xz)")
	{
		SYMMETRY_ELEMENT* elem;
		bool planeFound = false;
		int se;
		for(se=0;se<m_numberOfRefinedSymmetryElements;se++)
		{
			elem = m_RefinedElements[se];
			if(elem->transform_atom == mirror_atom &&
				fabs(elem->normal[0]) == 0. &&
				fabs(elem->normal[1]) == 1. &&
				fabs(elem->normal[2]) == 0.)
			{
// returns the first plane with normal along the 'y' axis. Probably OK.
				planeFound = true;
				break;
			}
			else
				continue;
		}
		if(!planeFound)
		{
			AfxMessageBox("CMadelungDoc: no sv(xz) axis found",MB_OK);
			return -1;
		}
		else
			return se;
	}
//
// OPERATION 'sv(yz)' - 'yz' mirror plane 
//
	else
	if(op == "sv(yz)")
	{
		SYMMETRY_ELEMENT* elem;
		bool planeFound = false;
		int se;
		for(se=0;se<m_numberOfRefinedSymmetryElements;se++)
		{
			elem = m_RefinedElements[se];
			if(elem->transform_atom == mirror_atom &&
				fabs(elem->normal[0]) == 1. &&
				fabs(elem->normal[1]) == 0. &&
				fabs(elem->normal[2]) == 0.)
			{
// returns the first plane with normal along the 'x' axis. Probably OK.
				planeFound = true;
				break;
			}
			else
				continue;
		}
		if(!planeFound)
		{
			AfxMessageBox("CMadelungDoc: no sv(yz) axis found",MB_OK);
			return -1;
		}
		else
			return se;
	}
//
// OPERATION 'sv(xy)' - 'xy' mirror plane 
//
	else
	if(op == "sv(xy)")
	{
		SYMMETRY_ELEMENT* elem;
		bool planeFound = false;
		int se;
		for(se=0;se<m_numberOfRefinedSymmetryElements;se++)
		{
			elem = m_RefinedElements[se];
			if(elem->transform_atom == mirror_atom &&
				fabs(elem->normal[0]) == 0. &&
				fabs(elem->normal[1]) == 0. &&
				fabs(elem->normal[2]) == 1.)
			{
// returns the first plane with normal along the 'z' axis. Probably OK.
				planeFound = true;
				break;
			}
			else
				continue;
		}
		if(!planeFound)
		{
			AfxMessageBox("CMadelungDoc: no sv(xy) axis found",MB_OK);
			return -1;
		}
		else
			return se;
	}
	else
	{
		AfxMessageBox("CMadelungDoc: unknown symmetry element",MB_OK);
		return -1;
	}
}

////////////////////////////////////////////////////////////////////////////////
//	decomposes a reducible representation into irreducibles.
//  used for the basis function counting in irreps of the site symmetry group
//
void CMadelungDoc::OnComputeDecompose() 
{
	this->FindIrreps();
}
////////////////////////////////////////////////////////////////////////////////
// counts ALL atoms, located on symmetry elements
//
int CMadelungDoc::CountAllAtomsOnSymmetryElements()
{
	int size = m_pLattice->GetSize();
	SYMMETRY_ELEMENT* elem;
	for(int se=0;se<m_numberOfRefinedSymmetryElements;se++)
	{
		elem = m_RefinedElements[se];
		if(elem->transform_atom == rotate_atom)
		{
			for(int atom=0;atom<size;atom++)
			{
				double la[3];
				la[0] = m_pLattice->m_X[atom];
				la[1] = m_pLattice->m_Y[atom];
				la[2] = m_pLattice->m_Z[atom];
				if(DistanceToAxis(elem,la) < DISTANCE_TOLERANCE)
					m_pAtomsOnSymElem->AddAtom(se,atom);
				else
					continue;
			}
		}
		else
		if(elem->transform_atom == mirror_atom)
		{
			for(int atom=0;atom<size;atom++)
			{
				double la[3];
				la[0] = m_pLattice->m_X[atom];
				la[1] = m_pLattice->m_Y[atom];
				la[2] = m_pLattice->m_Z[atom];
				if(DistanceToPlane(elem,la) < DISTANCE_TOLERANCE)
					m_pAtomsOnSymElem->AddAtom(se,atom);
				else
					continue;
			}
		}
		if(elem->transform_atom == invert_atom)
		{
			for(int atom=0;atom<size;atom++)
			{
				double la[3];
				la[0] = m_pLattice->m_X[atom];
				la[1] = m_pLattice->m_Y[atom];
				la[2] = m_pLattice->m_Z[atom];
				if(DistanceToInversionCenter(elem,la) < DISTANCE_TOLERANCE)
					m_pAtomsOnSymElem->AddAtom(se,atom);
				else
					continue;
			}
		}
	}
	return m_pAtomsOnSymElem->GetSize();
}
///////////////////////////////////////////////////////////////////////////////
//
void CMadelungDoc::AddMoleculeToUnitCell(Molecule* mol)
{
	double center[3]; // in units of unitcell axis lengths
	double orientation[3];
	double coord[3];
	int type;
	double charge;

	mol->GetCenter(center);
	mol->GetOrientation(orientation);
	int size = mol->GetSize();
	for(int i=0;i<size;i++)
	{
		mol->GetAtom(i,coord,type,charge);
// 'm' means that the atom belongs to a molecule
		this->m_pSpaceGroup->m_pUnitCell->AddAtomCart(center,coord,type,charge);
	}
}
