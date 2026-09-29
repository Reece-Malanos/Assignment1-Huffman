#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>

void printMenu () {
    std::cout << '\n' << "\nMenu Options" << std::endl;
    std::cout << "1. Compress File" << std::endl;
    std::cout << "2. DeCompress File" << std::endl;
    std::cout << "3. Display File Contents" << std::endl;
    std::cout << "4. Please Close this Program" << std::endl;
}



void compressFile(std::ifstream& file) { 
    std::unordered_map<char,int> frequency;
    char letter;            // first we need to get the frequency of all the characters in the text file. 
    while (file.get(letter)){
        frequency[letter]++;
    }
    file.clear();
    file.seekg(0);


    for(const auto& [character,count] : frequency) {
        std::cout << "'" << character << "': " << count << std::endl;    
        }
}








void decompressFile(std::ifstream& file) {
    std::ignore = file;
}









void displayContents (std::ifstream& file) {
    char letter;
    while (file.get(letter)){
        std::cout << letter;
    }
    file.clear();
    file.seekg(0);
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
            compressFile(file);
        }

        if (selection == 2){
            decompressFile(file);
        }

        if (selection == 3){
            displayContents(file);
        }
    }
    std::cout << "Tank you, good bye." << std::endl;
    return 0;
}
