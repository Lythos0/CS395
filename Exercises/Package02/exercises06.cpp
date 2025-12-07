#include "Utility.h"
#include <set>
#include <map>
using namespace tc;

bool isVariable(char c);
bool isTerminal(char c);
char getNewVariable(const Set<Rule>& rules, const std::string& available);

bool DefineNewStart(Set<Rule>& rules, const std::string& available) {
    if (rules.size() == 0) return false;
    
    char originalStart = rules[0].dom();
    char newStart = getNewVariable(rules, available);
    if (newStart == 0) return false;
    
    Rule newRule(newStart, std::string(1, originalStart));
    
    Set<Rule> newRules;
    newRules.first(newRule);
    for (size_t i = 0; i < rules.size(); i++) {
        newRules.insert(rules[i]);
    }
    
    rules = newRules;
    return true;
}

bool RemoveEpsilons(Set<Rule>& rules, const std::string& available) {
    if (rules.size() == 0) return false;
    
    std::set<char> epsilonVars;
    bool changed = true;
    
    while (changed) {
        changed = false;
        size_t oldSize = epsilonVars.size();
        
        for (size_t i = 0; i < rules.size(); i++) {
            Rule rule = rules[i];
            std::string rhs = rule.sub();
            
            if (rhs == ".") {
                if (epsilonVars.insert(rule.dom()).second) changed = true;
            } else {
                bool allEpsilon = true;
                for (char c : rhs) {
                    if (epsilonVars.find(c) == epsilonVars.end()) {
                        allEpsilon = false;
                        break;
                    }
                }
                if (allEpsilon && !rhs.empty()) {
                    if (epsilonVars.insert(rule.dom()).second) changed = true;
                }
            }
        }
        
        if (epsilonVars.size() != oldSize) changed = true;
    }
    
    char startVar = rules[0].dom();
    Set<Rule> newRules;
    
    for (size_t i = 0; i < rules.size(); i++) {
        Rule rule = rules[i];
        std::string rhs = rule.sub();
        
        if (rhs == ".") {
            if (rule.dom() == startVar) newRules.insert(rule);
            continue;
        }
        
        std::vector<int> epsilonPositions;
        for (size_t j = 0; j < rhs.size(); j++) {
            if (epsilonVars.find(rhs[j]) != epsilonVars.end()) {
                epsilonPositions.push_back(j);
            }
        }
        
        int n = epsilonPositions.size();
        int totalCombinations = 1 << n;
        
        for (int mask = 0; mask < totalCombinations; mask++) {
            std::string newRhs;
            for (size_t j = 0; j < rhs.size(); j++) {
                bool keep = true;
                for (int k = 0; k < n; k++) {
                    if (epsilonPositions[k] == j && (mask & (1 << k)) == 0) {
                        keep = false;
                        break;
                    }
                }
                if (keep) newRhs += rhs[j];
            }
            
            if (newRhs.empty()) {
                if (epsilonVars.find(rule.dom()) != epsilonVars.end()) {
                    Rule epsRule(rule.dom(), ".");
                    newRules.insert(epsRule);
                }
                continue;
            }
            
            Rule newRule(rule.dom(), newRhs);
            newRules.insert(newRule);
        }
    }
    
    rules = newRules;
    return true;
}

bool RemoveUnits(Set<Rule>& rules, const std::string& available) {
    if (rules.size() == 0) return false;
    
    bool changed;
    Set<Rule> currentRules = rules;
    
    do {
        changed = false;
        Set<Rule> newRules;
        
        for (size_t i = 0; i < currentRules.size(); i++) {
            const Rule& rule = currentRules[i];
            std::string rhs = rule.sub();
            
            if (!(rhs.size() == 1 && isVariable(rhs[0]))) {
                newRules.insert(rule);
            }
        }
        
        for (size_t i = 0; i < currentRules.size(); i++) {
            const Rule& rule = currentRules[i];
            std::string rhs = rule.sub();
            
            if (rhs.size() == 1 && isVariable(rhs[0])) {
                char targetVar = rhs[0];
                
                for (size_t j = 0; j < currentRules.size(); j++) {
                    const Rule& targetRule = currentRules[j];
                    
                    if (targetRule.dom() == targetVar && !targetRule.self()) {
                        Rule expandedRule(rule.dom(), targetRule.sub());
                        
                        bool exists = false;
                        for (size_t k = 0; k < newRules.size(); k++) {
                            if (newRules[k] == expandedRule) {
                                exists = true;
                                break;
                            }
                        }
                        
                        if (!exists) {
                            newRules.insert(expandedRule);
                            changed = true;
                        }
                    }
                }
            }
        }
        
        currentRules = newRules;
    } while (changed);
    
    rules = currentRules;
    return true;
}

