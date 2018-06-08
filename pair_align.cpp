#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstring>

std::string readFileReversed(const std::string& filename) {
    std::ifstream ifs(filename);
    std::stringstream ss;
    ss << ifs.rdbuf();
    const std::string& str = ss.str();

    std::string reversed;
    reversed.reserve(str.size());
    for (auto it = str.rbegin(); it != str.rend(); ++it) {
        if (isalpha(*it)) {
            reversed.push_back(toupper(*it));
        }
    }
    return reversed;
}

static const int GAP_COST = 2;
static const int MATCH_SCORE = 2;
static const int MISMATCH_SCORE = -1;

int main(int argc, char** argv) {
    std::string seqA, seqB;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-seq1") == 0) {
            seqA = readFileReversed(argv[++i]);
        } else if (std::strcmp(argv[i], "-seq2") == 0) {
            seqB = readFileReversed(argv[++i]);
        }
    }

    const int n = seqA.size() + 1;
    const int m = seqB.size() + 1;

    enum class Direction {
        UpLeft,
        Up,
        Left
    };
    struct Cell {
        int score;
        Direction from;
    };

    std::vector<Cell> cells(n * m);
    const auto F = [&cells, &n](int x, int y) -> Cell& {
        return cells.at(x + n * y);
    };
    for (int i = 0; i < n; ++i) {
        F(i, 0).score = -i * GAP_COST;
    }
    for (int j = 0; j < m; ++j) {
        F(0, j).score = -j * GAP_COST;
    }
    for (int j = 1; j < m; ++j) {
        for (int i = 1; i < n; ++i) {
            const int upleft = F(i - 1, j - 1).score
                + (seqA.at(i - 1) == seqB.at(j - 1) ? MATCH_SCORE : MISMATCH_SCORE);
            const int up = F(i, j - 1).score - GAP_COST;
            const int left = F(i - 1, j).score - GAP_COST;
            if (upleft >= up && upleft >= left) {
                F(i, j).score = upleft;
                F(i, j).from = Direction::UpLeft;
            } else if (up >= upleft && up >= left) {
                F(i, j).score = up;
                F(i, j).from = Direction::Up;
            } else {
                F(i, j).score = left;
                F(i, j).from = Direction::Left;
            }
        }
    }

    int i = n - 1;
    int j = m - 1;
    std::string alignedA, alignedB;
    while (i > 0 && j > 0) {
        switch (F(i, j).from) {
            case Direction::UpLeft:
                alignedA += seqA.at(i - 1);
                alignedB += seqB.at(j - 1);
                --i;
                --j;
                break;
            case Direction::Up:
                alignedA += '-';
                alignedB += seqB.at(j - 1);
                --j;
                break;
            case Direction::Left:
                alignedA += seqA.at(i - 1);
                alignedB += '-';
                --i;
                break;
        }
    }
    std::cout << "Score: " << F(n - 1, m - 1).score << std::endl
        << alignedA << std::endl
        << alignedB << std::endl;
}
