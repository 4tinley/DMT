#include "incl.h"

int main()
{
    /* Load some sample data at startup. */
    Load_Sample_Data();

    while (true)
    {
        std::cout << "\n-_-_-_- CAR'RISK MENU -_-_-_-\n\n"
            << "1. Show All Clients\n"
            << "2. Add New Client\n"
            << "3. Retrieve Client by ID\n"
            << "4. Update Client\n"
            << "5. Remove Client\n"
            << "6. Recompute All Scores\n"
            << "0. Exit\n"
            << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice))
        {
            std::cout << "Invalid input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n";

        if (choice == 0)
        {
            std::cout << "Exiting program...\n";
            break;
        }

        switch (choice)
        {
        case 1:
        {
            Show_All_Clients();
            break;
        }
        case 2:
        {
            Client new_client;
            new_client.client_id = Read_Unique_Id_Input();
            new_client.client_name = Read_String_Input("Enter client name (non-empty): ");
            new_client.client_age = Read_Int_Input("Enter client age (>= 18): ");
            new_client.phone_number = Read_String_Input("Enter phone number (non-empty): ");
            new_client.address = Read_String_Input("Enter address (non-empty): ");
            new_client.policy_type = Read_String_Input("Enter policy type (Basic/Premium/Gold): ");
            new_client.car_value = Read_Double_Input("Enter car value (>= 0): ");
            new_client.nb_accidents_due = Read_Int_Input("Enter number of accidents due to client (>= 0): ");
            new_client.nb_accidents_not_due = Read_Int_Input("Enter number of accidents not due to client (>= 0): ");
            new_client.nb_suspensions = Read_Int_Input("Enter number of license suspensions (>= 0): ");

            new_client.risk_score = 0.0;
            new_client.trust_score = 0.0;
            new_client.monthly_premium = 0.0;

            Insert_Client(new_client);
            Recompute_All_Scores();
            break;
        }
        case 3:
        {
            int id = Read_Int_Input("Enter client ID to retrieve: ");
            Node* found = Retrieve_Client_By_Id(id);
            if (!found)
            {
                std::cout << "Client with ID " << id << " not found.\n";
            }
            else
            {
                const Client& c = found->data;
                std::cout << "\n==== CLIENT FOUND ====\n"
                    << "ID: " << c.client_id
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
            }
            break;
        }
        case 4:
        {
            int id = Read_Int_Input("Enter client ID to update: ");
            Node* to_update = Retrieve_Client_By_Id(id);
            if (!to_update)
            {
                std::cout << "Client with ID " << id << " not found.\n";
            }
            else
            {
                Client& c = to_update->data;
                std::cout << "Updating client. Leave blank to keep current value.\n";

                std::cout << "Current name: " << c.client_name << "\nNew name: ";
                {
                    std::string input;
                    std::getline(std::cin, input);
                    if (!input.empty()) c.client_name = input;
                }

                std::cout << "Current age: " << c.client_age << "\nNew age: ";
                {
                    std::string input;
                    std::getline(std::cin, input);
                    if (!input.empty())
                    {
                        int temp_age = std::stoi(input);
                        c.client_age = temp_age;
                    }
                }

                std::cout << "Current phone: " << c.phone_number << "\nNew phone: ";
                {
                    std::string input;
                    std::getline(std::cin, input);
                    if (!input.empty()) c.phone_number = input;
                }

                std::cout << "Current address: " << c.address << "\nNew address: ";
                {
                    std::string input;
                    std::getline(std::cin, input);
                    if (!input.empty()) c.address = input;
                }

                std::cout << "Current policy: " << c.policy_type << "\nNew policy: ";
                {
                    std::string input;
                    std::getline(std::cin, input);
                    if (!input.empty()) c.policy_type = input;
                }

                std::cout << "Current car value: " << c.car_value << "\nNew car value: ";
                {
                    std::string input;
                    std::getline(std::cin, input);
                    if (!input.empty())
                    {
                        double temp_val = std::stod(input);
                        c.car_value = temp_val;
                    }
                }

                std::cout << "Current accidents due: " << c.nb_accidents_due
                    << "\nNew accidents due: ";
                {
                    std::string input;
                    std::getline(std::cin, input);
                    if (!input.empty())
                    {
                        int temp_accd = std::stoi(input);
                        c.nb_accidents_due = temp_accd;
                    }
                }

                std::cout << "Current accidents not due: " << c.nb_accidents_not_due
                    << "\nNew accidents not due: ";
                {
                    std::string input;
                    std::getline(std::cin, input);
                    if (!input.empty())
                    {
                        int temp_accn = std::stoi(input);
                        c.nb_accidents_not_due = temp_accn;
                    }
                }

                std::cout << "Current suspensions: " << c.nb_suspensions
                    << "\nNew suspensions: ";
                {
                    std::string input;
                    std::getline(std::cin, input);
                    if (!input.empty())
                    {
                        int temp_susp = std::stoi(input);
                        c.nb_suspensions = temp_susp;
                    }
                }

                if (!Is_Valid_Client_Data(c))
                {
                    std::cout << "Updated data is invalid. Changes not saved.\n";
                }
                else
                {
                    Recompute_All_Scores();
                    std::cout << "Client updated successfully.\n";
                }
            }
            break;
        }
        case 5:
        {
            int id = Read_Int_Input("Enter client ID to remove: ");
            bool removed = Remove_Client_By_Id(id);
            if (removed)
            {
                Recompute_All_Scores();
                std::cout << "Client with ID " << id << " has been removed.\n";
            }
            else
            {
                std::cout << "Client with ID " << id << " not found.\n";
            }
            break;
        }
        case 6:
        {
            Recompute_All_Scores();
            break;
        }
        default:
        {
            std::cout << "Invalid option. Please try again.\n";
            break;
        }
        }
    }

    /* Clear all client data before closing. */
    Clear_Tree();
    return 0;
}