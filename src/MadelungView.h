// MadelungView.h : interface of the CMadelungView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MADELUNGVIEW_H__77AC15CB_927F_4F9C_9675_472B03E8E8C4__INCLUDED_)
#define AFX_MADELUNGVIEW_H__77AC15CB_927F_4F9C_9675_472B03E8E8C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

#define ATOM_TYPES 200

class vtkRenderer;
class vtkWin32OpenGLRenderWindow;
class vtkSphereSource;
class vtkLineSource;
class vtkWin32RenderWindowInteractor;
class InteractorStyleNorm;
class InteractorStyleSelect;
class vtkActor;
class vtkPolyDataMapper;
class vtkPicker;
class UnitCell;

class CMadelungView : public CView
{
protected: // create from serialization only
	CMadelungView();
	DECLARE_DYNCREATE(CMadelungView)

// Member variables
protected:
	vtkRenderer* m_pRenderer;
	vtkWin32OpenGLRenderWindow* m_pRenWin;
	vector<vtkSphereSource*> m_sphere;
	vector<vtkLineSource*> m_pLine;
	vector<vtkPolyDataMapper*> m_pLineMapper;
	vtkWin32RenderWindowInteractor* m_pIrenNorm;
	vtkWin32RenderWindowInteractor* m_pIrenSelect;
	vector<vtkPolyDataMapper*> m_ppdmSpheres;
	vtkPicker* m_picker;
	InteractorStyleNorm* m_pIrenStyle;
	InteractorStyleSelect* m_pIrenStyleSelect;
	bool m_BondsAreRendered;	// set to true when all bonds are rendered
	bool m_CellBordersAreRendered;	// true if all cell borders are rendered
	bool m_AtomsAreRotated;		// true if atom rotation mode, false if selection mode
	vector<vtkActor*> m_pbondActor;
	vector<vtkActor*> m_pCellBorderActor;

public:
	vector<vtkActor*> m_patomActor;
	int m_NumberOfAtomsInPlot;		// TOTAL number of atoms in plot
// indicates that lattice can be plot, (a,b,c,alpha,beta,gamma and atoms are specified)
	bool m_ClusterIsRendered;
// indicates that atoms for which to calculate the potential, are selected
	bool m_AtomsAreSelected;
// indicates that isosurface data is ready
	bool m_IsosurfaceDataIsReady;
	bool m_bRenderExtension;	// TRUE if user wants to render lattice extension
	float m_CameraFocalPoint[3]; // stores camera focal point

// Member functions
protected:
	CMadelungDoc* GetDocument();
public:
	void AddUnitCellVerticesToRenderer(CLattice* pl);
	void PrepareForUnitCellPlot(); // sets up all rendering information	
	void Clear();
	void WriteStatusBarMessage(CString msg);
// removes all atoms but central cluster from the plot
	void DeleteLatticeExtensionActors();
// adds actors in the positions of lattice extension
	void ExtendLatticeActors();
// calculates the distance between two points
	double Distance(double d1[3],double d2[3]);
	void AddAtomToPlot(int atom);	// add the actor 'atom' to the plot
	void RemoveAtomFromPlot(int atom); // remove the actor 'atom' from the plot
	bool atomInCluster(int number);		// checks whether atom is in the initial cluster or not
	void ShiftActors(double shift[3]);	// shifts all actors by 'shift' vector

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMadelungView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMadelungView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMadelungView)
	afx_msg void OnToolsSelectatoms();
	afx_msg void OnUpdateToolsSelectatoms(CCmdUI* pCmdUI);
	afx_msg void OnToolsRotateatoms();
	afx_msg void OnViewBonds();
	afx_msg void OnUpdateViewBonds(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsRotateatoms(CCmdUI* pCmdUI);
	afx_msg void OnToolsExtendNormal();
	afx_msg void OnToolsExtendSphere();
	afx_msg void OnUpdateToolsExtendNormal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsExtendSphere(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintSetup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnToolsExtendDelete();
	afx_msg void OnUpdateToolsExtendDelete(CCmdUI* pCmdUI);
	afx_msg void OnViewCellborders();
	afx_msg void OnUpdateViewCellborders(CCmdUI* pCmdUI);
	afx_msg void OnToolsUnselectallatoms();
	afx_msg void OnUpdateToolsUnselectallatoms(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsPotentialOnAtoms(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsSetupLattice(CCmdUI* pCmdUI);
	afx_msg void OnPopupSelectatom();
	afx_msg void OnPopupUnselectatom();
	afx_msg void OnPopupHide();
	afx_msg void OnPopupHideall();
	afx_msg void OnPopupShowall();
	afx_msg void OnPopupHidelayerX();
	afx_msg void OnPopupHidelayerY();
	afx_msg void OnPopupHidelayerZ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/*
#ifndef _DEBUG  // debug version in MadelungView.cpp
inline CMadelungDoc* CMadelungView::GetDocument()
   { return (CMadelungDoc*)m_pDocument; }

#endif
*/
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MADELUNGVIEW_H__77AC15CB_927F_4F9C_9675_472B03E8E8C4__INCLUDED_)
