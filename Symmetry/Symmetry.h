#if !defined (__Symmetry_h)
#define __Symmetry_h

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SYMMETRY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SYMMETRY_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SYMMETRY_EXPORTS
#define SYMMETRY_API __declspec(dllexport)
#else
#define SYMMETRY_API __declspec(dllimport)
#endif

#include <stdio.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971694
#define	DIMENSION 3
#define MAXPARAM  7
#endif

typedef struct SYMMETRY_API{
	int     type ;
    double  x[ DIMENSION ] ;
    } ATOM_ ;


typedef struct SYMMETRY_API _SYMMETRY_ELEMENT_ {
	void    (*transform_atom)( struct _SYMMETRY_ELEMENT_ *el, ATOM_ *from, ATOM_ *to ) ;
        int *   transform ;     /*   Correspondence table for the transformation         */
        int     order ;         /*   Applying transformation this many times is identity */
        int     nparam ;        /*   4 for inversion and planes, 7 for axes              */
        double  maxdev ;        /*   Largest error associated with the element           */
        double  distance ;
        double  normal[ DIMENSION ] ;
        double  direction[ DIMENSION ] ;
    } SYMMETRY_ELEMENT ;


typedef struct {
        char *  group_name ;        /* Canonical group name                              */
        char *  symmetry_code ;     /* Group symmetry code                               */
        int     (*check)( void ) ;  /* Additional verification routine, not used         */
    } POINT_GROUP ;

static int True(void){ return 1 ;}

