#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <limits> // For numeric_limits
#include <iomanip> // For setw and setfill
#include <ctime>   // For time functions
#include <cstdlib>
#include <ctime>
using namespace std;



// ==================== Ticket Sales & Spectator Management (LOW TENG FONG TP073919 ) ====================

struct Spectator {
    int id;
    string name;
    int priority; // 0 = VIP, 1 = Early-bird, 2 = Normal
    string registrationTime; // Time of registration
    Spectator* next;

    Spectator(int i, string n, int p, string t) : id(i), name(n), priority(p), registrationTime(t), next(nullptr) {}
};

// Priority Queue class for spectators
class PriorityQueue {
private:
    Spectator* head;

public:
    PriorityQueue() : head(nullptr) {}

    // Add a spectator to the priority queue based on priority
    void enqueue(Spectator* newSpectator) {
        if (!head || newSpectator->priority < head->priority) {
            newSpectator->next = head;
            head = newSpectator;
        } else {
            Spectator* temp = head;
            while (temp->next && temp->next->priority <= newSpectator->priority) {
                temp = temp->next;
            }
            newSpectator->next = temp->next;
            temp->next = newSpectator;
        }
    }

    // Dequeue a spectator from the priority queue
    Spectator* dequeue() {
        if (!head) {
            return nullptr;
        }
        Spectator* temp = head;
        head = head->next;
        return temp;
    }

    // Check if the queue is empty
    bool isEmpty() {
        return head == nullptr;
    }

