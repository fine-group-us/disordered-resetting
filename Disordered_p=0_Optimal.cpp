/* DISORDERED RESETTING.*/

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <initializer_list>
#include <random>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>


std::random_device device;		// Random device engine, usually based on /dev/random on UNIX-like systems
std::mt19937 gen(device());		// Initialize Mersennes' twister using rd to generate the seed

int main() {

	std::uniform_real_distribution<double> dis(0.0, 1.0); // Real Random number between 0 and 1 with uniform distribution
	std::normal_distribution<double> gaussian(0.0, 1.0); // Normal distribution with (mean) mu = 0 and (variance) sigma2 = 1.
	
	// We are going to simulate the dynamics of a typical one-dimensional brownian particle spreads (propagation phase) but after certain time it resets to a position xr. 
	// The distribution of reset depends on if the sign of x, being constant either x is positive or negative.
	// We set a target where the particle is absorbed when it reached the target. The position of the target is xt with probability p and -xt with probability (1-p).

	///////////////////////////////////////////////////	
	// INPUT FILES
	///////////////////////////////////////////////////


	// Input is introduced manually. 
	// INPUT: PTARGET=0, NRUNS, FILE

	///////////////////////////////////////////////////	
	// PARAMETERS
	///////////////////////////////////////////////////
    
    double pTarget;
	double D = 1; // Diffusion constant
	double a = 1; // Target
	int nruns; // How many simulations we are carrying out
    int File;
    // std::cin >> r1 >> r2 >> pTarget >> nruns;
	std::cin >> pTarget >> nruns >> File;
	std::cout << " p = " << pTarget << " runs = " << nruns << std::endl;

	//double xr = 0.0; // Position of resetting
	//double x0 = 0.0; // Initial position
	

	double t = 0.0; // Current time 

	double dt = 1.0e-5; // Step between two consecutive times
    std::cout << "dt = " << dt << std::endl;

	double c1 = sqrt(2 * D * dt); // Amplitude of the white gaussian noise

	std::bernoulli_distribution distTarget(pTarget);

	///////////////////////////////////////////////////	
	// OUTPUT FILES
	///////////////////////////////////////////////////

	// We want to describe the PDF of the propagation phase for any time. 
	// We are going to obtain the number of counts for each value of x and t and what phase the system is in.
    
	std::string pString = "_p=" + std::to_string(pTarget).substr(0, 3);
	std::replace(pString.begin(), pString.end(), '.', '_');
    
	std::string MFTPFile = "Results_Paper/p=0/Results_" + std::to_string(File) + ".dat";

	std::ofstream output1(MFTPFile.c_str(), std::ofstream::trunc);

	///////////////////////////////////////////////////	
	// VARIABLES
	///////////////////////////////////////////////////

	double xOld = 0, xNew = 0;


	// Output matrix

	std::vector<double> FTP(nruns);


	///////////////////////////////////////////////////	
	// MEASURING TIME
	///////////////////////////////////////////////////

	// Start measuring time
	auto begin = std::chrono::high_resolution_clock::now();

	///////////////////////////////////////////////////	
	// STOCHASTIC GENERATION OF RESETTING TIMES
	///////////////////////////////////////////////////

	for (size_t p = 0; p < nruns; p++)
	{
		while (xOld <= a)
		{	
			if (xOld >= 0)
			{
				xNew = xOld + c1 * gaussian(gen);
            }
			else
			{
	    		xNew = 0;
			}

			xOld = xNew;
			t += dt;
		}

		FTP[p] = t;

		// Reset all the variables

		t = 0;

		// Initial position
		xOld = 0;
		xNew = 0;
        if (p % 10 == 0){
        std::cout << p << std::endl;
        }
	}

	for (size_t i = 0; i < FTP.size(); i++)
	{
		output1 << FTP[i] << " ";
	}

	output1.close();

	// Stop measuring time and calculate the elapsed time
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin);

	std::cout << " Time measured: " << elapsed.count() * 1e-9 << std::endl;

	return 0;

}