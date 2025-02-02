#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Base Train class to store train information
class BaseTrain {
public:
    string trainso;    // Train Source
    string traind;     // Train Destination
    int seatsno;       // Seat Availability
    double time;       // Departure Time

    BaseTrain() = default; //Creates an object, uses default contructor
    BaseTrain(const string &so, const string &d, int seats, double t)
        : trainso(so), traind(d), seatsno(seats), time(t) {}

    virtual void display() const {    // This fucntion can override derived classes , allows polymorphism**
        cout << "Source: " << trainso 
             << ", Destination: " << traind 
             << ", Seats Available: " << seatsno 
             << ", Departure Time: " << time << endl;
    }
};

// Train class inheriting from BaseTrain
class Train : public BaseTrain {
public:
    Train() = default;
    Train(const string &so, const string &d, int seats, double t)
        : BaseTrain(so, d, seats, t) {}

    void display() const override { //display() function is marked with override,
                                //  indicating that it is intended to override a virtual function in the base class (BaseTrain).
        BaseTrain::display(); // Call the base class display. Outputs all the details
    }
};

// Base Booking class to store booking information
class BaseBooking {
public:
    long long pnr;     // PNR Number
    int train_id;      // Train ID
    string trainso;    // Train Source
    string traind;     // Train Destination
    double time;       // Departure Time
    string name;       // Passenger Name
    int age;           // Passenger Age

    BaseBooking() = default;
    BaseBooking(long long p, int id, const string &so, const string &d, double t, const string &n, int a)
        : pnr(p), train_id(id), trainso(so), traind(d), time(t), name(n), age(a) {}

    virtual void display() const {
        cout << "PNR: " << pnr 
             << ", Source: " << trainso 
             << ", Destination: " << traind 
             << ", Departure Time: " << time 
             << ", Name: " << name 
             << ", Age: " << age << endl;
    }
};

// Booking class inheriting from BaseBooking
class Booking : public BaseBooking {
public:
    Booking() = default;
    Booking(long long p, int id, const string &so, const string &d, double t, const string &n, int a)
        : BaseBooking(p, id, so, d, t, n, a) {}

    void display() const override {
        BaseBooking::display(); // Call the base class display
    }
};

// TrainReservationSystem class to manage the entire system
class TrainReservationSystem {
private:
    map<int, Train> trains; //map stores trains, where the key is an train ID and the value is a Train object.
    vector<Booking> bookings; // this stores  list of all the bookings made in the current object

    string filepath_trains = "C:\\Users\\tashu\\OneDrive\\Desktop\\ALL TRAINS.csv";
string filepath_booked = "C:\\Users\\tashu\\OneDrive\\Desktop\\Book2.csv";
string filepath_admin = "C:\\Users\\tashu\\OneDrive\\Desktop\\Admin file.csv";