    // Clear the queue
    void clear() {
        while (!isEmpty()) {
            Spectator* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Display the current priority queue
    void displayQueue(const string& queueType) {
        if (!head) {
            cout << "\nCurrent " << queueType << " Queueing Situation:\n";
            cout << "No spectators in the queue.\n";
            return;
        }

        cout << "\nScanning now...\n";
        cout << "\n=====" << queueType << " Queueing Situation=====\n";
        Spectator* temp = head;
        while (temp) {
            string priorityStr = (temp->priority == 0) ? "VIP" : (temp->priority == 1) ? "Early-bird" : "Normal";
            cout << temp->name << " (" << priorityStr << ") queuing...\n";
            temp = temp->next;
        }
    }

    // Get the head of the queue
    Spectator* getHead() {
        return head;
    }

    // Generate a random ID from the valid range of spectators in the queue
    int getRandomID() {
        if (!head) {
            return -1; // No spectators in the queue
        }

        // Count the number of spectators in the queue
        int count = 0;
        Spectator* temp = head;
        while (temp) {
            count++;
            temp = temp->next;
        }

        // Generate a random index
        int randomIndex = rand() % count;

        // Traverse to the random index
        temp = head;
        for (int i = 0; i < randomIndex; i++) {
            temp = temp->next;
        }

        return temp->id; // Return the ID of the randomly selected spectator
    }

    // Remove a spectator by ID from the queue and return the removed spectator
    Spectator* removeSpectator(int id) {
        if (!head) {
            return nullptr;
        }

        // If the spectator to remove is at the head
        if (head->id == id) {
            Spectator* temp = head;
            head = head->next;
            temp->next = nullptr; // Disconnect the spectator from the queue
            return temp;
        }

        // Traverse the queue to find the spectator
        Spectator* temp = head;
        while (temp->next && temp->next->id != id) {
            temp = temp->next;
        }

        // If the spectator is found, remove them
        if (temp->next && temp->next->id == id) {
            Spectator* toRemove = temp->next;
            temp->next = toRemove->next;
            toRemove->next = nullptr; // Disconnect the spectator from the queue
            return toRemove;
        }

        return nullptr; // Spectator not found
    }
};

class SpectatorManager {
private:
    Spectator* head; // Linked list to store all registered spectators
    Spectator* enteredSpectators; // Linked list to store spectators who have entered
    Spectator* exitedSpectators; // Linked list to store spectators who have exited
    int idCounter;
    int earlyBirdCount; // Track the number of Early-bird registrations

public:
    SpectatorManager() : head(nullptr), enteredSpectators(nullptr), exitedSpectators(nullptr), idCounter(1), earlyBirdCount(0) {}

    // Add a spectator to the linked list (not a queue)
    void registerSpectator(string name, int priority, string registrationTime) {
        Spectator* newSpectator = new Spectator(idCounter++, name, priority, registrationTime);

        if (!head) {
            head = newSpectator;
        } else {
            Spectator* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newSpectator;
        }

        // Increment Early-bird count if priority is Early-bird
        if (priority == 1) {
            earlyBirdCount++;
        }
    }

    // Get the number of Early-bird registrations
    int getEarlyBirdCount() {
        return earlyBirdCount;
    }

    // Get the current value of idCounter
    int getIDCounter() {
        return idCounter;
    }

    // Move a spectator to the entered list
    void moveToEntered(Spectator* spectator) {
        if (!enteredSpectators) {
            enteredSpectators = spectator;
        } else {
            Spectator* temp = enteredSpectators;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = spectator;
        }
        spectator->next = nullptr; // Ensure the spectator is not part of any other list
    }

    // Move a spectator to the exited list
    void moveToExited(Spectator* spectator) {
        if (!exitedSpectators) {
            exitedSpectators = spectator;
        } else {
            Spectator* temp = exitedSpectators;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = spectator;
        }
        spectator->next = nullptr; // Ensure the spectator is not part of any other list
    }

    // Get the head of the main list
    Spectator* getHead() {
        return head;
    }

    // Get the head of the entered spectators list
    Spectator* getEnteredHead() {
        return enteredSpectators;
    }

    // Check if a spectator has entered
    bool hasEntered(int id) {
        Spectator* temp = enteredSpectators;
        while (temp) {
            if (temp->id == id) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // Check if a spectator has exited
    bool hasExited(int id) {
        Spectator* temp = exitedSpectators;
        while (temp) {
            if (temp->id == id) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // Remove a spectator from the main list
    void removeSpectator(Spectator* spectator) {
        if (!head) return;

        if (head == spectator) {
            head = head->next;
        } else {
            Spectator* temp = head;
            while (temp->next && temp->next != spectator) {
                temp = temp->next;
            }
            if (temp->next == spectator) {
                temp->next = spectator->next;
            }
        }
    }

    // Display all spectators in the linked list
    void displayAll() {
        if (!head) {
            cout << "No spectators in the list.\n";
            return;
        }

        Spectator* temp = head;
        while (temp) {
            string priorityStr = (temp->priority == 0) ? "VIP" : (temp->priority == 1) ? "Early-bird" : "Normal";
            cout << "Spectator ID: " << temp->id << ", Name: " << temp->name << ", Priority: " << priorityStr
                 << ", Registration Time: " << temp->registrationTime << endl;
            temp = temp->next;
        }
    }

    // Search for a spectator by ID or name
    void searchSpectator(int id = -1, string name = "") {
        if (!head) {
            cout << "No spectators in the list.\n";
            return;
        }

        Spectator* temp = head;
        bool anyFound = false; // Track if any match is found

        // Convert the search query to lowercase
        transform(name.begin(), name.end(), name.begin(), ::tolower);

        while (temp) {
            bool found = false; // Track if the current spectator is a match

            if (id != -1) {
                // Search by ID (exact match)
                if (temp->id == id) {
                    found = true;
                }
            } else if (name != "") {
                // Search by name (partial match, case-insensitive)
                string tempName = temp->name;
                transform(tempName.begin(), tempName.end(), tempName.begin(), ::tolower); // Convert to lowercase
                if (tempName.find(name) != string::npos) { // Check if the search query is a substring
                    found = true;
                }
            }

            if (found) {
                string priorityStr = (temp->priority == 0) ? "VIP" : (temp->priority == 1) ? "Early-bird" : "Normal";
                cout << "\nSpectator Found:\n";
                cout << "Spectator ID: " << temp->id << ", Name: " << temp->name << ", Priority: " << priorityStr
                     << ", Registration Time: " << temp->registrationTime << endl;
                anyFound = true; // Set the flag to true if at least one match is found
            }
            temp = temp->next;
        }

        if (!anyFound) {
            if (id != -1) {
                cout << "No matching spectator found with ID: " << id << ".\n";
            } else if (name != "") {
                cout << "No matching spectators found with name containing: " << name << ".\n";
            }
        }
    }
};

//-----------------------------ANG DAE RIEN TP075690 -----------------------------------------

//Schedule Management
// Match structure
struct Match
{
    string player1;
    string player2;
    string stage;
    bool attend1;
    bool attend2;
    Match *next;

    Match(string p1, string p2, string stg) : player1(p1), player2(p2), stage(stg), attend1(true), attend2(true), next(nullptr) {}
};

// WinnerNode structure
struct WinnerNode
{
    string winner;
    WinnerNode *next;

    WinnerNode(string w) : winner(w), next(nullptr) {}
};

// WinnerList class
class WinnerList
{
private:
    WinnerNode *head;

public:
    WinnerList() : head(nullptr) {}

    // Add a winner to the list
    void addWinner(string winner)
    {
        WinnerNode *newWinner = new WinnerNode(winner);
        if (!head)
        {
            head = newWinner;
        }
        else
        {
            WinnerNode *temp = head;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = newWinner;
        }
    }

    // Display all winners
    void display()
    {
        if (!head)
        {
            cout << "No winners yet." << endl;
            return;
        }
        WinnerNode *temp = head;
        while (temp)
        {
            cout << temp->winner << endl;
            temp = temp->next;
        }
    }

    // Get the head of the list (for scheduling next stage)
    WinnerNode *getHead()
    {
        return head;
    }
};

// TournamentScheduler class
class TournamentScheduler
{
private:
    Match *front;
    Match *rear;

public:
    TournamentScheduler() : front(nullptr), rear(nullptr) {}

    // Check if the queue is empty
    bool is_empty()
    {
        return front == nullptr;
    }

    // Add a match to the queue
    void enqueue(string p1, string p2, string stage)
    {
        if (p1 == "TBD" || p2 == "TBD")
        {
            cout << "Cannot add match with TBD players." << endl;
            return;
        }

        Match *newMatch = new Match(p1, p2, stage);
        cout << "Adding match: " << p1 << " vs " << p2 << " at stage: " << stage << endl;

        if (is_empty())
        {
            front = rear = newMatch;
            rear->next = front; // Make it circular
        }
        else
        {
            rear->next = newMatch;
            rear = newMatch;
            rear->next = front; // Maintain circularity
        }
    }

    // Display all matches in the queue
    void display()
    {
        if (is_empty())
        {
            cout << "No matches scheduled." << endl;
            return;
        }

        Match *temp = front;
        do
        {
            cout << "Match: " << temp->player1 << " vs " << temp->player2 << " (Stage: " << temp->stage << ")" << endl;
            temp = temp->next;
        } while (temp != front);
    }

    // Clear the queue
    void clearQueue()
    {
        if (is_empty())
            return;

        Match *temp = front;
        do
        {
            Match *nextMatch = temp->next;
            delete temp;
            temp = nextMatch;
        } while (temp != front);

        front = rear = nullptr;
    }

    // Get the front of the queue (for processing matches)
    Match *getFront()
    {
        return front;
    }

    //for task three

    void replacePlayer(const string& originalPlayer, const string& substitutePlayer) {
        if (is_empty()) {
            cout << "No matches scheduled.\n";
            return;
        }

        Match* temp = front;
        bool found = false;

        do {
            bool replaced = false; // Track if replacement happened in this match

            if (temp->player1 == originalPlayer) {
                temp->player1 = substitutePlayer;
                replaced = true;
            }
            if (temp->player2 == originalPlayer) {
                temp->player2 = substitutePlayer;
                replaced = true;
            }

            if (replaced) {
                found = true;
                cout << "Player " << originalPlayer << " replaced by " << substitutePlayer
                     << " in match (" << temp->player1 << " vs " << temp->player2 << ").\n";
            }

            temp = temp->next;
        } while (temp != front);

        if (!found) {
            cout << "Player " << originalPlayer << " not found in any scheduled matches.\n";
        }
    }

    void setPlayerAbsent(const string& playerName) {
        if (is_empty()) {
            cout << "No matches scheduled.\n";
            return;
        }

        Match* temp = front;
        bool found = false;

        do {
            if (temp->player1 == playerName) {
                temp->attend1 = false;
                found = true;
                cout << "Player " << playerName << " marked as absent in match ("
                     << temp->player1 << " vs " << temp->player2 << ").\n";
            }
            else if (temp->player2 == playerName) {
                temp->attend2 = false;
                found = true;
                cout << "Player " << playerName << " marked as absent in match ("
                     << temp->player1 << " vs " << temp->player2 << ").\n";
            }

//            cout << "DEBUG: Match: " << temp->player1 << " (attend1=" << temp->attend1
//             << ") vs " << temp->player2 << " (attend2=" << temp->attend2 << ")" << endl;

            temp = temp->next;
        } while (temp != front);

        if (!found) {
            cout << "Player " << playerName << " not found in any scheduled matches.\n";
        }
    }

    //task 3 until here


};

struct Group
{
    string participants[4]; // Participants in the group
    int wins[4];            // Wins for each participant
};

// Function to display the main menu
void displayMainMenu() {
    cout << "\nASIA PACIFIC UNIVERSITY TENNIS CHAMPIONSHIP MANAGEMENT SYSTEM:\n";
    cout << "1. Tournamnet Scheduling and Player Progression\n";
    cout << "2. Ticket Sales & Spectator Management\n";
    cout << "3. Tournament & Player Management\n";
    cout << "4. Match History Tracking \n";
    cout << "5. Exit\n";
    cout << "Choose an option: ";
}

// Function to display the ticket sales and spectator management main menu
void displayTSSMMainMenu() {
    cout << "\nTicket Sales & Spectator Management(TSSM) Main Menu:\n";
    cout << "1. Register Spectator\n";
    cout << "2. Display All Spectators\n";
    cout << "3. Entering and Exiting Situation\n";
    cout << "4. Search Spectator\n";
    cout << "5. Exit to Main Menu\n";
    cout << "Choose an option: ";
}

// Function to display the sub-menu for entering and exiting
void displayTSSMSubMenu() {
    cout << "\nSpectator Entering and Exiting Situation:\n";
    cout << "1. Show Entrance Queueing Situation\n";
    cout << "2. Entering the Venue Now\n";
    cout << "3. Show Exit Queueing Situation\n";
    cout << "4. Exiting the Venue Now\n";
    cout << "5. Leave Entrance Queue Early\n";
    cout << "6. Leave Exit Queue Early\n";
    cout << "7. Exit to TSSM Main Menu\n";
    cout << "Choose an option: ";
}

// Function to validate integer input
int getValidatedInput(int min, int max) {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail() || input < min || input > max) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input! Please enter a number between " << min << " and " << max << ": ";
        } else {
            cin.ignore(); // Clear the input buffer
            return input;
        }
    }
}

// Function to validate yes/no input (y/n)
char getYesNoInput() {
    char input;
    while (true) {
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the entire input buffer
        if (input == 'y' || input == 'Y' || input == 'n' || input == 'N') {
            return input;
        } else {
            cout << "Invalid input! Please enter 'y' or 'n': ";
        }
    }
}

// Function to validate spectator name input
string getValidatedName() {
    string name;
    while (true) {
        cout << "Name: ";
        getline(cin, name);
        if (name.empty()) {
            cout << "Name cannot be empty! Please enter a valid name.\n";
        } else {
            return name;
        }
    }
}

// Function to get the current time in HH:MM:SS format
string getCurrentTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[9]; // HH:MM:SS + null terminator
    strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);
    return string(buffer);
}

// Function to handle spectator registration
void registerSpectator(SpectatorManager& manager) {
    while (true) {
        cout << "Enter Spectator info:\n";
        string name = getValidatedName();
        int priority;

        // Display remaining Early-bird slots
        int remainingEarlyBird = 10 - manager.getEarlyBirdCount();
        if (remainingEarlyBird > 0) {
            cout << "Remaining Early-bird slots: " << remainingEarlyBird << endl;
        } else {
            cout << "Early-bird registrations are full. Choose VIP (0) or Normal (2): ";
        }

        // Validate priority input
        if (remainingEarlyBird <= 0) {
            priority = getValidatedInput(0, 2); // Allow only VIP (0) or Normal (2)
            if (priority == 1) {
                cout << "Early-bird registrations are full. Please choose VIP (0) or Normal (2).\n";
                continue;
            }
        } else {
            cout << "Choose Priority (0 = VIP, 1 = Early-bird, 2 = Normal): ";
            priority = getValidatedInput(0, 2); // Validate priority input
        }

        // Get the current time
        string registrationTime = getCurrentTime();

        // Add to linked list
        manager.registerSpectator(name, priority, registrationTime);

        // Display spectator details
        string priorityStr = (priority == 0) ? "VIP" : (priority == 1) ? "Early-bird" : "Normal";
        int price = (priority == 0) ? 300 : (priority == 1) ? 150 : 200;
        cout << "\nSpectator Registered:\n";
        cout << "Spectator ID: " << manager.getIDCounter() - 1 << ", Name: " << name << ", Priority: " << priorityStr
             << ", Price: RM" << price << ", Registration Time: " << registrationTime << endl;

        cout << "\nDo you want to register another spectator? (y/n): ";
        char cont = getYesNoInput(); // Validate continue/exit input
        if (cont == 'n' || cont == 'N') {
            break; // Exit to main menu
        }
    }
}

void handleSpectatorMenu(SpectatorManager& manager, PriorityQueue& entranceQueue, PriorityQueue& exitQueue) {
    int choice;

    while (true) {
        displayTSSMMainMenu();
        choice = getValidatedInput(1, 5); // Update the range to include the new option

        if (choice == 5) {
            cout << "Returning to main menu...\n";
            break;
        }

        switch (choice) {
            case 1: {
                // Register Spectator
                registerSpectator(manager);
                break;
            }
            case 2: // Display All Spectators
                cout << "\nAll Spectators (Based on registration time):\n";
                manager.displayAll();
                break;
            case 3: {
                int subChoice;
                do {
                    displayTSSMSubMenu();
                    subChoice = getValidatedInput(1, 7); // Update the range to include new options

                    switch (subChoice) {
                        case 1: { // Show Entrance Queueing Situation
                            entranceQueue.clear(); // Clear the previous queue
                            Spectator* temp = manager.getHead(); // Get the head of all registered spectators
                            while (temp) {
                                // Only enqueue spectators who haven't entered or exited
                                if (!manager.hasEntered(temp->id) && !manager.hasExited(temp->id)) {
                                    entranceQueue.enqueue(new Spectator(temp->id, temp->name, temp->priority, temp->registrationTime));
                                }
                                temp = temp->next;
                            }
                            entranceQueue.displayQueue("Entrance");
                            break;
                        }
                        case 2: { // Entering the Venue Now
                            if (entranceQueue.isEmpty()) {
                                cout << "\nNo spectators in the entrance queue.\n";
                            } else {
                                cout << "\nSpectator(s) entering the venue now...\n";
                                cout << "----------------------------------------\n";
                                while (!entranceQueue.isEmpty()) {
                                    Spectator* nextSpectator = entranceQueue.dequeue();
                                    string priorityStr = (nextSpectator->priority == 0) ? "VIP" : (nextSpectator->priority == 1) ? "Early-bird" : "Normal";
                                    cout << nextSpectator->name << " (" << priorityStr << ") entering...\n";
                                    manager.removeSpectator(nextSpectator); // Remove from main list
                                    manager.moveToEntered(nextSpectator); // Move to entered list
                                }
                            }
                            break;
                        }
                        case 3: { // Show Exit Queueing Situation
                            exitQueue.clear(); // Clear the previous queue
                            Spectator* temp = manager.getEnteredHead(); // Get the head of entered spectators
                            if (!temp) {
                                cout << "\nNo spectators have entered the venue yet.\n";
                            } else {
                                while (temp) {
                                    // Only enqueue spectators who haven't exited
                                    if (!manager.hasExited(temp->id)) {
                                        exitQueue.enqueue(new Spectator(temp->id, temp->name, temp->priority, temp->registrationTime));
                                    }
                                    temp = temp->next;
                                }
                                exitQueue.displayQueue("Exit");
                            }
                            break;
                        }
                        case 4: { // Exiting the Venue Now
                            if (exitQueue.isEmpty()) {
                                cout << "\nNo spectators in the exit queue.\n";
                            } else {
                                cout << "\nSpectator(s) exiting the venue now...\n";
                                cout << "----------------------------------------\n";
                                while (!exitQueue.isEmpty()) {
                                    Spectator* nextSpectator = exitQueue.dequeue();
                                    string priorityStr = (nextSpectator->priority == 0) ? "VIP" : (nextSpectator->priority == 1) ? "Early-bird" : "Normal";
                                    cout << nextSpectator->name << " (" << priorityStr << ") exiting...\n";
                                    manager.moveToExited(nextSpectator); // Move to exited list
                                }
                            }
                            break;
                        }
                        case 5: { // Leave Entrance Queue Early
                            if (entranceQueue.isEmpty()) {
                                cout << "\nNo spectators in the entrance queue to leave.\n";
                            } else {
                                // Generate a random ID from the valid range
                                int randomID = entranceQueue.getRandomID();
                                if (randomID != -1) {
                                    // Remove the spectator from the queue
                                    Spectator* removedSpectator = entranceQueue.removeSpectator(randomID);
                                    if (removedSpectator) {
                                        string priorityStr = (removedSpectator->priority == 0) ? "VIP" : (removedSpectator->priority == 1) ? "Early-bird" : "Normal";
                                        cout << "\nSpectator with ID " << randomID << " (" << removedSpectator->name << ", " << priorityStr << ") has left the entrance queue and will not return.\n";
                                        manager.moveToExited(removedSpectator); // Mark as exited
                                        delete removedSpectator; // Free memory
                                    }
                                    entranceQueue.displayQueue("Entrance"); // Display the updated queue
                                } else {
                                    cout << "\nNo valid spectators found in the entrance queue.\n";
                                }
                            }
                            break;
                        }
                        case 6: { // Leave Exit Queue Early
                            if (exitQueue.isEmpty()) {
                                cout << "\nNo spectators in the exit queue to leave.\n";
                            } else {
                                // Generate a random ID from the valid range
                                int randomID = exitQueue.getRandomID();
                                if (randomID != -1) {
                                    // Remove the spectator from the queue
                                    Spectator* removedSpectator = exitQueue.removeSpectator(randomID);
                                    if (removedSpectator) {
                                        string priorityStr = (removedSpectator->priority == 0) ? "VIP" : (removedSpectator->priority == 1) ? "Early-bird" : "Normal";
                                        cout << "\nSpectator with ID " << randomID << " (" << removedSpectator->name << ", " << priorityStr << ") has left the exit queue.\n";
                                        delete removedSpectator; // Free memory
                                    }
                                    exitQueue.displayQueue("Exit"); // Display the updated queue
                                } else {
                                    cout << "\nNo valid spectators found in the exit queue.\n";
                                }
                            }
                            break;
                        }
                        case 7: // Exit to Main Menu
                            cout << "Returning to Ticket Sales & Spectator Management Main Menu...\n";
                            break;
                    }
                } while (subChoice != 7);
                break;
            }
            case 4: { // Search Spectator
                if (manager.getIDCounter() == 1) {
                    cout << "\nThere are no spectators registered.\n";
                    break;
                }

                while (true) {
                    int searchChoice;
                    cout << "\nSearch Spectator by:\n";
                    cout << "1. ID\n";
                    cout << "2. Name\n";
                    cout << "Choose an option: ";
                    searchChoice = getValidatedInput(1, 2);

                    if (searchChoice == 1) {
                        // Display valid ID range
                        cout << "Valid ID range: 1 to " << manager.getIDCounter() - 1 << endl;
                        int id;
                        cout << "Enter Spectator ID: ";
                        id = getValidatedInput(1, manager.getIDCounter() - 1); // Validate ID input
                        manager.searchSpectator(id);
                    } else {
                        string name;
                        cout << "Enter Spectator Name (or part of the name): ";
                        getline(cin, name);
                        manager.searchSpectator(-1, name); // Search by partial name
                    }

                    cout << "\nDo you want to continue searching? (y/n): ";
                    char cont = getYesNoInput(); // Validate continue/exit input
                    if (cont == 'n' || cont == 'N') {
                        break; // Exit to the TSSM main menu
                    }
                }
                break;
            }
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}

// Schedule Management
// Process qualifier matches
void processQualifiers(TournamentScheduler &queue, WinnerList &winnersList)
{
    try
    {
        if (queue.is_empty())
        {
            throw runtime_error("No matches to process in Qualifiers.");
        }

        cout << "\nProcessing Qualifying Matches:\n";
        Match *temp = queue.getFront();

        do
        {
            if (!temp->attend1)
            {
                cout << temp->player2 << " advances (Opponent Withdrawn).\n";
                winnersList.addWinner(temp->player2);
            }
            else if (!temp->attend2)
            {
                cout << temp->player1 << " advances (Opponent Withdrawn).\n";
                winnersList.addWinner(temp->player1);
            }
            else
            {
                string winner;
                while (true)
                {
                    cout << temp->player1 << " vs " << temp->player2 << " - Enter Winner: ";
                    cin >> winner;

                    // Validate the winner input
                    if (winner == temp->player1 || winner == temp->player2)
                    {
                        winnersList.addWinner(winner);
                        break; // Exit the loop if the input is valid
                    }
                    else
                    {
                        cout << "Invalid input! Please enter either " << temp->player1 << " or " << temp->player2 << ".\n";
                    }
                }
            }
            temp = temp->next;
        } while (temp != queue.getFront());

        // Clear the queue after processing
        queue.clearQueue();
        cout << "\nQualifiers complete! Scheduling Round Robin matches...\n";
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }
}

// Schedule Round Robin matches
void scheduleRoundRobin(WinnerList &winnersList, TournamentScheduler &queue)
{
    WinnerNode *temp = winnersList.getHead();
    if (!temp)
    {
        cout << "No winners to schedule Round Robin matches.\n";
        return;
    }

    // Create pairs of winners for Round Robin matches
    while (temp && temp->next)
    {
        queue.enqueue(temp->winner, temp->next->winner, "Round Robin");
        temp = temp->next->next;
    }

    cout << "Round Robin matches have been scheduled!\n";
}

void simulateRoundRobin(Group &group)
{
    // Reset wins for each participant
    for (int i = 0; i < 4; i++)
    {
        group.wins[i] = 0;
    }

    // Simulate matches between all participants in the group
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            while (true)
            {
                string winner;
                cout << "Match: " << group.participants[i] << " vs " << group.participants[j] << " - Enter Winner: ";
                cin >> winner;

                // Validate the winner input
                if (winner == group.participants[i])
                {
                    group.wins[i]++;
                    break; // Exit the loop if the input is valid
                }
                else if (winner == group.participants[j])
                {
                    group.wins[j]++;
                    break; // Exit the loop if the input is valid
                }
                else
                {
                    cout << "Invalid input! Please enter either " << group.participants[i] << " or " << group.participants[j] << ".\n";
                }
            }
        }
    }
}

void determineTopWinners(Group &group, WinnerList &knockoutPlayers)
{
    // Create a list of participants and their wins
    pair<string, int> participantsWithWins[4];
    for (int i = 0; i < 4; i++)
    {
        participantsWithWins[i] = {group.participants[i], group.wins[i]};
    }

    // Sort participants by wins (descending order)
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            if (participantsWithWins[j].second > participantsWithWins[i].second)
            {
                swap(participantsWithWins[i], participantsWithWins[j]);
            }
        }
    }

    // Add the top 2 winners to the knockout list
    knockoutPlayers.addWinner(participantsWithWins[0].first);
    knockoutPlayers.addWinner(participantsWithWins[1].first);

    cout << "Top 2 winners from group: " << participantsWithWins[0].first << " and " << participantsWithWins[1].first << endl;
}

