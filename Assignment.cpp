#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <queue>



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
    std::cout << "4. Please Close this Program" << std::endl;
}

void binaryCharacterMap(Node* node, std::string binary, std::unordered_map<char, std::string>& map) {
    if(node == nullptr){return;}
    if(node->Left == nullptr && node->Right == nullptr)
    {
        map[node->letter] = binary;
        return;
    }
    binaryCharacterMap(node->Left,binary + "0",map);
    binaryCharacterMap(node->Right,binary + "1",map);

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
