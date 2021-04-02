#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <random>
#include <ctime>
#include <string>
#include "asymmetric.h"
#define ll long long

using namespace std;

const int bufferSize = 5000;

////////////////////// \/ keyGen \/ ///////////////////////////////
void ASYMMETRIC::printKeys() {
	cout << "p = " << p << endl;
	cout << "g = " << g << endl;
	cout << "d = " << d << endl;
	cout << "e2 = " << e2 << endl;
}
//https://stackoverflow.com/questions/37396278/how-to-generate-very-large-random-number-in-c/37396515
ll ASYMMETRIC::keyK() {
	default_random_engine generator(rand());
	uniform_int_distribution<long long unsigned> distribution(0, p-1);
	return distribution(generator);
}
bool ASYMMETRIC::keyMngr(int user_input) {
	if (user_input == NULL) {
		if (readKeys()) {
			cout << "Existing Key Set Found" << endl << endl;
			cout << "USE EXISTING KEY SET" << endl;
			return true;
		}
		cout << "Error: Could not locate existing key set" << endl << endl;
		return false;
	}
	else if (keyGen(user_input)) {
		cout << "Public/Private Keys Generated" << endl << endl;
		if (writeKeys()) {
			cout << "USE NEW KEY SET" << endl;
			return true;
		}
		else return false;
	}
	else return false;
}
bool ASYMMETRIC::keyGen(int user_input){
	p = prime();
	if (p) {
		seed = user_input;
		srand(seed);
		default_random_engine generator(rand());
		uniform_int_distribution<long long unsigned> distribution(0, p-1);
		d = distribution(generator);
		if (d) {
			e2 = modulo(g, d, p, 1);
			if (e2) {
				printKeys();
				return true;
			}
		}
	}
	cout << "Key Gen Error" << endl << endl;
	return false;
}

ll ASYMMETRIC::prime() {
	while (true) {
		ll q = random(xToTheN(2, 30, 1), xToTheN(2, 31, 1) - 1);
		if (isPrime(q))
			if (q % 12 == 5)
				if (isPrime((q * 2) + 1))
					return (q * 2) + 1;
	}
}

bool ASYMMETRIC::isPrime(ll num) {
	if (Miller(num))
		return true;
	else
		return false;
}

//https://www.tutorialspoint.com/cplusplus-program-to-implement-the-rabin-miller-primality-test-to-check-if-a-given-number-is-prime
//code for Rabin-Miller algorithm
bool ASYMMETRIC::Miller(ll p) {
	if (p < 2) {
		return false;
	}
	if (p != 2 && p % 2 == 0) {
		return false;
	}
	ll s = p - 1;
	while (s % 2 == 0) {
		s /= 2;
	}
	for (int i = 0; i < iterations; i++) {
		ll a = rand() % (p - 1) + 1, temp = s;
		ll mod = modulo(a, temp, p, 1);
		while (temp != p - 1 && mod != 1 && mod != p - 1) {
			mod = mulmod(mod, mod, p);
			temp *= 2;
		}
		if (mod != p - 1 && temp % 2 == 0) {
			return false;
		}
	}
	return true;
}
ll ASYMMETRIC::random(ll min, ll max){
	ll random = rand();
	random %= (max - min + 1);
	random += min;
	return random;
}
////////////////////// /\ keyGen /\ ////////////////////////////