// Function to get the size of the linked list
int getListSize(WinnerNode *head)
{
    int size = 0;
    WinnerNode *temp = head;
    while (temp)
    {
        size++;
        temp = temp->next;
    }
    return size;
}

// Function to get a node at a specific index
WinnerNode *getNodeAtIndex(WinnerNode *head, int index)
{
    WinnerNode *temp = head;
    for (int i = 0; i < index && temp; i++)
    {
        temp = temp->next;
    }
    return temp;
}

// Function to shuffle the linked list
void shuffleKnockoutPlayers(WinnerList &knockoutPlayers)
{
    int size = getListSize(knockoutPlayers.getHead());
    if (size <= 1)
        return; // No need to shuffle if there's only one player

    // Seed the random number generator
    srand(time(0));

    // Perform Fisher-Yates shuffle on the linked list
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1); // Random index between 0 and i

        // Swap the data of nodes at positions i and j
        WinnerNode *node1 = getNodeAtIndex(knockoutPlayers.getHead(), i);
        WinnerNode *node2 = getNodeAtIndex(knockoutPlayers.getHead(), j);

        if (node1 && node2)
        {
            swap(node1->winner, node2->winner);
        }
    }
}

void simulateKnockoutStage(WinnerList &knockoutPlayers)
{
    while (true)
    {
        WinnerNode *temp = knockoutPlayers.getHead();
        if (!temp)
        {
            cout << "No players in the knockout stage.\n";
            return;
        }

        // If only one player remains, declare them the champion
        if (!temp->next)
        {
            cout << "\nTournament Champion: " << temp->winner << "!\n";
            return;
        }

        // Display the current round
        cout << "\nKnockout Round:\n";

        // Simulate matches for the current round
        WinnerList nextRound;
        while (temp && temp->next)
        {
            string player1 = temp->winner;
            string player2 = temp->next->winner;

            while (true)
            {
                string winner;
                cout << "Match: " << player1 << " vs " << player2 << " - Enter Winner: ";
                cin >> winner;

                // Validate the winner input
                if (winner == player1 || winner == player2)
                {
                    nextRound.addWinner(winner);
                    break; // Exit the loop if the input is valid
                }
                else
                {
                    cout << "Invalid input! Please enter either " << player1 << " or " << player2 << ".\n";
                }
            }

            temp = temp->next->next; // Move to the next pair
        }

        // If there's an odd number of players, the last player gets a bye
        if (temp)
        {
            cout << temp->winner << " gets a bye and advances to the next round.\n";
            nextRound.addWinner(temp->winner);
        }

        // Update the knockoutPlayers list for the next round
        knockoutPlayers = nextRound;
    }
}

