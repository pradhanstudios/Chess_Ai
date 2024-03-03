#include "magic_values.hpp"

// const std::array<BB, 64> ROOK_MAGICS = {3602881900919717889, 1873506541737412736, 3460312648480989472, 3757268945666576392, 144120204782863396, 1513210033227175956, 1873570012820706568, 5206280466253091073, 5338454408294301696, 4756364435638199808, 27074582091401600, 583497798805095584, 4755819369921071106, 158329682998150144, 72849294218004482, 6917546628971413636, 2882974463610061056, 1188950513153089824, 920986136749310474, 8079616199153684480, 6205962486076942352, 2594150351448327297, 144119587566323728, 6953560024020959369, 1193737721282035776, 144115189150732314, 1587519418472078080, 2596360508332331032, 5102017421191680, 156500225029701892, 166650916106311168, 292887908480746796, 1225331492406689826, 1152921505714276624, 2900608448278299041, 2380577643483365392, 668785936236549128, 1446359760541582336, 124314674268226, 4611689180328689976, 220817257767763972, 6917529027642262032, 45053897865169921, 1585865276327464960, 4755829793925234704, 600210237890297890, 2920586592001458181, 2974067286082624, 3458836531851626752, 140737505134088, 1162526863974760464, 1193454047552143616, 563637433939456, 2309933334236758024, 432354434141520896, 598426568262144, 108791316271467010, 2305844108725322114, 302022719805850665, 85604711431963905, 1153067327472865294, 4613674493796237450, 434597846159852292, 1323813075782402};
// const std::array<BB, 64> ROOK_SHIFTS = {58, 52, 51, 51, 52, 52, 53, 51, 58, 54, 53, 53, 53, 54, 53, 52, 57, 54, 52, 53, 53, 53, 54, 53, 56, 55, 53, 53, 53, 53, 53, 52, 55, 57, 53, 53, 52, 54, 54, 52, 53, 58, 54, 53, 53, 53, 54, 52, 53, 59, 54, 54, 54, 53, 54, 53, 51, 59, 53, 53, 52, 52, 52, 51};
// const std::array<BB, 64> BISHOP_MAGICS = {4611969709611452480, 4612253367534632960, 563508307755008, 141356236410896, 1130435560079360, 282059159371779, 211382252355584, 317209708659712, 1134425034880, 2305985955389509668, 22007479549952, 576461045168647232, 1103940880385, 18176372899840, 4611695372883001600, 550301598724, 571814800523520, 1153071038733750576, 572025253134592, 71193919505536, 4539076329866244, 36204860615430160, 8934085640709, 2269393090257160, 4611976839274037280, 145960177042432, 5772436112941080, 2815300060348475, 5200602588222947872, 1126037652054020, 2252349586311168, 563087393456656, 282608848144456, 70678065840640, 4540038130180352, 1466097634964938784, 4611861944599892225, 294992372964540480, 142945101562120, 88514981036096, 70660869070888, 141016795451394, 18085901141803139, 137791285252, 36055223956931584, 141854196629729, 567554697330816, 4504703436064784, 2314889808134283264, 4611757487761264640, 1126175405768704, 562951060848642, 274919981088, 18157343678201856, 4645445225807872, 1154049328931672064, 141012912571016, 563508315980288, 4611686087283246096, 4567728640, 281475253666304, 8813411827776, 8804687284257, 283682594243072};
// const std::array<BB, 64> BISHOP_SHIFTS =  {56, 58, 58, 56, 59, 58, 57, 57, 57, 55, 59, 53, 58, 59, 57, 58, 58, 54, 57, 55, 56, 56, 56, 57, 56, 57, 56, 53, 52, 56, 59, 58, 57, 57, 56, 52, 53, 55, 58, 58, 57, 58, 56, 57, 57, 56, 57, 58, 56, 57, 58, 59, 59, 58, 58, 56, 56, 58, 57, 59, 59, 58, 57, 57};

// int calculate_index(BB input_bitboard, BB magic, int shift) {
//     BB out = input_bitboard;
//     out *= magic;
//     out >>= shift;
//     return out;
// }
BB _random_BB_helper()
{
    return random() | (random() << 32);
}

BB random_BB()
{
    return _random_BB_helper() & _random_BB_helper() & _random_BB_helper();
}

// inspired from https://github.com/Tearth/Fast-Magic-Bitboards/blob/master/FastMagicBitboards/Include/MagicStructure.hpp
magic_structure _find_magics_rook(int pos) {
    magic_structure m;
    m.mask = blocker_mask_rook(pos);
    std::array<BB, 4096> b_boards = blocker_boards(m.mask);
    std::array<BB, 4096> moveboards;
    const int count = real_count(m.mask);
    const int size = 1 << count;
    for (int i = 0; i < size; i++) {
        moveboards[i] = moveboard_rook(b_boards[i], pos);
    }
    m.shift = 64-count;

    magic_structure best_magic_structure;
    BB best_magic, magic, hash;
    int max_value, this_max;
    bool ismagic;
    max_value = INFINITY;
    for (int j = 0; j < 65535; j++) {
        ismagic = true;
        magic = random_BB();
        this_max = -1;
        std::fill(std::begin(m.attacks), std::end(m.attacks), 0);
        for (int k = 0; k < size; k++) {
            hash = (magic * b_boards[k]) >> m.shift;
            this_max = hash > this_max ? hash : this_max;
            if (m.attacks[hash] != 0 && m.attacks[hash] != moveboards[k])
			{
				ismagic = false;
				break;
			}

            m.attacks[hash] = moveboards[k];
        }

        if (ismagic && this_max < max_value) {
            m.magic = magic;
            best_magic_structure = m;
            max_value = this_max;
        }
    }
    // if (best_magic == 0) {
    //     std::cout << "it did not work" << std::endl;
    // }
    return best_magic_structure;
}

