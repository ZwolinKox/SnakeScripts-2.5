
define const NULL = 0
define const true = 0
define const false = 1

class self
{
public:

	virtual def missing
	{
		throw "Error! Proba wywolania nieistniejacej metody!"
	}
	
};