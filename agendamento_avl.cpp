#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

struct BookingNode {
    int start_time, end_time, height;
    string event_name;
    BookingNode *left, *right;
    BookingNode(int s, int e, string n)
        : start_time(s), end_time(e), height(1), event_name(n), left(nullptr), right(nullptr) {}
};

class SchedulerAVL {
    BookingNode* root = nullptr;
    static int height(BookingNode* n) { return n ? n->height : 0; }
    static int getBalance(BookingNode* n) { return n ? height(n->left) - height(n->right) : 0; }
    static void updateHeight(BookingNode* n) { n->height = 1 + max(height(n->left), height(n->right)); }
    static BookingNode* rotateRight(BookingNode* y) {
        BookingNode* x = y->left;
        BookingNode* middle = x->right;
        x->right = y;
        y->left = middle;
        updateHeight(y);
        updateHeight(x);
        return x;
    }
    static BookingNode* rotateLeft(BookingNode* x) {
        BookingNode* y = x->right;
        BookingNode* middle = y->left;
        y->left = x;
        x->right = middle;
        updateHeight(x);
        updateHeight(y);
        return y;
    }
    static BookingNode* insert(BookingNode* node, int start, int end, const string& name) {
        if (!node) return new BookingNode(start, end, name);
        if (start < node->start_time) node->left = insert(node->left, start, end, name);
        else if (start > node->start_time) node->right = insert(node->right, start, end, name);
        else return node;
        updateHeight(node);
        int fb = getBalance(node);
        if (fb > 1 && start < node->left->start_time) return rotateRight(node);
        if (fb < -1 && start > node->right->start_time) return rotateLeft(node);
        if (fb > 1 && start > node->left->start_time) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (fb < -1 && start < node->right->start_time) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }
    // Reservas armazenadas nao se sobrepoem; a busca percorre um unico ramo.
    static bool hasConflict(BookingNode* node, int start, int end) {
        while (node) {
            if (start < node->end_time && node->start_time < end) return true;
            if (end <= node->start_time) node = node->left;
            else node = node->right;
        }
        return false;
    }
    static void printAgenda(BookingNode* n) {
        if (!n) return;
        printAgenda(n->left);
        cout << setfill('0') << setw(2) << n->start_time / 60 << ':' << setw(2) << n->start_time % 60
             << " - " << setw(2) << n->end_time / 60 << ':' << setw(2) << n->end_time % 60
             << setfill(' ') << " | " << n->event_name << '\n';
        printAgenda(n->right);
    }
    static void destroy(BookingNode* n) {
        if (!n) return;
        destroy(n->left); destroy(n->right); delete n;
    }
public:
    ~SchedulerAVL() { destroy(root); }
    bool has_conflict(int start, int end) const { return hasConflict(root, start, end); }
    bool schedule(int start, int end, const string& name) {
        if (start < 0 || end > 1440 || start >= end || name.empty() || has_conflict(start, end)) return false;
        root = insert(root, start, end, name);
        return true;
    }
    void agenda() const {
        if (!root) { cout << "Agenda vazia.\n"; return; }
        printAgenda(root);
    }
};

bool parseTime(const string& s, int& minutes) {
    if (s.size() != 5 || s[2] != ':') return false;
    for (int i : {0, 1, 3, 4}) if (s[i] < '0' || s[i] > '9') return false;
    int h = (s[0] - '0') * 10 + s[1] - '0';
    int m = (s[3] - '0') * 10 + s[4] - '0';
    if (h > 23 || m > 59) return false;
    minutes = h * 60 + m;
    return true;
}

int main() {
    SchedulerAVL scheduler;
    scheduler.schedule(10 * 60, 11 * 60, "Daily Standup");
    scheduler.schedule(15 * 60, 17 * 60, "Planejamento Trimestral");
    string option;
    while (true) {
        cout << "\n=== AGENDAMENTO DE SALAS - AVL ===\n"
             << "1. Agendar Sala\n2. Ver Agenda do Dia\n3. Sair\nOpcao: ";
        if (!getline(cin, option)) break;
        if (option == "3") { cout << "Encerrando.\n"; break; }
        if (option == "2") { cout << "\n=== AGENDA DO DIA ===\n"; scheduler.agenda(); continue; }
        if (option != "1") { cout << "Opcao invalida.\n"; continue; }
        string name, begin, finish;
        cout << "Nome do evento: "; if (!getline(cin, name)) break;
        cout << "Inicio (HH:MM): "; if (!getline(cin, begin)) break;
        cout << "Fim (HH:MM): "; if (!getline(cin, finish)) break;
        int start, end;
        if (!parseTime(begin, start) || !parseTime(finish, end) || start >= end) {
            cout << "ERRO: Horario invalido. Use HH:MM e fim posterior ao inicio.\n";
        } else if (scheduler.has_conflict(start, end)) {
            cout << "ERRO: Conflito de horario! Sala ja reservada neste intervalo.\n";
        } else if (scheduler.schedule(start, end, name)) {
            cout << "Sala agendada com sucesso!\n";
        } else {
            cout << "ERRO: Nome do evento invalido.\n";
        }
    }
}
