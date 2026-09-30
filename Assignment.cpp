#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <filesystem>



struct Node {
    char letter;
    int frequency;
    Node* Left;
    Node* Right;
};

struct CompareFreq {
  bool operator()(Node* a, Node* b) {
    return a->frequency > b->frequency;
  }
};

void printMenu () {
    std::cout << '\n' << "\nMenu Options" << std::endl;
    std::cout << "1. Compress File" << std::endl;
    std::cout << "2. DeCompress File" << std::endl;
    std::cout << "3. Display File Contents" << std::endl;
    std::cout << "4. To compare contents of test.txt to decompressed.txt" << std::endl;
    std::cout << "5. Display Contents of decompressed file" << std::endl;
    std::cout << "6. Display Filesize of .bin vs original .txt" << std::endl;
    std::cout << "7. Please Close this Program" << std::endl << std::endl;
}

void binaryCharacterMap(Node* node, std::string binary, std::unordered_map<char, std::string>& map) {
    if(node == nullptr){return;}
    if(node->Left == nullptr && node->Right == nullptr)
    {
        map[node->letter] = binary.empty() ? "0" : binary; // any file that has only one unique character will get all zeros because the map won't build a tree because it doesn't need reduce to 1, its already 1 so it never really starts.
        return;
    }
    binaryCharacterMap(node->Left,binary + "0",map);
    binaryCharacterMap(node->Right,binary + "1",map);

}

void writeTree(Node* node, std::ofstream& output) {
    if (node == nullptr) {
        return;
    }

    // Leaf node
    if (node->Left == nullptr && node->Right == nullptr) {
        output.put('L');
        output.put(node->letter);
    }
    // Internal node
    else {
        output.put('I');
        writeTree(node->Left, output);
        writeTree(node->Right, output);
    }
}

Node* readTree(std::ifstream& input) {
    char type;

    input.get(type);

    if (type == 'L') {
        char letter;
        input.get(letter);

        return new Node{
            letter,
            0,
            nullptr,
            nullptr
        };
    }

    if (type == 'I') {
        Node* left = readTree(input);
        Node* right = readTree(input);

        return new Node{
            '\0',
            0,
            left,
            right
        };
    }

    return nullptr;
}




void compressFile(std::ifstream& file) { 
    std::ofstream output("compressed.bin", std::ios::binary);
    if(!output.is_open()){std::cout <<" can't make an output file" << std::endl; return;}
    std::unordered_map<char,int> frequency;
    char letter;            // first we need to get the frequency of all the characters in the text file. 
    while (file.get(letter)){
        frequency[letter]++;
    }
    file.clear();
    file.seekg(0);


    for(const auto& [character,count] : frequency) {
        std::cout << "'" << character << "': " << count << std::endl;    // just thought it would be cool to see the frequencies of each character. and new lines. also so i could test how easy it was to iterate through the unordered_map
        
        }

// QUEUE TIME :D 
    std::priority_queue<Node*,std::vector<Node*>,CompareFreq> TreeOfNodes;

    for(const auto& [character,count] : frequency) { // turned everything into nodes so i can actually build the binary tree paths.

        Node* node = new Node{
            character,
            count,
            nullptr,
            nullptr
        };
        TreeOfNodes.push(node);
        }
        auto testprint = TreeOfNodes;
        while(!testprint.empty()) { // i like seeing things happening.
            Node* printout = testprint.top();
            testprint.pop();
            std::cout << printout->letter << " appears " << printout->frequency << " times." << std::endl;
        }

        while(TreeOfNodes.size() > 1) {
            Node* ChildL = TreeOfNodes.top();
            TreeOfNodes.pop();
            Node* ChildR = TreeOfNodes.top();
            TreeOfNodes.pop();
            Node* Parent = new Node {
                '\0',
                ChildL->frequency + ChildR->frequency,
                ChildL,
                ChildR
            };
            TreeOfNodes.push(Parent);
        }
        
        if(TreeOfNodes.empty()){std::cout << "nice try blank file" << std::endl; return;}

        Node* Final = TreeOfNodes.top();
        
        std::unordered_map<char,std::string> binaryCodes;
        binaryCharacterMap(Final,"", binaryCodes);

        for(const auto& [character, code] : binaryCodes){
            std::cout << character << " -> " << code << std::endl; 
        } // this prints out each character and their 0 and 1 traversal assignments.


        file.clear();
        file.seekg(0);
        
        std::string textinbinary;
        while(file.get(letter)){
            textinbinary += binaryCodes[letter];
        }
        std::cout << std::endl << "Full Binary Code" << std::endl << textinbinary << std::endl;
        file.clear();
        file.seekg(0);

        // write frequency map to output file so it can be used to decompress later

        writeTree(Final, output);

        // and now to write the actual text in actual bits and bytes. 
        size_t numberOfBits = textinbinary.size();
        output.write(reinterpret_cast<const char*>(&numberOfBits), sizeof(numberOfBits));
        for (size_t i = 0; i < textinbinary.size(); i += 8) {
            unsigned char byte = 0;
            for (int j = 0; j < 8; j++) {
                byte <<= 1;
                if (i + j < textinbinary.size()) {
                    byte |= textinbinary[i + j] - '0';
                }
            }

        output.put(byte);
        }
        output.close();
    }
       







