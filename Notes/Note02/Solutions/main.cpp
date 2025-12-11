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

    /*
    C -> S *
    S -> AB
    A -> 11A
    A -> 11
    B -> 0
    B -> 00B
    S -> B *
    -------------------
    C -> S | AB | B 
    S -> AB | B | 0 | OOB
    A -> 11A | 11
    B -> 0 | 00B
    ------------------- 
    C -> AB | B * 
    S -> AB | 0 | OOB
    A -> 11A | 11
    B -> 0 | 00B
    -------------------
    C -> AB | B | 0 | 00B 
    S -> AB | 0 | OOB
    A -> 11A | 11
    B -> 0 | 00B
    --------------------
    C -> AB | 0 | 00B 
    S -> AB | 0 | OOB
    A -> 11A | 11
    B -> 0 | 00B
    */
    tc::RemoveUnits(A,a);
    std::cout << "Remove Unit Rules:\n";
    std::cout << A << "\n";

    /*
    Lexical Rules
    C -> AB | 0 | 00B 
    S -> AB | 0 | 00B
    A -> 11A | 11
    B -> 0 | 00B
    --------------------
    C -> AB | 0 | DDB 
    S -> AB | 0 | DDB
    A -> 11A | 11
    B -> 0 | DDB
    D -> 0
    --------------------
    C -> AB | 0 | DDB 
    S -> AB | 0 | DDB
    A -> EEA | EE
    B -> 0 | DDB
    D -> 0
    E -> 1
    */
    tc::ConstructLexicals(A,a);
    std::cout << "Construct Lexical Rules:\n";
    std::cout << A << "\n";

    return 0;
}