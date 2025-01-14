#include "incl.h"

/* Global BST root pointer. */
Node* g_root = nullptr;

/* == FORWARD DECLARATIONS (PRIVATE) == */
static Node* Create_Node(const Client& client);
static Node* Insert_Node(Node* root, const Client& client);
static Node* Search_Node(Node* root, int client_id);
static Node* Remove_Node(Node* root, int client_id);
static Node* Find_Min(Node* root);
static void Inorder_Traversal(Node* root);
static void Clear_BST(Node* root);

/* == BST OPERATION IMPLEMENTATIONS == */

void Insert_Client(const Client& new_client)
{
    /* Insert only if data is valid. */
    if (!Is_Valid_Client_Data(new_client))
    {
        std::cout << "Client data invalid. Cannot insert.\n";
        return;
    }
    g_root = Insert_Node(g_root, new_client);
}

Node* Retrieve_Client_By_Id(int client_id)
{
    return Search_Node(g_root, client_id);
}

bool Remove_Client_By_Id(int client_id)
{
    if (Search_Node(g_root, client_id) == nullptr)
    {
        return false; /* Not found. */
    }
    g_root = Remove_Node(g_root, client_id);
    return true;
}

void Show_All_Clients()
{
    if (!g_root)
    {
        std::cout << "No clients found in the system.\n";
        return;
    }
    std::cout << "\n===== ALL CLIENTS (Sorted by ID) =====\n";
    Inorder_Traversal(g_root);
}

/* Clear the BST from memory. */
void Clear_Tree()
{
    Clear_BST(g_root);
    g_root = nullptr;
}

/* == COMPUTATION & VALIDATION == */

double Compute_Risk_Score(const Client& c)
{
    /*
       - Car value relative to client's age (younger drivers with expensive cars are riskier).
       - Past accidents and suspensions increase risk.
       - Diminishing impact of additional accidents.
    */
    double age_factor = (c.client_age < 25) ? 1.5 : (c.client_age < 35 ? 1.2 : 1.0);
    double value_factor = c.car_value / 10000.0;

    double accidents_penalty = 0.0;
    for (int i = 1; i <= c.nb_accidents_due; ++i)
    {
        accidents_penalty += 10.0 / i;
    }

    double suspension_penalty = 3.0 * c.nb_suspensions;
    double random_factor = (std::rand() % 3); // 0..2

    double total = (age_factor * value_factor)
        + accidents_penalty
        + suspension_penalty
        + random_factor;

    return total;
}

double Compute_Trust_Score(const Client& c)
{
    /*
       - Reward not-at-fault accidents, penalize at-fault accidents and suspensions.
       - Policy bonuses. Deduct based on risk_score.
    */
    double base_trust = 60.0;
    double not_due_bonus = 5.0 * c.nb_accidents_not_due;
    double due_penalty = 15.0 * c.nb_accidents_due;
    double suspension_penalty = 20.0 * c.nb_suspensions;

    double policy_bonus = 0.0;
    if (c.policy_type == "Gold")    policy_bonus = 12.0;
    else if (c.policy_type == "Premium") policy_bonus = 7.0;

    double risk_deduction = c.risk_score / 2.0;

    double score = base_trust + not_due_bonus - due_penalty - suspension_penalty + policy_bonus - risk_deduction;
    if (score < 0.0) score = 0.0;

    return score;
}

double Compute_Monthly_Premium(const Client& c)
{
    /*
       - Base fee plus exponential risk factor, minus trust discount, with policy multiplier.
    */
    double base_premium = 40.0;
    double risk_component = std::exp(c.risk_score / 50.0) * 20.0;
    double trust_discount = (c.trust_score > 70) ? 10.0 : (c.trust_score > 50 ? 5.0 : 0.0);

    double policy_multiplier = 1.0;
    if (c.policy_type == "Premium") policy_multiplier = 1.1;
    else if (c.policy_type == "Gold") policy_multiplier = 1.2;

    double premium = (base_premium + risk_component - trust_discount) * policy_multiplier;
    if (premium < 35.0) premium = 35.0;

    return premium;
}

bool Is_Valid_Client_Data(const Client& c)
{
    if (c.client_id < 0)
    {
        std::cout << "Error: client_id cannot be negative.\n";
        return false;
    }
    if (c.client_age < 18)
    {
        std::cout << "Error: client_age must be >= 18.\n";
        return false;
    }
    if (c.car_value < 0.0)
    {
        std::cout << "Error: car_value cannot be negative.\n";
        return false;
    }
    if (c.nb_accidents_due < 0 || c.nb_accidents_not_due < 0 || c.nb_suspensions < 0)
    {
        std::cout << "Error: accidents/suspensions cannot be negative.\n";
        return false;
    }
    if (c.client_name.empty())
    {
        std::cout << "Error: client_name cannot be empty.\n";
        return false;
    }
    if (c.phone_number.empty())
    {
        std::cout << "Error: phone_number cannot be empty.\n";
        return false;
    }
    if (c.address.empty())
    {
        std::cout << "Error: address cannot be empty.\n";
        return false;
    }
    if (c.policy_type.empty())
    {
        std::cout << "Error: policy_type cannot be empty.\n";
        return false;
    }
    return true;
}

/* Recompute risk, premium, and trust for all clients in the BST. */
void Recompute_All_Scores()
{
    std::cout << "Recomputing all client scores...\n";

    auto Recompute_Recursive = [&](Node* root, auto&& self) -> void
        {
            if (!root) return;
            self(root->left, self);

            Client& c = root->data;
            c.risk_score = Compute_Risk_Score(c);
            c.monthly_premium = Compute_Monthly_Premium(c);
            c.trust_score = Compute_Trust_Score(c);

            self(root->right, self);
        };

    Recompute_Recursive(g_root, Recompute_Recursive);
    std::cout << "All client scores updated.\n";
}

