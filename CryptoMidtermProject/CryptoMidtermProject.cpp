// CryptoMidtermProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
using namespace std;
#include <stdlib.h>
#include <string>

//Function to find gcd(a,b) - from GeeksForGeeks
int gcd(int a, int b) {
    int t;
    while (1) {
        t = a % b;
        if (t == 0)
            return b;
        a = b;
        b = t;
    }
}

// Function for extended Euclidean Algorithm - from GeeksForGeeks
int gcdExtended(int a, int b, int* x, int* y);

// Function to find modulo inverse of a - from GeeksForGeeks
int modInverse(int A, int M)
{
    int x, y;
    int g = gcdExtended(A, M, &x, &y);
    if (g != 1) {
        cout << "Inverse doesn't exist";
        return 0;
    }
    else {
        // m is added to handle negative x
        int res = (x % M + M) % M;
        return res;
    }
}

int gcdExtended(int a, int b, int* x, int* y)
{

    // Base Case
    if (a == 0) {
        *x = 0, * y = 1;
        return b;
    }

    // To store results of recursive call
    int x1, y1;
    int gcd = gcdExtended(b % a, a, &x1, &y1);

    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

int main()
{
    cout << "Starting RSA!\n";

    cout << "Enter the name of the file that contains p, q and e: ";
    string inputpqefilename;
    cin >> inputpqefilename;
    
    //Read p,q,e from the file
    int p = 0, q = 0, e = 0;
    ifstream inputpqefile(inputpqefilename);
    if (inputpqefile.is_open()) {
        char drop;
        
        inputpqefile >> drop;
        inputpqefile >> drop;
        inputpqefile >> e;
        inputpqefile >> drop;
        inputpqefile >> drop;
        inputpqefile >> p;
        inputpqefile >> drop;
        inputpqefile >> drop;
        inputpqefile >> q;

        cout << "(p, q, e) = " << p << ", " << q << ", " << e;
        inputpqefile.close();
    }
    else {
        cout << "File" << inputpqefilename << "was not opened.";
    }

    int phi = 0;
    phi = (p - 1) * (q - 1);

    //Increments e until e is relative prime to phi
    int track;
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
    int n = 0, d = 0;
    n = p * q;
    d = modInverse(e, phi);

    cout << "Enter the name of the file that contains x to be encrypted using (N, e): ";
    string inputxfilename;
    cin >> inputxfilename;

    cout << "Enter the output file name to store E(x): ";
    string outputEfilename;
    cin >> outputEfilename;

    //caluate E(x) and store in the ouput file


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
