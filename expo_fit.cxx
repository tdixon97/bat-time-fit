// ***************************************************************
// This file was created using the bat-project script.
// bat-project is part of Bayesian Analysis Toolkit (BAT).
// BAT can be downloaded from http://mpp.mpg.de/bat
// ***************************************************************

#include "expo_fit.h"

#include <BAT/BCMath.h>
#include "TMath.h"
#include "TTree.h"
// ---------------------------------------------------------
expo_fit::expo_fit(const std::string& name,std::string input_file)
    : BCModel(name)
{


  /// ***** ADD PARAMETERS *********
  
  AddParameter("A", 0,.1, "A", "[counts/kg/day]");
  AddParameter("B", 0,.1, "B", "[counts/kg/day]");
  AddParameter("T", 0,2000, "T_{1/2}", "[day]");
  GetParameters().SetPriorConstantAll();
  GetParameters().SetNBins(300);

  /// ***** GET HISTOS  ****************

  TFile  * f = new TFile(input_file.c_str());

  counts = (TH1D*)f->Get("counts");
  mass = (TH1D*)f->Get("mass");

    
  
}

// ---------------------------------------------------------
expo_fit::~expo_fit()
{
    // destructor
}
void expo_fit::WriteResultsTree(std::string filename) {
    BCLog::OutSummary("Writing Parameters to output Tree in " + filename);
    TFile tf(filename.c_str(), "recreate");
    // define parameters                                                                                                                                                             
    std::string par_name;
    double marg_mode;
    double marg_qt16, marg_qt84, marg_qt90;
    double glob_mode, glob_mode_error;
 
    TTree tt("fit_par_results", "Results of the fitting procedure");
    tt.Branch("par_name",          &par_name);
    tt.Branch("marg_mode",         &marg_mode,       "marg_mode/D");
    tt.Branch("marg_quantile_16",  &marg_qt16,       "marg_quantile_16/D");
    tt.Branch("marg_quantile_84",  &marg_qt84,       "marg_quantile_84/D");
    tt.Branch("marg_quantile_90",  &marg_qt90,       "marg_quantile_90/D");
    tt.Branch("glob_mode",         &glob_mode,       "glob_mode/D");
    tt.Branch("glob_mode_error",   &glob_mode_error, "glob_mode_error/D");

    for (unsigned int p = 0; p < this->GetNVariables(); p++) {

      bool isfixed = (p < this->GetNParameters() and this->GetParameter(p).Fixed());
        par_name = std::string(this->GetVariable(p).GetName().data());
        if (!isfixed) {
            auto bch_marg = this->GetMarginalized(p);
            marg_mode = bch_marg.GetLocalMode();
            marg_qt16 = bch_marg.GetQuantile(0.16);
            marg_qt84 = bch_marg.GetQuantile(0.84);
            marg_qt90 = bch_marg.GetQuantile(0.90);
            glob_mode = p < this->GetNParameters() ? this->GetBestFitParameters()[p] : 0;
            glob_mode_error = p < this->GetNParameters() ? this->GetBestFitParameterErrors()[p] : 0;
        }
        else {
            marg_mode = this->GetParameter(p).GetFixedValue();
            marg_qt16 = 0;
            marg_qt84 = 0;
            marg_qt90 = 0;
            glob_mode = this->GetParameter(p).GetFixedValue();
            glob_mode_error = 0;
        }
        tt.Fill();
    }
    tt.Write();
}

// ---------------------------------------------------------
double expo_fit::LogLikelihood(const std::vector<double>& pars)
{
  double logL=0;
  double A=pars[0];
  double B=pars[1];
  double tau =pars[2]/TMath::Log(2);

  for (int b = 1; b <= counts->GetNbinsX(); ++b) {


    if (mass->GetBinContent(b)==0)
      {
	continue;
      }
    double t1=mass->GetBinLowEdge(b);
    double t2=mass->GetBinLowEdge(b)+mass->GetBinWidth(b);
    double dt=t2-t1;
    double pred = mass->GetBinContent(b)*(B*dt-A*tau*(TMath::Exp(-t2/tau)-TMath::Exp(-t1/tau)));

    logL += BCMath::LogPoisson(counts->GetBinContent(b), pred);
  }
  
  return logL;
}
