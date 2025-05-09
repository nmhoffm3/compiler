#include <iostream>

#include <cstdio>

#include <cstdlib>

#include "lexer.h"

#include <algorithm>

#include <unistd.h>

#include <string>

#include <map>



using namespace std;


vector<string> terms;

vector<string> nonTerms;

vector<string> tempTerms;

vector<string> input;

vector<string> tempNonTerms;

string pre;

vector<vector<string>> rules;

vector<bool> useful;





// read grammar

void ReadGrammar()

{

    //cout << "0\n";

    if(cin.eof())

    {

        cout<<"SYNTAX ERROR !!!\n";

        exit(1);

    }

        

    cin>>pre;

        

    

    if(pre != "#")

    {

        

        input.push_back(pre);

        ReadGrammar();

    }

    else

    {

        for(int i = 0; i < input.size(); i++)

        {

            if(input[i] == "->" && std::find(tempNonTerms.begin(), tempNonTerms.end(), input[i-1]) == tempNonTerms.end())

            {

                tempNonTerms.push_back(input[i - 1]);

            }

            

        }

        for(int i = 0; i < input.size(); i++)

        {

            if(i < input.size() - 1 && input[i] != "->" && input[i] != "*" && std::find(tempNonTerms.begin(), tempNonTerms.end(), input[i]) == tempNonTerms.end() && std::find(tempTerms.begin(), tempTerms.end(), input[i]) == tempTerms.end())

            {

                tempTerms.push_back(input[i]);

            }

        }

        for(int i = 0; i < input.size()-1; i++)

        {

            if(i != input.size() - 1 && i != 1)

            {

                if(input[i] == "->")

                {

                    if(input[i - 2] != "*")

                    {

                        

                        cout<<"SYNTAX ERROR !!!\n";

                        exit(1);

                    }

                }

            }

        }

        if(std::find(input.begin(), input.end(), "*") == input.end())

        {

            cout<<"SYNTAX ERROR !!!\n";

            exit(1);

        }

        if(std::find(input.begin(), input.end(), "->") == input.end())

        {

            cout<<"SYNTAX ERROR !!!\n";

            exit(1);

        }

    }

    

}


// Task 1

void printTerminalsAndNoneTerminals(int printFlag = 1)

{

    

    

    for(int i = 0; i < input.size(); i++)

    {

        

        

        if(std::find(tempTerms.begin(), tempTerms.end(), input[i]) != tempTerms.end())

        {

            tempTerms.erase(std::find(tempTerms.begin(), tempTerms.end(), input[i]));

            if(printFlag == 1)

            {

                cout<<input[i]<<" ";

            }

            terms.push_back(input[i]);

        }

                    

    }

    for(int i = 0; i < input.size(); i++)

    {

        

        

        if(std::find(tempNonTerms.begin(), tempNonTerms.end(), input[i]) != tempNonTerms.end())

        {

            tempNonTerms.erase(std::find(tempNonTerms.begin(), tempNonTerms.end(), input[i]));

            if(printFlag == 1)

            {

                cout<<input[i]<<" ";

            }

            nonTerms.push_back(input[i]);

        }

                    

    }    

    cout<<"\n";

    

}


// Task 2

void RemoveUselessSymbols(int printFlag = 1)

