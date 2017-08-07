#include <iostream>
#include <vector>

using namespace std;

class grid {
public:
    grid() {}
    ~grid() {}

    bool is_mine() const { return m_is_mine; }

    friend istream & operator >> (std::istream &is, grid &rhs);

private:
    bool m_is_mine;
};

istream & operator >> (std::istream &is, grid &rhs) {
    char token = '.';
    is >> token;
    rhs.m_is_mine = token == '*';
    return is;
};

class game_field {
public:
    game_field(size_t size) : m_size(size), m_grids(size*size) {};
    ~game_field() {};

    size_t size() const { return m_size; }
    bool is_mine(size_t x, size_t y) const;

    friend istream & operator >> (std::istream &is, game_field &rhs);

private:
    size_t m_size;
    vector<grid> m_grids;
};

bool game_field::is_mine(size_t x, size_t y) const {
    if(x < 0 || x >= m_size || y < 0 || y >= m_size) return false;
    size_t position = y*m_size + x;
    return m_grids[position].is_mine();
}

istream & operator >> (std::istream &is, game_field &rhs) {
    for(size_t g = 0; g < rhs.m_grids.size(); ++g) {
        is >> rhs.m_grids[g];
    }
    return is;
};

class sweeper {
public:
    sweeper(const game_field& field) : m_field(field), m_report(field.size() * field.size()), m_exploded(false) {}
    ~sweeper() {}

    void sweep(size_t x, size_t y);
    void peek(size_t x, size_t y);
    void get_report(ostream& os);
    size_t field_size() const { return m_field.size(); }

private:
    const game_field& m_field;
    vector<char> m_report;
    bool m_exploded;
};

void sweeper::sweep(size_t x, size_t y) {
    size_t position = y * m_field.size() + x;
    if(m_field.is_mine(x, y)) {
        m_report[position] = '*';
        m_exploded = true;
    }
    else {
        m_report[position] = '0';
        if(m_field.is_mine(x-1, y-1)) m_report[position]++;
        if(m_field.is_mine(x, y-1)) m_report[position]++;
        if(m_field.is_mine(x+1, y-1)) m_report[position]++;
        if(m_field.is_mine(x-1, y)) m_report[position]++;
        if(m_field.is_mine(x+1, y)) m_report[position]++;
        if(m_field.is_mine(x-1, y+1)) m_report[position]++;
        if(m_field.is_mine(x, y+1)) m_report[position]++;
        if(m_field.is_mine(x+1, y+1)) m_report[position]++;
    }
}

void sweeper::peek(size_t x, size_t y) {
    m_report[y * field_size() + x] = m_field.is_mine(x, y) ? '*' : '.';
}

void sweeper::get_report(ostream& os) {
    for(size_t r = 0; r < m_report.size(); ++r) {
        os << ((m_report[r] == '*' && !m_exploded) ? '.' : m_report[r]);
        if((r+1) % field_size() == 0) os << endl;
    }
}

istream & operator >> (std::istream &is, sweeper &rhs) {
    for(size_t y = 0; y < rhs.field_size(); ++y) {
        for(size_t x = 0; x < rhs.field_size(); ++x) {
            char operation = '.';
            is >> operation;
            if(operation == 'x') {
                rhs.sweep(x, y);
            }
            else {
                rhs.peek(x, y);
            }
        }
    }
    return is;
};

void test_case() {
    size_t field_size = 0;
    cin >> field_size;
    game_field field(field_size);
    cin >> field;
    sweeper explorer(field);
    cin >> explorer;
    explorer.get_report(cout);
}

int main() {
    size_t case_count = 0;
    cin >> case_count;
    for(size_t c = 0; c < case_count; ++c) {
        test_case();
        if(c+1 != case_count) cout <<endl;
    }
}