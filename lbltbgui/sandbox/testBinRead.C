{
	gSystem->Load("./lib/libTestGui");

	BinaryFileMonitor m("lib/libTestGui.dylib");
	m.Update();
	//std::vector<char> vbin = m.GetContent();
	//cout << vbin.size() << endl;

	TextFileMonitor tx("testBinRead.C");
	tx.Update();
	std::vector<std::string> v = tx.GetContent();
	cout << v[1] << endl;
	//cout << v.size() << endl;

}