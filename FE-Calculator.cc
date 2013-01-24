#include <iostream>
#include <fstream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <iomanip>

using namespace std;

bool CheckForInputError(int argc) {
	if(argc < 7) {
		cout << "Error in input. " << endl 
		<< "FE-Calc InputForces InputOps OutputFile NumFiles FileDigits [NumOPs]" << endl;
		return 1;
	}
	else
		return 0;
}

int FindNumOps(char* InputFile) {
    ifstream myfile(InputFile);    
    myfile.unsetf(ios_base::skipws);

    unsigned line_count = std::count(
        istream_iterator<char>(myfile),
        istream_iterator<char>(), '\n');

    return line_count;
}

void ReadInput(double* Vars, char* InputFile, const int NumFrames){
	ifstream Fp(InputFile,ifstream::in);

	for(int i = 0; i < NumFrames; i++)
		Fp >> Vars[i];
}

void WriteOutput(char* OutputFile, const double* const Vars, const int NumFrames){
	ofstream Fp;
	Fp.setf(ios::scientific);
	Fp.precision(6);
	Fp.open(OutputFile);

	for(int i = 0; i < NumFrames; i++)
		Fp << i << "\t" << Vars[i]  << endl;

	Fp.close();
}

double ComputeFE(const double* const Forces, const double* const OPs, const int length) {
	double FE = .0;

	for (int i = 1; i < length; i++)
		FE += + 0.5 * (Forces[i] + Forces[i-1]) * (OPs[i] - OPs[i-1]);

	return FE;
}

string* CreateFrameFile(const int NumFiles, const int length) {
	string* FrameDigits = new string[NumFiles];

	ofstream Fp;
	Fp.open("temp.dat");
	for(int i = 0; i < NumFiles; i++)
		Fp << setfill('0') << setw(length) << i << endl;
	Fp.close();

	ifstream Fpp("temp.dat",ifstream::in);
	for(int i = 0; i < NumFiles; i++)
		getline(Fpp,FrameDigits[i]);

	Fpp.close();
	return FrameDigits;
}

int main(int argc, char** argv){
	if(CheckForInputError(argc))
		return 1;
	else {
		int NumOPs;
		string InputForces = argv[1], InputOPs = argv[2], OutputFile = argv[3];
		const int NumFiles = atoi(argv[4]), FileDigits = atoi(argv[5]);
		double* FreeEnergy = new double[NumFiles+1];
		FreeEnergy[0] = .0;

		string* FrameDigits = CreateFrameFile(NumFiles,FileDigits);
		double *Forces = new double[NumOPs], *OPs = new double[NumOPs];

		if(argc > 6)
			NumOPs = atoi(argv[6]);
		else {
			cout << "Attempting to find the number of OPS ..." << endl;

			string InputForcesTemp;
			InputForcesTemp.append(InputForces);
			InputForcesTemp.append(FrameDigits[0]);
			InputForcesTemp.append(".dat");
			NumOPs = FindNumOps(InputForcesTemp.c_str());

			cout << "Number of OPs computed is " << NumOPs << endl;
		}

		for (int i = 0; i < NumFiles; i++){
			string InputForcesTemp;
			InputForcesTemp.append(InputForces);
			InputForcesTemp.append(FrameDigits[i]);
			InputForcesTemp.append(".dat");

			string InputOPsTemp;
			InputOPsTemp.append(InputOPs);
			InputOPsTemp.append(FrameDigits[i]);
			InputOPsTemp.append(".dat");

			ReadInput(Forces,InputForcesTemp.c_str(),NumOPs);
			ReadInput(OPs,InputOPsTemp.c_str(),NumOPs);

			FreeEnergy[i+1] = ComputeFE(Forces,OPs,NumOPs);
		}
		WriteOutput(OutputFile.c_str(),FreeEnergy,NumFiles+1);
	}

	return 0;
}