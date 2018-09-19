#include <iostream>
#include <cmath>
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TCanvas.h"
//#include "ROOT/TCanvas.hxx"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/VectorUtil.h"
R__LOAD_LIBRARY(libMathMore.so)
R__LOAD_LIBRARY(libGenVector.so)

#include "nlohmann/json.hpp"
#include "THcParmList.h"
R__LOAD_LIBRARY(libHallA.so)
R__LOAD_LIBRARY(libdc.so)
R__LOAD_LIBRARY(libHallC.so)

// fmt - string formatting library
#include "fmt/core.h"
#include "fmt/ostream.h"
R__LOAD_LIBRARY(libfmt.so)

void shms_hgc_npe_vs_p(){

  using namespace nlohmann;

  nlohmann::json j2;
  {
    ifstream  in_run_file("results/shms_cherenkov_hgc_npe.json");
    in_run_file >> j2;
  }

  std::vector<double> i_run;
  std::vector<double> run_numbers;
  std::vector<double> momenta;
  std::vector<double> theta;
  std::vector<double> ngc_momenta;
  std::vector<double> mean_npe;
  std::vector<double> sigma_npe;
  std::vector<double> ngc_mean_npe;
  std::vector<double> ngc_sigma_npe;
  std::vector<double> x_err;

  std::vector<std::vector<double>> hgc_mirror_mean_npe(4);
  std::vector<std::vector<double>> hgc_mirror_sigma_npe(4);
  std::cout << hgc_mirror_mean_npe.size() << "   size\n";

  int ir = 0;
  // special iterator member functions for objects
  for (json::iterator it = j2.begin(); it != j2.end(); ++it) {
    ir++;
    //std::cout << it.key() << " : " << it.value() << "\n";
    run_numbers.push_back(std::stof(it.key()));
    momenta.push_back(   it.value()["ppcentral"].get<double>() );
    theta.push_back(   it.value()["ptheta_lab"].get<double>() );
    ngc_momenta.push_back(   it.value()["ppcentral"].get<double>() +0.05 );
    mean_npe.push_back(  it.value()["hgc"]["sum_mean"].get<double>() );
    sigma_npe.push_back( it.value()["hgc"]["sum_sigma"].get<double>() );
    ngc_mean_npe.push_back(  it.value()["ngc_mu"].get<double>() );
    ngc_sigma_npe.push_back( it.value()["ngc_sigma"].get<double>() );
    std::cout << it.value()["ppcentral"].get<double>() 
    << ", " << it.value()["mu"].get<double>() 
    << ", " << it.value()["sigma"].get<double>()  << std::endl;
    i_run.push_back(     double(ir) );
    x_err.push_back( 0.0 );

    //mean_npe.push_back(  it.value()["mu"].get<double>() );
    std::vector<double> temp_means = it.value()["hgc"]["mean"];
    std::vector<double> temp_sigma = it.value()["hgc"]["sigma"];
    for(int imir = 0; imir< 4; imir++) {
      if(temp_means[imir]<8) {
        temp_means[imir] = 10;
      }
      hgc_mirror_mean_npe[imir].push_back(temp_means[imir]);
      hgc_mirror_sigma_npe[imir].push_back(temp_sigma[imir]);
    }

  }


  TCanvas* c = nullptr;
  TMultiGraph* mg = nullptr;

  // -------------------------------------
  // Vs momentum
  c = new TCanvas();
  mg = new TMultiGraph();

  TGraph* gr = new TGraph(run_numbers.size(),momenta.data(), mean_npe.data());
  //TGraphErrors* gr = new TGraphErrors(run_numbers.size(),momenta.data(), mean_npe.data(), x_err.data(), sigma_npe.data());
  gr->SetMarkerStyle(20);
  gr->SetLineWidth(1);
  gr->SetLineColor(1);
  gr->SetMarkerColor(1);
  //gr->SetFillColor(2);
  //gr->SetFillStyle(3001);
  //gr->Sort();
  gr->Fit("pol3");
  TF1* func = (TF1*)gr->GetListOfFunctions()->At(0);
  if(func){
    func->SetLineStyle(2);
    func->SetLineWidth(3);
    func->SetLineColor(4);
  }
  mg->Add(gr,"p");

  //TGraphErrors* gr2 = new TGraphErrors(run_numbers.size(),ngc_momenta.data(), ngc_mean_npe.data(), x_err.data(), ngc_sigma_npe.data());
  TGraph* gr2 = new TGraph(run_numbers.size(),momenta.data(), ngc_mean_npe.data());
  gr2->SetMarkerStyle(21);
  gr2->SetLineWidth(1);
  gr2->SetLineColor(2);
  gr2->SetMarkerColor(2);
  //gr2->SetFillColor(1);
  //gr2->SetFillStyle(3001);
  gr2->Sort();
  gr2->Fit("pol3");
  func = (TF1*)gr2->GetListOfFunctions()->At(0);
  if(func){
    func->SetLineStyle(2);
    func->SetLineWidth(3);
    func->SetLineColor(4);
  }

  gr->SetTitle("HGC");
  gr2->SetTitle("NGC");

  mg->Add(gr2,"p");

  mg->SetTitle(" ; SHMS P_{0} [GeV/c] ; photo-electrons");
  mg->Draw("a");
  mg->GetYaxis()->SetRangeUser(0.0,25.0);
  mg->GetXaxis()->CenterTitle(true);
  mg->GetYaxis()->CenterTitle(true);
  c->BuildLegend();

  c->SaveAs("results/shms_hgc_npe_vs_p.png");
  c->SaveAs("results/shms_hgc_npe_vs_p.pdf");

  // -------------------------------------
  // Vs theta
  c = new TCanvas();
  mg = new TMultiGraph();

  gr = new TGraph(run_numbers.size(),theta.data(), mean_npe.data());
  //TGraphErrors* gr = new TGraphErrors(run_numbers.size(),momenta.data(), mean_npe.data(), x_err.data(), sigma_npe.data());
  gr->SetMarkerStyle(20);
  gr->SetLineWidth(1);
  gr->SetLineColor(1);
  gr->SetMarkerColor(1);
  //gr->SetFillColor(2);
  //gr->SetFillStyle(3001);
  //gr->Sort();
  gr->Fit("pol3");
  func = (TF1*)gr->GetListOfFunctions()->At(0);
  if(func){
    func->SetLineStyle(2);
    func->SetLineWidth(3);
    func->SetLineColor(4);
  }
  mg->Add(gr,"p");

  gr2 = new TGraph(run_numbers.size(),theta.data(), ngc_mean_npe.data());
  gr2->SetMarkerStyle(21);
  gr2->SetLineWidth(1);
  gr2->SetLineColor(2);
  gr2->SetMarkerColor(2);
  //gr2->SetFillColor(1);
  //gr2->SetFillStyle(3001);
  gr2->Sort();
  gr2->Fit("pol3");
  func = (TF1*)gr2->GetListOfFunctions()->At(0);
  if(func){
    func->SetLineStyle(2);
    func->SetLineWidth(3);
    func->SetLineColor(4);
  }

  gr->SetTitle("HGC");
  gr2->SetTitle("NGC");

  mg->Add(gr2,"p");

  mg->SetTitle(" ; SHMS #theta_{0} [degrees] ; photo-electrons");
  mg->Draw("a");
  mg->GetYaxis()->SetRangeUser(0.0,25.0);
  mg->GetXaxis()->CenterTitle(true);
  mg->GetYaxis()->CenterTitle(true);
  c->BuildLegend();

  c->SaveAs("results/shms_hgc_npe_vs_theta.png");
  c->SaveAs("results/shms_hgc_npe_vs_theta.pdf");

  // -------------------------------------
  // HGC Mirrors 
  std::vector<int> colors = {1,2,4,8,6,7};
  c = new TCanvas();
  TMultiGraph* mg_hgc_mirrors = new TMultiGraph();
  for(int imir = 0; imir<4 ;imir++) {
    auto yvals = hgc_mirror_mean_npe[imir];
    //std::cout << "derp" << icol <<  " \n";
    TGraph* a_gr = new TGraph(run_numbers.size(),momenta.data(), yvals.data());
    int a_col = colors[imir];
    a_gr->SetMarkerStyle(20);
    a_gr->SetLineWidth(1);
    a_gr->SetLineColor(a_col);
    a_gr->SetMarkerColor(a_col);
    a_gr->SetTitle((std::string("pmt ") + std::to_string(imir+1)).c_str());
    a_gr->Sort();
    a_gr->Fit("pol3");
    auto ffun = (TF1*)a_gr->GetListOfFunctions()->At(0);
    std::cout << "derp\n";
    if(func){
      ffun->SetLineStyle(2);
      ffun->SetLineWidth(3);
      ffun->SetLineColor(a_col);
    }
    mg_hgc_mirrors->Add(a_gr,"p");
  }

  mg_hgc_mirrors->SetTitle(" ; SHMS P_{0} [GeV/c] ; photo-electrons");
  mg_hgc_mirrors->Draw("a");
  mg_hgc_mirrors->GetYaxis()->SetRangeUser(0.0,25.0);
  mg_hgc_mirrors->GetXaxis()->CenterTitle(true);
  mg_hgc_mirrors->GetYaxis()->CenterTitle(true);
  c->BuildLegend();

  c->SaveAs("results/shms_hgc_mirrors_vs_p.png");
  c->SaveAs("results/shms_hgc_mirrors_vs_p.pdf");

  // -------------------------------------
  // HGC Mirrors 
  c = new TCanvas();
  mg_hgc_mirrors = new TMultiGraph();
  for(int imir = 0; imir<4 ;imir++) {
    auto yvals = hgc_mirror_mean_npe[imir];
    //std::cout << "derp" << icol <<  " \n";
    TGraph* a_gr = new TGraph(run_numbers.size(),theta.data(), yvals.data());
    int a_col = colors[imir];
    a_gr->SetMarkerStyle(20);
    a_gr->SetLineWidth(1);
    a_gr->SetLineColor(a_col);
    a_gr->SetMarkerColor(a_col);
    a_gr->SetTitle((std::string("pmt ") + std::to_string(imir+1)).c_str());
    a_gr->Sort();
    a_gr->Fit("pol3");
    auto ffun = (TF1*)a_gr->GetListOfFunctions()->At(0);
    std::cout << "derp\n";
    if(func){
      ffun->SetLineStyle(2);
      ffun->SetLineWidth(3);
      ffun->SetLineColor(a_col);
    }
    mg_hgc_mirrors->Add(a_gr,"p");
  }

  mg_hgc_mirrors->SetTitle(" ; SHMS #theta_{0} [degrees] ; photo-electrons");
  mg_hgc_mirrors->Draw("a");
  mg_hgc_mirrors->GetYaxis()->SetRangeUser(0.0,25.0);
  mg_hgc_mirrors->GetXaxis()->CenterTitle(true);
  mg_hgc_mirrors->GetYaxis()->CenterTitle(true);
  c->BuildLegend();

  c->SaveAs("results/shms_hgc_mirrors_vs_theta.png");
  c->SaveAs("results/shms_hgc_mirrors_vs_theta.pdf");


}
