#include "CNF.h"

int main()
{
    tc::Set<tc::Rule> A;
    A.insert(tc::Rule('S',"AB"));
    A.insert(tc::Rule('A',"."));
    A.insert(tc::Rule('A',"11A"));
    A.insert(tc::Rule('B',"0"));
    A.insert(tc::Rule('B',"00B"));

    std::cout << "Original Grammar:\n";
    std::cout << A << "\n";
    std::string a = "CDEFGHIJKLMNOPQRTUVWXYZ";

    tc::NewStart(A,a);
    std::cout << "New Start Variable:\n";
    std::cout << A << "\n";

    tc::RemoveEpsilons(A,a);
    std::cout << "Remove Epsilon Rules:\n";
    std::cout << A << "\n";

    tc::RemoveUnits(A,a);
    std::cout << "Remove Unit Rules:\n";
    std::cout << A << "\n";

    tc::ConstructLexicals(A,a);
    std::cout << "Construct Lexical Rules:\n";
    std::cout << A << "\n";

    return 0;
}