{

        

    for(int i = 0; i < input.size(); i++)

    {

        vector<string> rule;

        int counter = 0;

        for(int j = i; j < input.size(); j++)

        {

            if(input[j] == "*")

            {

                break;

            }

            rule.push_back(input[j]);

            counter++;

        }

        i+=counter;

        if(input[i - 1] == "->")

        {

            rule.push_back("#");

        }

        useful.push_back(true);

        rules.push_back(rule);

        

        

    }

    

    for(int i = 0; i < rules.size(); i++)

    {

        for(int j = 0; j < rules[i].size(); j++)

        {

            if(j > 0 && std::find(tempNonTerms.begin(), tempNonTerms.end(), rules[i][j]) !=  tempNonTerms.end())

            {

                bool isUseless = false;

                

                for(int k = 0; k < rules.size(); k++)

                {

                    if(rules[k][0] == rules[i][j])

                    {

                        if(!useful[i])

                        {

                            isUseless = true;

                            break;

                        }                      

                        for(int l = 1; l <  rules[k].size(); l++)

                        {

                            if(std::find(tempNonTerms.begin(), tempNonTerms.end(), rules[k][l]) !=  tempNonTerms.end() && rules[k][l] != rules[i][j])

                            {

                                isUseless = false;

                            }

                        }

                        for(int l = 1; l <  rules[k].size(); l++)

                        {

                            if(std::find(tempNonTerms.begin(), tempNonTerms.end(), rules[k][l]) !=  tempNonTerms.end() && rules[k][l] == rules[k][0])

                            {

                                isUseless = true;

                            }

                        }

                        if(rules[k][rules[k].size()-1] == "#" || (std::find(tempTerms.begin(), tempTerms.end(), rules[k][rules[k].size()-1]) != tempTerms.end() && rules[k][rules[k].size()-1] == rules[k][2]))

                        {

                            isUseless = false;

                            break;

                        }

                        

                    }

                }

                

                if(isUseless)

                {

                    for(int k = 0; k < rules.size(); k++)

                    {   

                        if(rules[k][0] == rules[i][j])

                        {

                            useful[k] = false;

                            

                        }

                    }

                    useful[i] = false;

                }

            }

        }        

        

    }


    //parse rules and make rules containing useless rules, useless:

    for(int i = 0; i < rules.size(); i++)

    {        

        for(int j = 1; j < rules[i].size(); j++)

        {

            

            

            for(int k = i; k < rules.size(); k++)

            {   

                if(rules[i][j] == rules[k][0])

                {

                    if(!useful[k])

                    {

                        useful[i] = false;

                    }                    

                }

                

            }

            

        }

    }


    for(int x = 0; x < rules.size(); x++)

    {

        for(int i = 0; i < rules.size(); i++)

        {

            if(useful[i])

            {

                bool isUseful = false;

                for(int j = 0; j < rules.size(); j++)

                {

                    for(int k = 1; k < rules[j].size(); k++)

                    {

                        

                        if((rules[i][0] == rules[j][k] && rules[i][0] != rules[j][0] && useful[j]) || rules[i][0] == rules[0][0])

                        {

                            

                            isUseful = true;

                        }

                    }

                }

                if(!isUseful)

                {

                    useful[i] = false;

                }

            }

        }

    }

    

    bool isUseless = true;

    for(int i = 0; i < rules.size(); i++)

    {

        if((rules[i][rules[i].size() - 1] == "#" || std::find(tempTerms.begin(), tempTerms.end(), rules[i][rules[i].size() - 1]) != tempTerms.end())&& useful[i])

        {

            isUseless = false;

            break;

        }

    }


    if(isUseless)

    {

        for(int i = 0; i < useful.size(); i++)

        {

            useful[i] = false;

        }

    }


    

    if(printFlag == 1)

    {

        for(int i = 0; i < rules.size(); i++)

        {

            if(useful[i])

            {

                for(int j = 0; j < rules[i].size(); j++)

                {

                    

                    cout<<rules[i][j]<<" ";

                    

                    

                }

            

                cout<<"\n";

            }

            

        }

    }

    

}


// Task 3

map<string, vector<string>> firstSets;

vector<pair<string, vector<string>>> ruleList;

void CalculateFirstSets(int printFlag = 1)

