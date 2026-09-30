A)

I've built a functional example of a Huffman Compression Algorithm and given a couple of test.txt files to try out. The program uses an unordered map and priority queue to record the frequency of each character, then, after being turned into nodes, organises them by frequency and turns them into a binary tree.

The extra features in the tool, aside from compressing and decompressing, include viewing both the input and output files, comparing their contents byte by byte, and finally seeing the original size compared to the compressed size.

B)

The characters are first converted into nodes. Each node contains a letter, frequency, ChildL pointer, and ChildR pointer. This allows the nodes to be connected together, as each new parent node gets created, two child nodes get removed but are still pointed to.

All nodes are placed in a priority queue, with the smallest frequency value at the top. The two lowest frequencies are popped, added to a new parent, and then put back into the queue. This cycle is repeated until only one node remains.

While this creates the binary tree, a separate function, binarycharactermap, traverses the whole tree and starts mapping where each letter is through each child, assigning 0s for ChildL and 1s for ChildR. I print the frequencies and character + code assignments when doing the compression, as it's interesting (to me) to see how common letters, like vowels, have significantly shorter code assignments.

Lastly, the decompression function needs to know the exact layout of the binary tree before it can start reading off the 1s and 0s to rebuild the message. The two functions, readTree and writeTree, translate the entire binary tree into | starting at the root, I (expect two children) next, and L (expect a letter). This ensures the message is rebuilt using the identical tree.

test4.txt contains the entire Bee Movie script, while test3.txt contains the first initial self-made test text, which was my worked example and led me to some of my findings. text2.txt contains the assignment description page. try them out

C)

Initially, I thought I had messed something up since, when I compressed test3.txt, I ended up with a larger compressed file than the original text file. This led me to try a file containing just the letter "a" repeated a lot, and it successfully reduced the file size, so I knew the algorithm was working. I then tried the entire Bee Movie script, and it significantly reduced the file size.

I ultimately learned that, for my implementation at least, because I'm storing the character and the bit-code for the binary tree in the compressed .bin file, each new character ends up costing more than its initial cost until its repeated appearance in the message. So with every repeat character, a fractional amount of storage can be freed up to make the space back. It's only in really large texts that these fractional savings add up to big storage savings.

With a better implementation, by changing how I include the binary tree in the .bin file, I could optimise this code to see a compression advantage (original > compressed) sooner, rather than needing exceptionally large texts.

D) AI Help!

ChatGPT helped in 2 key areas.

ChatGPT created the sections about converting specifically the std::string that contained the translated compression message into 0's and 1's, but formatting it into real binary and not just 1's and 0's as bytes. It created the parts of the function that read and wrote the 1's and 0's to and from the compressed.bin file. Additionally, it mentioned that because the complete message broken down into bits might not fill the 8 bits in a byte, I had to fill out the byte, but then also write that information into the header so that my program wouldn't read whatever it filled with 0's and 1's as directions in my binary tree. This actually did happen for some of the earlier implementations. I was ending up with extra 'e's at the end of the texts and it really confused me, but testing with extra characters narrowed that down.

The second critical section that AI helped with was that, I was using the unordered_map as my "frequency table" of each letter, and including that as my header in the compressed .bin file. When decompressing, the function would then build its own binary tree based on those frequencies. However, this kept giving me jumbled garbarge. 

It turns out that since all my frequency values come from an unordered_map, when everything then goes into the priority queue to be ordered, it was only ordered by frequency and nothing else. Any ties for frequency caused the tree to be built differently depending on the order the nodes were added to the queue. Since they came from an unordered_map, this order was almost always wrong. 

ChatGPT's fix was to read and write the whole binary tree into the header of the file. However, now that I'm reading this, I'm wondering if I could have had a tiebreaker, ordering by frequency, then by letter, since the ASCII values would have put them in a repeatable order, and that probably would have saved me some bytes, instead of having to copy the entire Tree over, it also would have made the code neater and gotten rid of the tree read and tree write functions.