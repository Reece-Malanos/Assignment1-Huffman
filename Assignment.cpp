#include <iostream>

void printMenu () {
    std::cout << "Menu Options" << std::endl;
    std::cout << "1. Decompress File" << std::endl;
    std::cout << "2. Compress File" << std::endl;
    std::cout << "3. Display File Contents" << std::endl;
    std::cout << "4. Please Close this Program" << std::endl;
}


int main () {
    int selection = 0;
    while(selection != 4) {
        printMenu();
        std::cin >> selection;
        

    }

    std::cout << "Tank you, good bye." << std::endl;
}
