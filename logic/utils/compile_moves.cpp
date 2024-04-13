// #include "../sliding_moves.hpp"
// #include <iostream>

// const std::string FOLDER_PATH = "../moves/";

// // similar to implementation from https://github.com/Usama-Azad/Pickle-in-Cpp

// // template<typename T>
// // void save(T object, std::string filename) {
// //     std::ofstream file(FOLDER_PATH + filename, std::ios::binary);
// //     file.write((const char *)(&object), sizeof(object));
// //     file.close();
// // }

// void save1D(std::array<BB, 64> array, std::string filename) {

//     // Open file for writing
//     std::ofstream outFile(filename);

//     // Check if file opened successfully
//     if (!outFile) {
//         std::cerr << "Error opening file!" << std::endl;
//         return;
//     }

//     // Write array elements to file
//     for (int num : array) {
//         outFile << num << std::endl;
//     }

//     // Close file
//     outFile.close();
// }

// void save2D(std::array<std::array<BB, 4096>, 64> array, std::string filename) {
//     std::ofstream outFile(filename);
//     if (!outFile) {
//         std::cerr << "Error opening file " << filename << std::endl;
//         return;
//     }

//     for (const auto& row : array) {
//         for (int num : row) {
//             outFile << num << " ";
//         }
//         outFile << std::endl;
//     }
    
//     std::cout << "2D Array written to " << filename << " successfully." << std::endl;
// }


// std::array<std::array<BB, 4096>, 64> load2D(std::string filename) {
//     std::array<std::array<BB, 4096>, 64> array2D;
//     std::ifstream inFile(filename);
    
//     std::string line;
//     int row = 0;
//     while (getline(inFile, line) && row < 64) {
//         std::istringstream iss(line);
//         int col = 0;
//         int num;
//         while (iss >> num && col < 4096) {
//             array2D[row][col] = num;
//             col++;
//         }
//         row++;
//     }

//     return array2D;
// }

// int main() {

//     std::array<BB, 64> rook_masks, bishop_masks;
//     int i;
//     for (i = 0; i < 64; i++) {
//         rook_masks[i] = blocker_mask_rook(i);
//         bishop_masks[i] = blocker_mask_bishop(i);
//     }

//     // save<std::array<BB, 64>>(rook_masks, "ROOK_MASKS.bin");
//     // save<std::array<BB, 64>>(bishop_masks, "BISHOP_MASKS.bin");

//     std::array<std::array<BB, 4096>, 64> rook_moves, bishop_moves;
//     std::array<BB, 4096> rook_blockerboards, bishop_blockerboards;
//     int j;

//     for (i = 0; i < 64; i++) {
//         rook_blockerboards = blocker_boards(rook_masks[i]);
//         bishop_blockerboards = blocker_boards(bishop_masks[i]);
//         for (j = 0; j < (1 << real_count(rook_masks[i])); j++) {
//             rook_moves[i][j] = moveboard_rook(rook_blockerboards[j], i);
//             bishop_moves[i][j] = moveboard_bishop(bishop_blockerboards[j], i);
//         }
//     }
    
//     save1D(rook_masks, FOLDER_PATH+"ROOK_MASKS.txt");
//     save1D(bishop_masks, FOLDER_PATH+"BISHOP_MASKS.txt");
//     save2D(rook_moves, FOLDER_PATH+"ROOK_MOVES.txt");
//     save2D(bishop_moves, FOLDER_PATH+"BISHOP_MOVES.txt");


//     // save<std::array<std::array<BB, 4096>, 64>>(rook_moves, "ROOK_MOVES.bin");
//     // save<std::array<std::array<BB, 4096>, 64>>(bishop_moves, "BISHOP_MOVES.bin");

//     return 0;
// }