//////////////////// \/ Read and Write \/ //////////////////////
bool ASYMMETRIC::readKeys(){
	bool pk = false;
	if (readKey("pubKey.txt")) pk = true;
	else cout << "Error: could not find pubKey.txt" << endl;
	if (readKey("priKey.txt")) {
		if (pk == true) {
			printKeys();
			return true;
		}
		else return false;
	}
	else cout << "Error: could not find priKey.txt" << endl;
	return false;
}
bool ASYMMETRIC::readKey(string set) {
	if (seed == NULL) srand(time(NULL));
	ifstream input;
	input.open(set);
	input >> p;
	input >> g;
	input >> (set == "pubKey.txt"? e2 : d);
	return true; 
}
bool ASYMMETRIC::writeKeys() {
	bool pk = false;
	if (writeKey(p, g, e2, "pubKey.txt")) {
		cout << "Public key stored in 'pubKey.txt'" << endl;
		pk = true;
	}
	else cout << "Error: could not create 'pubKey.txt'" << endl;
	if (writeKey(p, g, d, "priKey.txt")) {
		if (pk == true) {
			cout << "Private key stored in 'priKey.txt'" << endl << endl;
			return true;
		}
		else return false;
	}
	else cout << "Error: could not create 'priKey.txt'" << endl << endl;
	return false;
}
bool ASYMMETRIC::writeKey(ll p, ll g, ll e, string set) {
	if (p != NULL && g != NULL && e != NULL) {
		ofstream outputTextFile(set);
		if (outputTextFile.is_open()) {
			outputTextFile << p << " " << g << " " << e << " " << endl;
			outputTextFile.close();
			return true;
		}
	}
	return false; 
}

bool ASYMMETRIC::readText(string task) {
	if (task == "encrypt") {
		if (readM()) return true;
		else return false;
	}
	else if (readC()) return true;
	else return false;
}

bool ASYMMETRIC::readM() {
	char buffer[bufferSize] = { NULL };
	ifstream* pStream = new ifstream("ptext.txt");
	if (pStream->is_open() == true) {
		pStream->read(buffer, bufferSize);
	}
	size_t strLength = strlen(buffer);
	cout << "Plaintext: " << endl << endl << buffer << endl << endl;
	int block = 0, j = 0;
	int* ascii_values = new int[strLength];
	for (int i = 0; i < strLength; i++)
		ascii_values[i] = (int)buffer[i];
	while (j < strLength) {
		int* int32 = new int[4];
		for (int i = 3; i >= 0; i--) {
			if (j < strLength) {
				//cout << "m" <<  hex << ascii_values[j];
				int32[i] = ascii_values[j++];
			}
			else {
				//cout << "00";
				int32[i] = 0;
			}
		}
		
		m[block] = ints_to_int32(int32);
		cout << "m" << block + 1 << /*dec << m[block] <<*/ " = " << hex << m[block] << " = " << dec << m[block] << endl;
		delete[] int32;
		block++;
	}
	delete[] ascii_values;
	cout << endl;
	blockcount = block;
	return true;
}
bool ASYMMETRIC::readC() { 
	ifstream input;
	int block = 0;
	input.open("ctext.txt");
	while (input >> c[block][0] >> c[block][1]) {
		//input >> c[block][0] >> c[block][1];
		cout << "block" << block+1 << " c1 = " << c[block][0] << ", c2 = " << c[block][1] << endl;
		block++;
	}
	cout << endl;
	blockcount = block;
	return true;
}

bool ASYMMETRIC::writeText(string task) {
	if (task == "encrypt") {
		if (writeC()) return true;
		else return false;
	}
	else if (writeM()) return true;
	else return false;
}

bool ASYMMETRIC::writeM() { 
	ofstream outputTextFile("ptext.txt");
	cout << "Plain text: " << endl;
	if (outputTextFile.is_open()) {
		for (int i = 0; i < blockcount; i++) {
			//outputTextFile << c[i][0] << " " << c[i][1] << endl;
			int* int32 = new int[8];
			int32 = int32_to_ints(m[i]);
			for (int i = 0; i < 4; i++)
				outputTextFile << (char)int32[i];
		}
		cout << endl << endl;;
		outputTextFile.close();
		cout << endl << "Write to 'ptext.txt' Successful" << endl;
		return true;
	}
	cout << "Write to 'ptext.txt' Unsuccessful" << endl;
	return false;
}
bool ASYMMETRIC::writeC() {
	ofstream outputTextFile("ctext.txt");
	if (outputTextFile.is_open()) {
		for (int i = 0; i < blockcount; i++) {
			outputTextFile << c[i][0] << " " << c[i][1] << endl;
		}
		outputTextFile.close();
		cout << "Write to 'ctext.txt' Successful" << endl;
		return true;
	}
	cout << "Write to 'ptext.txt' Unsuccessful" << endl;
	return false;
}

