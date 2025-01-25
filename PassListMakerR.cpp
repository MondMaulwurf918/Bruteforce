#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <algorithm> // For std::count

// Define the character set for password generation
const std::string allCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQR"
                                  "STUVWXYZ0123456789!@#$%^&*()-_=+";

// Function to generate a random password
std::string generateRandomPassword(int length, const std::vector<char>& fixedChars,
                                   const std::string& validCharacters, const std::vector<int>& specificPositions,
                                   const std::vector<char>& specificChars) {
    std::string password(length, ' ');

    // Place specific characters at the specified positions
    for (size_t i = 0; i < specificPositions.size(); ++i) {
        password[specificPositions[i] - 1] = specificChars[i];  // Adjust for 1-based index
    }

    // Fill remaining positions with random characters
    for (int i = 0; i < length; ++i) {
        if (password[i] == ' ') { // Only fill if not already occupied
            password[i] = validCharacters[rand() % validCharacters.size()];
        }
    }

    return password;
}

// Function to check if the password meets the specified requirements
bool isValidPassword(const std::string& password, const std::vector<int>& specificPositions,
                     const std::vector<char>& specificChars, const std::vector<int>& characterCounts) {
    // Check specific positions
    for (size_t i = 0; i < specificPositions.size(); ++i) {
        if (password[specificPositions[i] - 1] != specificChars[i]) {
            return false;  // Incorrect character at the specified position
        }
    }

    // Check if the specified characters appear the correct number of times
    for (size_t i = 0; i < specificChars.size(); ++i) {
        int count = std::count(password.begin(), password.end(), specificChars[i]);
        if (count != characterCounts[i]) {
            return false;  // Incorrect count of the specified character
        }
    }

    return true;  // The password is valid
}

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    while (true) {
        int passwordLength;

        // Ask for the password length
        std::cout << "Enter the number of characters in each password: ";
        std::cin >> passwordLength;

        if (passwordLength <= 0) {
            std::cerr << "Error: Password length must be greater than 0.\n";
            continue;
        }

        // Ask for excluded characters
        std::string excludeChars;
        std::cout << "Enter characters to exclude from the password set (e.g., abc123): ";
        std::cin >> excludeChars;

        char addMoreExclusions;
        std::cout << "Do you want to add/select more characters to exclude? (y/n): ";
        std::cin >> addMoreExclusions;

        while (addMoreExclusions == 'y' || addMoreExclusions == 'Y') {
            std::cout << "Enter additional characters to exclude: ";
            std::string additionalExclusions;
            std::cin >> additionalExclusions;
            excludeChars += additionalExclusions; // Add more exclusions to the string

            std::cout << "Do you want to add/select more characters to exclude? (y/n): ";
            std::cin >> addMoreExclusions;
        }

        // Ask if the user wants to specify any positions for characters
        char wantsToSpecifyPosition;
        std::vector<int> specificPositions;

        std::cout << "Is there a position you want to specify a character for? (y/n): ";
        std::cin >> wantsToSpecifyPosition;

        if (wantsToSpecifyPosition == 'y' || wantsToSpecifyPosition == 'Y') {
            while (true) {
                int position;
                char character;

                std::cout << "Which position (1-" << passwordLength << "): ";
                std::cin >> position;

                if (position < 1 || position > passwordLength) {
                    std::cout << "Invalid position! Please enter a position between 1 and " << passwordLength << ".\n";
                    continue;
                }

                std::cout << "Which character should be placed at position " << position << ": ";
                std::cin >> character;

                specificPositions.push_back(position);

                char morePositions;
                std::cout << "Do you want to specify another position? (y/n): ";
                std::cin >> morePositions;
                if (morePositions != 'y' && morePositions != 'Y') {
                    break;
                }
            }
        }

        // Ask if the user wants to specify any character's amount
        char wantsToSpecifyCharacterAmount;
        std::vector<char> specificChars;
        std::vector<int> characterCounts;

        std::cout << "Is there any character you want to specify the amount for? (y/n): ";
        std::cin >> wantsToSpecifyCharacterAmount;

        if (wantsToSpecifyCharacterAmount == 'y' || wantsToSpecifyCharacterAmount == 'Y') {
            while (true) {
                char character;
                int count;

                std::cout << "Which character do you want to specify the amount for? ";
                std::cin >> character;

                std::cout << "How many times should the character '" << character << "' appear in the password? ";
                std::cin >> count;

                specificChars.push_back(character);
                characterCounts.push_back(count);

                char moreCharacters;
                std::cout << "Do you want to specify another character? (y/n): ";
                std::cin >> moreCharacters;
                if (moreCharacters != 'y' && moreCharacters != 'Y') {
                    break;
                }
            }
        }

        // Create the valid character set
        std::string validCharacters = allCharacters;

        // Manually remove excluded characters from the validCharacters string
        for (size_t i = 0; i < excludeChars.size(); ++i) {
            char excludeChar = excludeChars[i];
            std::string temp;
            for (size_t j = 0; j < validCharacters.size(); ++j) {
                if (validCharacters[j] != excludeChar) {
                    temp.push_back(validCharacters[j]);
                }
            }
            validCharacters = temp; // Reassign the validCharacters without the excluded character
        }

        std::cout << "Valid character set after exclusions: " << validCharacters << "\n";

        // Calculate total possible combinations
        size_t numUnknowns = passwordLength - specificPositions.size(); // Subtract the number of specified positions

        size_t totalCombinations = 1;
        for (size_t i = 0; i < numUnknowns; ++i) {
            totalCombinations *= validCharacters.size();
        }

        // Adjust the total combinations based on the specified character counts
        for (size_t i = 0; i < specificChars.size(); ++i) {
            totalCombinations /= characterCounts[i]; // Divide by the counts for each specified character
        }

        std::cout << "Total possible combinations with specified positions and character counts: " << totalCombinations << "\n";

        // Confirm with the user
        char confirmation;
        std::cout << "Proceed with generating passwords? (y/n): ";
        std::cin >> confirmation;

        if (confirmation != 'y' && confirmation != 'Y') {
            std::cout << "Restarting...\n";
            continue;
        }

        // Output file path
        const std::string outputFilePath = "./passwords.txt";

        std::ofstream outputFile(outputFilePath.c_str());
        if (!outputFile.is_open()) {
            std::cerr << "Error: Unable to open file for writing.\n";
            return 1;
        }

        // Generate passwords
        size_t generatedPasswords = 0;
        while (generatedPasswords < totalCombinations) {
            std::string password = generateRandomPassword(passwordLength, {}, validCharacters, specificPositions, specificChars);

            // Check if the generated password meets the requirements
            if (isValidPassword(password, specificPositions, specificChars, characterCounts)) {
                outputFile << password << "\n";
                ++generatedPasswords;
            }

            // Display progress every 500,000 passwords
            if (generatedPasswords % 500000 == 0) {
                std::cout << "\rGenerated " << generatedPasswords << " valid passwords...";
                std::cout.flush();
            }
        }

        outputFile.close();
        std::cout << "\nPassword generation completed.\n";
        std::cout << "Passwords saved to " << outputFilePath << "\n";

        break; // Exit the loop after generating passwords
    }

    return 0;
}

