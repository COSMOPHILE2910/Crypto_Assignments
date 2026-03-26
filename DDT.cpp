#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<ctime>
using namespace std;

int main() {
    vector<int> sbox(16);
    vector<vector<int>> ddt(16, vector<int>(16, 0));

    // Step 1: Generate S-box
    for(int i = 0; i < 16; i++) {
        sbox[i] = i;
    }

    srand(time(0));
    random_shuffle(sbox.begin(), sbox.end());

    cout << "Program started...\n";   // 🔥 debug line

    // Print S-box
    cout << "S-box:\n";
    for(int i = 0; i < 16; i++) {
        cout << sbox[i] << " ";
    }
    cout << endl;

    // Step 2: Compute DDT
    for(int dx = 0; dx < 16; dx++) {
        for(int x = 0; x < 16; x++) {
            int x2 = x ^ dx;
            int dy = sbox[x] ^ sbox[x2];
            ddt[dx][dy]++;
        }
    }

    // Step 3: Print DDT
    cout << "\nDDT:\n";
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            cout << ddt[i][j] << " ";
        }
        cout << endl;
    }

    // Step 4: Differential uniformity
    int maxval = 0;
    for(int dx = 1; dx < 16; dx++) {
        for(int dy = 0; dy < 16; dy++) {
            if(ddt[dx][dy] > maxval) {
                maxval = ddt[dx][dy];
            }
        }
    }

    cout << "\nDifferential uniformity = " << maxval << endl;

    return 0;
}