#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>

// Function to generate a random password of specified length
std::string generatePassword(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string password;
    for (int i = 0; i < length; ++i) {
        password += characters[distribution(generator)];
    }

    std::cout << "Generated password: " << password << std::endl; // Debug log
    return password;
}

int main() {
    // Parameters: modify these to control the output
    const int passwordLength = 12; // Length of each password
    const int totalPasswords = 1000000; // Total number of passwords to generate

    // Path and file name for saving passwords
    std::string outputFilePath = "./passwords.txt"; // Modify this path as needed

    std::cout << "Output file path: " << outputFilePath << std::endl; // Debug log

    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return 1;
    }

    std::cout << "Generating " << totalPasswords << " passwords..." << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= totalPasswords; ++i) {
        std::string password = generatePassword(passwordLength);
        outputFile << password << "\n";

        // Debug log for progress
        if (i % 10000 == 0) {
            std::cout << "Generated " << i << " passwords so far..." << std::endl;
        }

        // Print progress for every 100,000 passwords generated
        if (i % 100000 == 0 || i == totalPasswords) {
            std::cout << i << " of " << totalPasswords << " passwords generated." << std::endl;
        }
    }

    outputFile.close();

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    std::cout << "Password generation completed in " << elapsed.count() << " seconds." << std::endl;
    std::cout << "Passwords saved to " << outputFilePath << std::endl;

    // Prevent the terminal from closing immediately
    std::cout << "Press Enter to exit...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
