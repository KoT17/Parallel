#include <thread>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;


bool is_prime(int number);
void find_primes(vector<char>* sieve, vector<int> numbers);

int main() {
	int cap = pow(10, 8), totalCount = 0;
	int64_t sumOfPrimes = 1;
	vector<vector<int> > numbers(8);
	vector<char> sieve;
	vector<int> lastPrimes;

	sieve.push_back(0); // Represents 2 in the prime sieve and it's an offset

	// Split up the numbers evenly between threads and create sieve for all nums
	for (int i = 3; i < cap; i += 16) {
		for (int j = 0; j < 8; j++) {
			if(i + 2*j < cap){
				numbers.at(j).push_back(i + 2*j);
			}
			sieve.push_back(0);
			sieve.push_back(1);
		}
	}
	auto start = high_resolution_clock::now();

	vector<thread> thrs(8);

	// Spawn 8 threads with their respective vectors
	for (int i = 0; i < 8; i++)
		thrs[i] = thread(find_primes, &sieve, numbers.at(i));

	for (int i = 0; i < 8; i++)
		thrs[i].join();


	auto end = high_resolution_clock::now();

	// Determine total number of primes and sum
	for (int i = 1; i < sieve.size(); i++) {
		if(sieve.at(i) == 0){
			totalCount++;
			sumOfPrimes+= i;
		}
	}

	// Determine the last 10 largest primes
	int count = 0;
	int size = sieve.size();
	for (int i = size-1; i > -1; i--) {
		if (count == 10)
		 	break;
		if(sieve.at(i) == 0) {
			lastPrimes.push_back(i);
			count++;
		}
	}

	ofstream file;
	file.open("primes.txt");
	// Print out all information
	file << duration_cast<milliseconds>(end - start).count() << "ms" << " " << totalCount << " " << sumOfPrimes << endl;
	for (int i = 9; i > -1; i--)
		file << lastPrimes.at(i) << " ";
	file << endl;
	return 0;
}

// Thread function to sieve through array
void find_primes(vector<char>* sieve, vector<int> numbers) {

	for (int i = 0; i < numbers.size(); i++) {
		int num = numbers.at(i);

		if (sieve->at(num) == 0) {
			for (int j = num*2; j < sieve->size(); j+=num) {
				sieve->at(j) = 1;
			}
		}
	}
}
