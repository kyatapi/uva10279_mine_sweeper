#include <iostream>
#include <vector>

using namespace std;

class game_field {
public:
    game_field(size_t size) : m_grids(size, vector<char>(size)) {};
    ~game_field() {};

    size_t size() const { return m_grids.size(); }
    bool is_mine(size_t x, size_t y) const { return (x >= 0 && x < size() && y >= 0 && y < size()) ? (m_grids[x][y] == '*') : false; }

    friend istream & operator >> (std::istream &is, game_field &rhs);

private:
    vector< vector<char> > m_grids;
};

istream & operator >> (std::istream &is, game_field &rhs) {
    for(size_t y = 0; y < rhs.size(); ++y) {
        for(size_t x = 0; x < rhs.size(); ++x) {
            is >> rhs.m_grids[x][y];
        }
    }
    return is;
};

class sweeper {
public:
    sweeper(const game_field& field) : m_field(field), m_report(field.size(), vector<char>(field.size())), m_exploded(false) {}
    ~sweeper() {}

    void read_instruction(istream& is);
    void get_report(ostream& os);
    size_t field_size() const { return m_field.size(); }

private:
    void sweep(size_t x, size_t y);
    void peek(size_t x, size_t y);

private:
    const game_field& m_field;
    vector< vector<char> > m_report;
    bool m_exploded;
};

void sweeper::sweep(size_t x, size_t y) {
    if(m_field.is_mine(x, y)) {
        m_report[x][y] = '*';
        m_exploded = true;
    }
    else {
        m_report[x][y] = '0';
        if(m_field.is_mine(x-1, y-1)) m_report[x][y]++;
        if(m_field.is_mine(x, y-1)) m_report[x][y]++;
        if(m_field.is_mine(x+1, y-1)) m_report[x][y]++;
        if(m_field.is_mine(x-1, y)) m_report[x][y]++;
        if(m_field.is_mine(x+1, y)) m_report[x][y]++;
        if(m_field.is_mine(x-1, y+1)) m_report[x][y]++;
        if(m_field.is_mine(x, y+1)) m_report[x][y]++;
        if(m_field.is_mine(x+1, y+1)) m_report[x][y]++;
    }
}

void sweeper::peek(size_t x, size_t y) {
    m_report[x][y] = m_field.is_mine(x, y) ? '*' : '.';
}

void sweeper::get_report(ostream& os) {
    for(size_t y = 0; y < m_report.size(); ++y) {
        for(size_t x = 0; x < m_report.size(); ++x) {
            os << ((m_report[x][y] == '*' && !m_exploded) ? '.' : m_report[x][y]);
        }
        os << endl;
    }
}

void sweeper::read_instruction(std::istream &is) {
    for(size_t y = 0; y < field_size(); ++y) {
        for(size_t x = 0; x < field_size(); ++x) {
            is >> m_report[x][y];
            (m_report[x][y] == 'x') ? sweep(x, y) : peek(x, y);
        }
    }
};

void test_case() {
    size_t field_size = 0;
    cin >> field_size;
    game_field field(field_size);
    cin >> field;
    sweeper explorer(field);
    explorer.read_instruction(cin);
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