{

    RemoveUselessSymbols(0);

    printTerminalsAndNoneTerminals(0);

    

    for(int i = 0; i < rules.size(); i++)

    {

        pair<string, vector<string>> rule;

        rule.first = rules[i][0]; // Left side of the rule

        vector<string> right;

        for(int j = 2; j < rules[i].size(); j++)

        {

            right.push_back(rules[i][j]); // Right side of the rule

        }

        rule.second = right;

        ruleList.push_back(rule);

    }

    bool isEnd = false;

    string left;

    vector<string> right;

    bool visited;

    loop:

    visited = false;

    for(auto i : ruleList)

    {

        left = i.first;

        right = i.second;

        

        if(right[0] == "#")

        {

            if(std::find(firstSets[left].begin(), firstSets[left].end(), "#") == firstSets[left].end())

            {

                firstSets[left].push_back("#");

                visited = true;

            }

        }

        else if(std::find(terms.begin(), terms.end(), right[0]) != terms.end())

        {

            if(std::find(firstSets[left].begin(), firstSets[left].end(), right[0]) == firstSets[left].end())

            {

                firstSets[left].push_back(right[0]);

                visited = true;

            }

        }

        else

        {

            for(auto j : right)

            {

                isEnd = false;

                if(std::find(nonTerms.begin(), nonTerms.end(), j) != nonTerms.end())

                {

                    for(auto &k : firstSets[j])

                    {

                        if(k != "#")

                        {

                            if(std::find(firstSets[left].begin(), firstSets[left].end(), k) == firstSets[left].end()) 

                            {

                                firstSets[left].push_back(k);

                                visited = true;

                            }

                        }else{

                            isEnd = true;

                        }

                    }

                    if(!isEnd)

                    {

                        break;

                    }

                } 

                else

                {

                    if(std::find(firstSets[left].begin(), firstSets[left].end(), j) == firstSets[left].end()) 

                    {

                        firstSets[left].push_back(j);

                        visited = true;

                    }

                    break;

                }

            }

            if(isEnd)

            {

                if (std::find(firstSets[left].begin(), firstSets[left].end(), "#") == firstSets[left].end()) 

                {

                    firstSets[left].insert(firstSets[left].begin(), "#");

                    visited = true;

                }

            }

        }

    }

    if(visited)

    {

        goto loop;

    }

    

    // vector<vector<string>> ruleSets;

    // vector<vector<string>> firstSets;

    // cout << "result\n";

    // RemoveUselessSymbols(0);

    // printTerminalsAndNoneTerminals(0);

    // for(int i = 0; i < rules.size(); i++)

    // {

    //     vector<string> set;

    //     set.push_back(rules[i][0]);

    //     for(int j = 0; j < rules.size(); j++)

    //     {

    //         if(rules[j][0] == rules[i][0])

    //         {

    //             string s = "";

    //             for(int k = 2; k < rules[j].size(); k++)

    //             {

    //                 s += rules[j][k];

    //             }

    //             set.push_back(s);

    //         }

            

    //     }

    //     if(std::find(ruleSets.begin( ), ruleSets.end( ), set) == ruleSets.end())

    //         ruleSets.push_back(set);

    // }

    // for(int i = 0; i < ruleSets.size(); i++)

    // {

    //     for(auto it : ruleSets[i])

    //     {

    //         cout << it << "\t";

    //     }

    //     cout<<endl;

    // }


    // for(int i = 0; i < nonTerms.size(); i++)

    // {

        

    //     vector<string> set;

    //     set.push_back(nonTerms[i]);

    //     for(int j = 0; j < rules.size(); j++)

    //     {

    //         if(rules[j][0] == nonTerms[i])

    //         {   

    //             for(int k = 2; k < rules[j].size(); k++)

    //             {

    //                 if(std::find(nonTerms.begin(), nonTerms.end(), rules[j][k]) != nonTerms.end())

    //                 {

    //                     for(int l = 0; l < ruleSets.size(); l++)

    //                     {

    //                         if(ruleSets[l][0] ==  rules[j][k])

    //                         {

    //                             if(std::find(ruleSets[l].begin(), ruleSets[l].end(), "#") != ruleSets[l].end())

    //                             {

    //                                 if(std::find(set.begin(), set.end(), rules[j][k]) == set.end())

    //                                 {

    //                                     set.push_back(rules[j][k]);

    //                                 }

                                    

    //                             }

    //                             else

    //                             {

    //                                 goto end;

    //                             }

    //                         }

    //                     }

    //                 }

    //                 else

    //                 {

    //                     end:

    //                     if(std::find(set.begin(), set.end(), rules[j][k]) == set.end())

    //                     {

    //                         set.push_back(rules[j][k]);

    //                     }

                        

    //                     break;

    //                 }

    //             }

                

                

    //         }

    //     }

        

    //     firstSets.push_back(set);

    // }

    // // for(int i = 0; i < input.size(); i++)

    // // {

    // //     if(std::find(nonTerms.begin(), nonTerms.end(), input[i]) != nonTerms.end())

    // //     {

    // //         vector<string> set;

    // //         int validRule = -1;

    // //         set.push_back(input[i]);

    // //         for(int j = 0; j < rules.size(); j++)

    // //         {

    // //             for(int k = 2; k < rules[j].size(); k++)

    // //             {

    // //                 if(rules[j][0] == input[i])

    // //                 {

    // //                     if(rules[j][k] == input[i])

    // //                     {

    // //                         continue;

    // //                     }

    // //                     if((std::find(terms.begin(), terms.end(), rules[j][k]) != terms.end() || rules[j][k] == "#")&& j != validRule && std::find(set.begin(), set.end(), rules[j][k]) == set.end())

    // //                     {

    // //                         if(k == 2)

    // //                         {

    // //                             set.push_back(rules[j][k]);

    // //                             validRule = j;

    // //                         }

    // //                     }

    // //                     if(j != validRule && std::find(nonTerms.begin(), nonTerms.end(), rules[j][k]) != nonTerms.end() &&std::find(set.begin(), set.end(), rules[j][k]) == set.end())

    // //                     {

    // //                         set.push_back(rules[j][k]);

    // //                     }

    // //                 }

                    

    // //             }

    // //         }

    // //         if(std::find(firstSets.begin(), firstSets.end(), set) == firstSets.end())

    // //         {

    // //             firstSets.push_back(set);

    // //         }

            

    // //     }

    // // }

    // // for(int x = 0; x < 2; x++)

    // // {

    //     for(int i = 0; i < firstSets.size(); i++)

    //     {

    //         int size = firstSets[i].size();

            

    //         for(int j = 1; j < firstSets[i].size(); j++)

    //         {

    //             if(std::find(nonTerms.begin(), nonTerms.end(), firstSets[i][j]) !=  nonTerms.end())

    //             {

    //                 for(int k = 0; k < firstSets.size(); k++)

    //                 {

    //                     if(firstSets[k].size() == 1)

    //                     {

    //                         firstSets[i].erase(firstSets[i].begin()+j);

    //                         break;

    //                     }

    //                     if(firstSets[k][0] == firstSets[i][j])

    //                     {

    //                         int l = 1;

    //                         loop:

    //                         if(firstSets[k][l] != firstSets[i][0] && std::find(firstSets[i].begin(), firstSets[i].end(), firstSets[k][l]) == firstSets[i].end())

    //                         {

    //                             firstSets[i].push_back(firstSets[k][l]);

    //                         }

    //                         if(l < firstSets[k].size() - 1)

    //                         {

    //                             l++;

    //                             goto loop;

    //                         }

                            

    //                         // if(std::find(nonTerms.begin(), nonTerms.end(), firstSets[i][j]) != nonTerms.end())

    //                         // {

    //                         //     firstSets[i].erase(firstSets[i].begin()+j);

    //                         // }

    //                     }

    //                 }


    //             }

    //         }

    //     }

    // // }

    // // for(int x = 0; x < firstSets.size(); x++)

    // // {

    // //     for(int i = 0; i < firstSets.size(); i++)

    // //     {

    // //         for(int j = 1; j < firstSets[i].size(); j++)

    // //         {

    // //             if(std::find(nonTerms.begin(), nonTerms.end(), firstSets[i][j]) != nonTerms.end())

    // //             {

    // //                 firstSets[i].erase(firstSets[i].begin()+j);

    // //             }

    // //         }

    // //     }

    // // }

            

   

    if(printFlag != 1)

    {

        return;

    }

    terms.insert(terms.begin(), "#");

    for(string i : nonTerms)

    {

        int j = 0;

        cout<< "FIRST(" + i + ") = { ";

        for(string k : terms)

        {

            if(std::find(firstSets[i].begin(), firstSets[i].end(), k) != firstSets[i].end())

            {

                j++;

                if(j != firstSets[i].size())

                {

                    cout<< k + ", ";

                }

                else

                {

                    cout << k;

                }

            }

        }

        cout<< " }\n";

    }

    

}


