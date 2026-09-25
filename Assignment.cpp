#include <iostream>
#include <string>
#include <fstream>

void printMenu () {
    std::cout << "\nMenu Options" << std::endl;
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
    std::string fileName = "test.txt";
    std::ifstream file(fileName);

    if(!file.is_open()){
        std::cout << "cannot find file called 'test.txt' " << std::endl;
        return 0;
    }



    int selection = 0;
    while(selection != 4) {
        printMenu();
        std::cin >> selection;
        if (selection == 1){

        }

        if (selection == 2){

        }

        if (selection == 3){
            char letter;
            while (file.get(letter)){
                std::cout << letter;
            }

        }
    }
    std::cout << "Tank you, good bye." << std::endl;
    return 0;
}