static POINT_GROUP PointGroups[] = {
    {  "C1",    "",                                                          True  },
    {  "Cs",    "(sigma) ",                                                  True  },
    {  "Ci",    "(i) ",                                                      True  },
    {  "C2",    "(C2) ",                                                     True  },
    {  "C3",    "(C3) ",                                                     True  },
    {  "C4",    "(C4) (C2) ",                                                True  },
    {  "C5",    "(C5) ",                                                     True  },
    {  "C6",    "(C6) (C3) (C2) ",                                           True  },
    {  "C7",    "(C7) ",                                                     True  },
    {  "C8",    "(C8) (C4) (C2) ",                                           True  },
    {  "D2",    "3*(C2) ",                                                   True  },
    {  "D3",    "(C3) 3*(C2) ",                                              True  },
    {  "D4",    "(C4) 5*(C2) ",                                              True  },
    {  "D5",    "(C5) 5*(C2) ",                                              True  },
    {  "D6",    "(C6) (C3) 7*(C2) ",                                         True  },
    {  "D7",    "(C7) 7*(C2) ",                                              True  },
    {  "D8",    "(C8) (C4) 9*(C2) ",                                         True  },
    {  "C2v",   "(C2) 2*(sigma) ",                                           True  },
    {  "C3v",   "(C3) 3*(sigma) ",                                           True  },
    {  "C4v",   "(C4) (C2) 4*(sigma) ",                                      True  },
    {  "C5v",   "(C5) 5*(sigma) ",                                           True  },
    {  "C6v",   "(C6) (C3) (C2) 6*(sigma) ",                                 True  },
    {  "C7v",   "(C7) 7*(sigma) ",                                           True  },
    {  "C8v",   "(C8) (C4) (C2) 8*(sigma) ",                                 True  },
    {  "C2h",   "(i) (C2) (sigma) ",                                         True  },
    {  "C3h",   "(C3) (S3) (sigma) ",                                        True  },
    {  "C4h",   "(i) (C4) (C2) (S4) (sigma) ",                               True  },
    {  "C5h",   "(C5) (S5) (sigma) ",                                        True  },
    {  "C6h",   "(i) (C6) (C3) (C2) (S6) (S3) (sigma) ",                     True  },
    {  "C7h",   "(C7) (S7) (sigma) ",                                        True  },
    {  "C8h",   "(i) (C8) (C4) (C2) (S8) (S4) (sigma) ",                     True  },
    {  "D2h",   "(i) 3*(C2) 3*(sigma) ",                                     True  },
    {  "D3h",   "(C3) 3*(C2) (S3) 4*(sigma) ",                               True  },
    {  "D4h",   "(i) (C4) 5*(C2) (S4) 5*(sigma) ",                           True  },
    {  "D5h",   "(C5) 5*(C2) (S5) 6*(sigma) ",                               True  },
    {  "D6h",   "(i) (C6) (C3) 7*(C2) (S6) (S3) 7*(sigma) ",                 True  },
    {  "D7h",   "(C7) 7*(C2) (S7) 8*(sigma) ",                               True  },
    {  "D8h",   "(i) (C8) (C4) 9*(C2) (S8) (S4) 9*(sigma) ",                 True  },
    {  "D2d",   "3*(C2) (S4) 2*(sigma) ",                                    True  },
    {  "D3d",   "(i) (C3) 3*(C2) (S6) 3*(sigma) ",                           True  },
    {  "D4d",   "(C4) 5*(C2) (S8) 4*(sigma) ",                               True  },
    {  "D5d",   "(i) (C5) 5*(C2) (S10) 5*(sigma) ",                          True  },
    {  "D6d",   "(C6) (C3) 7*(C2) (S12) (S4) 6*(sigma) ",                    True  },
    {  "D7d",   "(i) (C7) 7*(C2) (S14) 7*(sigma) ",                          True  },
    {  "D8d",   "(C8) (C4) 9*(C2) (S16) 8*(sigma) ",                         True  },
    {  "S4",    "(C2) (S4) ",                                                True  },
    {  "S6",    "(i) (C3) (S6) ",                                            True  },
    {  "S8",    "(C4) (C2) (S8) ",                                           True  },
    {  "T",     "4*(C3) 3*(C2) ",                                            True  },
    {  "Th",    "(i) 4*(C3) 3*(C2) 4*(S6) 3*(sigma) ",                       True  },
    {  "Td",    "4*(C3) 3*(C2) 3*(S4) 6*(sigma) ",                           True  },
    {  "O",     "3*(C4) 4*(C3) 9*(C2) ",                                     True  },
    {  "Oh",    "(i) 3*(C4) 4*(C3) 9*(C2) 4*(S6) 3*(S4) 9*(sigma) ",         True  },
    {  "Cinfv", "(Cinf) (sigma) ",                                           True  },
    {  "Dinfh", "(i) (Cinf) (C2) 2*(sigma) ",                                True  },
    {  "I",     "6*(C5) 10*(C3) 15*(C2) ",                                   True  },
    {  "Ih",    "(i) 6*(C5) 10*(C3) 15*(C2) 6*(S10) 10*(S6) 15*(sigma) ",    True  },
    {  "Kh",    "(i) (Cinf) (sigma) ",                                       True  },
    } ;

// Global functions. It's more easy to use pointers to them, therefore I did not put them as members
// of class

void SYMMETRY_API mirror_atom( SYMMETRY_ELEMENT *plane, ATOM_ *from, ATOM_ *to );
void SYMMETRY_API invert_atom( SYMMETRY_ELEMENT *center, ATOM_ *from, ATOM_ *to );
void SYMMETRY_API rotate_atom( SYMMETRY_ELEMENT *axis, ATOM_ *from, ATOM_ *to );
void SYMMETRY_API rotate_reflect_atom( SYMMETRY_ELEMENT *axis, ATOM_ *from, ATOM_ *to );
int compare_axes( const void *a, const void *b );

// file functions
// file for the output in the CSymmetry listbox
static FILE*				OutputTextFile;
// file with symmetry elements for further refinement in CMadelungDoc
static FILE*				SymmetryElementFile;

//////////////////////////////////////////////////////////////////////////////////
class CSymmetry {
private:

