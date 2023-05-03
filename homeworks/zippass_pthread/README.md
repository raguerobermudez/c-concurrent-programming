# Homework 01: zippas_serial

## CI-0117 Programación Paralela y Concurrente

## Student:
-  <em>Randy Jossué Agüero Bermúdez</em>, Studend card: <em>B90082</em>, email address: <em>randy.aguero@ucr.ac.cr </em>


### Program description:
This program is designed to find the password for a password-protected ZIP file using the brute force algorithm. The algorithm involves trying all possible combinations of characters allowed by the given alphabet until the correct password is found.</n>

The program will read an input file in *.txt format from the standard input, which will include:
> - An allowed alphabet. For example: "1032ABC". The program will use this alphabet to attempt all possible combinations of characters until it finds the correct password for each ZIP file
> - A list of ZIP files to unlock.

The program's output will include the name of each ZIP file and the password found. If a password for a ZIP file is not found, it will be indicated in the output.

#### Program Input:
1. The program will receive a .txt file which will be given to the program as an argument.
2. The txt file must have the following format:

    - The first line corresponds to the alphabet with which the password(s) was/were written. It corresponds to a subset of ASCII characters.
    - The maximum amount of characters that the password can have.
    - A blank line for separation.
    - The fourth and subsequent lines correspond to a list of ZIP files that the password must be found for. Each line contains a file name and a path relative to the folder where its project is located.  
    - Example of a txt file:

     >0123456789  
     5  
  
     > tests/zip_05/f01.zip  
     tests/zip_05/f23.zip  
     tests/zip_05/f09.zip  

#### Program Output
1. For each zip file, the program will search and show the password:
    - Example of program output:  
    >
        tests/zip_05/f01.zip 00112  
        tests/zip_05/f23.zip  
        tests/zip_05/f09.zip 9209  

### Implemented Functions:
> - Password Generation
> - Password Verification
> - File Reading
> - Error handling in zip files and txt files

### Functionalities not Implemented:
> - UML diagrams were not created
> - The program has memory leaks that have not been resolved.
> - The program takes a long time to generate passwords.

### Program compilation
> - Using a terminal, navigate to the project folder (../zippass_serial)".
> - Use the command 'make' to compile the program".
### How to use it:
> - Using a terminal, and within the project folder (../zippass_serial), type the command:
"bin/zippass_serial" and add a file that you want to test, for example "tests/input001.txt". For example: bin/zippas_serial tests/input001.txt

### Credits
> - This program utilized a code block sourced from
> <https://stackoverflow.com/questions/23044184/c-or-c-combination-with-repetition>, which was created by the user BLUEPIXY
