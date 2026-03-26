#include <bits/stdc++.h>
using namespace std;

const int ROUNDS = 2;
const int SBOX_SIZE = 16;
const int BITS = 4;

// Example S-box (Sypher004 style – replace if needed)
int sbox[SBOX_SIZE] = {
    0xC, 0x5, 0x6, 0xB,
    0x9, 0x0, 0xA, 0xD,
    0x3, 0xE, 0xF, 0x8,
    0x4, 0x7, 0x1, 0x2
};

// DDT
int ddt[SBOX_SIZE][SBOX_SIZE];

void computeDDT() {
    for(int i = 0; i < SBOX_SIZE; i++) {
        for(int j = 0; j < SBOX_SIZE; j++) {
            int dx = i ^ j;
            int dy = sbox[i] ^ sbox[j];
            ddt[dx][dy]++;
        }
    }
}

string varX(int r, int b) {
    return "x_" + to_string(r) + "_" + to_string(b);
}

string varY(int r, int b) {
    return "y_" + to_string(r) + "_" + to_string(b);
}

string varA(int r) {
    return "a_" + to_string(r); // active S-box
}

int main() {
    computeDDT();

    ofstream file("model.lp");

    // Objective
    file << "Minimize\n obj: ";
    for(int r = 0; r < ROUNDS; r++) {
        file << varA(r);
        if(r != ROUNDS - 1) file << " + ";
    }
    file << "\n";

    file << "\nSubject To\n";

    // Activity constraints
    for(int r = 0; r < ROUNDS; r++) {
        for(int b = 0; b < BITS; b++) {
            file << varX(r, b) << " - " << varA(r) << " <= 0\n";
        }
    }

    // Non-zero input constraint
    file << "non_zero: ";
    for(int b = 0; b < BITS; b++) {
        file << varX(0, b);
        if(b != BITS - 1) file << " + ";
    }
    file << " >= 1\n";

    // Simple S-box transition constraints (DDT-based)
    for(int r = 0; r < ROUNDS; r++) {
        for(int dx = 0; dx < SBOX_SIZE; dx++) {
            for(int dy = 0; dy < SBOX_SIZE; dy++) {
                if(ddt[dx][dy] == 0) {
                    // forbid impossible transitions
                    file << "c_" << r << "_" << dx << "_" << dy << ": ";
                    
                    for(int b = 0; b < BITS; b++) {
                        if((dx >> b) & 1)
                            file << varX(r, b) << " + ";
                    }

                    for(int b = 0; b < BITS; b++) {
                        if((dy >> b) & 1)
                            file << varY(r, b) << " + ";
                    }

                    file << " <= " << (__builtin_popcount(dx) + __builtin_popcount(dy) - 1) << "\n";
                }
            }
        }
    }

    // Linear layer (simple identity for demo)
    for(int r = 0; r < ROUNDS - 1; r++) {
        for(int b = 0; b < BITS; b++) {
            file << varY(r, b) << " - " << varX(r + 1, b) << " = 0\n";
        }
    }

    // Binary variables
    file << "\nBinary\n";
    for(int r = 0; r < ROUNDS; r++) {
        for(int b = 0; b < BITS; b++) {
            file << varX(r, b) << "\n";
            file << varY(r, b) << "\n";
        }
        file << varA(r) << "\n";
    }

    file << "End\n";

    file.close();

    cout << "MILP model generated: model.lp\n";
    return 0;
}