    vector<string> split(const string &s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) { // in our code delimiter is comma since csv
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    void load_trains() {
        try {
            ifstream file(filepath_trains); //Creates an ifstream object to open the CSV file
            if (!file.is_open()) throw runtime_error("Could not open the train file!");//If the file fails to open, it throws a runtime error with a message.

            string line; // declares a string variable to store an entire line
            getline(file, line);  // Skip header

            while (getline(file, line)) {
                auto tokens = split(line, ',');
                int train_id = stoi(tokens[0]); // converts string into integer value so "101" now becomes 101 . We store this one seperately due to repeated use
                Train train(tokens[1], tokens[2], stoi(tokens[3]), stod(tokens[4]));
                trains[train_id] = train; // stores into map , where key is train_id and value is train object
            }
            file.close();
        } catch (const exception &e) {
            cerr << "Error loading trains: "  << endl;
        }
    }

    void load_bookings() { //showing all the booked tickets
        try {
            ifstream file(filepath_booked); //open the file for using
            if (!file.is_open()) throw runtime_error("Could not open the booking file!");

            string line; // get the first line of csv file
            getline(file, line);  // Skip header

            while (getline(file, line)) { // while we do not get an empty line i.e csv ends
                auto tokens = split(line, ','); // splitting csv into indexes for easier comparison
                Booking booking(stoll(tokens[0]), -1, tokens[1], tokens[2], stod(tokens[3]), tokens[4], stoi(tokens[5])); // -1 becuase placeholder for train id being loaded from another csv,error prevention
                bookings.push_back(booking); //stoll=string into long integer
            }
            file.close();
        } catch (const exception &e) { //exception handling
            cerr << "Error loading bookings: " << e.what() << endl;
        }
    }

     void save_train() {
    try {
        // Open the file for output in append mode
        ofstream file(filepath_trains, ios::app); // Use ios::app to append data meaning new data will be added at the end of file
        if (!file.is_open()) throw runtime_error("Could not open the file for writing!");

        // Check if the file is empty to decide whether to write the header
        file.seekp(0, ios::end); // Move the pointer  the end of the file
        if (file.tellp() == 0) {  // Check if the file is empty
            file << "Train ID,Train Source,Train Destination,Seats Availability,Departure Time\n";
        }

        // Write train data
        for (const auto &[train_id, train] : trains) { //& creates a reference to the object with key=train id and value=train and iterates through map trains
            file << train_id << "," << train.trainso << "," << train.traind << ","
                 << train.seatsno << "," << train.time << "\n"; //train details written into csv
        }
        file.close();
    } catch (const exception &e) {
        cerr << "Error saving train data: " << e.what() << endl;
    }
}
    long long generate_pnr() {
        return 10000 + rand() % 90000;  // Generates a random 5-digit number between 10000 and 99999
    }

    void save_all_bookings() {
    try {
        // Open the file for output in append mode
        ofstream file(filepath_booked, ios::app); // Use ios::app to append data
        if (!file.is_open()) throw runtime_error("Could not open the booking file!");

        // Check if the file is empty to decide whether to write the header
        file.seekp(0, ios::end); // Move to the end of the file
        if (file.tellp() == 0) {  // Check if the file is empty
            file << "PNR,Source,Destination,Departure Time,Name,Age\n"; // Write header , serves as label for the data that follows 
        }

        // Write booking data
        for (const auto &booking : bookings) { //auto allows the compiler to automatically deduce the type of booking based on the elements in bookings
            file << booking.pnr << "," << booking.trainso << "," //writes the data into file
                 << booking.traind << "," << booking.time << "," 
                 << booking.name << "," << booking.age << "\n";
        }
        file.close();
    } catch (const exception &e) {
        cerr << "Error saving bookings: " << e.what() << endl;
    }
}



    void book_ticket() {
    string source, destination, name;
    int age;

    cout << "Enter Source: ";
    cin.ignore(); // clears input buffers like a newline or a whitespace or newline character previously present in input
    getline(cin, source);
    cout << "Enter Destination: ";
    getline(cin, destination);

    vector<pair<int, Train>> available_trains; //vector name availabe trains is going to store the details as id and details , id primary
    // int = id , trains = data  
    for (const auto &[train_id, train] : trains) {
        if (train.trainso == source && train.traind == destination) {
            available_trains.emplace_back(train_id, train); // for protection , makes sure no copies made
        }
    }

    if (available_trains.empty()) {
        cout << "No trains available.\n";
        return;
    }

    cout << "Available Trains:\n";
    for (const auto &[train_id, train] : available_trains) {
        cout << "Train ID: " << train_id << ", Departure Time: " << train.time << endl;
    }

    int chosen_train_id;
    cout << "Enter Train ID to book: ";
    cin >> chosen_train_id;

    auto it = find_if(available_trains.begin(), available_trains.end(),
                      [chosen_train_id](const auto &pair) {// this is a lamda fucntion meaning we use this id only till here , in this part of code
                      // safety to ensure a value . here pair refers to a pair<int, Train> from the vector meaning id and train
                          return pair.first == chosen_train_id; // first object is id , second is train object details
                      });

    if (it != available_trains.end()) {
        auto &[id, train] = *it;
        if (train.seatsno > 0) {
            cout << "Enter Passenger Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Passenger Age: ";
            cin >> age;

            Booking booking(generate_pnr(), id, train.trainso, train.traind, train.time, name, age);
            bookings.push_back(booking);
            train.seatsno--;

            save_train();  // Save train data to CSV
            save_all_bookings();  // Save all bookings to CSV (No argument needed)
            cout << "Ticket booked! PNR: " << booking.pnr << endl;
        } else {
            cout << "No seats available.\n";
        }
    } else {
        cout << "Invalid Train ID.\n";
    }
}


    void view_booked_tickets() {
        if (bookings.empty()) {
            cout << "No tickets booked.\n";
            return;
        }

        long long pnr;
        cout << "Enter PNR: ";
        cin >> pnr;

        auto it = find_if(bookings.begin(), bookings.end(), [pnr](const Booking &b) {
            return b.pnr == pnr;
            /* Lambda function [pnr](const Booking &b) { return b.pnr == pnr; }:
            This lambda captures the PNR provided by the user.
            For each Booking object (b) in the vector, it checks if b.pnr (the PNR of the current booking) matches the entered PNR.
            If it finds a matching booking, find_if returns an iterator pointing to that booking.*/
        });

        if (it != bookings.end()) { //it is not equal to booking end emaning iterator was found
            it->display();
        } else {
            cout << "Booking not found.\n";
        }
    }

 void cancel_ticket() {
    if (bookings.empty()) {
        cout << "No tickets to cancel.\n";
        return;
    }

    long long pnr;
    cout << "Enter PNR: ";
    cin >> pnr;

    // Find the booking to cancel
    auto it = find_if(bookings.begin(), bookings.end(), [pnr](const Booking &b) {
        return b.pnr == pnr;
    });

    if (it != bookings.end()) {
        // Booking found
        cout << "Booking found: ";
        it->display(); // Show booking details

        int train_id = it->train_id; // Get the train ID from the booking

        // Check if the train ID exists in the train map
        if (trains.find(train_id) != trains.end()) {
            trains[train_id].seatsno++; // Increment the seat count for the train
            cout << "Restored seat for Train ID " << train_id << ". New seat count: "
                 << trains[train_id].seatsno << endl;
        } else {
            cout << "Warning: Train ID not found in the system.\n"; // Safety check
        }

        // Remove the booking from the vector
        bookings.erase(it); //removes the entry pointed by the iterator 
        cout << "Booking removed from the system.\n";

        // Save changes to CSV files
        save_all_bookings();
        save_train();
        cout << "Ticket canceled successfully and data saved to CSV.\n";
    } else {
        cout << "Booking not found for PNR: " << pnr << ".\n";
    }
}


    void modify_train() {
        int train_id;
        cout << "Enter the Train ID to modify: ";
        while (!(cin >> train_id)) {
            cout << "Invalid input! Please enter a valid Train ID: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //numeric limits tells us to ignore as many unwanted digits as possible
        }
        cin.ignore();

        if (trains.find(train_id) != trains.end()) { //if train id exists then find will not be the end of file so program continues
            Train &train = trains[train_id]; //this creates a reference to train object
            cout << "Enter the New Train Source: ";
            getline(cin, train.trainso); //new value assigned 
            cout << "Enter the New Train Destination: ";
            getline(cin, train.traind);//new value assigned

            cout << "Enter the New Seat Availability: ";
            while (!(cin >> train.seatsno)) { // provides a valid integer , while loop until user provides the correct input
                cout << "Invalid input! Please enter a valid seat number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');//same as before , ignore input buffer
            }

            cout << "Enter the New Time: ";
            while (!(cin >> train.time)) {
                cout << "Invalid input! Please enter a valid time: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            save_train();
            cout << "Train details updated successfully!\n";
        } else {
            cout << "Train not found!\n";
        }
    }

    void show_admin_menu() {
        cout << "\nAdmin Menu:\n";
        cout << "1. View All Bookings\n";
        cout << "2. View All Trains\n";
        cout << "3. Modify Train Details\n";
        cout << "4. Exit Admin Menu\n";
    }
    // Method to load admin credentials from a CSV file
bool validate_admin_credentials(const string &username, const string &password) { // const makes sure we dont accidently modify username or password
    try {
        ifstream file(filepath_admin);
        if (!file.is_open()) throw runtime_error("Could not open the admin credentials file!");

        string line;
        getline(file, line);  // Skip header if present

        while (getline(file, line)) {
            auto tokens = split(line, ',');
            if (tokens.size() == 2) { // Ensure we have both username and password i.e only two values
                if (tokens[0] == username && tokens[1] == password) {
                    return true; // Credentials match
                }
            }
        }
        file.close();
    } catch (const exception &e) {
        cerr << "Error loading admin credentials: " << e.what() << endl;
    }
    return false; // No match found
}


    void admin_panel() {
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    // Validate credentials against the CSV file
    if (validate_admin_credentials(username, password)) {
        while (true) {
            show_admin_menu();
            int choice;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    // View all bookings
                    if (bookings.empty()) {
                        cout << "No bookings available.\n";
                    } else {
                        for (const auto &booking : bookings) {
                            booking.display();
                        }
                    }
                    break;
                }
                case 2: {
                    // View all trains
                    cout << "Available Trains:\n";
                    for (const auto &entry : trains) {
                        const Train &train = entry.second;
                        cout << "Train ID: " << entry.first //exists keyword in map
                             << ", Source: " << train.trainso 
                             << ", Destination: " << train.traind 
                             << ", Seats Available: " << train.seatsno 
                             << ", Departure Time: " << train.time << endl;
                    }
                    break;
                }
                case 3:
                    modify_train();  // Call modify train function
                    break;
                case 4:
                    cout << "Exiting admin menu.\n";
                    return;
                default:
                    cout << "Invalid option! Please try again.\n";
            }
        }
    } else {
        cout << "Invalid credentials. Access denied.\n";
    }
}


   void main_menu() {
    int ab;
    while (true) {
        cout << "\nTrain Reservation System:\n";
        cout << "1. Book a Ticket\n";
        cout << "2. View Booked Tickets\n";
        cout << "3. Cancel a Ticket\n";
        cout << "4. Admin Login\n";
        cout << "5. Exit\n";  // Added newline for better formatting
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                book_ticket();
                break;
            case 2:
                view_booked_tickets();
                break;
            case 3:
                cancel_ticket();
                break;
            case 4:
                admin_panel();
                break;
            case 5:
                cout << "Please provide feedback!" << endl;
                cout << "1) Satisfactory" << endl;
                cout << "2) Non-Satisfactory" << endl;
                cin >> ab;
                cout << "Thank you for your feedback!" << endl;  // Added feedback acknowledgment
                cout << "Exiting...\n";
                return; // Exit the loop and the program
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
};


int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
    // Used for random number generation
    TrainReservationSystem system;

    system.load_trains(); // Load train data
    system.load_bookings(); // Load booking data
    system.main_menu(); // Start the main menu

    return 0;
}
