// ***************************************************************
// This file was created using the bat-project script.
// bat-project is part of Bayesian Analysis Toolkit (BAT).
// BAT can be downloaded from http://mpp.mpg.de/bat
// ***************************************************************

#ifndef __BAT__EXPO_FIT__H
#define __BAT__EXPO_FIT__H

#include <BAT/BCModel.h>

#include <string>
#include <vector>
#include "TH1D.h"
#include "TFile.h"
// This is a expo_fit header file.
// Model source code is located in file expo_fit/expo_fit.cxx

// ---------------------------------------------------------
class expo_fit : public BCModel
{

public:

    // Constructor
  expo_fit(const std::string& name,std::string input_file);
  void WriteResultsTree(std::string filename);

  TH1D * counts;
  TH1D *mass;
    // Destructor
    ~expo_fit();

    // Overload LogLikelihood to implement model
    double LogLikelihood(const std::vector<double>& pars);

    // Overload LogAprioriProbability if not using built-in 1D priors
    // double LogAPrioriProbability(const std::vector<double> & pars);

    // Overload CalculateObservables if using observables
    // void CalculateObservables(const std::vector<double> & pars);

};
// ---------------------------------------------------------

#endif
