// ***************************************************************
// This file was created using the bat-project script
// for project expo_fit.
// bat-project is part of Bayesian Analysis Toolkit (BAT).
// BAT can be downloaded from http://mpp.mpg.de/bat
// ***************************************************************

#include <BAT/BCLog.h>
#include <sys/stat.h>
#include "expo_fit.h"

int main(int argc, char**argv)
{
    // open log file
    BCLog::OpenLog("log.txt", BCLog::detail, BCLog::detail);
    std::string inputFileName = "";
    std::string outputDir= "";
    bool verbose = false;

    // Parse command-line arguments using getopt
    int opt;
    while ((opt = getopt(argc, argv, "i:o:h")) != -1) {
        switch (opt) {
            case 'i':
                inputFileName = optarg;
                break;
            case 'o':
                outputDir = optarg;
                break;
	     
            default:
                std::cerr << "Usage: " << argv[0] << " -i <inputfile> -o <outputDir> " << std::endl;
                return 1;
        }
    }
    // create new expo_fit object
    expo_fit m(outputDir,inputFileName);

    int status = mkdir(("outputs/"+outputDir).c_str(), 0777); 
    // set precision
    m.SetPrecision(BCEngineMCMC::kHigh);

    BCLog::OutSummary("Test model created");

    //////////////////////////////
    // perform your analysis here

     m.WriteMarkovChain("outputs/"+m.GetSafeName() + "/mcmc.root", "RECREATE");
     m.SetKnowledgeUpdateDrawingStyle(BCAux::kKnowledgeUpdateDetailedPosterior);
     m.PrintKnowledgeUpdatePlots("outputs/"+m.GetSafeName() +"/know-update.pdf");

     // run MCMC, marginalizing posterior
    m.MarginalizeAll(BCIntegrate::kMargMetropolis);

    // run mode finding; by default using Minuit
    m.FindMode(m.GetBestFitParameters());

    // draw all marginalized distributions into a PDF file
    m.PrintAllMarginalized("outputs/"+m.GetSafeName() + "/plots.pdf");
    m.WriteResultsTree("outputs/"+m.GetSafeName()+"/analysis.root");
   
    // print results of the analysis into a text file
    m.PrintSummary();

    // close log file
    BCLog::OutSummary("Exiting");
    BCLog::CloseLog();

    return 0;
}
