// MadelungDoc.h : interface of the CMadelungDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MADELUNGDOC_H__5AA88EC3_0EA8_41F3_8132_862274543A2F__INCLUDED_)
#define AFX_MADELUNGDOC_H__5AA88EC3_0EA8_41F3_8132_862274543A2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <list>
#include <afxtempl.h>
using namespace std;

#include "DPQ1.h"
#include "Symmetry.h"

class CLattice;
class AtomProperties;
class AtomView;
class CTextWindowThread;
class SpaceGroup;
class ofstream;
class CMadelungView;
class PointGroupIrreps;
class Molecule;

class CMadelungDoc : public CDocument
{
private:
// returns a pointer to the CMadelungView
	CMadelungView* GetView();
	char CheckForSpecialPosition(double geomCenter[3],double geomCenter_UnitCell[3]);
protected: // create from serialization only
	CMadelungDoc();
	DECLARE_DYNCREATE(CMadelungDoc)
// Member variables
protected:
	bool m_PotentialsCalculatedOK;
// Wyckoff symmetry of the center of cluster (center of something in Symmetry.dll)
	char m_WPos;
public:
	SpaceGroup* m_pSpaceGroup; // see in EnumTypes.h
	int m_FitFunction; // 1-9, notes which function fits point charges
	bool m_latticeAsSphere;	// shows whether the lattice is extended as sphere or brick
	int m_extend_a;		// cluster extension along a (doesn't count initial cluster)
	int m_extend_b;		// cluster extension along a (doesn't count initial cluster)
	int m_extend_c;		// cluster extension along a (doesn't count initial cluster)
	int m_extend_sphere;	// extends spherically (r=ia+jb+kc;i+j+k=m_extend_sphere)

	CLattice* m_pLattice;
	AtomProperties* m_pAtomProperties;
	AtomView* m_pAtomView;	// AtomView window, containing Wyckoff position coordinates

// the total number of atoms is m_atomsInCluster + m_atomsInExtension
	int m_atomsInCluster;	// how many atoms are in the initial cluster
	int m_atomsInExtension;	// how many atoms are in the extension
	bool m_latticeExtended; // true if initial cluster extended, false if not
	bool m_IsFirstDocument; // used in onNewDocument() for meny operations

	int m_selectedAtoms;	// number of selected atoms
	double m_eta;	// charge distribution parameter

	int m_NumberOfUnitCellsForPlotAlongA;	// number of unit cells for plot
	int m_NumberOfUnitCellsForPlotAlongB;
	int m_NumberOfUnitCellsForPlotAlongC;

	int m_numberOfUnitCells;	// number of unit cells in document
//	ElStatEnergyThreadInfo m_ElStatEnergyThreadInfo;
//	CWinThread* m_pElStatEnergyThread;
//	HANDLE m_hEnergyThread;

	CTextWindowThread* m_pTextWindowThread;
// deferred processing queue for job handling in ONE worker thread
// stop timeout = 10ms
	CDPQueue2 *m_queue;
// Space Group Site Symmetry elements, refined from the Symmetry.dll output
	SYMMETRY_ELEMENT** m_RefinedElements;
	int m_numberOfRefinedSymmetryElements;
	bool m_bSymmetryElementsFound;	// set to true if symmetry elements found, otherwise false
// set to true if to use symmetry in lattice extensions and charge optimizations
	bool m_bUseSymmetry;
// cartesian coordinates of the center of the selected atoms
	double m_geomCenter[3];
// stores the point group of the selected atoms cluster
	string m_siteSymmetry;
// Stores info about the site symmetry (geom center of selected atoms)irreps of the 's','p','d' functions.
// Used for the MOLCAS SCF module 'OCCUPIED' keyword. Used in WriteMolcasFile()
	string m_siteSymmetryIrreps;
// 'true' when calculation thread running. Used for menu modifications in ON_UPDATE_COMMAND_UI
	bool m_bCalculationInProgress; 
// wrapper class of a pointer to the array of optimized charges
// The last two m_charges pointers points to two arrays of size 'CLattice->GetSize()' for
// storing averaged charges on atoms and deviations from the mean value

// wrapper class of a pointer to the array of pointers to the coordinate dimensions for
// charge optimizations. One array for each atom from the selected atom set.
// The last two m_charges pointers points to two arrays of size 'CLattice->GetSize()' for
// storing averaged charges on atoms and deviations from the mean value

	class OptimizedCharges	{
	public:
		int* m_atoms;		// numbers of optimized atoms (as in the CLattice object)
		double* m_charges;	// optimized charges, corresponding to m_atoms
		int* m_selected_atoms; // atoms on which the potential will be optimized
		int m_size;					// size of m_charges
		int m_size_selected;		// size of m_selected_atoms

		OptimizedCharges(int size, int size_selected)
		{
			m_atoms = new int[size];
			m_charges = new double[size];
			m_selected_atoms = new int[size_selected];
			m_size = size;
			m_size_selected = size_selected;
		}

		~OptimizedCharges()
		{
			if(m_atoms) delete [] m_atoms;
			if(m_charges) delete [] m_charges;
			if(m_selected_atoms) delete [] m_selected_atoms;
		}
		int GetSize() {return m_size;}
		int GetSizeSelected() {return m_size_selected;}
	};