vector<string> firstOfRule(vector<string> right) 

{

    

    vector<string> result;

    bool empty = true;

    if (right[0] == "#") 

    {

        result.emplace_back("#");

    }

    else

    {

        for(auto i : right)

        {

            empty = true;

            if(std::find(firstSets[i].begin(), firstSets[i].end(), "#") == firstSets[i].end())

            {

                empty = false;

            }

            for(auto j : firstSets[i])

            {

                if(std::find(result.begin(), result.end(), j) == result.end())

                {

                    if(j != "#")

                    {

                        result.push_back(j);

                    }

                }

            }

            if(!empty)

            {

                break;

            }

        }

        if(empty)

        {

            result.emplace_back("#");

        }

    }

    return result;

}  



// Task 4

map<string, vector<string>> followSets;

void CalculateFollowSets(int printFlag = 1)

{

    //cout << "result\n";

    CalculateFirstSets(0);

    for(auto i : terms)

    {

        firstSets[i].push_back(i);

    }

    firstSets["#"].push_back("#");

    bool visited;

    bool empty;

    string left;

    vector<string> right;

    

    followSets[nonTerms[0]].push_back("$");

    

    for (auto & i : ruleList) 

    {

        left = i.first;

        right = i.second;

        for (int j = 0; j < right.size(); j++) 

        {

            if (std::find(nonTerms.begin(), nonTerms.end(), right[j]) != nonTerms.end()) 

            {

                for(int k = j + 1; k < right.size(); k++)

                {

                    empty = false;

                    if (std::find(firstSets[right[k]].begin(), firstSets[right[k]].end(), "#") != firstSets[right[k]].end()) 

                    {

                        empty = true;

                    }

                    for (auto l : firstSets[right[k]]) 

                    {

                        if (std::find(followSets[right[j]].begin(), followSets[right[j]].end(), l) == followSets[right[j]].end()) 

                        {

                            if (l != "#") 

                            {

                                followSets[right[j]].push_back(l);

                            }

                        }

                    }

                    if (!empty) 

                    {

                        break;

                    }

                }

            }

        }

    }

    

    loop1:


    visited = false;

    for (auto i : ruleList) {

        left = i.first;

        right = i.second;

        for(int j = 0; j < right.size(); j++) {

            empty = true;

            if (std::find(nonTerms.begin(), nonTerms.end(), right[j]) != nonTerms.end()) 

            {

                if(j != right.size() - 1)

                {

                    for(int k = j + 1; k < right.size(); k++) {

                        if(std::find(firstSets[right[k]].begin(), firstSets[right[k]].end(), "#") == firstSets[right[k]].end())

                        {

                            empty = false;

                            break;

                        }

                    }

                    if(empty)

                    {

                        for(auto k : followSets[i.first])

                        {

                            if(std::find(followSets[right[j]].begin(), followSets[right[j]].end(), k) == followSets[right[j]].end())

                            {

                                followSets[right[j]].push_back(k);

                                visited = true;

                            }

                        }

                    }

                }else{

                    for(auto k : followSets[i.first])

                    {

                        if(std::find(followSets[right[j]].begin(), followSets[right[j]].end(), k) == followSets[right[j]].end())

                        {

                            followSets[right[j]].push_back(k);

                            visited = true;

                        }

                    }

                }

            }

        }

    }


    if(visited)

    {

        goto loop1;

    }


    if(printFlag != 1)

    {

        return;

    }

    terms.insert(terms.begin(), "$");

    for(auto i : nonTerms)

    {

        int j = 0;

        cout << "FOLLOW(" + i + ") = { ";

        for(auto k : terms)

        {

            if(std::find(followSets[i].begin(), followSets[i].end(), k) != followSets[i].end())

            {

                j++;

                if(j < followSets[i].size())

                {

                    cout << k << ", ";

                }

                else

                {

                    cout << k;

                }

            }


        }

        cout << " }\n";

    }


}


