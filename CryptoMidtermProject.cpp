// CryptoMidtermProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
using namespace std;
#include <stdlib.h>
#include <string>
#include <cstring>
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

//Function to find gcd(a,b) - from GeeksForGeeks
int1024_t gcd(int1024_t a, int1024_t b);

// Function for extended Euclidean Algorithm - from GeeksForGeeks
int1024_t gcdExtended(int1024_t a, int1024_t b, int1024_t* x, int1024_t* y);

// Function to find modulo inverse of a - from GeeksForGeeks
int1024_t modInverse(int1024_t A, int1024_t M);

int1024_t encrypt(int1024_t x, int1024_t e, int1024_t n);

int1024_t decrypt(int1024_t c, int1024_t d, int1024_t n);

//Iterative Function to calculate (x^y)%p in O(log y) - from GeeksForGeeks
int1024_t powerMod(int1024_t x, int1024_t y, int1024_t p);

int main()
{
    cout << "Starting RSA!\n";

    cout << "Enter the name of the file that contains p, q and e: ";
    string inputpqefilename;
    cin >> inputpqefilename;

    //Read p,q,e from the file
    int1024_t p, q, e;

    ifstream inputpqefile(inputpqefilename);
    if (inputpqefile) {
        string drop;

        inputpqefile >> drop;
        inputpqefile >> drop;
        inputpqefile >> e;
        inputpqefile >> drop;
        inputpqefile >> drop;
        inputpqefile >> p;
        inputpqefile >> drop;
        inputpqefile >> drop;
        inputpqefile >> q;

        cout << "(p, q, e) = " << p << ", " << q << ", " << e << "\n";
        inputpqefile.close();
    }
    else {
        cout << "File" << inputpqefilename << "was not opened.";
        return 0;
    }

    int1024_t phi = 0;
    phi = (p - 1) * (q - 1);
    cout << "phi = " << phi << "\n";

    //Increments e until e is relative prime to phi
    int1024_t track;
    while (e < phi) {
        track = gcd(e, phi);
        if (track == 1)
            break;
        else
            e++;
    }

    cout << "Enter the output file name to store d and N: ";
    string outputNdfilename;
    cin >> outputNdfilename;

    //calculate d and N and store in the output file
    int1024_t n = 0, d = 0;
    n = p * q;
    d = modInverse(e, phi);

    cout << "(d, N) = " << d << ", " << n << "\n";

    ofstream outputNdfile(outputNdfilename);
    if (outputNdfile.is_open()) {
        outputNdfile << "d = " << d << "\n";
        outputNdfile << "n = " << n << "\n";
        outputNdfile.close();
    }
    else {
        cout << "File" << outputNdfilename << "was not opened.";
        return 0;
    }

    cout << "Enter the name of the file that contains x to be encrypted using (N, e): ";
    string inputxfilename;
    cin >> inputxfilename;

    int1024_t x = 0;
    ifstream inputxfile(inputxfilename);
    if (inputxfile.is_open()) {
        string drop;

        inputxfile >> drop;
        inputxfile >> drop;
        inputxfile >> x;

        cout << "x = " << x << "\n";

        inputxfile.close();
    }
    else {
        cout << "File" << inputxfilename << "was not opened.";
        return 0;
    }

    cout << "Enter the output file name to store E(x): ";
    string outputEfilename;
    cin >> outputEfilename;

    //caluate E(x) and store in the ouput file
    int1024_t ex = 0;
    ex = encrypt(x, e, n);

    cout << "E(x) = " << ex << "\n";

    ofstream outputEfile(outputEfilename);
    if (outputEfile.is_open()) {
        outputEfile << "E(x) = " << ex << "\n";
        outputEfile.close();
    }
    else {
        cout << "File" << outputEfilename << "was not opened.";
        return 0;
    }

    cout << "Enter the name of the file that contains c to be decrypted using d: ";
    string inputcfilename;
    cin >> inputcfilename;

    int1024_t c = 0;
    ifstream inputcfile(inputcfilename);
    if (inputcfile.is_open()) {
        string drop;

        inputcfile >> drop;
        inputcfile >> drop;
        inputcfile >> c;

        cout << "c = " << c << "\n";
        inputcfile.close();
    }
    else {
        cout << "File" << inputcfilename << "was not opened.";
        return 0;
    }


    cout << "Enter the output file name to store D(c): ";
    string outputDfilename;
    cin >> outputDfilename;

    //calculate D(c) and store in the output file
    int1024_t dc = 0;
    dc = decrypt(c, d, n);

    cout << "D(c) = " << dc << "\n";

    ofstream outputDfile(outputDfilename);
    if (outputDfile.is_open()) {
        outputDfile << "D(c) = " << dc << "\n";
        outputDfile.close();
    }
    else {
        cout << "File" << outputDfilename << "was not opened.";
        return 0;
    }

}

int1024_t gcd(int1024_t a, int1024_t b) {
    int1024_t t;
    while (1) {
        t = a % b;
        if (t == 0)
            return b;
        a = b;
        b = t;
    }
}

int1024_t modInverse(int1024_t A, int1024_t M) {
    int1024_t x, y;
    int1024_t g = gcdExtended(A, M, &x, &y);
    if (g != 1) {
        cout << "Inverse doesn't exist";
        return 0;
    }
    else {
        // m is added to handle negative x
        int1024_t res = (x % M + M) % M;
        return res;
    }
}

int1024_t gcdExtended(int1024_t a, int1024_t b, int1024_t* x, int1024_t* y) {
    // Base Case
    if (a == 0) {
        *x = 0, * y = 1;
        return b;
    }

    // To store results of recursive call
    int1024_t x1, y1;
    int1024_t gcd = gcdExtended(b % a, a, &x1, &y1);

    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

int1024_t encrypt(int1024_t x, int1024_t e, int1024_t n) {
    return (int1024_t)powerMod(x, e, n);
}

int1024_t decrypt(int1024_t c, int1024_t d, int1024_t n) {
    return (int1024_t)powerMod(c, d, n);
}

int1024_t powerMod(int1024_t x, int1024_t y, int1024_t p)
{
    int1024_t res = 1;     // Initialize result

    x = x % p; // Update x if it is more than or
    // equal to p

    if (x == 0) return 0; // In case x is divisible by p;

    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = (res * x) % p;

        // y must be even now
        y = y >> 1; // y = y/2
        x = (x * x) % p;
    }
    return res;
}