void decompressFile() {
    std::ifstream input("compressed.bin", std::ios::binary);

    if (!input.is_open()) {
        std::cout << "Could not open compressed file." << std::endl;
        return;
    }

    // Read the exact Huffman tree that was used during compression
    Node* Final = readTree(input);

    if (Final == nullptr) {
        std::cout << "Could not read Huffman tree." << std::endl;
        return;
    }

    // Read number of encoded bits
    size_t numberOfBits;

    input.read(
        reinterpret_cast<char*>(&numberOfBits),
        sizeof(numberOfBits)
    );

    // Read compressed bytes
    std::vector<unsigned char> compressedData(
        (numberOfBits + 7) / 8
    );

    input.read(
        reinterpret_cast<char*>(compressedData.data()),
        compressedData.size()
    );

    // Convert bytes back into bits
    std::string bits;

    for (unsigned char byte : compressedData) {
        for (int i = 7; i >= 0; i--) {
            bits += ((byte >> i) & 1) ? '1' : '0';
        }
    }

    bits.resize(numberOfBits);

    std::ofstream decompressed(
        "decompressed.txt",
        std::ios::binary
    );

    if (!decompressed.is_open()) {
        std::cout << "Could not create decompressed.txt" << std::endl;
        return;
    }

    // Special case: file contains only one unique character
    if (Final->Left == nullptr && Final->Right == nullptr) {

        for (size_t i = 0; i < numberOfBits; i++) {
            decompressed << Final->letter;
        }

        decompressed.close();
        input.close();

        return;
    }

    // Decode the bits
    Node* current = Final;

    for (char bit : bits) {

        if (bit == '0') {
            current = current->Left;
        }
        else {
            current = current->Right;
        }

        // We reached a character
        if (current->Left == nullptr &&
            current->Right == nullptr) {

            decompressed << current->letter;

            current = Final;
        }
    }

    decompressed.close();
    input.close();

    std::cout << "File successfully decompressed." << std::endl;
}










void displayContents (std::ifstream& file) {
    char letter;
    while (file.get(letter)){
        std::cout << letter;
    }
    file.clear();
    file.seekg(0);
}

void displayContentsDecompressed () {

    std::ifstream decompressed("decompressed.txt", std::ios::binary);

    if (!decompressed.is_open()) {
        std::cout << "Could not open decompressed.txt" << std::endl;
        return;
    }

    char letter;

    while (decompressed.get(letter)) {
        std::cout << letter;
    }

    decompressed.clear();
    decompressed.seekg(0);
}



bool compareFiles() {
    std::ifstream original("test.txt", std::ios::binary);
    std::ifstream decompressed("decompressed.txt", std::ios::binary);

    if (!original.is_open() || !decompressed.is_open()) {
        return false;
    }

    char originalChar;
    char decompressedChar;

    while (true) {
        bool originalRead = static_cast<bool>(original.get(originalChar));
        bool decompressedRead = static_cast<bool>(decompressed.get(decompressedChar));

        if (originalRead != decompressedRead) {
            return false;
        }

        if (!originalRead) {
            break;
        }

        if (originalChar != decompressedChar) {
            return false;
        }
    }

    return true;
}









int main () {
    std::string fileName = "test.txt";
    std::ifstream file(fileName);

    if(!file.is_open()){
        std::cout << "cannot find file called 'test.txt' " << std::endl;
        return 0;
    }



    int selection = 0;
    while(selection != 7) {
        printMenu();
        std::cin >> selection;
        if (selection == 1){
            compressFile(file);
        }

        if (selection == 2){
            decompressFile();
        }

        if (selection == 3){
            displayContents(file);
        }

        if (selection == 4){
            if (compareFiles()) {
                std::cout << "\nFiles are 100% identical!" << std::endl;
            } else {
                std::cout << "\nFiles are NOT identical!" << std::endl;
            }
        }
        if (selection == 5){
            displayContentsDecompressed();
        }

        if (selection == 6 ) {
            std::uintmax_t originalSize = std::filesystem::file_size("test.txt");
            std::uintmax_t compressedSize = std::filesystem::file_size("compressed.bin");
            std::cout << "\nOriginal file size:   " << originalSize << " bytes" << std::endl;
            std::cout << "Compressed file size: " << compressedSize << " bytes" << std::endl;
        }

        
    }
    std::cout << "Tank you, good bye." << std::endl;
    return 0;
}