	OptimizedCharges* m_optimized_charges;
	int m_optJobsDone;	// how many optimization jobs are done
// file i/o functions
	CFileDialog* m_pFileDialog;	// file save dialog
	int m_SaveFileType;		// type of file (extension) to save
// two lists, USED TOGETHER, to keep the numbers of those atoms, which are on symmetry elements
// Used in FindIrreps(), to find the number of basis functions in different irreps for MOLCAS
// 'scf' module
	class atomsOnSymmetryElements {
	private:
// two lists used in pair
		list<int>m_symmetryElement; // number of symmetry element as in m_RefinedElements
		list<int>m_atomOnSymmetryElement;	// atom number, as in m_pLattice
		list<int>::iterator p1;
		list<int>::iterator p2;
		int m_size; // size of lists
	public:
		atomsOnSymmetryElements() {m_size = 0;}
		~atomsOnSymmetryElements() {}
		void AddAtom(int symElem, int atom)
		{
			m_symmetryElement.push_back(symElem);
			m_atomOnSymmetryElement.push_back(atom);
			m_size++;
		}
		void GetNextAtom(int &symElem,int &atom)
		{
			symElem = *p1;
			atom = *p2;
			p1++;
			p2++;
		}
		void ResetIterators()
		{
			p1 = m_symmetryElement.begin();
			p2 = m_atomOnSymmetryElement.begin();
		}
		int GetSize() {return m_size;}
		void Clear()
		{
			m_symmetryElement.clear();
			m_atomOnSymmetryElement.clear();
			ResetIterators();
		}
	};

	atomsOnSymmetryElements* m_pAtomsOnSymElem;
// Irreducible representations of the site symmetry point group. Used in FindIrreps().
	PointGroupIrreps* m_pgi;
// holds molecules to be inserted in the lattice
	vector<Molecule*>m_molecules;
	int m_currentMolecule; // molecule, opened in the EditMolecule

// Member functions
public:
	virtual ~CMadelungDoc();
	bool GetCalculatedOK() {return m_PotentialsCalculatedOK;}
	void SetCalculatedOK(bool b) {m_PotentialsCalculatedOK = b;}
	void CalculateUnitCellVertices();	// for the initial cluster
// add atoms from the UnitCell to the CLattice
	void AddUnitCellToLattice(int na,int nb,int nc);
	void DeleteLatticeExtension();
	void ExtendLatticeAsParallelepiped(bool UseSymmetry);
	void ExtendLatticeAsSphere(bool UseSymmetry);
	void SetMenu(UINT m); // used in file->New to change to the start menu
	void CreateSpaceGroup(int sgNumber);	// number is from the International Tables
	void ReinitializeDocument();	// clears all except of m_pSpaceGroup
// CDocument overrides for file saving
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	BOOL WriteMolcasFile(std::ofstream& out);
	BOOL WriteGaussianFile(std::ofstream& out);
	BOOL WriteHyperChemFile(std::ofstream& out);
	BOOL WriteMadelungFile(std::ofstream& out);
// returns the number of selected atoms from CLattice
	int GetNumberOfSelectedAtoms();
// returns the number of selected, symmetry independent atoms from CLattice
	int GetNumberOfSelectedSymmIndependentAtoms();

// finds symmetry elements for a given Wyckoff position, finds symmetry independent atoms
	void ApplySymmetry();
// find the point symmetry elements for the selected atoms cluster
	void FindPointGroupForSelectedAtoms();
// use only the oriented site-symmetry operations listed in the International Tables
// for the selected Wyckoff position (geom.center of the selected atoms cluster)
	void RefineSymmetryElements();
// rotate atoms to align symmetry elements with coordinate axes, find symmetry independent atoms
	void FinalizeSymmetrization();
// find irreducible representations of the point group. Used for counting of the number of
// orbitals in each representation. Used for the SCF module of MOLCAS program
	void FindIrreps();
// counts ALL atoms on symmetry elements, will be used in the FindIrreps().
	int CountAllAtomsOnSymmetryElements();

protected:
// symmetry element refination routines
	 int RefineTriclinic(string siteSymmetry,FILE* f);
	 int RefineMonoclinic(string siteSymmetry,FILE* f);
	 int RefineOrthorhombic(string siteSymmetry,FILE* f);
 	 int RefineTetragonal(string siteSymmetry,FILE* f);
 	 int RefineRhombohedral(string siteSymmetry,FILE* f);
 	 int RefineHexagonal(string siteSymmetry,FILE* f);
	 int RefineCubic(string siteSymmetry,FILE* f);
	 int FindSymmetryElementsInFile(string crystalSystem,FILE* f,int elementType,int direction);
	 SYMMETRY_ELEMENT* AllocSymmetryElement(void(*p)(SYMMETRY_ELEMENT *plane,ATOM_ *from,ATOM_ *to));
	 void FindSymmetryIndependentAtoms(FILE*);
	 void MarkSymmetryDependentAtoms(FILE*,int);
	 int SymmetrizeExtension(FILE*);
	 void TransformExtension(FILE*);
	 double DistanceToPlane(SYMMETRY_ELEMENT* elem,double la[3]);
	 double DistanceToAxis(SYMMETRY_ELEMENT* elem,double la[3]);
	 double DistanceToInversionCenter(SYMMETRY_ELEMENT* elem,double la[3]);
// finds the corresponding operator in the Gamma3N[] array, for the site symmetry point group
	 int FindCorrespondingOperatorInGamma3N(string);
// adds a molecule to the unit cell
	 void AddMoleculeToUnitCell(Molecule* mol);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMadelungDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	LRESULT OnOptJobDone(WPARAM wp,LPARAM lp);
// Generated message map functions
protected:
	//{{AFX_MSG(CMadelungDoc)
	afx_msg void OnToolsPotentialOnAtoms();
	afx_msg void OnCancelCalculation();
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnOptimize();
	afx_msg void OnUpdateOptimize(CCmdUI* pCmdUI);
	afx_msg void OnToolsSymmetry();
	afx_msg void OnUpdateToolsSymmetry(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnComputeDecompose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MADELUNGDOC_H__5AA88EC3_0EA8_41F3_8132_862274543A2F__INCLUDED_)
