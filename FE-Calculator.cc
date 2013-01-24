#include <iostream>
#include <fstream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

bool CheckForInputError(int argc) {
	if(argc < 4) {
		cout << "Error in input. " << endl 
		<< "FE-Calc InputForces InputOps OutputFile [NumFrames]" << endl;
		return 1;
	}
	else
		return 0;
}

int FindNumFrames(char* InputFile) {
    ifstream myfile(InputFile);    
    myfile.unsetf(ios_base::skipws);

    unsigned line_count = std::count(
        istream_iterator<char>(myfile),
        istream_iterator<char>(), '\n');

    return line_count;
}

double* ReadInput(char* InputFile, const int NumFrames){
	ifstream Fp(InputFile,ifstream::in);
	double *Vars = new double[NumFrames];

	for(int i = 0; i < NumFrames; i++)
		Fp >> Vars[i];

	return Vars;
}

void WriteOutput(char* OutputFile, double* Vars, const int NumFrames){
	ofstream Fp;
	Fp.setf(ios::scientific);
	Fp.precision(6);
	Fp.open(OutputFile);

	for(int i = 0; i < NumFrames; i++)
		Fp << Vars[i] << "\t" << i << endl;

	Fp.close();
}

double* ComputeFE(const double* Forces, const double* OPs, const int NumFrames) {
	double* FE = new double[NumFrames];
	FE[0] = .0;

	for (int i = 1; i < NumFrames; i++)
		FE[i] = FE[i-1] + 0.5 * (Forces[i] + Forces[i-1]) * (OPs[i] - OPs[i-1]);

	return FE;
}

int main(int argc, char** argv){
	if(CheckForInputError(argc))
		return 1;
	else {
		int Numframes;
		char *InputForces = argv[1], *InputOPs = argv[2], *OutputFile = argv[3];

		if(argc > 4)
			Numframes = atoi(argv[4]);
		else {
			cout << "Computing number of frames ... " << endl; 
			Numframes = FindNumFrames(InputForces);
			cout << "Number of frames is " << Numframes << endl; 
		}
		 
		double *Forces = ReadInput(InputForces,Numframes);
		double *OPs = ReadInput(InputOPs,Numframes);
		double *FreeEnergy = ComputeFE(Forces,OPs,Numframes);
		WriteOutput(OutputFile,FreeEnergy,Numframes);
	}

	return 0;
}