void handleTournamentMenu(TournamentScheduler &tournament, WinnerList &winnersList, WinnerList &knockoutPlayers)
{
    int choice;

    do
    {
        cout << "\n--- Tournament Management Menu ---\n";
        cout << "1. Schedule Qualifier Match\n";
        cout << "2. Display Scheduled Matches\n";
        cout << "3. Process Qualifiers\n";
        cout << "4. Display Winners\n";
        cout << "5. Process Round Robin\n";
        cout << "6. Display Knockout Players\n";
        cout << "7. Process Knockout Stage\n";
        cout << "8. Return to Main Menu\n";
        cout << "Enter your choice: ";
        choice = getValidatedInput(1, 8); // Validate input between 1 and 8

        switch (choice)
        {
        case 1:
        {
            string p1, p2;
            cout << "Enter Player 1: ";
            cin >> p1;
            cout << "Enter Player 2: ";
            cin >> p2;
            tournament.enqueue(p1, p2, "Qualifiers");
            break;
        }
        case 2:
            cout << "\nScheduled Matches:\n";
            tournament.display();
            break;
        case 3:
            processQualifiers(tournament, winnersList);
            break;
        case 4:
            cout << "\nWinners:\n";
            winnersList.display();
            break;
        case 5:
        {
            // Group participants into groups of 4
            WinnerNode *temp = winnersList.getHead();
            int groupCount = 0;

            while (temp)
            {
                Group group;
                for (int i = 0; i < 4 && temp; i++)
                {
                    group.participants[i] = temp->winner;
                    temp = temp->next;
                }

                cout << "\nSimulating Round Robin for Group " << ++groupCount << ":\n";
                simulateRoundRobin(group);
                determineTopWinners(group, knockoutPlayers);
            }
            break;
        }
        case 6:
            cout << "\nKnockout Players:\n";
            knockoutPlayers.display();
            break;
        case 7:
            cout << "\nSimulating Knockout Stage...\n";
            simulateKnockoutStage(knockoutPlayers);
            break;
        case 8:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 8);
}


// ==================== Tournament & Player Management (HENG JUN YONG TP073769 part) ====================

// Structure to represent a withdrawal
struct Withdrawal {
    string playerName;
    string substituteName;  // Empty string indicates no substitute
    bool processed;
    Withdrawal* next;

    Withdrawal(string p, string s = "") : playerName(p), substituteName(s), processed(false), next(nullptr) {}
};

// Class to manage player withdrawals
class WithdrawalQueue {
private:
    Withdrawal* front;
    Withdrawal* rear;

public:
    WithdrawalQueue() : front(nullptr), rear(nullptr) {}

    ~WithdrawalQueue() {
        while (front != nullptr) {
            Withdrawal* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueueWithdrawal(const string& playerName, const string& substituteName = "") {
        Withdrawal* newNode = new Withdrawal(playerName, substituteName);

        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }

        cout << "Player " << playerName << " has withdrawn";
        if (!substituteName.empty()) {
            cout << " and will be replaced by Player " << substituteName;
        }
        cout << ".\n";
    }


    void processWithdrawal(TournamentScheduler& scheduler) {
        if (front == nullptr) {
            cout << "No withdrawals to process.\n";
            return;
        }

        Withdrawal* temp = front;
        cout << "Processing withdrawal: Player " << temp->playerName;

        // Update the match data
        if (!temp->substituteName.empty()) {
            cout << " (Substitute: Player " << temp->substituteName << ")";
            // Replace the player name with the substitute in all matches
            scheduler.replacePlayer(temp->playerName, temp->substituteName);
        } else {
            cout << " (No substitute available)";
            // Set attendance to false for this player
            scheduler.setPlayerAbsent(temp->playerName);
        }
        cout << endl;

        temp->processed = true;
        if (front == rear) {
            front = rear = nullptr;
        } else {
            front = front->next;
        }

//        front = front->next;
//        if (front == nullptr) rear = nullptr;
//
//        delete temp;
    }

    void displayAllWithdrawals() {
        if (front == nullptr) {
            cout << "No withdrawal records." << endl;
            return;
        }

        cout << "\nAll Withdrawals:\n";
        cout << "------------------------------------------------------\n";
        cout << "Player Name | Substitute Name | Status\n";
        cout << "------------------------------------------------------\n";

        Withdrawal* current = front;
        bool hasRecords = false;
        int count = 0;

        do {
            cout << current->playerName << " | "
                 << (current->substituteName.empty() ? "None" : current->substituteName) << " | "
                 << (current->processed ? "Processed" : "Pending") << endl;
            current = current->next;
        } while (current != front && current != nullptr);


        cout << "------------------------------------------------------\n";
    }


    void searchWithdrawal(const string& playerName) {
        if (front == nullptr) {
            cout << "No withdrawals in the system." << endl;
            return;
        }

        bool found = false;
        Withdrawal* current = front;

        cout << "\nWithdrawal records for Player " << playerName << ":\n";
        cout << "------------------------------------------------------\n";
        cout << "Substitute Name\n";
        cout << "------------------------------------------------------\n";

        while (current != nullptr) {
            if (current->playerName == playerName) {
                if (current->substituteName.empty()) {
                    cout << "None";
                } else {
                    cout << current->substituteName;
                }
                cout << endl;
                found = true;
            }
            current = current->next;
        }

        if (!found) {
            cout << "No withdrawal records found for Player " << playerName << "." << endl;
        }
        cout << "------------------------------------------------------\n";
    }
};

// Function to handle player withdrawal
void withdrawPlayer(WithdrawalQueue &queue, TournamentScheduler &scheduler) {
    string playerName, substituteName = "";
    char hasSubstitute;

    cin.ignore();
    cout << "Enter Player Name to withdraw: ";
    getline(cin, playerName);

    cout << "Is there a substitute player? (y/n): ";
    cin >> hasSubstitute;

    if (hasSubstitute == 'y' || hasSubstitute == 'Y') {
        cin.ignore();
        cout << "Enter Substitute Player Name: ";
        getline(cin, substituteName);
        cout << "Substitute Player " << substituteName << " will replace Player " << playerName << ".\n";
    } else {
        cout << "No substitute available. Opponent will get a Win.\n";
    }

    queue.enqueueWithdrawal(playerName, substituteName);
    cout << "Player " << playerName << " has been added to the withdrawal queue.\n";

    // 🔹 Process the withdrawal immediately instead of waiting
//    queue.processWithdrawal(scheduler);
}

// Function to search for a player's withdrawal records
void searchWithdrawalByPlayer(WithdrawalQueue &queue) {
    string playerName;

    cin.ignore(); // Clear input buffer
    cout << "Enter Player Name to search: ";
    getline(cin, playerName);

    queue.searchWithdrawal(playerName);
}



// Function to handle the withdrawal management menu
void handleWithdrawalMenu(WithdrawalQueue &queue, TournamentScheduler &tournament) {
    int subChoice;

    while (true) {
        cout << "\n === Withdrawal Management === \n";
        cout << "1. Register Player Withdrawal\n";
        cout << "2. View All Withdrawals\n";
        cout << "3. Search for Withdrawals by Player\n";
        cout << "4. Process Next Withdrawal\n";
        cout << "5. Return to Main Menu\n";
        cout << "Enter choice: ";
        cin >> subChoice;

        if (subChoice == 5) {
            cout << "Returning to main menu...\n";
            break;
        }

        switch (subChoice) {
            case 1:
                withdrawPlayer(queue, tournament);
                break;
            case 2:
                queue.displayAllWithdrawals();
                break;
            case 3:
                searchWithdrawalByPlayer(queue);
                break;
            case 4:
                queue.processWithdrawal(tournament);
                break;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}


// TESHWINDEV SINGH BHATT TP068387 MATCH HISTORY TRACKING
struct MatchHistory {
    int matchID;
    string player1;
    string player2;
    int score1;
    int score2;
    string winner;
    string stage;
    string date;
    MatchHistory* next;

    MatchHistory(int id, string p1, string p2, int s1, int s2, string win, string stg = "Unknown", string dt = "")
        : matchID(id), player1(p1), player2(p2), score1(s1), score2(s2), winner(win),
          stage(stg), date(dt), next(nullptr) {
        // If date is empty, use current date
        if (dt.empty()) {
            time_t now = time(nullptr);
            tm* localTime = localtime(&now);
            char buffer[11]; // YYYY-MM-DD + null terminator
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
            date = string(buffer);
        }
    }
};

// Statistics structure to track player performance
struct PlayerStats {
    string playerName;
    int matchesPlayed;
    int matchesWon;
    int totalPointsScored;
    double winRate;
    PlayerStats* next;

    PlayerStats(string name) : playerName(name), matchesPlayed(0), matchesWon(0),
                             totalPointsScored(0), winRate(0.0), next(nullptr) {}
};

class MatchHistoryTracker {
private:
    MatchHistory* top;
    PlayerStats* statsHead;
    const string MATCH_FILENAME = "match_history.txt";
    const string STATS_FILENAME = "player_stats.txt";
    int nextMatchID;

public:
    MatchHistoryTracker() : top(nullptr), statsHead(nullptr), nextMatchID(1) {
        loadFromFile();
    }

    ~MatchHistoryTracker() {
        saveToFile();
        while (top) {
            MatchHistory* temp = top;
            top = top->next;
            delete temp;
        }
        while (statsHead) {
            PlayerStats* temp = statsHead;
            statsHead = statsHead->next;
            delete temp;
        }
    }

    // Records a new match and updates statistics
    void recordMatch(string player1, string player2, int score1, int score2, string stage = "Unknown") {
        string winner = (score1 > score2) ? player1 : player2;

        MatchHistory* newMatch = new MatchHistory(nextMatchID++, player1, player2, score1, score2, winner, stage);

        newMatch->next = top;
        top = newMatch;

        updatePlayerStats(player1, score1, (winner == player1));
        updatePlayerStats(player2, score2, (winner == player2));

        cout << "Match recorded successfully!" << endl;
        saveToFile();
    }

    // Display all match history
    void displayMatchHistory() {
        if (!top) {
            cout << "No match history available." << endl;
            return;
        }

        cout << "\n===== MATCH HISTORY =====\n";
        cout << "+----------+----------------+----------------+--------+--------+----------------+----------------+------------+\n";
        cout << "| Match ID |    Player 1    |    Player 2    | Score1 | Score2 |     Winner     |     Stage      |    Date    |\n";
        cout << "+----------+----------------+----------------+--------+--------+----------------+----------------+------------+\n";

        MatchHistory* temp = top;
        while (temp) {
            cout << "| " << setw(8) << temp->matchID << " | "
                 << setw(14) << temp->player1 << " | "
                 << setw(14) << temp->player2 << " | "
                 << setw(6) << temp->score1 << " | "
                 << setw(6) << temp->score2 << " | "
                 << setw(14) << temp->winner << " | "
                 << setw(14) << temp->stage << " | "
                 << setw(10) << temp->date << " |\n";
            temp = temp->next;
        }
        cout << "+----------+----------------+----------------+--------+--------+----------------+----------------+------------+\n";
    }

    // Search match by ID
    void searchMatchByID(int matchID) {
        MatchHistory* temp = top;
        bool found = false;

        while (temp) {
            if (temp->matchID == matchID) {
                found = true;
                cout << "\n===== MATCH DETAILS =====\n";
                cout << "Match ID: " << temp->matchID << "\n"
                     << "Date: " << temp->date << "\n"
                     << "Stage: " << temp->stage << "\n"
                     << "Player 1: " << temp->player1 << "\n"
                     << "Player 2: " << temp->player2 << "\n"
                     << "Score: " << temp->score1 << " - " << temp->score2 << "\n"
                     << "Winner: " << temp->winner << "\n";
                break;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "No match found with ID " << matchID << "!" << endl;
        }
    }

    // Export match history to a CSV file
    void exportMatchHistory() {
        ofstream file("match_history_export.csv");
        if (!file) {
            cout << "Error opening file for export!" << endl;
            return;
        }

        file << "Match ID,Date,Stage,Player 1,Player 2,Score 1,Score 2,Winner\n";
        MatchHistory* temp = top;
        while (temp) {
            file << temp->matchID << ","
                 << temp->date << ","
                 << temp->stage << ","
                 << temp->player1 << ","
                 << temp->player2 << ","
                 << temp->score1 << ","
                 << temp->score2 << ","
                 << temp->winner << "\n";
            temp = temp->next;
        }
        file.close();
        cout << "Match history exported to match_history_export.csv" << endl;
    }

    // Display player statistics
    void displayPlayerStats() {
        if (!statsHead) {
            cout << "No player statistics available." << endl;
            return;
        }

        cout << "\n===== PLAYER STATISTICS =====\n";
        cout << "+----------------+----------------+----------------+----------------------+---------------+\n";
        cout << "|   Player Name  | Matches Played |  Matches Won   | Total Points Scored  |   Win Rate    |\n";
        cout << "+----------------+----------------+----------------+----------------------+---------------+\n";

        PlayerStats* temp = statsHead;
        while (temp) {
            cout << "| " << setw(14) << temp->playerName << " | "
                 << setw(14) << temp->matchesPlayed << " | "
                 << setw(14) << temp->matchesWon << " | "
                 << setw(20) << temp->totalPointsScored << " | "
                 << setw(11) << fixed << setprecision(2) << temp->winRate * 100 << "% |\n";
            temp = temp->next;
        }
        cout << "+----------------+----------------+----------------+----------------------+---------------+\n";
    }

    // Find top performers
    void displayTopPerformers() {
        if (!statsHead) {
            cout << "No player statistics available." << endl;
            return;
        }

        // Find player with most wins
        PlayerStats* mostWins = statsHead;
        PlayerStats* highestWinRate = statsHead;
        PlayerStats* highestScorer = statsHead;

        PlayerStats* temp = statsHead->next;
        while (temp) {
            if (temp->matchesWon > mostWins->matchesWon) {
                mostWins = temp;
            }
            if (temp->winRate > highestWinRate->winRate && temp->matchesPlayed >= 3) {
                highestWinRate = temp;
            }
            if (temp->totalPointsScored > highestScorer->totalPointsScored) {
                highestScorer = temp;
            }
            temp = temp->next;
        }

        cout << "\n===== TOP PERFORMERS =====\n";
        cout << "Player with most wins: " << mostWins->playerName
             << " (" << mostWins->matchesWon << " wins)\n";

        cout << "Player with highest win rate (min 3 matches): " << highestWinRate->playerName
             << " (" << fixed << setprecision(2) << highestWinRate->winRate * 100 << "%)\n";

        cout << "Player with highest total score: " << highestScorer->playerName
             << " (" << highestScorer->totalPointsScored << " points)\n";
    }

    // Generate tournament summary
    void generateTournamentSummary() {
        if (!top) {
            cout << "No match data available for summary." << endl;
            return;
        }

        int totalMatches = 0;
        int totalPoints = 0;
        double avgPointsPerMatch = 0.0;
        map<string, int> stageMatches;

        MatchHistory* temp = top;
        while (temp) {
            totalMatches++;
            totalPoints += (temp->score1 + temp->score2);
            stageMatches[temp->stage]++;
            temp = temp->next;
        }

        avgPointsPerMatch = totalMatches > 0 ? static_cast<double>(totalPoints) / totalMatches : 0;

        cout << "\n===== TOURNAMENT SUMMARY =====\n";
        cout << "Total Matches Played: " << totalMatches << "\n";
        cout << "Total Points Scored: " << totalPoints << "\n";
        cout << "Average Points Per Match: " << fixed << setprecision(2) << avgPointsPerMatch << "\n\n";

        cout << "Matches by Stage:\n";
        for (const auto& stage : stageMatches) {
            cout << "  " << stage.first << ": " << stage.second << " matches\n";
        }
    }

private:
    // Update or create player statistics
    void updatePlayerStats(const string& playerName, int pointsScored, bool isWinner) {
        PlayerStats* player = findOrCreatePlayer(playerName);

        player->matchesPlayed++;
        player->totalPointsScored += pointsScored;

        if (isWinner) {
            player->matchesWon++;
        }

        // Update win rate
        player->winRate = static_cast<double>(player->matchesWon) / player->matchesPlayed;
    }

    // Find player in stats list
    PlayerStats* findOrCreatePlayer(const string& playerName) {
        // Check if player already exists
        PlayerStats* temp = statsHead;
        while (temp) {
            if (temp->playerName == playerName) {
                return temp;
            }
            temp = temp->next;
        }

        PlayerStats* newPlayer = new PlayerStats(playerName);

        if (!statsHead) {
            statsHead = newPlayer;
        } else {
            newPlayer->next = statsHead;
            statsHead = newPlayer;
        }

        return newPlayer;
    }

    // Save match history and stats to files
    void saveToFile() {
        ofstream matchFile(MATCH_FILENAME);
        if (matchFile) {
            MatchHistory* temp = top;
            while (temp) {
                matchFile << temp->matchID << ","
                         << temp->player1 << ","
                         << temp->player2 << ","
                         << temp->score1 << ","
                         << temp->score2 << ","
                         << temp->winner << ","
                         << temp->stage << ","
                         << temp->date << "\n";
                temp = temp->next;
            }
            matchFile.close();
        }

        // Save player statistics
        ofstream statsFile(STATS_FILENAME);
        if (statsFile) {
            PlayerStats* temp = statsHead;
            while (temp) {
                statsFile << temp->playerName << ","
                         << temp->matchesPlayed << ","
                         << temp->matchesWon << ","
                         << temp->totalPointsScored << ","
                         << temp->winRate << "\n";
                temp = temp->next;
            }
            statsFile.close();
        }
    }

    // Load match history and stats from files
    void loadFromFile() {
        ifstream matchFile(MATCH_FILENAME);
        if (matchFile) {
            string line;
            int maxID = 0;

            while (getline(matchFile, line)) {
                stringstream ss(line);
                string token;
                vector<string> tokens;

                while (getline(ss, token, ',')) {
                    tokens.push_back(token);
                }

                if (tokens.size() >= 6) {
                    int id = stoi(tokens[0]);
                    string p1 = tokens[1];
                    string p2 = tokens[2];
                    int s1 = stoi(tokens[3]);
                    int s2 = stoi(tokens[4]);
                    string winner = tokens[5];
                    string stage = (tokens.size() > 6) ? tokens[6] : "Unknown";
                    string date = (tokens.size() > 7) ? tokens[7] : "";

                    MatchHistory* newMatch = new MatchHistory(id, p1, p2, s1, s2, winner, stage, date);
                    newMatch->next = top;
                    top = newMatch;

                    if (id > maxID) maxID = id;
                }
            }

            nextMatchID = maxID + 1;
            matchFile.close();
        }

        // Load player statistics
        ifstream statsFile(STATS_FILENAME);
        if (statsFile) {
            string line;

            while (getline(statsFile, line)) {
                stringstream ss(line);
                string token;
                vector<string> tokens;

                while (getline(ss, token, ',')) {
                    tokens.push_back(token);
                }

                if (tokens.size() >= 5) {
                    string name = tokens[0];
                    PlayerStats* player = new PlayerStats(name);
                    player->matchesPlayed = stoi(tokens[1]);
                    player->matchesWon = stoi(tokens[2]);
                    player->totalPointsScored = stoi(tokens[3]);
                    player->winRate = stod(tokens[4]);

                    player->next = statsHead;
                    statsHead = player;
                }
            }

            statsFile.close();
        }
    }
};

void handleMatchHistoryMenu(MatchHistoryTracker &tracker) {
    int choice;
    while (true) {
        cout << "\n===== MATCH HISTORY TRACKING =====\n";
        cout << "1. Record a Match Result\n";
        cout << "2. Display Match History\n";
        cout << "3. Search Match by Match ID\n";
        cout << "4. Export Match History\n";
        cout << "5. Display Player Statistics\n";
        cout << "6. Show Top Performers\n";
        cout << "7. Generate Tournament Summary\n";
        cout << "8. Return to Main Menu\n";
        cout << "\nEnter your choice: ";
        choice = getValidatedInput(1, 8);

        if (choice == 8) break;

        switch (choice) {
            case 1: {
                string player1, player2, stage;
                int score1, score2;

                cin.ignore();
                cout << "Enter Player 1 Name: ";
                getline(cin, player1);

                cout << "Enter Player 2 Name: ";
                getline(cin, player2);

                cout << "Enter Tournament Stage (Qualifiers/Round Robin/Knockout): ";
                getline(cin, stage);

                cout << "Enter Score for " << player1 << ": ";
                while (!(cin >> score1) || score1 < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid score. Please enter a non-negative number: ";
                }

                cout << "Enter Score for " << player2 << ": ";
                while (!(cin >> score2) || score2 < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid score. Please enter a non-negative number: ";
                }

                tracker.recordMatch(player1, player2, score1, score2, stage);
                break;
            }
            case 2:
                tracker.displayMatchHistory();
                break;
            case 3: {
                int matchID;
                cout << "Enter Match ID to search: ";
                cin >> matchID;
                tracker.searchMatchByID(matchID);
                break;
            }
            case 4:
                tracker.exportMatchHistory();
                break;
            case 5:
                tracker.displayPlayerStats();
                break;
            case 6:
                tracker.displayTopPerformers();
                break;
            case 7:
                tracker.generateTournamentSummary();
                break;
            default:
                cout << "Invalid choice! Try again.";
                break;
        }
    }
}


// ===============================Main Menu================================
int main() {
    SpectatorManager manager;
    PriorityQueue entranceQueue, exitQueue;
    WithdrawalQueue withdrawalQueue;
    TournamentScheduler tournament;
    WinnerList winnersList;
    WinnerList knockoutPlayers;
    MatchHistoryTracker matchHistoryTracker;
    int choice;

    while (true) {
        displayMainMenu();
        choice = getValidatedInput(1, 5);

        if (choice == 5) {
            cout << "Exiting the program...\n";
            break;
        }

        switch (choice) {
            case 1:
                handleTournamentMenu(tournament, winnersList, knockoutPlayers);
                break;
            case 2:
                handleSpectatorMenu(manager, entranceQueue, exitQueue);
                break;
            case 3:
                handleWithdrawalMenu(withdrawalQueue, tournament);
                break;
            case 4:
                handleMatchHistoryMenu(matchHistoryTracker);
                break;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
