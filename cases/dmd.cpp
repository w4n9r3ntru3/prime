/***********************************************************************

  FileName    [main.cpp]

  Author      [Yang Chien Yi]

  The main function.

***********************************************************************/

#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    // start
    if (argc != 2) {
        std::cout << "Usage: ./dmd <dmd.txt>"
                  << std::endl;
        return 0;
    }

    // open files
    std::fstream inputfile(argv[1]);
    if (!inputfile.is_open()) {
        std::cout << "No such file or directory: " << argv[1] << std::endl;
        return 0;
    }
    std::fstream outputfile;
    outputfile.open("dmd.txt", std::ios::out);
    if (!outputfile.is_open()) {
        std::cout << "No such file or directory: " << argv[2] << std::endl;
        return 0;
    }

    std::string str;
    for (unsigned i = 0; i < 5; ++i) inputfile >> str;
    int supply, demand;
    while(inputfile >> str)
    {
        inputfile >> str;
        inputfile >> str;
        inputfile >> supply >> demand;
        outputfile << supply - demand << '\n';
    }

    // Test QuadForest functionality
    //QuadForest qf(Mgr);
    //std::cout << qf.size() << std::endl;
    //std::cout << qf << std::endl;
    // Mgr.output(outputfile);
    return 0;
}