bool ConstructLexicals(Set<Rule>& rules, const std::string& available) {
    if (rules.size() == 0) return false;
    
    Set<Rule> newRules = rules;
    std::map<char, char> terminalToVar;
    
    for (size_t i = 0; i < rules.size(); i++) {
        Rule rule = rules[i];
        std::string rhs = rule.sub();
        
        for (char c : rhs) {
            if (isTerminal(c) && terminalToVar.find(c) == terminalToVar.end()) {
                char newVar = getNewVariable(newRules, available);
                if (newVar == 0) continue;
                
                terminalToVar[c] = newVar;
                Rule terminalRule(newVar, std::string(1, c));
                newRules.insert(terminalRule);
            }
        }
    }
    
    Set<Rule> finalRules;
    
    for (size_t i = 0; i < newRules.size(); i++) {
        Rule rule = newRules[i];
        std::string rhs = rule.sub();
        
        if (rhs.size() == 1 && isTerminal(rhs[0])) {
            finalRules.insert(rule);
            continue;
        }
        
        std::string newRhs;
        for (char c : rhs) {
            if (isTerminal(c)) {
                auto it = terminalToVar.find(c);
                if (it != terminalToVar.end()) {
                    newRhs += it->second;
                } else {
                    newRhs += c;
                }
            } else {
                newRhs += c;
            }
        }
        
        if (!newRhs.empty()) {
            Rule newRule(rule.dom(), newRhs);
            finalRules.insert(newRule);
        }
    }
    
    rules = finalRules;
    return true;
}

bool ConstructBinarys(Set<Rule>& rules, const std::string& available) {
    if (rules.size() == 0) return false;
    
    Set<Rule> newRules;
    std::map<std::string, char> pairToVar;
    
    for (size_t i = 0; i < rules.size(); i++) {
        Rule rule = rules[i];
        std::string rhs = rule.sub();
        
        if (rhs.size() <= 2) {
            newRules.insert(rule);
            continue;
        }
        
        std::string currentRhs = rhs;
        char currentLhs = rule.dom();
        
        while (currentRhs.size() > 2) {
            std::string firstTwo = currentRhs.substr(0, 2);
            
            char pairVar;
            auto it = pairToVar.find(firstTwo);
            
            if (it != pairToVar.end()) {
                pairVar = it->second;
            } else {
                pairVar = getNewVariable(newRules, available);
                if (pairVar == 0) return false;
                
                pairToVar[firstTwo] = pairVar;
                Rule binaryRule(pairVar, firstTwo);
                newRules.insert(binaryRule);
            }
            
            currentRhs = std::string(1, pairVar) + currentRhs.substr(2);
        }
        
        Rule finalRule(currentLhs, currentRhs);
        newRules.insert(finalRule);
    }
    
    rules = newRules;
    return true;
}

bool isVariable(char c) {
    return std::isupper(c);
}

bool isTerminal(char c) {
    return std::islower(c) || std::isdigit(c);
}

char getNewVariable(const Set<Rule>& rules, const std::string& available) {
    for (char c : available) {
        bool used = false;
        for (size_t i = 0; i < rules.size(); i++) {
            if (rules[i].dom() == c) {
                used = true;
                break;
            }
        }
        if (!used) return c;
    }
    
    for (char c = 'A'; c <= 'Z'; c++) {
        bool used = false;
        for (size_t i = 0; i < rules.size(); i++) {
            if (rules[i].dom() == c) {
                used = true;
                break;
            }
        }
        if (!used) return c;
    }
    
    return 0;
}