void Load_Sample_Data()
{
    /* Clear existing data. */
    Clear_Tree();

    Client c1{ 100, "Alice Johnson",   25, "0612345678", "123 Maple St",
               "Basic",  8000.0,  2, 1, 0,  0.0, 0.0, 0.0 };
    Client c2{ 150, "Bob Wilson",      40, "0755664433", "456 Oak Ave",
               "Premium", 15000.0, 1, 0, 1, 0.0, 0.0, 0.0 };
    Client c3{ 80,  "Charlie Adams",   30, "0788991122", "789 Pine Rd",
               "Gold",  20000.0,  3, 2, 2,  0.0, 0.0, 0.0 };
    Client c4{ 200, "Diana Roberts",   55, "0755123456", "234 Elm St",
               "Basic",  5000.0,  0, 0, 0,  0.0, 0.0, 0.0 };
    Client c5{ 120, "Evan Harris",     29, "0687654321", "567 Birch Ln",
               "Gold",  12000.0,  1, 2, 0,  0.0, 0.0, 0.0 };

    Insert_Client(c1);
    Insert_Client(c2);
    Insert_Client(c3);
    Insert_Client(c4);
    Insert_Client(c5);

    Recompute_All_Scores();
}

/* Input Handling. */

int Read_Unique_Id_Input()
{
    while (true)
    {
        int id = Read_Int_Input("Enter client ID (>= 0): ");

        if (id < 0)
        {
            std::cout << "Error: ID cannot be negative.\n";
            continue;
        }
        if (Retrieve_Client_By_Id(id) != nullptr)
        {
            std::cout << "Error: client with this ID already exists. Please try a different ID.\n";
            continue;
        }
        return id;
    }
}

int Read_Int_Input(const std::string& prompt)
{
    while (true)
    {
        std::cout << prompt;
        int value;
        if (std::cin >> value)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        else
        {
            std::cout << "Invalid integer input. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

double Read_Double_Input(const std::string& prompt)
{
    while (true)
    {
        std::cout << prompt;
        double value;
        if (std::cin >> value)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        else
        {
            std::cout << "Invalid numeric input. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string Read_String_Input(const std::string& prompt, bool allow_empty)
{
    while (true)
    {
        std::cout << prompt;
        std::string str;
        std::getline(std::cin, str);

        if (!allow_empty && str.empty())
        {
            std::cout << "Input cannot be empty. Please try again.\n";
            continue;
        }
        return str;
    }
}

/* == PRIVATE HELPER (INTERNAL) == */

static Node* Create_Node(const Client& client)
{
    Node* new_node = new Node;
    new_node->data = client;
    new_node->left = nullptr;
    new_node->right = nullptr;
    return new_node;
}

static Node* Insert_Node(Node* root, const Client& client)
{
    if (root == nullptr)
    {
        return Create_Node(client);
    }
    if (client.client_id < root->data.client_id)
    {
        root->left = Insert_Node(root->left, client);
    }
    else if (client.client_id > root->data.client_id)
    {
        root->right = Insert_Node(root->right, client);
    }
    else
    {
        std::cout << "Client with ID " << client.client_id
            << " already exists. Insertion skipped.\n";
    }
    return root;
}

static Node* Search_Node(Node* root, int client_id)
{
    if (root == nullptr) return nullptr;
    if (client_id == root->data.client_id) return root;

    if (client_id < root->data.client_id)
        return Search_Node(root->left, client_id);
    else
        return Search_Node(root->right, client_id);
}

static Node* Find_Min(Node* root)
{
    while (root && root->left)
    {
        root = root->left;
    }
    return root;
}

static Node* Remove_Node(Node* root, int client_id)
{
    if (root == nullptr) return nullptr;

    if (client_id < root->data.client_id)
    {
        root->left = Remove_Node(root->left, client_id);
    }
    else if (client_id > root->data.client_id)
    {
        root->right = Remove_Node(root->right, client_id);
    }
    else
    {
        /* Found the node to remove. */
        if (root->left == nullptr && root->right == nullptr)
        {
            delete root;
            return nullptr;
        }
        else if (root->left == nullptr)
        {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr)
        {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        else
        {
            /* Node with two children. */
            Node* temp = Find_Min(root->right);
            root->data = temp->data;
            root->right = Remove_Node(root->right, temp->data.client_id);
        }
    }
    return root;
}

static void Inorder_Traversal(Node* root)
{
    if (!root) return;
    Inorder_Traversal(root->left);

    const Client& c = root->data;
    std::cout << "ID: " << c.client_id
        << " | Name: " << c.client_name
        << " | Age: " << c.client_age
        << " | Phone: " << c.phone_number
        << " | Address: " << c.address
        << " | Policy: " << c.policy_type
        << " | Car Value: " << c.car_value
        << " | Accidents Due: " << c.nb_accidents_due
        << " | Accidents Not Due: " << c.nb_accidents_not_due
        << " | Suspensions: " << c.nb_suspensions
        << " | Risk: " << c.risk_score
        << " | Trust: " << c.trust_score
        << " | Monthly: " << c.monthly_premium << "\n";

    Inorder_Traversal(root->right);
}

static void Clear_BST(Node* root)
{
    if (!root) return;
    Clear_BST(root->left);
    Clear_BST(root->right);
    delete root;
}
