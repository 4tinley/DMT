#ifndef INCL_H
#define INCL_H

/* Necessary includes. */
#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <vector>
#include <cmath>

/*
   Data structure representing a single client in the car assurance system.
*/
struct Client
{
    int client_id;
    std::string client_name;
    int client_age;
    std::string phone_number;
    std::string address;
    std::string policy_type;   
    double car_value;          
    int nb_accidents_due;   
    int nb_accidents_not_due;  
    int nb_suspensions;       

    double risk_score;          
    double trust_score;        
    double monthly_premium;   
};

struct Node
{
    Client data;
    Node* left;
    Node* right;
};

/* Global pointer to the BST root. */
extern Node* g_root;

/* ---- BST Operations ---- */
void Insert_Client(const Client& new_client);
Node* Retrieve_Client_By_Id(int client_id);
bool Remove_Client_By_Id(int client_id);
void Show_All_Clients();      /* Inorder traversal to display sorted clients. */
void Clear_Tree();            

/* Computations & Validation. */
double Compute_Risk_Score(const Client& c);
double Compute_Trust_Score(const Client& c);
double Compute_Monthly_Premium(const Client& c);
bool Is_Valid_Client_Data(const Client& c);

/* Recompute fields for entire data set. */
void Recompute_All_Scores();

/* Demo/Utility. */
void Load_Sample_Data();

/* Input Handling. */
int Read_Unique_Id_Input();
int Read_Int_Input(const std::string& prompt);
double Read_Double_Input(const std::string& prompt);
std::string Read_String_Input(const std::string& prompt, bool allow_empty = false);

#endif // INCL_H