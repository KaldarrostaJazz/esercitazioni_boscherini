double error(std::string file) {
  double stdDev;
  std::vector<double> err;
  std::ifstream in;
  in.open(file);
  while (in.good()) {
    double a;
    in >> a;
    err.push_back(a);
  }
  in.close();
  double mean = std::accumulate(err.begin(), err.end(), 0.) / err.size();
  int N = err.size();
  for (int i = 0; i < N; i++) {
    stdDev += (err[i] - mean) * (err[i] - mean);
  }
  return std::sqrt(stdDev / (N - 1));
}

void analisi() {
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(0);

  double err = error("rumore.txt");
  TGraphErrors* graph = new TGraphErrors("duegaussiane.txt", "%lg %lg");
  int N = graph->GetN();
  for (int i = 0; i < N; i++) {
    graph->SetPointError(i, 0., err);
  }

  graph->SetTitle("Grafico dati;; Asse Y");
  graph->SetMarkerStyle(2);
  graph->SetMarkerColor(kAzure + 3);
  graph->SetLineColor(kAzure + 2);
  graph->GetXaxis()->SetLabelSize(0);

  TF1* gaus1 = new TF1("g1", "gaus", 20, 26);
  TF1* gaus2 = new TF1("g2", "gaus", 26, 30);
  TF1* total = new TF1("gtotal", "gaus(0) + gaus(3)", 20, 30);
  total->SetLineWidth(1);
  total->SetLineColor(kRed + 2);

  graph->Fit("g1", "Q0R");
  graph->Fit("g2", "Q0R+");
  double par[6];
  gaus1->GetParameters(&par[0]);
  gaus2->GetParameters(&par[3]);
  total->SetParameters(par);
  total->SetParNames("Costant_1", "Mean_1", "Sigma_1", "Costant_2", "Mean_2", "Sigma_2");
  graph->Fit("gtotal", "R+", "SAME");

  double x[N];
  double y[N];
  for (int i = 0; i < N; i++) {
    x[i] = graph->GetPointX(i);
    y[i] = graph->GetPointY(i) - total->Eval(graph->GetPointX(i));
  }
  TGraph* residui = new TGraph(N, x, y);
  residui->SetTitle(";Asse X;");
  residui->SetLineColor(kAzure + 2);
  residui->GetXaxis()->SetTitleSize(0.08);
  residui->GetXaxis()->SetTitleOffset(0.7);
  residui->GetYaxis()->SetNdivisions(510);
  residui->GetXaxis()->SetLabelSize(0.08);
  residui->GetXaxis()->SetLabelOffset(0.04);
  residui->GetYaxis()->SetLabelSize(0.08);

  TCanvas* c = new TCanvas("", "", 800, 600);
  c->Divide(1, 2);
  c->cd(1)->SetPad(0, 0.30, 1, 1);
  c->cd(2)->SetPad(0, 0, 1, 0.30);
  c->cd(1)->SetBottomMargin(0.02);
  c->cd(2)->SetTopMargin(-0);
  c->cd(2)->SetBottomMargin(0.18);
  c->cd(1)->SetGrid();

  TLegend* leg = new TLegend(0.7, 0.75, 0.89, 0.88);
  leg->AddEntry(graph, "Dati", "pe");
  leg->AddEntry(total, "Fit", "l");

  graph->Draw("APE");
  leg->Draw("SAME");
  c->cd(2)->SetGrid();
  residui->Draw("ALE");

  std::cout << std::setfill('=') << std::setw(76) << '\n';
  std::cout << std::setfill('.') << "DOF: " << std::setw(70) << total->GetNDF()
            << '\n';
  std::cout << std::setfill('.') << "Chi Square: " << std::setw(63)
            << total->GetChisquare() << '\n';
  std::cout << std::setfill('.') << "Reduced Chi Square: " << std::setw(55)
            << total->GetChisquare() / total->GetNDF() << '\n';
  std::cout << std::setfill('.') << "Probability: " << std::setw(62)
            << total->GetProb() << '\n';
  std::cout << std::setfill('=') << std::setw(76) << std::endl;
}
