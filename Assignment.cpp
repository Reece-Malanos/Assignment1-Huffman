#include <iostream>

void printMenu () {
    std::cout << "Menu Options" << std::endl;
    std::cout << "1. Decompress File" << std::endl;
    std::cout << "2. Compress File" << std::endl;
    std::cout << "3. Display File Contents" << std::endl;
    std::cout << "4. Please Close this Program" << std::endl;
}



void encryptFile () {
}




void decryptFile () {
}





void displayContents () {
}




int main () {
    int selection = 0;
    while(selection != 4) {
        printMenu();
        std::cin >> selection;
        if (selection = 1){}
        if (selection = 2){}
        if (selection = 3){}
    }
    std::cout << "Tank you, good bye." << std::endl;
    return 1;
}
