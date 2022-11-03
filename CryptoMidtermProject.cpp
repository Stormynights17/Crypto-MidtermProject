// CryptoMidtermProject.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
using namespace std;
#include <stdlib.h>
#include <string>
#include <cstring>
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

//Function to find gcd(a,b) - from GeeksForGeeks
cpp_int gcd(cpp_int a, cpp_int b);

// Function for extended Euclidean Algorithm - from GeeksForGeeks
cpp_int gcdExtended(cpp_int a, cpp_int b, cpp_int* x, cpp_int* y);

// Function to find modulo inverse of a - from GeeksForGeeks
cpp_int modInverse(cpp_int A, cpp_int M);

cpp_int encrypt(cpp_int x, cpp_int e, cpp_int n);

cpp_int decrypt(cpp_int c, cpp_int d, cpp_int n);

cpp_int encode_pkcs(cpp_int x, cpp_int n);

cpp_int decode_pkcs(cpp_int c, cpp_int n);

//Iterative Function to calculate (x^y)%p in O(log y) - from GeeksForGeeks
cpp_int powerMod(cpp_int x, cpp_int y, cpp_int p);

//Function to turn a int into binary string - found on stack overflow
string toBinary(cpp_int n);

// Function to convert binary to decimal - from GeeksForGeeks
cpp_int binaryToDecimal(string binaryString);

// Function to generate a random binary string of length N - from GeeksForGeeks
string generateBinaryString(int n);

int main()
{
    cout << "Starting RSA!\n";

    cout << "Enter the name of the file that contains p, q and e: ";
    string inputpqefilename;
    cin >> inputpqefilename;

    //Read p,q,e from the file
    cpp_int p, q, e;

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

    cpp_int phi = 0;
    cpp_int pmin1 = p - 1;
    cpp_int qmin1 = q - 1;
    phi = pmin1 * qmin1;
    cout << "phi = " << phi << "\n";

    //Increments e until e is relative prime to phi
    cpp_int track;
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
    cpp_int n = 0, d = 0;
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

    cpp_int x = 0;
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
    cpp_int ex = 0;
    ex = encrypt(x, e, n);
    cout << "E(x) = " << ex << "\n";

    cpp_int ex_pad = encrypt(encode_pkcs(x, n), e, n);
    cout << "E(encodeX) = " << ex_pad << "\n";

    ofstream outputEfile(outputEfilename);
    if (outputEfile.is_open()) {
        outputEfile << "E(x) = " << ex << "\n";
        outputEfile << "E(encodeX) = " << ex_pad << "\n";
        outputEfile.close();
    }
    else {
        cout << "File" << outputEfilename << "was not opened.";
        return 0;
    }

    cout << "Enter the name of the file that contains c to be decrypted using d: ";
    string inputcfilename;
    cin >> inputcfilename;

    cpp_int c = 0;
    cpp_int c_pad = 0;
    ifstream inputcfile(inputcfilename);
    if (inputcfile.is_open()) {
        string drop;

        inputcfile >> drop;
        inputcfile >> drop;
        inputcfile >> c;

        inputcfile >> drop;
        inputcfile >> drop;
        inputcfile >> c_pad;

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
    cpp_int dc = 0;
    dc = decrypt(c, d, n);
    cout << "D(c) = " << dc << "\n";

    cpp_int dc_pad = decrypt(decode_pkcs(c_pad, n), d, n);
    cout << "D(decodeC) = " << dc_pad << "\n";

    ofstream outputDfile(outputDfilename);
    if (outputDfile.is_open()) {
        outputDfile << "D(c) = " << dc << "\n";
        outputDfile << "D(decodeC) = " << dc_pad << "\n";
        outputDfile.close();
    }
    else {
        cout << "File" << outputDfilename << "was not opened.";
        return 0;
    }

}

cpp_int gcd(cpp_int a, cpp_int b) {
    cpp_int t;
    while (1) {
        t = a % b;
        if (t == 0)
            return b;
        a = b;
        b = t;
    }
}

cpp_int modInverse(cpp_int A, cpp_int M) {
    cpp_int x, y;
    cpp_int g = gcdExtended(A, M, &x, &y);
    if (g != 1) {
        cout << "Inverse doesn't exist";
        return 0;
    }
    else {
        // m is added to handle negative x
        cpp_int res = x % M;
        res = res + M;
        res = res % M;
        return res;
    }
}

cpp_int gcdExtended(cpp_int a, cpp_int b, cpp_int* x, cpp_int* y) {
    // Base Case
    if (a == 0) {
        *x = 0, * y = 1;
        return b;
    }

    // To store results of recursive call
    cpp_int x1, y1;
    cpp_int gcd = gcdExtended(b % a, a, &x1, &y1);

    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

cpp_int encrypt(cpp_int x, cpp_int e, cpp_int n) {
    return (cpp_int)powerMod(x, e, n);
}

cpp_int decrypt(cpp_int c, cpp_int d, cpp_int n) {
    return (cpp_int)powerMod(c, d, n);
}

cpp_int encode_pkcs(cpp_int x, cpp_int n) {
    unsigned int lengthX = msb(x);
    unsigned int lengthN = msb(n);
    if (lengthX <= lengthN - 11) {
        unsigned int lengthR = (lengthN/8 - lengthX/8 - 3);

        string encodedX = "00000010" + generateBinaryString(lengthR) + "0000" + toBinary(x);

        return binaryToDecimal(encodedX);
    }
    else {
        cout << "Error in encode_pkcs\n";
        return 0;
    }
}

cpp_int decode_pkcs(cpp_int c, cpp_int n) {
    unsigned int lengthC = msb(c);
    unsigned int lengthN = msb(n);
    if (lengthC < lengthN) {
        string binaryString = toBinary(c);

        int i = 8;
        //Find how many bits before the 4 zeros
        int count = 0;
        while (i < lengthN) {
            if (binaryString[i] == '0') {
                count++;
            }
            if (count == 4) {
                break;
            }
        }
        string decoded = binaryString.substr(i);
        return binaryToDecimal(decoded);
    }
}

cpp_int powerMod(cpp_int x, cpp_int y, cpp_int p)
{
    cpp_int res = 1;     // Initialize result

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

string toBinary(cpp_int n)
{
    string r;
    while (n != 0) {
        r = (n % 2 == 0 ? "0" : "1") + r;
        n /= 2;
    }
    return r;
}

// Function to convert binary to decimal
cpp_int binaryToDecimal(string binaryString) {
    string b = binaryString;
    cpp_int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    cpp_int base = 1;

    size_t i = b.length();
    while (i > 0) {
        char last_bit = b[i];
        i--;

        if (last_bit == '1') {
            dec_value += 1 * base;
        }

        base = base * 2;
    }

    return dec_value;
}

string generateBinaryString(int n) {
    srand(time(NULL));
    string s = "";

    // Iterate over the range [0, N - 1]
    for (int i = 0; i < n; i++) {

        // Store the random number
        int x = ((int)rand() % 2);

        // Append it to the string
        s += to_string(x);
    }

    //Make sure there are never 4 zeros in a row
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == '0') {
            count++;
        }
        if (count == 4) {
            s[i] = '1';
        }
    }

    return s;
}