magic_structure _find_magics_bishop(int pos) {
    magic_structure m;
    m.mask = blocker_mask_bishop(pos);
    std::array<BB, 4096> b_boards = blocker_boards(m.mask);
    std::array<BB, 4096> moveboards;
    const int count = real_count(m.mask);
    const int size = 1 << count;
    for (int i = 0; i < size; i++) {
        moveboards[i] = moveboard_bishop(b_boards[i], pos);
    }
    m.shift = 64-count;

    magic_structure best_magic_structure;
    BB magic, hash;
    int max_value, this_max;
    bool ismagic;
    max_value = INFINITY;
    for (int j = 0; j < 65535; j++) {
        ismagic = true;
        magic = random_BB();
        this_max = -1;
        std::fill(std::begin(m.attacks), std::end(m.attacks), 0);
        for (int k = 0; k < size; k++) {
            hash = (magic * b_boards[k]) >> m.shift;
            this_max = hash > this_max ? hash : this_max;
            if (m.attacks[hash] != 0 && m.attacks[hash] != moveboards[k])
			{
				ismagic = false;
				break;
			}

            m.attacks[hash] = moveboards[k];
        }

        if (ismagic && this_max < max_value) { // basically we are trying to get the magic number which results in the smallest size
            m.magic = magic;
            best_magic_structure = m;
            // best_magic_structure.attacks = m.attacks;
            max_value = this_max;
        }
    }
    // if (best_magic == 0) {
    //     std::cout << "it did not work" << std::endl;
    // }
    // m.magic = best_magic;
    return best_magic_structure;
}

// similar to implementation from https://github.com/Usama-Azad/Pickle-in-Cpp

// void _save(std::array<magic_structure, 64> array, std::string filename) {

//     // Open file for writing
//     std::ofstream outfile(filename, std::ios::binary);

//     // Check if file opened successfully
//     // if (!outFile) {
//     //     std::cerr << "Error opening file!" << std::endl;
//     //     return;
//     // }

//     // Write array elements to file
//     for (int i = 0; i < 64; i++)
//         outfile.write((const char *)&array[i], sizeof(array[i]));

//     // Close file
//     outfile.close();
// }

void generate_magics_and_save() {
    // _find_magics_rook(27);
    // _find_magics_bishop(27);
    // print_BB(blocker_mask_bishop(27));
    std::array<magic_structure, 64> RK_MAGICS, BP_MAGICS;
    std::cout << "starting" << std::endl;
    for (int i = 0; i < 64; i++) {
        RK_MAGICS[i] = _find_magics_rook(i);
        BP_MAGICS[i] = _find_magics_bishop(i);
        std::cout << "done with " << i << std::endl;
    }
    std::ofstream save_rook_magics("../magics/ROOK_MAGICS.bin", std::ios::binary);
    for (int i = 0; i < 64; i++) {
        std::cout << "saved rook magics " << i << std::endl;
        save_rook_magics.write((const char *)&RK_MAGICS[i], sizeof(RK_MAGICS[i]));
    }
    // Close file
    save_rook_magics.close();
    std::ofstream save_bishop_magics("../magics/BISHOP_MAGICS.bin", std::ios::binary);
    for (int i = 0; i < 64; i++) {
        std::cout << "saved bishop magics " << i << std::endl;
        save_bishop_magics.write((const char *)&BP_MAGICS[i], sizeof(BP_MAGICS[i]));
    }
    // Close file
    save_bishop_magics.close();
}

void load_magics(std::array<magic_structure, 64> &ROOK_MAGICS, std::array<magic_structure, 64> &BISHOP_MAGICS) {
    std::ifstream rook_file("../magics/ROOK_MAGICS.bin", std::ios::binary);

    for (int i = 0; i < 64; i++)
        rook_file.read((char *)&ROOK_MAGICS[i], sizeof(ROOK_MAGICS[i]));
    rook_file.close();

    // std::cout << ROOK_MAGICS[2].magic << std::endl;

    std::ifstream bishop_file("../magics/BISHOP_MAGICS.bin", std::ios::binary);

    for (int i = 0; i < 64; i++)
        bishop_file.read((char *)&BISHOP_MAGICS[i], sizeof(BISHOP_MAGICS[i]));

    bishop_file.close();
    // std::cout << "got here" << std::endl;
}

BB get_sliding_moves(BB current_position, BB friendlies, int pos, magic_structure magic) {
    // fast_reverse_bit(current_position, pos); // get rid of the piece itself
    // for (BB b : magic.attacks) {
    //     if (b) {
    //         return b;
    //     }
    // }
    // print_BB(current_position);
    // print_BB((current_position ^ (1 << pos)));
    return magic.attacks[((current_position & magic.mask) * magic.magic) >> magic.shift] & ~friendlies;
}