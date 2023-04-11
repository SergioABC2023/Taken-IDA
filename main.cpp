#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <ctime>

using namespace std;

const int N = 4;
const int infinito = numeric_limits<int>::max();

// Funcion para realizar la estructura
struct State {
    int board[N][N];
    int x, y;

    State() {

        int val = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                board[i][j] = val++;
            }
        }
        board[N-1][N-1] = 0;
        x = N-1;
        y = N-1;
    }


    bool operator==(const State& other) const {
        bool isEqual = true;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] != other.board[i][j]) {
                    return false;
                }
            }
        }
        return isEqual;
    }


    void print() const {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

// Función de la distancia Manhattan
int Manhattan(const State& state) {
    int distancia = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int value = state.board[i][j];
            if (value != 0) {
                int targetX = (value - 1) / N;
                int targetY = (value - 1) % N;
                distancia += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return distancia;
}


struct Move {
int dx, dy;
};

// Función para  los sucesores de un estado
vector<State> successors(const State& state) {
    vector<State> succ;
    int x = state.x;
    int y = state.y;
    vector<Move> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto& move : moves) {
        int new_x = x + move.dx;
        int new_y = y + move.dy;
        if (new_x < 0 || new_x >= N || new_y < 0 || new_y >= N) {
            continue;
    }
        State s = state;
        swap(s.board[x][y], s.board[new_x][new_y]);
        s.x = new_x;
        s.y = new_y;
        succ.push_back(s);
    }
    return succ;
}

// Función para la profundidad
int search(State state, int g, int bound, int (*Manhattan)(const State&), int& min_cost) {
    int f = g + Manhattan(state);
    if (f > bound) {
        return f;
    }
    if (Manhattan(state) == 0) {
        return -1;
    }
    int min_t = infinito;
    vector<State> succ = successors(state);
    for (State s : succ) {
        if (s == state) {
            continue;
        }
        int t = search(s, g+1, bound, Manhattan, min_cost);
        if (t == -1) {
            return -1;
        } else if (t < min_t) {
            min_t = t;
        }
    }
    if (min_t == infinito) {
        return infinito;
    }
    if (min_t < min_cost) {
        min_cost = min_t;
    }
    return min_t;
}

// Función IDA*
int ida(State state, int (*Manhattan)(const State&)) {
    int bound = Manhattan(state);
    int min_cost = infinito;
    while (true) {
        int t = search(state, 0, bound, Manhattan, min_cost);
        if (t == -1) {
            return min_cost;
        } else if (t == infinito) {
            return -1;
        } else {
            bound = t;
        }
    }
}



int main() {

    State initial;

    // Funcion para hacer el tablero aleatoriamente
    srand(time(nullptr));
    for (int i = 0; i < 100; i++) {
        vector<State> succ = successors(initial);
        initial = succ[rand() % succ.size()];
    }

    cout << "Tablero inicial:" << endl;
    initial.print();

    int movimientos = ida(initial, Manhattan);

    if (movimientos == -1) {
        cout << "No tiene solucion." << endl;
    } else {
        cout << "Se logro realizar con exito en " << movimientos << " movimientos" << endl;
    }

    return 0;
}
