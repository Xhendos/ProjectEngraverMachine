class Plotter
{
public:
	Plotter();
	void moveTo(int x, int y, bool laser);
	void findLine();
	void loadStart();
	void loadLine();
	void run(bool laser);
	std::array<bool, 4> getOutputstepper(int nextStep);
	void setOutput(bool c1, bool c2, bool c3, bool c4, bool c5, bool c6, bool c7, bool c8, bool c9);
private:

};

