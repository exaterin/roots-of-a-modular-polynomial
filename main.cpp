#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>
#include <memory>
using namespace std;

vector <string> tokens;
int m;
string formula;
 
struct Node;
using NodePointer = std::shared_ptr<Node>;

struct Node{
    string data;
    NodePointer left, right;
    string evaluation;

    Node(string data){
        this->data = data;
        this->left = this->right = nullptr;
    };

    Node(string data, NodePointer left, NodePointer right){
        this->data = data;
        this->left = left;
        this->right = right;
    };
};
 
bool is_operator(char c){ return (c == '+' || c == '*'); }

int priority(const string &c){
    if(c == "*")
        return 2;
    if(c== "+")
        return 1;
    else
        return 0;
}

bool if_number(string str){
    int i = 0;
    for(auto it = str.cbegin(); it != str.cend(); ++it){
        if(isdigit(*it)) {}
        else 
            return false; 
        ++i;
    }
    return true;
}

vector <string> infix_to_postfix(string line){

    vector<string> tokens;

    string number = "";
    for(unsigned int i = 0; i < line.size(); ++i){

        if(line[i] == '(' || line[i] == ')' || line[i] == 'x' || line[i] == '*' || line[i] == '+'){
            if(number != ""){
                tokens.push_back(number);
                number = "";
            }
            tokens.push_back(string(1,line[i]));
        }
        else
            number += line[i];
    }

    vector<string> outputList;
    stack<string> s;

    for(vector<int>::size_type i = 0; i < tokens.size(); ++i){
        if(if_number(tokens[i]) || tokens[i] == "x") 
            outputList.push_back(tokens[i]);

        if(tokens[i] == "(")
            s.push(tokens[i]);
      
        if(tokens[i] == ")"){
            while(!s.empty() && s.top() != "("){
                outputList.push_back(s.top());
                s.pop();
            }
            s.pop();
        }
        if(is_operator(tokens[i][0]) == true) {
            while(!s.empty() && priority(s.top()) >= priority(tokens[i])) {
                outputList.push_back(s.top());
                s.pop();
            }
            s.push(tokens[i]);
        }
    }
    while(!s.empty()){
        outputList.push_back(s.top());
        s.pop();
    }
    return outputList;
}

uint_fast64_t evaluate(NodePointer root, int m, int x){

    if(root -> left == nullptr && root -> right == nullptr)
        if(root->data == "x")
            return x;
        else
            return stoi(root->data);
    else{
        switch (root->data[0]) {
            case '+': 
                return (evaluate(root->left, m, x) + evaluate(root->right, m, x)) % m;
                break;

            case '*': 
                return (evaluate(root->left, m, x) * evaluate(root->right, m, x)) % m;
                break;

            default:
                break;
        }
    }
    return {};
}

NodePointer construct(vector <string> postfix){
    if (postfix.size() == 0) 
        return nullptr;

    stack<NodePointer> s;
    for (vector<int>::size_type i = 0; i < postfix.size(); ++i){

        if (is_operator(postfix[i][0])){
                NodePointer x = s.top();
                s.pop();
                NodePointer y = s.top();
                s.pop();
                NodePointer node = std::make_shared<Node>(postfix[i], y, x);
                s.push(node); 
        } else
            s.push(std::make_shared<Node>(postfix[i]));
    }
    return s.top();
}

int main(){

    string line;
    string token;

    getline(cin,line); 
    stringstream buffer(line);
    
    buffer >> token;
    m = stoi(token);
    buffer >> formula;

    vector <string> postfix = infix_to_postfix(formula);
    NodePointer root = construct(postfix);

    int n = 0;
    for(int i = 0; i < m; ++i){
        uint_fast32_t result = evaluate(root, m, i);
        if(result == 0){
            n += 1;
            cout << "x=" << i << endl;
        }
    }
    cout << "n=" << n << endl;

    return 0;
}