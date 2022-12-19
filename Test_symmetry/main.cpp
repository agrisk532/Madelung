#include <stdlib.h>
#include <string.h>
#include "Symmetry.h"

int main( int argc, char **argv )
{
	char *program = *argv ;
    FILE          *in ;
	CSymmetry sym;
/*
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
*/
if(argc == 1){
    exit( EXIT_FAILURE ) ;
}
if( sym.read_coordinates( argv[1] ) < 0 ){
    fprintf( stderr, "Error reading in atomic coordinates\n" ) ;
    exit( EXIT_FAILURE ) ;
    }
sym.find_symmetry_elements() ;
sym.sort_symmetry_elements() ;
sym.summarize_symmetry_elements() ;
//if( BadOptimization )
//    printf( "Refinement of some symmetry elements was terminated before convergence was reached.\n"
//            "Some symmetry elements may remain unidentified.\n" ) ;
//if( verbose >= 0 )
//    sym.report_symmetry_elements_verbose() ;
sym.report_symmetry_elements_brief() ;
sym.identify_point_group() ;
exit( EXIT_SUCCESS ) ;
return 0;
}
