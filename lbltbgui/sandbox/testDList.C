{
	gSystem->Load("./lib/libTestGui");

	h = new TH1F("H", "H", 100, 0, 1);
	h->FillRandom("gaus", 1000);
	//DObject obj(h, kTRUE, "+l1 +k2 +m20 +w2 +s10 noleg +a50");
	//obj->GetObject()->Draw("LP");

	DList *hl = new DList("hl","hl");
	hl->Add(h, "hist p +l1 +k2 +m2 +s10 +a50");
	h->Scale(3.);
	hl->Add(h, "p l +l1 +k3 +m24 +s10 +a50", "scaled");
	hl->Draw("");
	hl->Legend("");
}