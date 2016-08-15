{
	//TBits (UInt_t nbits=8)
	TBits b(32);
	cout << b << endl;

	Int_t a[] = { 0, 5 };
	b.Set(2, a);
	cout << b << endl;

	b.Set(32, "A2zf");
	cout << b << endl;
	b.Set(32, "0xffffffff");
	cout << b << endl;
	b.Set(32, "0000000000000000");
	cout << b << endl;
}