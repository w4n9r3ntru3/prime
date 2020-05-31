/***********************************************************************

  FileName    [main.cpp]

  Author      [Yang, Chien Yi]

  The main function.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "primeMan.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                             MAIN                                 ///
////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    // strart
    if (argc != 3) { std::cout << "Usage: ./cell_move_router <input.txt> <output.txt>" << std::endl; return 0; }
    
    // open files
    std::fstream inputfile(argv[1]);
    if(!inputfile.is_open()) { std::cout << "No such file or directory: " << argv[1] << std::endl; return 0; }
    std::fstream outputfile;
    outputfile.open(argv[2],std::ios::out);
    if(!outputfile.is_open()) { std::cout << "No such file or directory: " << argv[2] << std::endl; return 0; }
    
    //call solver
    PrimeMan Mgr = PrimeMan(inputfile);
    return 0;
}
