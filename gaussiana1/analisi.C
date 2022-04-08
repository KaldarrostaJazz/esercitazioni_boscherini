
void shade(TVirtualPad* c1, TF1* f1, TF1* f2, TF1* f3) {
  TGraph* gr = new TGraph();
  gr->SetFillColor(f1->GetFillColor());
  gr->SetFillStyle(f1->GetFillStyle());
  f3->Draw("lsame");
  c1->Update();
  Double_t xmin = c1->GetUxmin();
  Double_t xmax = c1->GetUxmax();
  Double_t ymin = c1->GetUymin();
  Double_t ymax = c1->GetUymax();

  Int_t npx = f3->GetNpx();
  Int_t npoints = 0;
  Double_t dx = (xmax - xmin) / npx;
  Double_t x = xmin + 0.5 * dx;
  while (x <= xmax) {
    Double_t y = f1->Eval(x);
    if (y < ymin) y = ymin;
    if (y > ymax) y = ymax;
    gr->SetPoint(npoints, x, y);
    npoints++;
    x += dx;
  }
  x = xmax - 0.5 * dx;
  while (x >= xmin) {
    Double_t y = f2->Eval(x);
    if (y < ymin) y = ymin;
    if (y > ymax) y = ymax;
    gr->SetPoint(npoints, x, y);
    npoints++;
    x -= dx;
  }
  gr->Draw("fsame");  // draw graph with fill area option
  f3->Draw("lsame");  // superimpose function
}

void analisi() {
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(1111);

  TGraphErrors* graph = new TGraphErrors("./gaussiana1.txt", "%lg %lg %lg");
  graph->SetTitle("Grafico Dati;; AsseY");
  graph->GetXaxis()->SetLabelSize(0);
  graph->GetYaxis()->SetTitleSize(0.05);
  graph->GetYaxis()->SetTitleOffset(0.8);
  graph->SetMarkerStyle(kOpenCircle);
  graph->SetMarkerSize(0.8);
  graph->SetMarkerColor(kBlack);
  graph->SetLineColor(kBlack);

  TFitResultPtr r = graph->Fit("gaus", "S");
  TF1* func = graph->GetFunction("gaus");
  func->SetLineColor(kRed);
  func->SetRange(-5, 5);
  func->SetLineWidth(1);
  TMatrixD cov = r->GetCovarianceMatrix();

  auto N = graph->GetN();
  double x_vec[N];
  double y_vec[N];

  for (int i = 0; i != N; i++) {
    x_vec[i] = graph->GetPointX(i);
    y_vec[i] = graph->GetPointY(i) - func->Eval(graph->GetPointX(i));
  }

  TGraph* res = new TGraph(N, x_vec, y_vec);
  res->SetTitle(";Asse X;");
  res->GetXaxis()->SetTitleSize(0.08);
  res->GetXaxis()->SetTitleOffset(0.5);
  res->GetXaxis()->SetLabelSize(0.08);
  res->GetYaxis()->SetLabelSize(0.08);
  res->SetMarkerStyle(kOpenCircle);
  res->SetMarkerSize(0.8);
  res->SetMarkerColor(kBlack);
  res->SetLineColor(kBlack);

  TF1* zero = new TF1("zero", "0", -10, 10);
  zero->SetLineColor(kRed);
  zero->SetLineStyle(kDashed);
  TF1* s_up = new TF1("s_up", "0.045", -10, 10);
  s_up->SetFillColor(46);
  s_up->SetFillStyle(3002);
  TF1* s_down = new TF1("s_down", "-0.045", -10, 10);

  TCanvas* canva1 = new TCanvas("", "", 800, 600);
  canva1->Divide(1, 2);
  canva1->cd(1)->SetPad(0, 0.35, 1, 1);
  canva1->cd(2)->SetPad(0., 0., 1, 0.35);
  canva1->cd(1)->SetBottomMargin(0.015);
  canva1->cd(2)->SetTopMargin(-0.);
  canva1->cd(2)->SetBottomMargin(0.2);
  canva1->cd(1)->SetGrid();
  canva1->cd(2)->SetGrid();
  canva1->cd(1);
  graph->Draw("APE");
  func->Draw("SAME");
  canva1->cd(2);
  res->Draw("APL");
  shade(canva1->cd(2), s_up, s_down, zero);
}