/////////////////// /\ Read and Write /\ ///////////////////////

/////////////////////// \/ Cipher \/ ///////////////////////////
bool ASYMMETRIC::cipher(string task) {
	if (task == "encrypt") {
		if (encryption()) {
			cout << "Encryption Successful" << endl << endl;
			return true;
		}
		else {
			cout << "Encryption Unsuccessful" << endl << endl;
			return false;
		}
	}
	if (decryption()) {
		cout << "Decryption Successful" << endl << endl;
		return true;
	}
		cout << "Decryption Unsuccessful" << endl << endl;
		return false;
}
bool ASYMMETRIC::encryption() {
	//ll key[5] = { 1161138792, 1465945692, 1688335579, 832679255, 1414089043 };
	for (int i = 0; i < blockcount; i++) {
		k = keyK();
		//ll k = key[i];
		cout << "Block " << i + 1 << ":" << endl;
		cout << "With ephemeral key k = " << dec << k << endl;
		c[i][0] = modulo(g, k, p, 1);
		cout << "C1 = " << dec << c[i][0] << endl;
		c[i][1] = modulo(e2, k, p, m[i]);
		cout << "C2 = " << dec << c[i][1] << endl << endl;
	}
	cout << endl;
	if(m[0] != NULL)
		return true;
	return false;
}
bool ASYMMETRIC::decryption(){
		for (int i = 0; i < blockcount; i++) {
			m[i] = modulo(c[i][0], (p - 1 - d), p, c[i][1]);
			cout << "m" << i + 1 << " = " << m[i] << endl;// << endl;

		}
		cout << endl;
		if (m[0] != NULL)
			return true;
		return false;
}
/////////////////////// /\ Cipher /\ ///////////////////////////

/////////////// \/ Arithmetic Functions \/ /////////////////////

//https://www.tutorialspoint.com/cplusplus-program-to-implement-the-rabin-miller-primality-test-to-check-if-a-given-number-is-prime
ll ASYMMETRIC::mulmod(ll a, ll b, ll m) {//It returns true if number is prime otherwise false {
	ll x = 0, y = a % m;
	while (b > 0) {
		if (b % 2 == 1) {
			x = (x + y) % m;
		}
		y = (y * 2) % m;
		b /= 2;
	}
	return x % m;
}

ll ASYMMETRIC::modulo(uint64_t base, ll exp, ll mod, ll res) {
	if (exp > 1)
		if (exp % 2 == 0)
			return modulo(((base * base) % mod), (exp / 2), mod, res);
		else
			return modulo(((base * base) % mod), (exp / 2), mod, (base * res) % mod);
	else
		//cout << "base = " << base << " res = " << res << endl;
	return (base * res) % mod;
}


ll ASYMMETRIC::xToTheN(int x, int n, uint32_t i) {
	while (--n >= 0) { return xToTheN(x, n, i *= x); };
	return i;
}

ll ASYMMETRIC::ints_to_int32(int * int4) {
	ll int32 = 0;
	for (int i = 0; i < 4; i++) {
		int32 += (int4[i] * xToTheN(16, (i*2), 1));
		//cout << hex << int32 << endl;
	}
	return int32;
}
int* ASYMMETRIC::int32_to_ints(ll m) {
	int* ints = new int[8];
	for (int i = 3; i >= 0; i--) {
		ints[((-1)*i) + 3] = (m/(xToTheN(16, (i*2), 1)));
		m %= (xToTheN(16, (i * 2), 1));
		cout << (char)ints[((-1) * i) + 3];
	}
	return ints;
};
/////////////// /\ Arithmetic Functions /\ //////////////////
