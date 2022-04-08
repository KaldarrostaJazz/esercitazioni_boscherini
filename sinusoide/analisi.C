#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

bool conversion(std::string num) {
  try {
    std::stod(num);
    return true;
  } catch (const std::exception& e) {
    return false;
  }
}

double StdDev(std::string file) {
  std::vector<std::string> data;
  std::string line;
  std::ifstream in{file};
  while (std::getline(in, line, '\n')) {
    std::stringstream line_stream(line);
    std::string cell;
    while (std::getline(line_stream, cell, '\t')) data.push_back(cell);
    if (!line_stream && cell.empty()) data.push_back(" ");
  }
  in.close();

  std::vector<double> d_vector;
  int counter{0};
  for (int i = 0; i < data.size(); i++) {
    if (conversion(data[i]) == true) {
      double num = std::stod(data[i]);
      d_vector.push_back(num);
    } else {
      counter++;
      continue;
    }
  }

  int N = d_vector.size();
  double variance = 0.;
  double mean = std::accumulate(d_vector.begin(), d_vector.end(), 0.) / N;
  for (int i = 0; i < N; i++) {
    variance += (d_vector[i] - mean) * (d_vector[i] - mean);
  }
  std::cout << "Converted: " << N << "\tNot converted: " << counter << '\n';
  return std::sqrt(variance / (N - 1));
}

Double_t sinusoide_fondo(Double_t* x, Double_t* par) {
    Float_t xx = x[0];
    return par[0] * sin(par[1] * xx + par[2]) + par[3] * xx + par[4];
}

void analisi() {
    gROOT->SetStyle("Modern");
    gStyle->SetOptStat(0);
    //gStyle->SetPalette(kCool);

    double error = StdDev("rumore.TXT");
    TGraphErrors* graph = new TGraphErrors("sinusoide-fondo-costante.txt", "%lg %lg");
    int N = graph->GetN();
    for (int i = 0; i < N; i++) {
        graph->SetPointError(i, 0, error);
    }
    graph->SetTitle("Grafico dati;;Asse Y");
    graph->GetXaxis()->SetLabelSize(0);
    graph->GetYaxis()->SetTitleSize(0.04);
    graph->GetYaxis()->SetTitleOffset(0.9);
    graph->SetMarkerStyle(2);
    graph->SetMarkerColor(12);
    graph->SetLineColor(12);

    TF1* func = new TF1("func", "sinusoide_fondo", -100, 1000, 5);
    func->SetLineWidth(1);
    func->SetLineColor(46);
    func->SetParNames("Amp", "Omega", "Phi", "Slope", "Intercept");
    func->SetParameters(400, 0.02, 0.01, 0.1, 0.5);

    graph->Fit("func", "Q0");

    double x_vec[N];
    double y_vec[N];
    for (int i = 0; i < N; ++i) {
        x_vec[i] = graph->GetPointX(i);
        y_vec[i] = graph->GetPointY(i) - func->Eval(graph->GetPointX(i));
    }
    TGraph* residui = new TGraph(N, x_vec, y_vec);
    residui->SetLineColor(46);
    residui->SetLineWidth(1);
    residui->SetTitle(";Asse X;");
    residui->GetXaxis()->SetLabelSize(0.08);
    residui->GetXaxis()->SetTitleOffset(-0.8);
    residui->GetXaxis()->SetTitleSize(0.08);
    residui->GetYaxis()->SetLabelSize(0.08);
    residui->GetYaxis()->SetNdivisions(505);

    TCanvas* canva = new TCanvas("", "", 800, 600);
    canva->Divide(1, 2);
    canva->cd(1)->SetPad(0, 0, 1, 0.30);
    canva->cd(2)->SetPad(0, 0.30, 1, 1);
    canva->cd(1)->SetTopMargin(0.025);
    canva->cd(2)->SetBottomMargin(0.005);
    canva->cd(1)->SetGrid();
    residui->Draw("AL");
    canva->cd(2)->SetGrid();
    graph->Draw("APE");
    func->Draw("SAME");

    std::cout << std::setfill('=') << std::setw(76) << '\n';
    std::cout << std::setfill('.') << "DOF: " << std::setw(70) << func->GetNDF() << '\n';
    std::cout << std::setfill('.') << "Chi Square: " << std::setw(63) << func->GetChisquare() << '\n';
    std::cout << std::setfill('.') << "Reduced Chi Square: " << std::setw(55) << func->GetChisquare() / func->GetNDF() << '\n';
    std::cout << std::setfill('.') << "Probability: " << std::setw(62) << func->GetProb() << '\n';
    std::cout << std::setfill('-') << std::setw(76) << '\n';
    std::string p_tot[5];
    for (int i = 0; i < 5; i++) {
      std::string p = std::to_string(func->GetParameter(i)) + " +/- ";
      p_tot[i] = p + std::to_string(func->GetParError(i));
    }
    std::cout << std::setfill('.') << func->GetParName(0) << std::setw(72) << p_tot[0] << '\n';
    std::cout << std::setfill('.') << func->GetParName(1) << std::setw(70) << p_tot[1] << '\n';
    std::cout << std::setfill('.') << func->GetParName(2) << std::setw(72) << p_tot[2] << '\n';
    std::cout << std::setfill('.') << func->GetParName(3) << std::setw(70) << p_tot[3] << '\n';
    std::cout << std::setfill('.') << func->GetParName(4) << std::setw(66) << p_tot[4] << '\n';
    std::cout << std::setfill('=') << std::setw(76) << std::endl;
}