// Symmetry.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Symmetry.h"
/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
SYMMETRY_API int nSymmetry=0;

// This is an example of an exported function.
SYMMETRY_API int fnSymmetry(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Symmetry.h for the class definition
CSymmetry::CSymmetry()
{ 
	return; 
}
*/

/*
 *  Brute force symmetry analyzer.
 *  This is actually C++ program, masquerading as a C one!
 *
 *  (C) 1996 S. Pachkovsky, ps@oci.unizh.ch
 *  This code comes with NO WARRANTY, neither explicit nor implied.
 *  Non-profit use of the code is welcome, provided that the
 *  copyright above is retained in all derived works and the
 *  origin of the code is clearly indicated. Any for-profit use
 *  requires prior written permission of the author.
 *
 * $Log: symmetry.c,v $
 * Revision 1.15  2000/01/25  16:47:17  patchkov
 * *** empty log message ***
 *
 * Revision 1.14  2000/01/25  16:39:08  patchkov
 * *** empty log message ***
 *
 * Revision 1.13  1996/05/24  12:32:08  ps
 * Revision 1.12  1996/05/23  16:10:47  ps
 * First reasonably stable version.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/*
 *  All specific structures should have corresponding elements in the
 *  same position generic structure does.
 *
 *  Planes are characterized by the surface normal direction
 *  (taken in the direction *from* the coordinate origin)
 *  and distance from the coordinate origin to the plane
 *  in the direction of the surface normal.
 *
 *  Inversion is characterized by location of the inversion center.
 *
 *  Rotation is characterized by a vector (distance+direction) from the origin
 *  to the rotation axis, axis direction and rotation order. Rotations
 *  are in the clockwise direction looking opposite to the direction
 *  of the axis. Note that this definition of the rotation axis
 *  is *not* unique, since an arbitrary multiple of the axis direction
 *  can be added to the position vector without changing actual operation.
 *
 *  Mirror rotation is defined by the same parameters as normal rotation,
 *  but the origin is now unambiguous since it defines the position of the
 *  plane associated with the axis.
 *
 */


/*
 *   Generic functions
 */

double CSymmetry::pow2( double x )
{
	return x * x ;
}
//--------------- tested ok -------------------------------------------------------------------
int CSymmetry::establish_pairs( SYMMETRY_ELEMENT *elem )
{
	int       i, j, k, best_j ;
    char *    atom_used = (char*) calloc( AtomsCount, 1 ) ;	// memory initialized to zero
    double    distance, best_distance ;
    ATOM_      symmetric ;

	if( atom_used == NULL ){
		fprintf( OutputTextFile, "Out of memory for tagging array in establish_pairs()\n" ) ;
		exit( EXIT_FAILURE ) ;
    }
	for( i = 0 ; i < AtomsCount ; i++ ){
		if( elem->transform[i] >= AtomsCount ) /* No symmetric atom yet, by default it's AtomsCount+1 */
		{ 
			if( verbose > 2 ) fprintf( OutputTextFile,"        looking for a pair for %d\n", i ) ;
			elem->transform_atom( elem, Atoms+i, &symmetric ) ;
			if( verbose > 2 ) fprintf( OutputTextFile,"        new coordinates are: (%g,%g,%g)\n", 
                            symmetric.x[0], symmetric.x[1], symmetric.x[2] ) ;
			best_j        = i ;
			best_distance = 2*TolerancePrimary ;/* Performance value we'll reject */
			for( j = 0 ; j < AtomsCount ; j++ )
			{
				if( Atoms[j].type != symmetric.type || atom_used[j] )
					continue ;
				for( k = 0, distance = 0 ; k < DIMENSION ; k++ ){
					distance += pow2( symmetric.x[k] - Atoms[j].x[k] ) ;
				}
				distance = sqrt( distance ) ;
				if( verbose > 2 ) fprintf(OutputTextFile,"        distance to %d is %g\n", j, distance ) ;
				if( distance < best_distance ){
					best_j        = j ;
					best_distance = distance ;
				}
			}
			if( best_distance > TolerancePrimary ){ /* Too bad, there is no symmetric atom */
				if( verbose > 0 ) 
					fprintf(OutputTextFile,"        no pair for atom %d - best was %d with err = %g\n", i, best_j, best_distance ) ;
				free( atom_used ) ;
				return -1 ;
			}
			elem->transform[i] = best_j ;
			atom_used[best_j]  = 1 ;
			if( verbose > 1 ) fprintf(OutputTextFile,"        atom %d transforms to the atom %d, err = %g\n", i, best_j, best_distance ) ;
		}
    }
	free( atom_used ) ;
	return 0 ;
}
//------------ tested ok --------------------------------------------------------------------
int CSymmetry::check_transform_order( SYMMETRY_ELEMENT *elem )
{
    int             i, j, k ;
    void            rotate_reflect_atom( SYMMETRY_ELEMENT *, ATOM_ *, ATOM_ *) ;

	for( i = 0 ; i < AtomsCount ; i++ ){
		if( elem->transform[i] == i )   /* Identity transform is Ok for any order */
			continue ;
		if( elem->transform_atom == rotate_reflect_atom ){
			j = elem->transform[i] ;
			if( elem->transform[j] == i )
				continue ; /* Second-order transform is Ok for improper axis */
        }
		for( j = elem->order - 1, k = elem->transform[i] ; j > 0 ; j--, k = elem->transform[k] ){
			if( k == i ){
				if( verbose > 0 ) fprintf(OutputTextFile,"        transform looped %d steps too early from atom %d\n", j, i ) ;
				return -1 ;
            }
        }
		if( k != i && elem->transform_atom == rotate_reflect_atom ){
/* For improper axes, the complete loop may also take twice the order */
	        for( j = elem->order ; j > 0 ; j--, k = elem->transform[k] ){
		        if( k == i ){
			        if( verbose > 0 ) fprintf(OutputTextFile,"  (improper) transform looped %d steps too early from atom %d\n", j, i ) ;
				    return -1 ;
                }
            }
        }
		if( k != i ){
			if( verbose > 0 ) fprintf(OutputTextFile,"        transform failed to loop after %d steps from atom %d\n", elem->order, i ) ;
			return -1 ;
        }
    }
	return 0 ;
}
//------------------------------------------------------------------------
int CSymmetry::same_transform( SYMMETRY_ELEMENT *a, SYMMETRY_ELEMENT *b )
{
	int  i, j ;
    int  code ;

	if( ( a->order != b->order ) || ( a->nparam != b->nparam ) || ( a->transform_atom != b->transform_atom ) )
	    return 0 ;
	for( i = 0, code = 1 ; i < AtomsCount ; i++ ){
	    if( a->transform[i] != b->transform[i] ){
			code = 0 ;
			break ;
        }
    }
	if( code == 0 && a->order > 2 ){  /* b can also be a reverse transformation for a */
		for( i = 0 ; i < AtomsCount ; i++ ){
			j = a->transform[i] ;
			if( b->transform[j] != i )
				return 0 ;
        }
		return 1 ;
    }
	return code ;
}
//-------------------------------------------------------------------------------------------
SYMMETRY_ELEMENT* CSymmetry::alloc_symmetry_element( void )
{
        SYMMETRY_ELEMENT* elem = (SYMMETRY_ELEMENT*)calloc( 1, sizeof( SYMMETRY_ELEMENT ) ) ;
        int                i ;

	if( elem == NULL ){
	    fprintf( OutputTextFile, "Out of memory allocating symmetry element\n" ) ;
	    exit( EXIT_FAILURE ) ;
	}
	elem->transform = (int*)calloc( AtomsCount, sizeof( int ) ) ;
	if( elem->transform == NULL ){
		fprintf( OutputTextFile, "Out of memory allocating transform table for symmetry element\n" ) ;
		exit( EXIT_FAILURE ) ;
    }
	for( i = 0 ; i < AtomsCount ; i++ ){
		elem->transform[i] = AtomsCount + 1 ; /* An impossible value */
    }
	return elem ;
}
//----------- tested ok ---------------------------------------------------------------------
void CSymmetry::destroy_symmetry_element( SYMMETRY_ELEMENT *elem )
{
	if( elem != NULL ){
		if( elem->transform != NULL )
	        free( elem->transform ) ;
	    free( elem ) ;
    }
}
//--------------------------------------------------------------------------------------------
int CSymmetry::check_transform_quality( SYMMETRY_ELEMENT *elem )
{
        int               i, j, k ;
        ATOM_              symmetric ;
        double            r, max_r ;

	for( i = 0, max_r = 0 ; i < AtomsCount ; i++ ){
		j = elem->transform[i] ;
		elem->transform_atom( elem, Atoms + i, &symmetric ) ;
		for( k = 0, r = 0 ; k < DIMENSION ; k++ ){
			r += pow2( symmetric.x[k] - Atoms[j].x[k] ) ;
        }
		r = sqrt( r ) ;
		if( r > ToleranceFinal ){
			if( verbose > 0 ) fprintf(OutputTextFile,"        distance to symmetric atom (%g) is too big for %d\n", r, i ) ;
			return -1 ;
        }
		if( r > max_r ) max_r = r ;
    }
	elem->maxdev = max_r ;
	return 0 ;
}
//--------------- tested ok ------------------------------------------------------------------
double CSymmetry::eval_optimization_target_function( SYMMETRY_ELEMENT *elem, int *finish )
{
	int               i, j, k ;
    ATOM_              symmetric ;
    double            target, r, maxr ;

	if( elem->nparam >= 4 ){	// for inversions, planes and axes
		for( k = 0, r = 0 ; k < DIMENSION ; k++ ){
	        r += elem->normal[k]*elem->normal[k] ;
		}
	    r = sqrt( r ) ;
		if( r < ToleranceSame ){
			fprintf( OutputTextFile, "Normal collapsed!\n" ) ;
			exit( EXIT_FAILURE ) ;
        }
		for( k = 0 ; k < DIMENSION ; k++ ){
			elem->normal[k] /= r ;
        }
		if( elem->distance < 0 ){
			elem->distance = -elem->distance ;
			for( k = 0 ; k < DIMENSION ; k++ ){
				elem->normal[k] = -elem->normal[k] ;
            }
        }
    }
	if( elem->nparam >= 7 ){	// for axes
		for( k = 0, r = 0 ; k < DIMENSION ; k++ ){
			r += elem->direction[k]*elem->direction[k] ;
        }
		r = sqrt( r ) ;
		if( r < ToleranceSame ){
	        fprintf( OutputTextFile, "Direction collapsed!\n" ) ;
			exit( EXIT_FAILURE ) ;
        }
		for( k = 0 ; k < DIMENSION ; k++ ){
	        elem->direction[k] /= r ;
        }
    }
	for( i = 0, target = maxr = 0 ; i < AtomsCount ; i++ ){
		elem->transform_atom( elem, Atoms + i, &symmetric ) ;
		j = elem->transform[i] ;
		for( k = 0, r = 0 ; k < DIMENSION ; k++ ){
	        r += pow2( Atoms[j].x[k] - symmetric.x[k] ) ;
        }
		if( r > maxr ) maxr = r ;
		target += r ;
    }
	if( finish != NULL ){
		*finish = 0 ;
		if( sqrt( maxr ) < ToleranceFinal )
			*finish = 1 ;
    }
	return target ;
}
//-------------------------------------------------------------------------------
void CSymmetry::get_params( SYMMETRY_ELEMENT *elem, double values[] )
{
// this copies elem->distance, elem->normal[], elem->direction[] into values
memcpy( values, &elem->distance, elem->nparam * sizeof( double ) ) ;
}
//-------------------------------------------------------------------------------
void CSymmetry::set_params( SYMMETRY_ELEMENT *elem, double values[] )
{
memcpy( &elem->distance, values, elem->nparam * sizeof( double ) ) ;
}
//------------- tested ok -------------------------------------------------------
void CSymmetry::optimize_transformation_params( SYMMETRY_ELEMENT *elem )
{
        double            values[ MAXPARAM ] ;
        double            grad  [ MAXPARAM ] ;
        double            force [ MAXPARAM ] ;
        double            step  [ MAXPARAM ] ;
        double            f, fold, fnew, fnew2, fdn, fup, snorm ;
        double            a, b, x ;
        int               vars  = elem->nparam ;
        int               cycle = 0 ;
        int               i, finish ;
        int               hits = 0 ;

	if( vars > MAXPARAM ){
		fprintf( OutputTextFile, "Catastrophe in optimize_transformation_params()!\n" ) ;
		exit( EXIT_FAILURE ) ;
    }
	f = 0 ;
	do {
		fold = f ;
		f = eval_optimization_target_function( elem, &finish ) ;
/* Evaluate function, gradient and diagonal force constants */
		if( verbose > 1 ) fprintf(OutputTextFile,"            function value = %g\n", f ) ;
		if( finish ){
			if( verbose > 1 ) fprintf(OutputTextFile,"        function value is small enough\n" ) ;
				break ;
		}
		if( cycle > 0 ){
			if( fabs( f-fold ) > OptChangeThreshold )
				hits = 0 ;
			else
				hits++ ;
			if( hits >= OptChangeHits ){
				if( verbose > 1 ) fprintf(OutputTextFile,"        no progress is made, stop optimization\n" ) ;
				break ;
            }
        }
		get_params( elem, values ) ;
		for( i = 0 ; i < vars ; i++ ){	// vars == elem->nparam
	        values[i] -= GradientStep ;
			set_params( elem, values ) ;
			fdn        = eval_optimization_target_function( elem, NULL ) ;
			values[i] += 2*GradientStep ;
			set_params( elem, values ) ;
			fup        = eval_optimization_target_function( elem, NULL ) ;
			values[i] -= GradientStep ;	// better is to use set_params(elem,values)
			grad[i]    = ( fup - fdn ) / ( 2 * GradientStep ) ;	// first derivative
			force[i]   = ( fup + fdn - 2*f ) / ( GradientStep * GradientStep ) ; // second derivative
			if( verbose > 1 ) fprintf(OutputTextFile,"        i = %d, grad = %12.6e, force = %12.6e\n", i, grad[i], force[i] ) ;
       }
// Do a quasy-Newton step
	    for( i = 0, snorm = 0 ; i < vars ; i++ ){
		    if( force[i] <  0   ) force[i] = -force[i] ;
			if( force[i] < 1e-3 ) force[i] = 1e-3 ;
			if( force[i] > 1e3  ) force[i] = 1e3 ;
			step[i] = - grad[i]/force[i] ;
			snorm += step[i] * step[i] ;
        }
		snorm = sqrt( snorm ) ;
// If the step is too big, renormalize it
		if( snorm > MaxOptStep ){ 
			for( i = 0 ; i < vars ; i++ )
				step[i] *= MaxOptStep/snorm ;
			snorm = MaxOptStep ;
        }
// divide the step until the function value decreases and step is bigger than MinOptStep
		do {
			for( i = 0 ; i < vars ; i++ ){
	            values[i] += step[i] ;
			}
			set_params( elem, values ) ;
	        fnew = eval_optimization_target_function( elem, NULL ) ;
			if( fnew < f )
				break ;
			for( i = 0 ; i < vars ; i++ ){
	            values[i] -= step[i] ;
				step  [i] /= 2 ;
            }
			set_params( elem, values ) ;
			snorm /= 2 ;
        } while( snorm > MinOptStep ) ;
// coordinates are the following : f(0),fnew(step),fnew2(2*step)
        if( (snorm > MinOptStep) && (snorm < MaxOptStep / 2) ){  /* try to do quadratic interpolation */
            for( i = 0 ; i < vars ; i++ )
                values[i] += step[i] ;
            set_params( elem, values ) ;
            fnew2 = eval_optimization_target_function( elem, NULL ) ;
            if( verbose > 1 ) fprintf(OutputTextFile,"        interpolation base points: %g, %g, %g\n", f, fnew, fnew2 ) ;
            for( i = 0 ; i < vars ; i++ )
                values[i] -= 2*step[i] ;
            a     = ( 4*fnew - fnew2 - 3*f ) / 2 ;	// must be (4*fnew - fnew2 - 3*f)
            b     = ( f + fnew2 - 2*fnew ) / 2 ;
            if( verbose > 1 ) fprintf(OutputTextFile,"        linear interpolation coefficients %g, %g\n", a, b ) ;
            if( b > 0 ){
                x = -a/(2*b) ;
                if( x > 0.2 && x < 1.8 ){
                    if( verbose > 1 ) fprintf(OutputTextFile,"        interpolated: %g\n", x ) ;
                    for( i = 0 ; i < vars ; i++ )
                        values[i] += x*step[i] ;
                }
                else b = 0 ;	// do the linear extrapolation
            }
            if( b <= 0 ){
                if( fnew2 < fnew ){
                    for( i = 0 ; i < vars ; i++ )
                        values[i] += 2*step[i] ;
                }
                else {
                    for( i = 0 ; i < vars ; i++ )
                        values[i] += step[i] ;
                }
            }
            set_params( elem, values ) ;
        }
    } while( snorm > MinOptStep && ++cycle < MaxOptCycles ) ;

	f = eval_optimization_target_function( elem, NULL ) ;
	if( cycle >= MaxOptCycles ) BadOptimization = 1 ;
	if( verbose > 0 ) {
		if( cycle >= MaxOptCycles )
			fprintf(OutputTextFile,"        maximum number of optimization cycles made\n" ) ;
			fprintf(OutputTextFile,"        optimization completed after %d cycles with f = %g\n", cycle, f ) ;
    }
}
//------------------------- tested ok -------------------------------------------------------
int CSymmetry::refine_symmetry_element( SYMMETRY_ELEMENT *elem, int build_table )
{
	int i;

	if( build_table && (establish_pairs( elem ) < 0) ){
	    StatPairs++ ;
		if( verbose > 0 ) fprintf(OutputTextFile,"        no transformation correspondence table can be constructed\n" ) ;
		return -1 ;
    }

for( i = 0 ; i < PlanesCount ; i++ ){
    if( same_transform( Planes[i], elem ) ){
        StatDups++ ;
        if( verbose > 0 ) fprintf(OutputTextFile,"        transformation is identical to plane %d\n", i ) ;
        return -1 ;
        }
    }
for( i = 0 ; i < InversionCentersCount ; i++ ){
    if( same_transform( InversionCenters[i], elem ) ){
        StatDups++ ;
        if( verbose > 0 ) fprintf(OutputTextFile,"        transformation is identical to inversion center %d\n", i ) ;
        return -1 ;
        }
    }
for( i = 0 ; i < NormalAxesCount ; i++ ){
    if( same_transform( NormalAxes[i], elem ) ){
        StatDups++ ;
        if( verbose > 0 ) fprintf(OutputTextFile,"        transformation is identical to normal axis %d\n", i ) ;
        return -1 ;
        }
    }
for( i = 0 ; i < ImproperAxesCount ; i++ ){
    if( same_transform( ImproperAxes[i], elem ) ){
        StatDups++ ;
        if( verbose > 0 ) fprintf(OutputTextFile,"        transformation is identical to improper axis %d\n", i ) ;
        return -1 ;
        }
    }
if( check_transform_order( elem ) < 0 ){
    StatOrder++ ;
    if( verbose > 0 ) fprintf(OutputTextFile,"        incorrect transformation order\n" ) ;
    return -1 ;
    }
optimize_transformation_params( elem ) ;
if( check_transform_quality( elem ) < 0 ){
    StatOpt++ ;
    if( verbose > 0 ) fprintf(OutputTextFile,"        refined transformation does not pass the numeric threshold\n" ) ;
    return -1 ;
    }
StatAccept++ ;
return 0 ;
}
//-------------------------------------------------------------------------------------------------
/*
 *   Plane-specific functions
 */

void mirror_atom( SYMMETRY_ELEMENT *plane, ATOM_ *from, ATOM_ *to )
{
    int                i ;
    double             r ;

	for( i = 0, r = plane->distance ; i < DIMENSION ; i++ ){
		r -= from->x[i] * plane->normal[i] ;
    }
	to->type = from->type ;
	for( i = 0 ; i < DIMENSION ; i++ ){
	    to->x[i] = from->x[i] + 2*r*plane->normal[i] ;
    }
}
//--------------------------------------------------------------------------------------------------
SYMMETRY_ELEMENT* CSymmetry::init_mirror_plane( int i, int j )
{
	SYMMETRY_ELEMENT * plane = alloc_symmetry_element() ;
    double             dx[ DIMENSION ], midpoint[ DIMENSION ], rab, r ;
    int                k ;

	if( verbose > 0 ) fprintf(OutputTextFile,"Trying mirror plane for atoms %d,%d\n", i, j ) ;
	StatTotal++ ;
	plane->transform_atom = mirror_atom ;
	plane->order          = 2 ;
	plane->nparam         = 4 ;
	for( k = 0, rab = 0 ; k < DIMENSION ; k++ ){
	    dx[k] = Atoms[i].x[k] - Atoms[j].x[k] ;
	    midpoint[k] = ( Atoms[i].x[k] + Atoms[j].x[k] ) / 2.0 ;
	    rab += dx[k]*dx[k] ;
    }
	rab = sqrt(rab) ;
	if( rab < ToleranceSame ){
		fprintf( OutputTextFile, "Atoms %d and %d coincide (r = %g)\n", i, j, rab ) ;
		exit( EXIT_FAILURE ) ;
    }
	for( k = 0, r = 0 ; k < DIMENSION ; k++ ){
	    plane->normal[k] = dx[k]/rab ;
		r += midpoint[k]*plane->normal[k] ;
    }
	if( r < 0 ){  /* Reverse normal direction, distance is always positive! */
		r = -r ;
		for( k = 0 ; k < DIMENSION ; k++ ){
	        plane->normal[k] = -plane->normal[k] ;
        }
    }
	plane->distance = r ;
	if( verbose > 0 ) fprintf(OutputTextFile,"    initial plane is at %g from the origin\n", r ) ;
	if( refine_symmetry_element( plane, 1 ) < 0 ){
		if( verbose > 0 ) fprintf(OutputTextFile,"    refinement failed for the plane\n" ) ;
		destroy_symmetry_element( plane ) ;
		return NULL ;
    }
	return plane ;
}
//---------------------------------------------------------------------------------------
SYMMETRY_ELEMENT* CSymmetry::init_ultimate_plane( void )
{
    SYMMETRY_ELEMENT * plane = alloc_symmetry_element() ;
    double             d0[ DIMENSION ], d1[ DIMENSION ], d2[ DIMENSION ] ;
    double             p[ DIMENSION ] ;
    double             r, s0, s1, s2 ;
    double *           d ;
    int                i, j, k ;

	if( verbose > 0 ) fprintf(OutputTextFile,"Trying whole-molecule mirror plane\n" ) ;
	StatTotal++ ;
	plane->transform_atom = mirror_atom ;
	plane->order          = 1 ;
	plane->nparam         = 4 ;
	for( k = 0 ; k < DIMENSION ; k++ )
	    d0[k] = d1[k] = d2[k] = 0 ;
	d0[0] = 1 ; d1[1] = 1 ; d2[2] = 1 ;
	for( i = 1 ; i < AtomsCount ; i++ ){
	    for( j = 0 ; j < i ; j++ ){
			for( k = 0, r = 0 ; k < DIMENSION ; k++ ){
	            p[k] = Atoms[i].x[k] - Atoms[j].x[k] ;	// r
				r   += p[k]*p[k] ;
            }
			r = sqrt(r) ;	// distance between two atoms
			for( k = 0, s0=s1=s2=0 ; k < DIMENSION ; k++ ){
	            p[k] /= r ;
				s0   += p[k]*d0[k] ;	// scalar product r*BasisVector(x)
				s1   += p[k]*d1[k] ;	// scalar product r*BasisVector(y)
				s2   += p[k]*d2[k] ;	// scalar product r*BasisVector(z)
            }
	        for( k = 0 ; k < DIMENSION ; k++ ){
				d0[k] -= s0*p[k] ;
				d1[k] -= s1*p[k] ;
				d2[k] -= s2*p[k] ;
            }
        }
    }
	for( k = 0, s0=s1=s2=0 ; k < DIMENSION ; k++ ){
	    s0 += d0[k] ;	// why not adeed absolute values of d[k]?????
	    s1 += d1[k] ;
	    s2 += d2[k] ;
    }
	d = NULL ;
	if( s0 >= s1 && s0 >= s2 ) d = d0 ;
	if( s1 >= s0 && s1 >= s2 ) d = d1 ;
	if( s2 >= s0 && s2 >= s1 ) d = d2 ;
	if( d == NULL ){
		fprintf( OutputTextFile, "Catastrophe in init_ultimate_plane(): %g, %g and %g have no ordering!\n", s0, s1, s2 ) ;
		exit( EXIT_FAILURE ) ;
    }
	for( k = 0, r = 0 ; k < DIMENSION ; k++ )
	    r += d[k]*d[k] ;
	r = sqrt(r) ;
	if( r > 0 ){
	    for( k = 0 ; k < DIMENSION ; k++ )
			plane->normal[k] = d[k]/r ;
    }
	else {
		for( k = 1 ; k < DIMENSION ; k++ )
			plane->normal[k] = 0 ;
		plane->normal[0] = 1 ;
    }
	for( k = 0, r = 0 ; k < DIMENSION ; k++ )
		r += CenterOfSomething[k]*plane->normal[k] ;	// plane goes through the CenterOfSomething[]
// distance of plane from the coordinate origin (perpendicular)
	plane->distance = r ;
	for( k = 0 ; k < AtomsCount ; k++ )
		plane->transform[k] = k ;
	if( refine_symmetry_element( plane, 0 ) < 0 ){
	    if( verbose > 0 ) fprintf(OutputTextFile,"    refinement failed for the plane\n" ) ;
		destroy_symmetry_element( plane ) ;
		return NULL ;
    }
	return plane ;
}
/*
 *   Inversion-center specific functions
 */
//---------- tested ok ----------------
void invert_atom( SYMMETRY_ELEMENT *center, ATOM_*from, ATOM_*to )
{
	int i ;
	to->type = from->type ;
	for( i = 0 ; i < DIMENSION ; i++ ){
		to->x[i] = 2*center->distance*center->normal[i] - from->x[i] ;
    }
}
//----------------------------- tested ok ------------------------------------------------
SYMMETRY_ELEMENT* CSymmetry::init_inversion_center( void )
{
	SYMMETRY_ELEMENT * center = alloc_symmetry_element() ;
    int                k ;
    double             r ;
// this routine tries only the geometrical center of the atom cluster as an inversion center
 if (verbose > 0 ) fprintf(OutputTextFile,"Trying inversion center at the center of something\n" ) ;
	StatTotal++ ;
	center->transform_atom = invert_atom ;	// transformation function
	center->order          = 2 ;
	center->nparam         = 4 ;
// distance from the coordinate origin
	for( k = 0, r = 0 ; k < DIMENSION ; k++ )
		r += CenterOfSomething[k]*CenterOfSomething[k] ;
	r = sqrt(r) ;
// normal to the center
	if( r > 0 ){
		for( k = 0 ; k < DIMENSION ; k++ )
			center->normal[k] = CenterOfSomething[k]/r ;
    }
	else {
		center->normal[0] = 1 ;
		for( k = 1 ; k < DIMENSION ; k++ )
			center->normal[k] = 0 ;
    }
// distance from the coordinate origin
	center->distance = r ;
	if( verbose > 0 ) fprintf(OutputTextFile,"    initial inversion center is at %g from the origin\n", r ) ;
	if( refine_symmetry_element( center, 1 ) < 0 ){
	    if( verbose > 0 ) fprintf(OutputTextFile,"    refinement failed for the inversion center\n" ) ;
	    destroy_symmetry_element( center ) ;
		return NULL ;
    }
	return center ;
}
//----------------------------------------------------------------------------------
/*
 *   Normal rotation axis-specific routines.
 */
void rotate_atom( SYMMETRY_ELEMENT *axis, ATOM_*from, ATOM_*to )
{
	double             x[3], y[3], a[3], b[3], c[3] ;
    double             angle = axis->order ? 2*M_PI/axis->order : 1.0 ;
    double             a_sin = sin( angle ) ;
    double             a_cos = cos( angle ) ;
    double             dot ;
    int                i ;

	if( DIMENSION != 3 ){
		fprintf( OutputTextFile, "Catastrophe in rotate_atom!\n" ) ;
		exit( EXIT_FAILURE ) ;
    }
	for( i = 0 ; i < 3 ; i++ )
	    x[i] = from->x[i] - axis->distance * axis->normal[i] ;
	for( i = 0, dot = 0 ; i < 3 ; i++ )
	    dot += x[i] * axis->direction[i] ;
	for( i = 0 ; i < 3 ; i++ )
		a[i] = axis->direction[i] * dot ;
	for( i = 0 ; i < 3 ; i++ )
	    b[i] = x[i] - a[i] ;
	c[0] = b[1]*axis->direction[2] - b[2]*axis->direction[1] ;
	c[1] = b[2]*axis->direction[0] - b[0]*axis->direction[2] ;
	c[2] = b[0]*axis->direction[1] - b[1]*axis->direction[0] ;
	for( i = 0 ; i < 3 ; i++ )
	    y[i] = a[i] + b[i]*a_cos + c[i]*a_sin ;
	for( i = 0 ; i < 3 ; i++ )
	    to->x[i] = y[i] + axis->distance * axis->normal[i] ;
	to->type = from->type ;
}
//----------------------------------------------------------------------------------
SYMMETRY_ELEMENT* CSymmetry::init_ultimate_axis(void)
{
	SYMMETRY_ELEMENT * axis = alloc_symmetry_element() ;
    double             dir[ DIMENSION ], rel[ DIMENSION ] ;
    double             s ;
    int                i, k ;

	if( verbose > 0 ) fprintf(OutputTextFile,"Trying infinity axis\n" ) ;
	StatTotal++ ;
	axis->transform_atom = rotate_atom ;
	axis->order          = 0 ;	// for ultimate axis
	axis->nparam         = 7 ;
	for( k = 0 ; k < DIMENSION ; k++ )
	    dir[k] = 0 ;
	for( i = 0 ; i < AtomsCount ; i++ ){
	    for( k = 0, s = 0 ; k < DIMENSION ; k++ ){
			rel[k] = Atoms[i].x[k] - CenterOfSomething[k] ;
			s     += rel[k]*dir[k] ;
        }
		if( s >= 0 )
			for( k = 0 ; k < DIMENSION ; k++ )
				dir[k] += rel[k] ;
		else
			for( k = 0 ; k < DIMENSION ; k++ )
				dir[k] -= rel[k] ;
    }
	for( k = 0, s = 0 ; k < DIMENSION ; k++ )
		s += pow2( dir[k] ) ;
	s = sqrt(s) ;
	if( s > 0 )
		for( k = 0 ; k < DIMENSION ; k++ )
			dir[k] /= s ;
	else dir[0] = 1 ;
	for( k = 0 ; k < DIMENSION ; k++ )
		axis->direction[k] = dir[k] ;
	for( k = 0, s = 0 ; k < DIMENSION ; k++ )
		s += pow2( CenterOfSomething[k] ) ;
	s = sqrt(s) ;
	if( s > 0 )
	    for( k = 0 ; k < DIMENSION ; k++ )
		    axis->normal[k] = CenterOfSomething[k]/s ;	// but this is not really the normal !!!
	else {
		for( k = 1 ; k < DIMENSION ; k++ )
			axis->normal[k] = 0 ;
		axis->normal[0] = 1 ;
    }
	axis->distance = s ;	// this will be not the perpendicular distance
	for( k = 0 ; k < AtomsCount ; k++ )
		axis->transform[k] = k ;
	if( refine_symmetry_element( axis, 0 ) < 0 ){
		if( verbose > 0 ) fprintf(OutputTextFile,"    refinement failed for the infinity axis\n" ) ;
		destroy_symmetry_element( axis ) ;
		return NULL ;
    }
	return axis ;
}
//-----------------------------------------------------------------------------------------------
SYMMETRY_ELEMENT* CSymmetry::init_c2_axis( int i, int j, double support[ DIMENSION ] )
{
	SYMMETRY_ELEMENT * axis ;
    int                k ;
    double             ris, rjs ;
    double             r, center[ DIMENSION ] ;

	if( verbose > 0 ) 
		fprintf(OutputTextFile,"Trying c2 axis for the pair (%d,%d) with the support (%g,%g,%g)\n", 
             i, j, support[0], support[1], support[2] ) ;
	StatTotal++ ;
/* First, do a quick sanity check */
	for( k = 0, ris = rjs = 0 ; k < DIMENSION ; k++ ){
		ris += pow2( Atoms[i].x[k] - support[k] ) ;
		rjs += pow2( Atoms[j].x[k] - support[k] ) ;
    }
	ris = sqrt( ris ) ;
	rjs = sqrt( rjs ) ;
	if( fabs( ris - rjs ) > TolerancePrimary ){
	    StatEarly++ ;
	    if( verbose > 0 ) fprintf(OutputTextFile,"    Support can't actually define a rotation axis\n" ) ;
	    return NULL ;
    }
	axis			     = alloc_symmetry_element() ;
	axis->transform_atom = rotate_atom ;
	axis->order          = 2 ;
	axis->nparam         = 7 ;
	for( k = 0, r = 0 ; k < DIMENSION ; k++ )
		r += CenterOfSomething[k]*CenterOfSomething[k] ;
	r = sqrt(r) ;
	if( r > 0 ){
	    for( k = 0 ; k < DIMENSION ; k++ )
		    axis->normal[k] = CenterOfSomething[k]/r ;
    }
	else {
		axis->normal[0] = 1 ;
		for( k = 1 ; k < DIMENSION ; k++ )
			axis->normal[k] = 0 ;
    }
	axis->distance = r ;
	for( k = 0, r = 0 ; k < DIMENSION ; k++ ){
	    center[k] = ( Atoms[i].x[k] + Atoms[j].x[k] ) / 2 - support[k] ;
		r        += center[k]*center[k] ;
    }
	r = sqrt(r) ;
	if( r <= TolerancePrimary ){ /* c2 is underdefined, let's do something special */
		if( MolecularPlane != NULL ){
			if( verbose > 0 ) fprintf(OutputTextFile,"    c2 is underdefined, but there is a molecular plane\n" ) ;
			for( k = 0 ; k < DIMENSION ; k++ )
				axis->direction[k] = MolecularPlane->normal[k] ;
        }
		else {
			if( verbose > 0 ) fprintf(OutputTextFile,"    c2 is underdefined, trying random direction\n" ) ;
			for( k = 0 ; k < DIMENSION ; k++ )
				center[k] = Atoms[i].x[k] - Atoms[j].x[k] ;
			if( fabs( center[2] ) + fabs( center[1] ) > ToleranceSame ){
				axis->direction[0] =  0 ;
				axis->direction[1] =  center[2] ;
				axis->direction[2] = -center[1] ;
            }
			else {
				axis->direction[0] = -center[2] ;
				axis->direction[1] =  0 ;
				axis->direction[2] =  center[0] ;
            }
			for( k = 0, r = 0 ; k < DIMENSION ; k++ )
				r += axis->direction[k] * axis->direction[k] ;
			r = sqrt(r) ;
			for( k = 0 ; k < DIMENSION ; k++ )
				axis->direction[k] /= r ;
        }
    }
	else { /* direction is Ok, renormalize it */
		for( k = 0 ; k < DIMENSION ; k++ )
			axis->direction[k] = center[k]/r ;
    }
	if( refine_symmetry_element( axis, 1 ) < 0 ){
		if( verbose > 0 ) fprintf(OutputTextFile,"    refinement failed for the c2 axis\n" ) ;
		destroy_symmetry_element( axis ) ;
		return NULL ;
    }
	return axis ;
}
//--------------------------------------------------------------------------------------------
SYMMETRY_ELEMENT* CSymmetry::init_axis_parameters( double a[3], double b[3], double c[3] )
{
	SYMMETRY_ELEMENT * axis ;
    int                i, order, sign ;
    double             ra, rb, rc, rab, rbc, rac, r ;
    double             angle ;

	ra = rb = rc = rab = rbc = rac = 0 ;
	for( i = 0 ; i < DIMENSION ; i++ ){
	    ra  += a[i]*a[i] ;
	    rb  += b[i]*b[i] ;
	    rc  += c[i]*c[i] ;
    }
	ra = sqrt(ra) ; rb  = sqrt(rb) ; rc  = sqrt(rc) ;
	if( fabs( ra - rb ) > TolerancePrimary || fabs( ra - rc ) > TolerancePrimary || fabs( rb - rc ) > TolerancePrimary ){
	    StatEarly++ ;
	    if( verbose > 0 ) fprintf(OutputTextFile,"    points are not on a sphere\n" ) ;
	    return NULL ;
    }
	for( i = 0 ; i < DIMENSION ; i++ ){
	    rab += (a[i]-b[i])*(a[i]-b[i]) ;
	    rac += (a[i]-c[i])*(a[i]-c[i]) ;
	    rbc += (c[i]-b[i])*(c[i]-b[i]) ;
    }
	rab = sqrt(rab) ;
	rac = sqrt(rac) ;
	rbc = sqrt(rbc) ;
	if( fabs( rab - rbc ) > TolerancePrimary ){
	    StatEarly++ ;
	    if( verbose > 0 ) fprintf(OutputTextFile,"    points can't be rotation-equivalent\n" ) ;
	    return NULL ;
    }
	if( rab <= ToleranceSame || rbc <= ToleranceSame || rac <= ToleranceSame ){
	    StatEarly++ ;
	    if( verbose > 0 ) fprintf(OutputTextFile,"    rotation is underdefined by these points\n" ) ;
	    return NULL ;
    }
	rab   = (rab+rbc)/2 ;
	angle = M_PI - 2*asin( rac/(2*rab) ) ;
	if( verbose > 1 ) fprintf(OutputTextFile,"    rotation angle is %f\n", angle ) ;
	if( fabs(angle) <= M_PI/(MaxAxisOrder+1) ){
	    StatEarly++ ;
		if( verbose > 0 ) fprintf(OutputTextFile,"    atoms are too close to a straight line\n" ) ;
		return NULL ;
    }
	order = (int)floor( (2*M_PI)/angle + 0.5 ) ;
	if( order <= 2 || order > MaxAxisOrder ){
	    StatEarly++ ;
	    if( verbose > 0 ) fprintf(OutputTextFile,"    rotation axis order (%d) is not from 3 to %d\n", order, MaxAxisOrder ) ;
		return NULL ;
    }
	axis = alloc_symmetry_element() ;
	axis->order          = order ;
	axis->nparam         = 7 ;
	for( i = 0, r = 0 ; i < DIMENSION ; i++ )
		r += CenterOfSomething[i]*CenterOfSomething[i] ;
	r = sqrt(r) ;
	if( r > 0 ){
	    for( i = 0 ; i < DIMENSION ; i++ )
			axis->normal[i] = CenterOfSomething[i]/r ;
    }
	else {
		axis->normal[0] = 1 ;
		for( i = 1 ; i < DIMENSION ; i++ )
	        axis->normal[i] = 0 ;
    }
	axis->distance = r ;
	axis->direction[0] = (b[1]-a[1])*(c[2]-b[2]) - (b[2]-a[2])*(c[1]-b[1]) ;
	axis->direction[1] = (b[2]-a[2])*(c[0]-b[0]) - (b[0]-a[0])*(c[2]-b[2]) ;
	axis->direction[2] = (b[0]-a[0])*(c[1]-b[1]) - (b[1]-a[1])*(c[0]-b[0]) ;
/*
 *  Arbitrarily select axis direction so that first non-zero component
 *  or the direction is positive.
 */
	sign = 0 ;
	if( axis->direction[0] <= 0 )
	    if( axis->direction[0] < 0 )
			sign = 1 ;
		else if( axis->direction[1] <= 0 )
				if( axis->direction[1] < 0 )
					sign = 1 ;
				else if( axis->direction[2] < 0 )
						sign = 1 ;
	if( sign )
	    for( i = 0 ; i < DIMENSION ; i++ )
		    axis->direction[i] = -axis->direction[i] ;
	for( i = 0, r = 0 ; i < DIMENSION ; i++ )
	    r += axis->direction[i]*axis->direction[i] ;
	r = sqrt(r) ;
	for( i = 0 ; i < DIMENSION ; i++ )
	    axis->direction[i] /= r ;
	if( verbose > 1 ){
	    fprintf(OutputTextFile,"    axis origin is at (%g,%g,%g)\n", 
			axis->normal[0]*axis->distance, axis->normal[1]*axis->distance, axis->normal[2]*axis->distance ) ;
		fprintf(OutputTextFile,"    axis is in the direction (%g,%g,%g)\n", axis->direction[0], axis->direction[1], axis->direction[2] ) ;
	}
	return axis ;
}
//------------------------------------------------------------------------------------
SYMMETRY_ELEMENT* CSymmetry::init_higher_axis( int ia, int ib, int ic )
{
	SYMMETRY_ELEMENT * axis ;
    double             a[ DIMENSION ], b[ DIMENSION ], c[ DIMENSION ] ;
    int                i ;

	if( verbose > 0 ) fprintf(OutputTextFile,"Trying cn axis for the triplet (%d,%d,%d)\n", ia, ib, ic ) ;
	StatTotal++ ;
/* Do a quick check of geometry validity */
	for( i = 0 ; i < DIMENSION ; i++ ){
	    a[i] = Atoms[ia].x[i] - CenterOfSomething[i] ;
	    b[i] = Atoms[ib].x[i] - CenterOfSomething[i] ;
	    c[i] = Atoms[ic].x[i] - CenterOfSomething[i] ;
    }
	if( ( axis = init_axis_parameters( a, b, c ) ) == NULL ){
	    if( verbose > 0 ) fprintf(OutputTextFile,"    no coherrent axis is defined by the points\n" ) ;
	    return NULL ;
    }
	axis->transform_atom = rotate_atom ;
	if( refine_symmetry_element( axis, 1 ) < 0 ){
		if( verbose > 0 ) fprintf(OutputTextFile,"    refinement failed for the c%d axis\n", axis->order ) ;
		destroy_symmetry_element( axis ) ;
		return NULL ;
    }
	return axis ;
}

/*
 *   Improper axes-specific routines.
 *   These are obtained by slight modifications of normal rotation
 *       routines.
 */
void rotate_reflect_atom( SYMMETRY_ELEMENT *axis, ATOM_*from, ATOM_*to )
{
	double             x[3], y[3], a[3], b[3], c[3] ;
    double             angle = 2*M_PI/axis->order ;
    double             a_sin = sin( angle ) ;
    double             a_cos = cos( angle ) ;
    double             dot ;
    int                i ;

	if( DIMENSION != 3 ){
		fprintf( OutputTextFile, "Catastrophe in rotate_reflect_atom!\n" ) ;
		exit( EXIT_FAILURE ) ;
    }
	for( i = 0 ; i < 3 ; i++ )
	    x[i] = from->x[i] - axis->distance * axis->normal[i] ;
	for( i = 0, dot = 0 ; i < 3 ; i++ )
	    dot += x[i] * axis->direction[i] ;
	for( i = 0 ; i < 3 ; i++ )
	    a[i] = axis->direction[i] * dot ;
	for( i = 0 ; i < 3 ; i++ )
	    b[i] = x[i] - a[i] ;
	c[0] = b[1]*axis->direction[2] - b[2]*axis->direction[1] ;
	c[1] = b[2]*axis->direction[0] - b[0]*axis->direction[2] ;
	c[2] = b[0]*axis->direction[1] - b[1]*axis->direction[0] ;
	for( i = 0 ; i < 3 ; i++ )
	    y[i] = -a[i] + b[i]*a_cos + c[i]*a_sin ;
	for( i = 0 ; i < 3 ; i++ )
	    to->x[i] = y[i] + axis->distance * axis->normal[i] ;
	to->type = from->type ;
}
//-------------------------------------------------------------------------------
SYMMETRY_ELEMENT* CSymmetry::init_improper_axis( int ia, int ib, int ic )
{
	SYMMETRY_ELEMENT * axis ;
    double             a[ DIMENSION ], b[ DIMENSION ], c[ DIMENSION ] ;
    double             centerpoint[ DIMENSION ] ;
    double             r ;
    int                i ;

	if( verbose > 0 ) fprintf(OutputTextFile,"Trying sn axis for the triplet (%d,%d,%d)\n", ia, ib, ic ) ;
	StatTotal++ ;
/* First, reduce the problem to Cn case */
	for( i = 0 ; i < DIMENSION ; i++ ){
	    a[i] = Atoms[ia].x[i] - CenterOfSomething[i] ;
	    b[i] = Atoms[ib].x[i] - CenterOfSomething[i] ;
	    c[i] = Atoms[ic].x[i] - CenterOfSomething[i] ;
    }
	for( i = 0, r = 0 ; i < DIMENSION ; i++ ){
	    centerpoint[i] = a[i] + c[i] + 2*b[i] ;
	    r += centerpoint[i]*centerpoint[i] ;
    }
	r = sqrt(r) ;
	if( r <= ToleranceSame ){
	    StatEarly++ ;
	    if( verbose > 0 ) fprintf(OutputTextFile,"    atoms can not define improper axis of the order more than 2\n" ) ;
	    return NULL ;
    }
	for( i = 0 ; i < DIMENSION ; i++ )
	    centerpoint[i] /= r ;
	for( i = 0, r = 0 ; i < DIMENSION ; i++ )
	    r += centerpoint[i] * b[i] ;
	for( i = 0 ; i < DIMENSION ; i++ )
	    b[i] = 2*r*centerpoint[i] - b[i] ;
/* Do a quick check of geometry validity */
	if( ( axis = init_axis_parameters( a, b, c ) ) == NULL ){
	    if( verbose > 0 ) fprintf(OutputTextFile,"    no coherrent improper axis is defined by the points\n" ) ;
	    return NULL ;
    }
	axis->transform_atom = rotate_reflect_atom ;
	if( refine_symmetry_element( axis, 1 ) < 0 ){
		if( verbose > 0 ) fprintf(OutputTextFile,"    refinement failed for the s%d axis\n", axis->order ) ;
		destroy_symmetry_element( axis ) ;
		return NULL ;
    }
	return axis ;
}

/*
 *   Control routines
 */
//---------------------  finds the geometric center of the atom cluster -------------------------------------
// AtomsCount = total number of atoms in the cluster
// Atoms[] - atom parameters (type and coordinates)
// DIMENSION == 3
// DistanceFromCenter = dist. of the geom. center from the coordinate origin
void CSymmetry::find_center_of_something( void )
{
	int                i, j ;
    double             coord_sum[ DIMENSION ] ;
    double             r ;

	for( j = 0 ; j < DIMENSION ; j++ )
		coord_sum[j] = 0 ;
	for( i = 0 ; i < AtomsCount ; i++ ){
	    for( j = 0 ; j < DIMENSION ; j++ )
		    coord_sum[j] += Atoms[i].x[j] ;
    }
	for( j = 0 ; j < DIMENSION ; j++ )
		CenterOfSomething[j] = coord_sum[j]/AtomsCount ;
	if( verbose > 0 )
		fprintf(OutputTextFile,"Center of something is at %15.10f, %15.10f, %15.10f\n", 
			    CenterOfSomething[0], CenterOfSomething[1], CenterOfSomething[2] ) ;
	DistanceFromCenter = (double *) calloc( AtomsCount, sizeof( double ) ) ;		// Global variable - FREE IT LATER!
	if( DistanceFromCenter == NULL ){
		fprintf( OutputTextFile, "Unable to allocate array for the distances\n" ) ;
		exit( EXIT_FAILURE ) ;
    }
	for( i = 0 ; i < AtomsCount ; i++ ){
		for( j = 0, r = 0 ; j < DIMENSION ; j++ )
			r += pow2( Atoms[i].x[j] - CenterOfSomething[j] ) ;
		DistanceFromCenter[i] = r ;
    }
}
//----------------------------------------------------------------------------------
void CSymmetry::find_planes(void)
{
    int                i, j ;
    SYMMETRY_ELEMENT * plane ;

	plane = init_ultimate_plane() ;	// tries to find a whole-molecule mirror plane
	if( plane != NULL ){
		MolecularPlane = plane ;
		PlanesCount++ ;
		Planes = (SYMMETRY_ELEMENT **) realloc( Planes, sizeof( SYMMETRY_ELEMENT* ) * PlanesCount ) ;
		if( Planes == NULL ){
	        perror( "Out of memory in find_planes" ) ;
			exit( EXIT_FAILURE ) ;
        }
		Planes[ PlanesCount - 1 ] = plane ;
    }
	for( i = 1 ; i < AtomsCount ; i++ ){
		for( j = 0 ; j < i ; j++ ){
			if( Atoms[i].type != Atoms[j].type )
				continue ;
			if( ( plane = init_mirror_plane( i, j ) ) != NULL ){
				PlanesCount++ ;
				Planes = (SYMMETRY_ELEMENT **) realloc( Planes, sizeof( SYMMETRY_ELEMENT* ) * PlanesCount ) ;
				if( Planes == NULL ){
	                perror( "Out of memory in find_planes" ) ;
					exit( EXIT_FAILURE ) ;
                }
				Planes[ PlanesCount - 1 ] = plane ;
            }
        }
    }
}
//------------------------------------------------------------------------------------
void CSymmetry::find_inversion_centers(void)
{
	SYMMETRY_ELEMENT * center ;

	if((center = init_inversion_center()) != NULL ){
		InversionCenters = (SYMMETRY_ELEMENT **) calloc(1, sizeof( SYMMETRY_ELEMENT* )) ;
		InversionCenters[0]   = center ;
		InversionCentersCount = 1 ;
    }
}
//-------------------------------------------------------------------------------------
void CSymmetry::find_infinity_axis(void)
{
	SYMMETRY_ELEMENT * axis ;

	if( ( axis = init_ultimate_axis() ) != NULL ){
		NormalAxesCount++ ;
		NormalAxes = (SYMMETRY_ELEMENT **) realloc( NormalAxes, sizeof( SYMMETRY_ELEMENT* ) * NormalAxesCount ) ;
		if( NormalAxes == NULL ){
			perror( "Out of memory in find_infinity_axes()" ) ;
			exit( EXIT_FAILURE ) ;
        }
		NormalAxes[ NormalAxesCount - 1 ] = axis ;
    }
}
//-------------------------------------------------------------------------------------
void CSymmetry::find_c2_axes(void)
{
	int                i, j, k, l, m ;
    double             center[ DIMENSION ] ;
    double *           distances = (double*)calloc( AtomsCount, sizeof( double ) ) ;
    double             r ;
    SYMMETRY_ELEMENT * axis ;

	if( distances == NULL ){
		fprintf( OutputTextFile, "Out of memory in find_c2_axes()\n" ) ;
		exit( EXIT_FAILURE ) ;
    }
	for( i = 1 ; i < AtomsCount ; i++ ){
	    for( j = 0 ; j < i ; j++ ){
		    if( Atoms[i].type != Atoms[j].type )
			    continue ;
			if( fabs( DistanceFromCenter[i] - DistanceFromCenter[j] ) > TolerancePrimary )
				continue ; /* A very cheap, but quite effective check */
        /*
         *   First, let's try to get it cheap and use CenterOfSomething
         */
			for( k = 0, r = 0 ; k < DIMENSION ; k++ ){
				center[k] = ( Atoms[i].x[k] + Atoms[j].x[k] ) / 2 ;
				r += pow2( center[k] - CenterOfSomething[k] ) ;
            }
			r = sqrt(r) ;
			if( r > 5*TolerancePrimary ){ /* It's Ok to use CenterOfSomething */
				if( ( axis = init_c2_axis( i, j, CenterOfSomething ) ) != NULL ){
	                NormalAxesCount++ ;
					NormalAxes = (SYMMETRY_ELEMENT **) realloc( NormalAxes, sizeof( SYMMETRY_ELEMENT* ) * NormalAxesCount ) ;
					if( NormalAxes == NULL ){
						perror( "Out of memory in find_c2_axes" ) ;
						exit( EXIT_FAILURE ) ;
                    }
					NormalAxes[ NormalAxesCount - 1 ] = axis ;
                }
				continue ;
            }
        /*
         *  Now, C2 axis can either pass through an atom, or through the
         *  middle of the other pair.
         */
			for( k = 0 ; k < AtomsCount ; k++ ){
				if( ( axis = init_c2_axis( i, j, Atoms[k].x ) ) != NULL ){
	                NormalAxesCount++ ;
		            NormalAxes = (SYMMETRY_ELEMENT **) realloc( NormalAxes, sizeof( SYMMETRY_ELEMENT* ) * NormalAxesCount ) ;
					if( NormalAxes == NULL ){
						perror( "Out of memory in find_c2_axes" ) ;
						exit( EXIT_FAILURE ) ;
                    }
					NormalAxes[ NormalAxesCount - 1 ] = axis ;
                }
            }
        /*
         *  Prepare data for an additional pre-screening check
         */
			for( k = 0 ; k < AtomsCount ; k++ ){
				for( l = 0, r = 0 ; l < DIMENSION ; l++ )
					r += pow2( Atoms[k].x[l] - center[l] ) ;
				distances[k] = sqrt(r) ;
            }
			for( k = 0 ; k < AtomsCount ; k++ ){
				for( l = 0 ; l < AtomsCount ; l++ ){
					if( Atoms[k].type != Atoms[l].type )
						continue ;
					if( fabs( DistanceFromCenter[k] - DistanceFromCenter[l] ) > TolerancePrimary ||
						fabs( distances[k] - distances[l] ) > TolerancePrimary )
							continue ; /* We really need this one to run reasonably fast! */
					for( m = 0 ; m < DIMENSION ; m++ )
						center[m] = ( Atoms[k].x[m] + Atoms[l].x[m] ) / 2 ;
					if( ( axis = init_c2_axis( i, j, center ) ) != NULL ){
						NormalAxesCount++ ;
						NormalAxes = (SYMMETRY_ELEMENT **) realloc( NormalAxes, sizeof( SYMMETRY_ELEMENT* ) * NormalAxesCount ) ;
						if( NormalAxes == NULL ){
							perror( "Out of memory in find_c2_axes" ) ;
							exit( EXIT_FAILURE ) ;
                        }
						NormalAxes[ NormalAxesCount - 1 ] = axis ;
                    }
                }
            }
        }
    }
	free( distances ) ;
}
//----------------------------------------------------------------------------------------------
void CSymmetry::find_higher_axes(void)
{
	int                i, j, k ;
    SYMMETRY_ELEMENT * axis ;

	for( i = 0 ; i < AtomsCount ; i++ ){
	    for( j = i + 1 ; j < AtomsCount ; j++ ){
			if( Atoms[i].type != Atoms[j].type )
	            continue ;
			if( fabs( DistanceFromCenter[i] - DistanceFromCenter[j] ) > TolerancePrimary )
	            continue ; /* A very cheap, but quite effective check */
			for( k = 0 ; k < AtomsCount ; k++ ){
	            if( Atoms[i].type != Atoms[k].type )
					continue ;
				if( ( fabs( DistanceFromCenter[i] - DistanceFromCenter[k] ) > TolerancePrimary ) ||
	                ( fabs( DistanceFromCenter[j] - DistanceFromCenter[k] ) > TolerancePrimary ) )
						continue ;
				if( ( axis = init_higher_axis( i, j, k ) ) != NULL ){
	                NormalAxesCount++ ;
					NormalAxes = (SYMMETRY_ELEMENT **) realloc( NormalAxes, sizeof( SYMMETRY_ELEMENT* ) * NormalAxesCount ) ;
					if( NormalAxes == NULL ){
						perror( "Out of memory in find_higher_axes" ) ;
						exit( EXIT_FAILURE ) ;
                    }
					NormalAxes[ NormalAxesCount - 1 ] = axis ;
                }
            }
        }
    }
}
//----------------------------------------------------------------------------------------------
void CSymmetry::find_improper_axes(void)
{
	int                i, j, k ;
    SYMMETRY_ELEMENT * axis ;

	for( i = 0 ; i < AtomsCount ; i++ ){
	    for( j = i + 1 ; j < AtomsCount ; j++ ){
			for( k = 0 ; k < AtomsCount ; k++ ){
	            if( ( axis = init_improper_axis( i, j, k ) ) != NULL ){
					ImproperAxesCount++ ;
					ImproperAxes = (SYMMETRY_ELEMENT **) realloc( ImproperAxes, sizeof( SYMMETRY_ELEMENT* ) * ImproperAxesCount ) ;
					if( ImproperAxes == NULL ){
	                    perror( "Out of memory in find_higher_axes" ) ;
						exit( EXIT_FAILURE ) ;
                    }
					ImproperAxes[ ImproperAxesCount - 1 ] = axis ;
                }
            }
        }
    }
}
//-------------------------------------------------------------------------------------------
void CSymmetry::report_planes( void )
{
        int           i ;

	if( PlanesCount == 0 )
	{
		fprintf(OutputTextFile,"There are no planes of symmetry in the molecule\n" ) ;
//		fprintf(SymmetryElementFile,"There are no planes of symmetry in the molecule\n" ) ;
	}
	else {
//		if( PlanesCount == 1 )
//			fprintf(OutputTextFile,"There is a plane of symmetry in the molecule\n" ) ;
//		else
		fprintf(OutputTextFile,"There are %d planes of symmetry in the molecule\n", PlanesCount ) ;
//		fprintf(SymmetryElementFile,"There are %d planes of symmetry in the molecule\n", PlanesCount ) ;
		fprintf(OutputTextFile,"     Residual          Direction of the normal           Distance\n" ) ;
		for( i = 0 ; i < PlanesCount ; i++ ){
			fprintf(OutputTextFile,"%3d %8.4e ", i, Planes[i]->maxdev);
// format in the SymmetryElementFile: type=3, direction of normal, distance
			fprintf(SymmetryElementFile,"%3d %3d %3d %14.8f %14.8f ",3,Planes[i]->order,
				Planes[i]->nparam,Planes[i]->maxdev,Planes[i]->distance);
			fprintf(OutputTextFile,"(%11.8f,%11.8f,%11.8f) ", Planes[i]->normal[0], Planes[i]->normal[1], Planes[i]->normal[2] ) ;
			fprintf(SymmetryElementFile,"%14.8f %14.8f %14.8f ", Planes[i]->normal[0], Planes[i]->normal[1], Planes[i]->normal[2] ) ;
			fprintf(OutputTextFile,"%14.8f\n", Planes[i]->distance);
			fprintf(SymmetryElementFile,"%14.8f %14.8f %14.8f\n",Planes[i]->direction[0],
				Planes[i]->direction[1],Planes[i]->direction[2]);
        }
    }
}
//-------------------------------------------------------------------------------------------
void CSymmetry::report_inversion_centers( void )
{
	if( InversionCentersCount == 0 )
	{
		fprintf(OutputTextFile,"There is no inversion center in the molecule\n");
//		fprintf(SymmetryElementFile,"There is no inversion center in the molecule\n");
	}
	else {
		fprintf(OutputTextFile,"There is an inversion center in the molecule\n" );
//		fprintf(SymmetryElementFile,"There is an inversion center in the molecule\n");
		fprintf(OutputTextFile,"     Residual                      Position\n" ) ;
		fprintf(OutputTextFile,"   %8.4e ", InversionCenters[0]->maxdev ) ;
		fprintf(OutputTextFile,"(%14.8f,%14.8f,%14.8f)\n",
			InversionCenters[0]->distance * InversionCenters[0]->normal[0],
			InversionCenters[0]->distance * InversionCenters[0]->normal[1],
			InversionCenters[0]->distance * InversionCenters[0]->normal[2] ) ;
// format in the SymmetryElementFile: type=0
		fprintf(SymmetryElementFile,"%3d %3d %3d %14.8f %14.8f ",0,InversionCenters[0]->order,
			InversionCenters[0]->nparam,InversionCenters[0]->maxdev,
			InversionCenters[0]->distance);
		fprintf(SymmetryElementFile,"%14.8f %14.8f %14.8f ", InversionCenters[0]->normal[0],
			InversionCenters[0]->normal[1], InversionCenters[0]->normal[2]) ;
		fprintf(SymmetryElementFile,"%14.8f %14.8f %14.8f\n",InversionCenters[0]->direction[0],
				InversionCenters[0]->direction[1],InversionCenters[0]->direction[2]);
    }
}
//-------------------------------------------------------------------------------------------
void CSymmetry::report_axes( void )
{
    int           i ;

	if( NormalAxesCount == 0 )
	{
		fprintf(OutputTextFile,"There are no normal axes in the molecule\n");
//		fprintf(SymmetryElementFile,"There are no normal axes in the molecule\n");
	}
	else {
//	    if( NormalAxesCount == 1 )
//			fprintf(OutputTextFile,"There is a normal axis in the molecule\n" ) ;
//		else 
		fprintf(OutputTextFile,"There are %d normal axes in the molecule\n", NormalAxesCount ) ;
//		fprintf(SymmetryElementFile,"There are %d normal axes in the molecule\n", NormalAxesCount ) ;
		fprintf(OutputTextFile,"     Residual  Order         Direction of the axis                         Supporting point\n" ) ;
		for( i = 0 ; i < NormalAxesCount ; i++ ){
			fprintf(OutputTextFile,"%3d %8.4e ", i, NormalAxes[i]->maxdev );
// format in the SymmetryElementFile: type=1
		fprintf(SymmetryElementFile,"%3d %3d %3d %14.8f %14.8f ",1,NormalAxes[i]->order,
			NormalAxes[i]->nparam,NormalAxes[i]->maxdev,NormalAxes[i]->distance);
		fprintf(SymmetryElementFile,"%14.8f %14.8f %14.8f ", NormalAxes[i]->normal[0],
			NormalAxes[i]->normal[1], NormalAxes[i]->normal[2]) ;
		fprintf(SymmetryElementFile,"%14.8f %14.8f %14.8f\n",NormalAxes[i]->direction[0],
				NormalAxes[i]->direction[1],NormalAxes[i]->direction[2]);

			if( NormalAxes[i]->order == 0 )
				fprintf(OutputTextFile,"Inf ");
			else
				fprintf(OutputTextFile,"%3d ", NormalAxes[i]->order ) ;
			fprintf(OutputTextFile,"(%11.8f,%11.8f,%11.8f) ", 
	           NormalAxes[i]->direction[0], NormalAxes[i]->direction[1], NormalAxes[i]->direction[2] ) ;
			fprintf(OutputTextFile,"(%14.8f,%14.8f,%14.8f)\n", 
	            NormalAxes[0]->distance * NormalAxes[0]->normal[0],
				NormalAxes[0]->distance * NormalAxes[0]->normal[1],
				NormalAxes[0]->distance * NormalAxes[0]->normal[2] ) ;
        }
    }
}
//------------------------------------------------------------------------------------
void CSymmetry::report_improper_axes( void )
{
	int i ;

	if( ImproperAxesCount == 0 )
	{
		fprintf(OutputTextFile,"There are no improper axes in the molecule\n" ) ;
//		fprintf(SymmetryElementFile,"There are no improper axes in the molecule\n" ) ;
	}
	else {
//		if( ImproperAxesCount == 1 )
//			fprintf(OutputTextFile,"There is an improper axis in the molecule\n" ) ;
//		else
		fprintf(OutputTextFile,"There are %d improper axes in the molecule\n", ImproperAxesCount ) ;
//		fprintf(SymmetryElementFile,"There are %d improper axes in the molecule\n", ImproperAxesCount ) ;
		fprintf(OutputTextFile,"     Residual  Order         Direction of the axis                         Supporting point\n" ) ;
		for( i = 0 ; i < ImproperAxesCount ; i++ ){
			fprintf(OutputTextFile,"%3d %8.4e ",i, ImproperAxes[i]->maxdev ) ;
// format in the SymmetryElementFile: type=2
		fprintf(SymmetryElementFile,"%3d %3d %3d %14.8f %14.8f ",2,ImproperAxes[i]->order,
			ImproperAxes[i]->nparam,ImproperAxes[i]->maxdev,ImproperAxes[i]->distance);
		fprintf(SymmetryElementFile,"%14.8f %14.8f %14.8f ", ImproperAxes[i]->normal[0],
			ImproperAxes[i]->normal[1], ImproperAxes[i]->normal[2]) ;
		fprintf(SymmetryElementFile,"%14.8f %14.8f %14.8f\n",ImproperAxes[i]->direction[0],
				ImproperAxes[i]->direction[1],ImproperAxes[i]->direction[2]);

			if( ImproperAxes[i]->order == 0 )
				fprintf(OutputTextFile,"Inf ") ;
			else
				fprintf(OutputTextFile,"%3d ", ImproperAxes[i]->order ) ;

			fprintf(OutputTextFile,"(%11.8f,%11.8f,%11.8f) ", 
	            ImproperAxes[i]->direction[0], ImproperAxes[i]->direction[1], ImproperAxes[i]->direction[2] ) ;

			fprintf(OutputTextFile,"(%14.8f,%14.8f,%14.8f)\n", 
				ImproperAxes[0]->distance * ImproperAxes[0]->normal[0],
				ImproperAxes[0]->distance * ImproperAxes[0]->normal[1],
				ImproperAxes[0]->distance * ImproperAxes[0]->normal[2]);
        }
    }
}
//------------------------------------------------------------------------------------
/*
 *  General symmetry handling
 */
void CSymmetry::report_and_reset_counters( void )
{
fprintf(OutputTextFile,"  %10ld candidates examined\n"
        "  %10ld removed early\n"
        "  %10ld removed during initial mating stage\n"
        "  %10ld removed as duplicates\n"
        "  %10ld removed because of the wrong transformation order\n"
        "  %10ld removed after unsuccessful optimization\n"
        "  %10ld accepted\n",
    StatTotal, StatEarly, StatPairs, StatDups, StatOrder, StatOpt, StatAccept ) ;
StatTotal = StatEarly = StatPairs = StatDups = StatOrder = StatOpt = StatAccept = 0 ;
}
//---------------------------------------------------------------------------------
void CSymmetry::find_symmetry_elements( void )
{
	find_center_of_something() ;
	if(verbose > -1 ){
		fprintf(OutputTextFile,"Looking for the inversion center\n" ) ;
    }
	find_inversion_centers() ;
	if( verbose > -1 ){
		report_and_reset_counters() ;
		fprintf(OutputTextFile,"Looking for the planes of symmetry\n" ) ;
    }
	find_planes() ;
	if( verbose > -1 ){
		report_and_reset_counters() ;
		fprintf(OutputTextFile,"Looking for infinity axis\n" ) ;
    }
	find_infinity_axis() ;
	if( verbose > -1 ){
		report_and_reset_counters() ;
		fprintf(OutputTextFile,"Looking for C2 axes\n" ) ;
    }
	find_c2_axes() ;
	if( verbose > -1 ){
		report_and_reset_counters() ;
		fprintf(OutputTextFile,"Looking for higher axes\n" ) ;
    }
	find_higher_axes() ;
	if( verbose > -1 ){
	    report_and_reset_counters() ;
	    fprintf(OutputTextFile,"Looking for the improper axes\n" ) ;
    }
	find_improper_axes() ;
	if( verbose > -1 ){
	    report_and_reset_counters() ;
    }
}
//---------------------------------------------------------------------------------------
int compare_axes( const void *a, const void *b )
{	// sorts in decreasing order
    SYMMETRY_ELEMENT * axis_a = *(SYMMETRY_ELEMENT**) a ;
    SYMMETRY_ELEMENT * axis_b = *(SYMMETRY_ELEMENT**) b ;
    int                i, order_a, order_b ;

	order_a = axis_a->order ; if( order_a == 0 ) order_a = 10000 ;
	order_b = axis_b->order ; if( order_b == 0 ) order_b = 10000 ;
	if( ( i = order_b - order_a ) != 0 ) return i ;
	if( axis_a->maxdev > axis_b->maxdev ) return -1 ;
	if( axis_a->maxdev < axis_b->maxdev ) return  1 ;
	return 0 ;
}
//--------------------------------------------------------------------------------------
void CSymmetry::sort_symmetry_elements( void )
{
if( PlanesCount > 1 ){
    qsort( Planes, PlanesCount, sizeof( SYMMETRY_ELEMENT * ), compare_axes ) ;
    }
if( NormalAxesCount > 1 ){
    qsort( NormalAxes, NormalAxesCount, sizeof( SYMMETRY_ELEMENT * ), compare_axes ) ;
    }
if( ImproperAxesCount > 1 ){
    qsort( ImproperAxes, ImproperAxesCount, sizeof( SYMMETRY_ELEMENT * ), compare_axes ) ;
    }
}
//--------------------------------------------------------------------------------
void CSymmetry::report_symmetry_elements_verbose( void )
{
	report_inversion_centers() ;
	report_axes() ;
	report_improper_axes() ;
	report_planes() ;
}
//--------------------------------------------------------------------------------
void CSymmetry::summarize_symmetry_elements( void )
{
	int i ;

	NormalAxesCounts   = (int*) calloc( MaxAxisOrder+1, sizeof( int ) ) ;
	ImproperAxesCounts = (int*) calloc( MaxAxisOrder+1, sizeof( int ) ) ;
	for( i = 0 ; i < NormalAxesCount ; i++ )
	    NormalAxesCounts[ NormalAxes[i]->order ]++ ;
	for( i = 0 ; i < ImproperAxesCount ; i++ )
	    ImproperAxesCounts[ ImproperAxes[i]->order ]++ ;
}
//--------------------------------------------------------------------------------
void CSymmetry::report_symmetry_elements_brief( void )
{
	int          i ;
    char *       symmetry_code = (char*)calloc( 1, 10*(PlanesCount+NormalAxesCount+ImproperAxesCount+InversionCentersCount+2) ) ;
    char         buf[ 100 ] ;

	if( symmetry_code == NULL ){
	    fprintf( OutputTextFile, "Unable to allocate memory for symmetry ID code in report_symmetry_elements_brief()\n" ) ;
		exit( EXIT_FAILURE ) ;
    }
	if( PlanesCount + NormalAxesCount + ImproperAxesCount + InversionCentersCount == 0 )
		fprintf(OutputTextFile,"Molecule has no symmetry elements\n" ) ;
	else {
		fprintf(OutputTextFile,"Molecule has the following symmetry elements: " ) ;
		if( InversionCentersCount > 0 ) strcat( symmetry_code, "(i) " ) ;
		if( NormalAxesCounts[0] == 1 ) strcat( symmetry_code, "(Cinf) " ) ;
		if( NormalAxesCounts[0] >  1 ) {
			sprintf( buf, "%d*(Cinf) ", NormalAxesCounts[0] ) ;
			strcat( symmetry_code, buf ) ;
        }
		for( i = MaxAxisOrder ; i >= 2 ; i-- ){
			if( NormalAxesCounts[i] == 1 ){ sprintf( buf, "(C%d) ", i ) ; strcat( symmetry_code, buf ) ; }
			if( NormalAxesCounts[i] >  1 ){ sprintf( buf, "%d*(C%d) ", NormalAxesCounts[i], i ) ; strcat( symmetry_code, buf ) ; }
        }
		for( i = MaxAxisOrder ; i >= 2 ; i-- ){
	        if( ImproperAxesCounts[i] == 1 ){ sprintf( buf, "(S%d) ", i ) ; strcat( symmetry_code, buf ) ; }
			if( ImproperAxesCounts[i] >  1 ){ sprintf( buf, "%d*(S%d) ", ImproperAxesCounts[i], i ) ; strcat( symmetry_code, buf ) ; }
        }
		if( PlanesCount == 1 ) strcat( symmetry_code, "(sigma) " ) ;
		if( PlanesCount >  1 ){ sprintf( buf, "%d*(sigma) ", PlanesCount ) ; strcat( symmetry_code, buf ) ; }
		fprintf(OutputTextFile,"%s\n", symmetry_code ) ;
    }
	SymmetryCode = symmetry_code ;
}
//------------------------------------------------------------------------------------
char* CSymmetry::identify_point_group( void )
{
	int i ;
    int last_matching = -1 ;
    int matching_count = 0 ;

	for( i = 0 ; i < PointGroupsCount ; i++ ){
	    if( strcmp( SymmetryCode, PointGroups[i].symmetry_code ) == 0 ){
		    if( PointGroups[i].check() == 1 ){
				last_matching = i ;
				matching_count++ ;
            }
			else {
				if( verbose > -2 ){
					fprintf(OutputTextFile,"It looks very much like %s, but it is not since %s\n", 
						PointGroups[i].group_name, PointGroupRejectionReason ) ;
				}
			}
		}
    }
	if( matching_count == 0 ){
		fprintf(OutputTextFile,"These symmetry elements match no point group I know of. Sorry.\n" ) ;
    }
	if( matching_count >  1 ){
	    fprintf(OutputTextFile,"These symmetry elements match more than one group I know of.\n"
            "SOMETHING IS VERY WRONG\n" ) ;
		fprintf(OutputTextFile,"Matching groups are:\n" ) ;
		for( i = 0 ; i < PointGroupsCount ; i++ ){
			if( ( strcmp( SymmetryCode, PointGroups[i].symmetry_code ) == 0 ) && ( PointGroups[i].check() == 1 ) ){
				fprintf(OutputTextFile,"    %s\n", PointGroups[i].group_name ) ;
            }
        }
    }
	if( matching_count == 1 ){
		fprintf(OutputTextFile,"It seems to be the %s point group\n", PointGroups[last_matching].group_name ) ;
		return PointGroups[last_matching].group_name;
    }
}

/*
 *  Input/Output
 */
//-------------------------------------------------------------------------------
int CSymmetry::read_coordinates( char *fn )
{
	int i ;
	FILE* in;

	if((in = fopen( fn, "rt" )) == NULL )
        exit( EXIT_FAILURE ) ;

	if( fscanf( in, "%d", &AtomsCount ) != 1 ){
		fprintf( OutputTextFile, "Error reading atom count\n" ) ;
		return -1 ;
    }
	if( verbose > 0 ) fprintf(OutputTextFile,"Atoms count = %d\n", AtomsCount ) ;
	Atoms = (ATOM_*)calloc( AtomsCount, sizeof( ATOM_ ) ) ;
	if( Atoms == NULL ){
	    fprintf( OutputTextFile, "Out of memory for atoms coordinates\n" ) ;
	    return -1 ;
    }
	for( i = 0 ; i < AtomsCount ; i++ ){
	    if( fscanf( in, "%d %lg %lg %lg\n", &Atoms[i].type, &Atoms[i].x[0], &Atoms[i].x[1], &Atoms[i].x[2] ) != 4 ){
			fprintf( OutputTextFile, "Error reading description of the atom %d\n", i ) ;
			return -1 ;
        }
    }
	fclose(in);
	return 0 ;
}
//-------------------------------------------------------------------------------
/*
int main( int argc, char **argv )
{
	char *program = *argv ;
    FILE          *in ;

	for( argc--, argv++ ; argc > 0 ; argc -= 2, argv += 2 ){
		if( **argv != '-' )
			break ;
		if( strcmp( *argv, "-help"         ) == 0 ||
			strcmp( *argv, "-h"            ) == 0 ||
			strcmp( *argv, "-?"            ) == 0 ){
				argc++ ; argv-- ;
				printf( "%s [option value ...] [filename]\n" 
                "Valid options are:\n"
                "  -verbose      (%3d) Determines verbosity level\n"
                "                      All values above 0 are intended for debugging purposes\n"
                "  -maxaxisorder (%3d) Maximum order of rotation axis to look for\n"
                "  -maxoptcycles (%3d) Maximum allowed number of cycles in symmetry element optimization\n"
                "  --                  Terminates option processing\n"
                "Defaults should be Ok for these:\n"
                "  -same         (%8g) Atoms are colliding if distance falls below this value\n"
                "  -primary      (%8g) Initial loose criterion for atom equivalence\n"
                "  -final        (%8g) Final criterion for atom equivalence\n"
                "  -maxoptstep   (%8g) Largest step allowed in symmetry element optimization\n"
                "  -minoptstep   (%8g) Termination criterion in symmetry element optimization\n"
                "  -gradstep     (%8g) Finite step used in numeric gradient evaluation\n" 
                "  -minchange    (%8g) Minimum allowed change in target function\n" 
                "  -minchgcycles (%8d)  Number of minchange cycles before optimization stops\n",
            program, verbose, MaxAxisOrder, MaxOptCycles, ToleranceSame, TolerancePrimary,
            ToleranceFinal, MaxOptStep, MinOptStep, GradientStep, OptChangeThreshold, OptChangeHits ) ;
        printf( "\n"
                "Input is expected in the following format:\n"
                "number_of_atoms\n"
                "AtomicNumber X Y Z\n"
                "...\n" ) ;
        printf( "\n"
                "Note that only primitive rotations will be reported\n" ) ;
        printf( "This is version $Revision: 1.15 $ ($Date: 2000/01/25 16:47:17 $)\n" ) ;
        exit( EXIT_SUCCESS ) ;
        }
    else
    if( strcmp( *argv, "--"            ) == 0 ){
        argc-- ; argv++ ; break ;
        }
    if( argc < 2 ){
        fprintf( stderr, "Missing argument for \"%s\"\n", *argv ) ;
        exit( EXIT_FAILURE ) ;
        }
    if( strcmp( *argv, "-minchgcycles" ) == 0 ){
        if( sscanf( argv[1], "%d", &OptChangeHits ) != 1 ){
            fprintf( stderr, "Invalid parameter for -minchgcycles: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-minchange"    ) == 0 ){
        if( sscanf( argv[1], "%lg", &OptChangeThreshold ) != 1 ){
            fprintf( stderr, "Invalid parameter for -minchange: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-same"         ) == 0 ){
        if( sscanf( argv[1], "%lg", &ToleranceSame ) != 1 ){
            fprintf( stderr, "Invalid parameter for -same: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-primary"      ) == 0 ){
        if( sscanf( argv[1], "%lg", &TolerancePrimary ) != 1 ){
            fprintf( stderr, "Invalid parameter for -primary: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-final"        ) == 0 ){
        if( sscanf( argv[1], "%lg", &ToleranceFinal ) != 1 ){
            fprintf( stderr, "Invalid parameter for -final: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-maxoptstep"   ) == 0 ){
        if( sscanf( argv[1], "%lg", &MaxOptStep ) != 1 ){
            fprintf( stderr, "Invalid parameter for -maxoptstep: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-minoptstep"   ) == 0 ){
        if( sscanf( argv[1], "%lg", &MinOptStep ) != 1 ){
            fprintf( stderr, "Invalid parameter for -minoptstep: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-gradstep"     ) == 0 ){
        if( sscanf( argv[1], "%lg", &GradientStep ) != 1 ){
            fprintf( stderr, "Invalid parameter for -gradstep: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-verbose"      ) == 0 ){
        if( sscanf( argv[1], "%d", &verbose ) != 1 ){
            fprintf( stderr, "Invalid parameter for -verbose: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-maxoptcycles" ) == 0 ){
        if( sscanf( argv[1], "%d", &MaxOptCycles ) != 1 ){
            fprintf( stderr, "Invalid parameter for -maxoptcycles: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else
    if( strcmp( *argv, "-maxaxisorder" ) == 0 ){
        if( sscanf( argv[1], "%d", &MaxAxisOrder ) != 1 ){
            fprintf( stderr, "Invalid parameter for -maxaxisorder: \"%s\"\n", argv[1] ) ;
            exit( EXIT_FAILURE ) ;
            }
        }
    else {
        fprintf( stderr, "Unrecognized option \"%s\"\n", *argv ) ;
        exit( EXIT_FAILURE ) ;
        }
    }
if( argc > 0 ){
    if( ( in = fopen( *argv, "rt" ) ) == NULL ){
        perror( *argv ) ;
        exit( EXIT_FAILURE ) ;
        }
    }
else {
    in = stdin ;
    }
if( read_coordinates( in ) < 0 ){
    fprintf( stderr, "Error reading in atomic coordinates\n" ) ;
    exit( EXIT_FAILURE ) ;
    }
fclose( in ) ;
find_symmetry_elements() ;
sort_symmetry_elements() ;
summarize_symmetry_elements() ;
if( BadOptimization )
    printf( "Refinement of some symmetry elements was terminated before convergence was reached.\n"
            "Some symmetry elements may remain unidentified.\n" ) ;
if( verbose >= 0 )
    report_symmetry_elements_verbose() ;
report_symmetry_elements_brief() ;
identify_point_group() ;
exit( EXIT_SUCCESS ) ;
}
*/
//////////////////////////////////////////////////////////////////////////////////
//	Construction / Destruction
//

CSymmetry::CSymmetry(void)
{
// output file operations
	OutputTextFile = fopen("Symmetry.txt","w");
// file with symmetry elements for further refinement in CMadelungDoc
	SymmetryElementFile = fopen("SymElem.txt","w");
// original settings for symmetry determination
	ToleranceSame         = 1e-3 ;
	TolerancePrimary      = 5e-2 ;
	ToleranceFinal        = 1e-4 ;
	MaxOptStep            = 5e-1 ;
	MinOptStep            = 1e-7 ;
	GradientStep          = 1e-7 ;
	OptChangeThreshold    = 1e-10 ;
	DistanceFromCenter    = NULL ;
	verbose               = 0 ;
	MaxOptCycles          = 200 ;
	OptChangeHits         = 5 ;
	MaxAxisOrder          = 20 ;
	AtomsCount            = 0 ;
	Atoms                 = NULL ;
	PlanesCount           = 0 ;
	Planes                = NULL ;
	MolecularPlane        = NULL ;
	InversionCentersCount = 0 ;
	InversionCenters      = NULL ;
	NormalAxesCount       = 0 ;
	NormalAxes            = NULL ;
	ImproperAxesCount     = 0 ;
	ImproperAxes          = NULL ;
	NormalAxesCounts      = NULL ;
	ImproperAxesCounts    = NULL ;
	BadOptimization       = 0 ;
	SymmetryCode          = "" ;
/*
 *    Statistics
 */
	StatTotal             = 0 ;
	StatEarly             = 0 ;
	StatPairs             = 0 ;
	StatDups              = 0 ;
	StatOrder             = 0 ;
	StatOpt               = 0 ;
	StatAccept            = 0 ;

/*
 *    Point groups I know about
 */

	PointGroupsCount = (sizeof(PointGroups)/sizeof(POINT_GROUP));
	PointGroupRejectionReason = NULL;
}
/////////////////////////////////////////////

CSymmetry::~CSymmetry(void)
{
	fclose(OutputTextFile);
	fclose(SymmetryElementFile);

	if(DistanceFromCenter)
	{
		free(DistanceFromCenter);
		DistanceFromCenter = NULL;
	}
	if(Atoms)
	{
		free(Atoms);
		Atoms = NULL;
	}
	if(Planes)
	{
		for(int i=0; i<PlanesCount; i++)
		{
			free(Planes[i]);
			Planes[i] = NULL;
		}
		free(Planes);
		Planes = NULL;
	}
	if(InversionCenters)
	{
		for(int i=0;i<InversionCentersCount;i++)
		{
			free(InversionCenters[i]);
			InversionCenters[i] = NULL;
		}
		free(InversionCenters);
		InversionCenters = NULL;
	}
	if(NormalAxes)
	{
		for(int i=0;i<NormalAxesCount;i++)
		{
			free(NormalAxes[i]);
			NormalAxes[i] = NULL;
		}
		free(NormalAxes);
		NormalAxes = NULL;
	}
	if(ImproperAxes)
	{
		for(int i=0;i<ImproperAxesCount;i++)
		{
			free(ImproperAxes[i]);
			ImproperAxes[i] = NULL;
		}
		free(ImproperAxes);
		ImproperAxes = NULL;
	}
	if(NormalAxesCounts)
	{
		free(NormalAxesCounts);
		NormalAxesCounts = NULL;
	}
	if(ImproperAxesCounts)
	{
		free(ImproperAxesCounts);
		ImproperAxesCounts = NULL;
	}
	if(SymmetryCode)
	{
		free(SymmetryCode);
		SymmetryCode = NULL;
	}
}
