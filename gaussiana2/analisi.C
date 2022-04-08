void analisi() {
    gROOT->SetStyle("Modern");
    gStyle->SetOptFit(1111);
    
    TGraphErrors* graph = new TGraphErrors("gaussiana2.txt", "%lg %lg %lg");
    graph->SetTitle("Grafico dati;; Asse Y");
    graph->GetXaxis()->SetLabelSize(0);
    graph->GetYaxis()->SetTitleOffset(0.8);
    graph->GetYaxis()->SetTitleSize(0.045);
    graph->SetMarkerColor(kCyan + 2);
    graph->SetMarkerStyle(kFullSquare);
    graph->SetMarkerSize(0.8);
    graph->SetLineColor(kCyan + 2);
    graph->Fit("gaus");

    TF1* func = graph->GetFunction("gaus");
    func->SetRange(-5, 5);
    func->SetLineColor(kRed - 3);
    func->SetLineWidth(2);

    int N = graph->GetN();
    double x_vec[N];
    double y_vec[N];
    for (int i = 0; i < N; ++i) {
        x_vec[i] = graph->GetPointX(i);
        y_vec[i] = graph->GetPointY(i) - func->Eval(graph->GetPointX(i));
    }
    TF1* zero = new TF1("zero", "0", -10, 10);
    zero->SetLineStyle(kDashed);
    zero->SetLineColor(kRed - 3);
    zero->SetLineWidth(2);
    TGraph* residui = new TGraph(N, x_vec, y_vec);
    residui->SetTitle(";Asse X;");
    residui->GetXaxis()->SetTitleSize(0.08);
    residui->GetXaxis()->SetTitleOffset(0.7);
    residui->GetYaxis()->SetNdivisions(5);
    residui->GetXaxis()->SetLabelSize(0.08);
    residui->GetXaxis()->SetLabelOffset(0.04);
    residui->GetYaxis()->SetLabelSize(0.08);
    residui->SetLineColor(kCyan + 2);

    TCanvas* c = new TCanvas("", "", 800, 600);
    c->Divide(1, 2);
    c->cd(1)->SetPad(0, 0.35, 1, 1);
    c->cd(2)->SetPad(0, 0, 1, 0.35);
    c->cd(1)->SetBottomMargin(0.02);
    c->cd(2)->SetTopMargin(- 0);
    c->cd(2)->SetBottomMargin(0.18);
    c->cd(1)->SetGrid();
    graph->Draw("APLE");
    func->Draw("SAME");
    c->cd(2)->SetGrid();
    residui->Draw("AL");
    zero->Draw("SAME");
}
