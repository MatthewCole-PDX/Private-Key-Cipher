#ifndef ASYMMETRIC_H
#define ASYMMETRIC_H
#include <string>
#include <fstream>

#define ll long long

using namespace std;

class ASYMMETRIC {
public:
	bool keyMngr(int user_input);
	bool readText(string task);
	bool writeText(string task);
	bool cipher(string task);
private:
	ll seed = NULL;
	ll g = 2;
	int iterations = 40;
	int blockcount = 0;
	ll m[1000] = { NULL };
	ll c[3000][2] = { NULL };
	ll p = NULL;
	ll e2 = NULL;
	ll k = NULL;
	ll d = NULL;
	ll keyK();
	ll random(ll min, ll max);
	void printKeys();
	bool keyGen(int user_input);
	bool readKeys();
	bool readKey(string set);
	bool writeKeys();
	bool writeKey(ll p, ll g, ll e, string set);
	bool readM();
	bool readC();
	bool writeM();
	bool writeC();
	ll prime();
	bool isPrime(ll num);
	bool Miller(ll p);
	bool encryption();
	bool decryption();
	ll mulmod(ll a, ll b, ll m);
	ll modulo(uint64_t base, ll exp, ll mod, ll res);
	ll xToTheN(int x, int n, uint32_t i);
	ll ints_to_int32(int * int4);
	int* int32_to_ints(ll m);
};

#endif // !"ASYMMETRIC"
#pragma once
	
