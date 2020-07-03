/***********************************************************************

  FileName    [main.cpp]

  Author      [Yang Chien Yi]

  The main function.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "include/Chip.h"
#include "include/QuadForest.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                             MAIN                                 ///
////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    // start
    if (argc != 3) {
        std::cout << "Usage: ./cell_move_router <input.txt> <output.txt>"
                  << std::endl;
        return 0;
    }

    std::cout << argv[0] << " " << argv[1] << " " << argv[2] << std::endl;

    // open files
    std::fstream inputfile(argv[1]);
    if (!inputfile.is_open()) {
        std::cout << "No such file or directory: " << argv[1] << std::endl;
        return 0;
    }
    std::fstream outputfile;
    outputfile.open(argv[2], std::ios::out);
    if (!outputfile.is_open()) {
        std::cout << "No such file or directory: " << argv[2] << std::endl;
        return 0;
    }

    // call solver
    Chip Mgr(inputfile);

    Mgr.log();

    // Test QuadForest functionality
    QuadForest qf(Mgr);
    std::cout << qf.size() << std::endl;
    std::cout << qf << std::endl;
    // Mgr.output(outputfile);
    return 0;
}