	double                 ToleranceSame;
	double                 TolerancePrimary;
	double                 ToleranceFinal;
	double                 MaxOptStep;
	double                 MinOptStep;
	double                 GradientStep;
	double                 OptChangeThreshold;
	double                 CenterOfSomething[ DIMENSION ] ;
	double *               DistanceFromCenter;
	int                    verbose;
	int                    MaxOptCycles;
	int                    OptChangeHits;
	int                    MaxAxisOrder;
	int                    AtomsCount;
	ATOM_ *                 Atoms;
	int                    PlanesCount;
	SYMMETRY_ELEMENT **    Planes;
	SYMMETRY_ELEMENT *     MolecularPlane;
	int                    InversionCentersCount;
	SYMMETRY_ELEMENT **    InversionCenters;
	int                    NormalAxesCount;
	SYMMETRY_ELEMENT **    NormalAxes;
	int                    ImproperAxesCount;
	SYMMETRY_ELEMENT **    ImproperAxes;
	int *                  NormalAxesCounts;
	int *                  ImproperAxesCounts;
	int                    BadOptimization;
	char *                 SymmetryCode;
	int					   PointGroupsCount;
/*
 *    Statistics
 */
	long                   StatTotal;
	long                   StatEarly;
	long                   StatPairs;
	long                   StatDups;
	long                   StatOrder;
	long                   StatOpt;
	long                   StatAccept;

/*
 *    Point groups I know about
 */
	char *                 PointGroupRejectionReason;

public:
	SYMMETRY_API CSymmetry(void);
	SYMMETRY_API ~CSymmetry(void);
	SYMMETRY_API int read_coordinates( char *fn );
	SYMMETRY_API void report_planes( void );
	SYMMETRY_API void report_inversion_centers( void );
	SYMMETRY_API void report_axes( void );
	SYMMETRY_API void report_improper_axes( void );
	SYMMETRY_API void find_symmetry_elements( void );
	SYMMETRY_API void sort_symmetry_elements( void );
	SYMMETRY_API void summarize_symmetry_elements( void );
	SYMMETRY_API void report_symmetry_elements_verbose() ;
	SYMMETRY_API void report_symmetry_elements_brief( void );
	SYMMETRY_API char* identify_point_group( void );

private:

	double pow2( double x );
	int establish_pairs(SYMMETRY_ELEMENT *elem);
	int check_transform_order(SYMMETRY_ELEMENT *elem);
	int same_transform( SYMMETRY_ELEMENT *a, SYMMETRY_ELEMENT *b );
	SYMMETRY_ELEMENT * alloc_symmetry_element( void );
	void destroy_symmetry_element( SYMMETRY_ELEMENT *elem );
	int check_transform_quality( SYMMETRY_ELEMENT *elem );
	double eval_optimization_target_function( SYMMETRY_ELEMENT *elem, int *finish );
	void get_params( SYMMETRY_ELEMENT *elem, double values[] );
	void set_params( SYMMETRY_ELEMENT *elem, double values[] );
	void optimize_transformation_params( SYMMETRY_ELEMENT *elem );
	int refine_symmetry_element( SYMMETRY_ELEMENT *elem, int build_table );
	SYMMETRY_ELEMENT* init_mirror_plane( int i, int j );
	SYMMETRY_ELEMENT* init_ultimate_plane( void );
	SYMMETRY_ELEMENT * init_inversion_center( void );
	SYMMETRY_ELEMENT* init_ultimate_axis(void);
	SYMMETRY_ELEMENT* init_c2_axis( int i, int j, double support[ DIMENSION ] );
	SYMMETRY_ELEMENT* init_axis_parameters( double a[3], double b[3], double c[3] );
	SYMMETRY_ELEMENT* init_higher_axis( int ia, int ib, int ic );
	SYMMETRY_ELEMENT* init_improper_axis( int ia, int ib, int ic );
	void find_center_of_something( void );
	void find_planes(void);
	void find_inversion_centers(void);
	void find_infinity_axis(void);
	void find_c2_axes(void);
	void find_higher_axes(void);
	void find_improper_axes(void);
	void report_and_reset_counters( void );
};


#endif