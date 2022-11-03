# Crypto-MidtermProject
Midterm Project for 8001 Cryptography: Coding RSA in C++
Olivia LaVal

-------------------------------------
The main function and supporting functions are located in the "CryptoMidtermProject.cpp" file.
This file is dependent on the Boost Multiprecision library (boost/multiprecision/cpp_int.hpp) and so must be compiled with that library.
- Here are instruction for downloading and using Boost on Windows: https://www.boost.org/doc/libs/1_80_0/more/getting_started/windows.html
- I used the section called "4.1   Build From the Visual Studio IDE" to get Boost set up on my computer and connected with my project in Visual Studio 2022.

To execute, run the file CryptoMidtermProject.cpp

Please see the included demo video to see the RSA algorithm working correctly (and also for a note on why I did not get the PKCS#1v1.5 message padding scheme version of RSA working).

--------------------------
The files "inputpqe1.txt" and "inputpqe2.txt" hold the given test values for p, q, e
The file "inputx.txt" holds an example value of 89 for x
Create your own input file for c or feed your output E(x) file back in for c to get your original x back as D(c)