// Task 5

void CheckIfGrammarHasPredictiveParser()

{

    //cout<< "result\n";

    CalculateFollowSets(0);

    string result;

    bool hasUseless = false;

    for(bool i : useful)

    {

        if(!i)

        {

            hasUseless = true;

            break;

        }

    }

    if(hasUseless)

    {

        result = "NO";

        

    }

    else

    {

        bool notSat = false;

        bool breakLoop = false;

        


        for(int i = 0; i < ruleList.size(); i++)

        {

            

            for(int j = i + 1; j < ruleList.size(); j++)

            {

                

                if(ruleList[i].first == ruleList[j].first)

                {

                    vector<string> iFirst = firstOfRule(ruleList[i].second);

                    vector<string> jFirst = firstOfRule(ruleList[j].second);

                    

                    for(auto k : iFirst)

                    {

                        if(find(jFirst.begin(), jFirst.end(), k) != jFirst.end())

                        {

                            notSat = true;

                            goto declared;

                        }

                    }

                    notSat = false;

                    declared:

                    if(notSat)

                    {

                        breakLoop = true;

                        break;

                    }

                }

            }

            if(breakLoop) 

            {

                break;

            }

        }


        bool notSat2 = false;

        if(notSat)

        {

            result = "NO";

        }

        else

        {


            

            for(auto i : nonTerms)

            {

                if(find(firstSets[i].begin(), firstSets[i].end(), "#") != firstSets[i].end())

                {

                    for(auto j : firstSets[i])

                    {

                        if(find(followSets[i].begin(), followSets[i].end(), j) != followSets[i].end())

                        {

                            notSat2 = true;

                            goto declared2;

                        }

                    }

                    notSat2 = false;

                    declared2:

                    notSat2;

                }

                if(notSat2)

                {

                    break;

                }

            }

            if(notSat2)

            {

                result = "NO";

            }

            else

            {

                result = "YES";

            }

        }

    }

    cout << result;

}




int main (int argc, char* argv[])

{

    int task;


    if (argc < 2)

    {

        cout << "Error: missing argument\n";

        return 1;

    }


    /*

       Note that by convention argv[0] is the name of your executable,

       and the first argument to your program is stored in argv[1]

     */



    task = atoi(argv[1]);

    

    ReadGrammar();  // Reads the input grammar from standard input

                    // and represent it internally in data structures

                    // ad described in project 2 presentation file


    switch (task) {

        case 1: printTerminalsAndNoneTerminals();

            break;


        case 2: RemoveUselessSymbols();

            break;


        case 3: CalculateFirstSets();

            break;


        case 4: CalculateFollowSets();

            break;


        case 5: CheckIfGrammarHasPredictiveParser();

            break;


        default:

            cout << "Error: unrecognized task number " << task << "\n";

            break;

    }

    return 0;

}
