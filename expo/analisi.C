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

Double_t expo_f(Double_t* x, Double_t* par) {
    Float_t xx = x[0];
    return par[0] * exp(- xx / par[1]) + par[2] * xx + par[3];
}

void analisi() {
    gROOT->SetStyle("Modern");
    gStyle->SetOptFit(0);
    double error = StdDev("rumore.txt");
    TGraphErrors* graph = new TGraphErrors("esponenziale-fondo.txt", "%lg %lg");
    graph->SetTitle("Grafico;;Asse Y");
    graph->GetYaxis()->SetLabelSize(0.04);
    graph->GetYaxis()->SetTitleSize(0.04);
    graph->GetXaxis()->SetLabelSize(0);
    graph->SetMarkerStyle(2);
    graph->SetMarkerColor(kGray + 3);
    graph->SetLineColor(kGray + 3);
    
    int N = graph->GetN();
    for (int i = 0; i < N; i++) {
        graph->SetPointError(i, 0, error);
    }
    double min_x = graph->GetPointX(0);
    double max_x = graph->GetPointX(N -1);

    TF1* func = new TF1("func", "expo_f", min_x - 0.1, max_x + 0.1, 4);
    func->SetLineColor(kOrange - 4);
    func->SetLineWidth(1);
    func->SetParNames("Costant", "Tau", "Slope", "Intercept");
    func->SetParameters(0.13, 1.11, 0.001, 3.2);

    graph->Fit("func", "Q0");

    double x_vec[N];
    double y_vec[N];
    for (int i = 0; i < N; i++) {
      x_vec[i] = graph->GetPointX(i);
      y_vec[i] = graph->GetPointY(i) - func->Eval(graph->GetPointX(i));
    }
    TGraph* res = new TGraph(N, x_vec, y_vec);
    res->SetTitle(";Asse X;");
    res->GetXaxis()->SetLabelSize(0.11);
    res->GetXaxis()->SetLabelOffset(0.05);
    res->GetXaxis()->SetTitleSize(0.11);
    res->GetXaxis()->SetTitleOffset(-0.8);
    res->GetYaxis()->SetLabelSize(0.11);
    res->SetLineColor(kOrange - 4);

    TCanvas* c = new TCanvas("", "" , 800, 600);
    c->Divide(1, 2);
    c->cd(1)->SetPad(0, 0.25, 1, 1);
    c->cd(1)->SetBottomMargin(0.01);
    c->cd(2)->SetPad(0, 0, 1, 0.25);
    c->cd(2)->SetTopMargin(0);
    c->cd(2)->SetBottomMargin(0.2);
    c->cd(1)->SetGrid();
    graph->Draw("APE");
    func->Draw("SAME");
    c->cd(2)->SetGrid();
    res->Draw("ALE");

    std::cout << std::setfill('=') << std::setw(76) << '\n';
    std::cout << std::setfill('.') << "DOF: " << std::setw(70) << func->GetNDF() << '\n';
    std::cout << std::setfill('.') << "Chi Square: " << std::setw(63) << func->GetChisquare() << '\n';
    std::cout << std::setfill('.') << "Reduced Chi Square: " << std::setw(55) << func->GetChisquare() / func->GetNDF() << '\n';
    std::cout << std::setfill('.') << "Probability: " << std::setw(62) << func->GetProb() << '\n';
    std::cout << std::setfill('-') << std::setw(76) << '\n';
    std::string p_tot[4];
    for (int i = 0; i < 4; i++) {
      std::string p = std::to_string(func->GetParameter(i)) + " +/- ";
      p_tot[i] = p + std::to_string(func->GetParError(i));
    }
    std::cout << std::setfill('.') << func->GetParName(0) << std::setw(68) << p_tot[0] << '\n';
    std::cout << std::setfill('.') << func->GetParName(1) << std::setw(72) << p_tot[1] << '\n';
    std::cout << std::setfill('.') << func->GetParName(2) << std::setw(70) << p_tot[2] << '\n';
    std::cout << std::setfill('.') << func->GetParName(3) << std::setw(66) << p_tot[3] << '\n';
    std::cout << std::setfill('=') << std::setw(76) << std::endl;
}