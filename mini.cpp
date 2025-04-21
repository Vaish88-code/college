#include <iostream>
#include <vector>
#include <string>
#include <limits> // Required for numeric_limits
#include <cstdlib> // Required for system("cls") or system("clear")
#include <ios>     // Required for streamsize
#include <map>     // For storing attendance temporarily

// --- Clear Screen Function ---
// Platform specific clear screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // Assume POSIX (Linux, macOS)
    system("clear");
#endif
}

// --- Role Enumeration ---
enum class Role {
    STUDENT,
    TEACHER,
    NON_TEACHING_STAFF
};

// --- User Data Structure ---
struct User {
    std::string name;
    std::string email;
    std::string password;
    std::string phone;
    Role role;
    std::string roleSpecificData; // Student ID, Teacher Position, Staff Job
};

// --- Data Structures for Shared Info ---
struct Complaint {
    std::string studentEmail; // Identify who submitted
    std::string studentName;
    std::string message;
    // Could add timestamp later
};

struct LeaveNotice {
    std::string studentEmail;
    std::string studentName;
    std::string dates;
    std::string reason;
    // Could add submission timestamp later
};

// --- Global User & Shared Data Storage (In-memory "database") ---
std::vector<User> users;
std::vector<Complaint> studentComplaints;
std::vector<LeaveNotice> studentLeaveNotices;

// --- Function Declarations ---
void displayMainMenu();
void handleRegistration();
bool handleLogin(User& loggedInUser); // Pass by reference to store logged-in user info

// Student Dashboard & Functions
void showStudentDashboard(const User& user);
void displayCollegeEvents();
void displayAttendance();
void handleComplaintBox(const User& student); // Pass student info
void handleLeaveNotice(const User& student); // Pass student info
void displayResults();

// Teacher Dashboard & Functions
void showTeacherDashboard(const User& user);
void displayTeacherMeetings();
void handleResultUpload();
void handleAttendanceTaking();
void viewStudentComplaints();
void viewStudentLeaveNotices();

// Non-Teaching Staff Dashboard
void showNonTeachingStaffDashboard(const User& user);

// Helper Function
void ignoreLine();

// --- Main Function ---
int main() {
    // Optional: Add default users for easy testing
    users.push_back({"Alice Smith", "student@test.com", "pass123", "1234567890", Role::STUDENT, "S1001"});
    users.push_back({"Bob Johnson", "student2@test.com", "pass123", "1122334455", Role::STUDENT, "S1002"});
    users.push_back({"Prof. Davis", "teacher@test.com", "pass456", "0987654321", Role::TEACHER, "Professor"});
    users.push_back({"Mr. Lee", "staff@test.com", "pass789", "5556667777", Role::NON_TEACHING_STAFF, "Librarian"});

    int choice;
    bool loggedIn = false;
    User currentUser; // To store details of the logged-in user

    while (true) {
        clearScreen();
        displayMainMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            ignoreLine();
            std::cout << "Press Enter to continue...";
            ignoreLine(); // Consume the Enter press
            continue;
        }
        ignoreLine(); // Consume the newline character left by cin

        switch (choice) {
            case 1: // Login
                clearScreen();
                loggedIn = handleLogin(currentUser);
                if (loggedIn) {
                    switch (currentUser.role) {
                        case Role::STUDENT:
                            showStudentDashboard(currentUser);
                            break;
                        case Role::TEACHER:
                            showTeacherDashboard(currentUser);
                            break;
                        case Role::NON_TEACHING_STAFF:
                            showNonTeachingStaffDashboard(currentUser);
                            break;
                    }
                    loggedIn = false; // Reset login status after logout/dashboard exit
                    currentUser = {}; // Clear current user data
                }
                break;
            case 2: // Register
                clearScreen();
                handleRegistration();
                break;
            case 3: // Exit
                std::cout << "Exiting College Alerter. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cout << "Press Enter to continue...";
                std::cin.get(); // Wait for user press Enter
                break;
        }
    }

    return 0;
}

// --- Function Implementations ---

void ignoreLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayMainMenu() {
    std::cout << "=====================================\n";
    std::cout << "        Welcome to College Alerter\n";
    std::cout << "=====================================\n";
    std::cout << "1. Login\n";
    std::cout << "2. Register\n";
    std::cout << "3. Exit\n";
    std::cout << "=====================================\n";
}

void handleRegistration() {
    User newUser;
    int roleChoice;
    int staffChoice;

    std::cout << "--- Registration ---\n";
    std::cout << "Enter Name: ";
    std::getline(std::cin, newUser.name);

    std::cout << "Enter Email: ";
    std::getline(std::cin, newUser.email);

    // Basic check if email already exists
    for(const auto& user : users) {
        if (user.email == newUser.email) {
            std::cout << "\nError: Email already registered.\n";
            std::cout << "Press Enter to return to the main menu...";
            std::cin.get();
            return;
        }
    }

    std::cout << "Enter Phone Number: ";
    std::getline(std::cin, newUser.phone);

    std::cout << "Enter Password: ";
    std::getline(std::cin, newUser.password);

    std::cout << "\nSelect Role:\n";
    std::cout << "1. Student\n";
    std::cout << "2. Teacher\n";
    std::cout << "3. Non-Teaching Staff\n";
    std::cout << "Enter role choice: ";
    std::cin >> roleChoice;

    while (std::cin.fail() || (roleChoice < 1 || roleChoice > 3)) {
         std::cout << "Invalid input. Please enter 1, 2, or 3: ";
         std::cin.clear();
         ignoreLine();
         std::cin >> roleChoice;
    }
    ignoreLine(); // Consume newline

    switch (roleChoice) {
        case 1:
            newUser.role = Role::STUDENT;
            std::cout << "Enter Student ID Number: ";
            std::getline(std::cin, newUser.roleSpecificData);
            break;
        case 2:
            newUser.role = Role::TEACHER;
            std::cout << "Enter Teacher Position (e.g., Professor, Asst. Professor): ";
            std::getline(std::cin, newUser.roleSpecificData);
            break;
        case 3:
            newUser.role = Role::NON_TEACHING_STAFF;
            std::cout << "\nSelect Non-Teaching Staff Role:\n";
            std::cout << "1. Librarian\n";
            std::cout << "2. Watchman\n";
            std::cout << "3. Other Clerical/Support Staff\n";
            std::cout << "Enter staff role choice: ";
            std::cin >> staffChoice;

             while (std::cin.fail() || (staffChoice < 1 || staffChoice > 3)) {
                std::cout << "Invalid input. Please enter 1, 2, or 3: ";
                std::cin.clear();
                ignoreLine();
                std::cin >> staffChoice;
            }
             ignoreLine(); // Consume newline

            switch (staffChoice) {
                case 1: newUser.roleSpecificData = "Librarian"; break;
                case 2: newUser.roleSpecificData = "Watchman"; break;
                case 3: newUser.roleSpecificData = "Clerical/Support Staff"; break;
            }
            break;
    }

    users.push_back(newUser);
    std::cout << "\nRegistration Successful!\n";
    std::cout << "Press Enter to return to the main menu...";
    std::cin.get();
}

bool handleLogin(User& loggedInUser) {
    std::string email, password;
    std::cout << "--- Login ---\n";
    std::cout << "Enter Email: ";
    std::getline(std::cin, email);
    std::cout << "Enter Password: ";
    std::getline(std::cin, password);

    for (const auto& user : users) {
        if (user.email == email && user.password == password) {
            std::cout << "\nLogin Successful! Welcome, " << user.name << ".\n";
            loggedInUser = user;
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return true;
        }
    }

    std::cout << "\nLogin Failed: Invalid email or password.\n";
    std::cout << "Press Enter to return to the main menu...";
    std::cin.get();
    return false;
}

// --- Student Dashboard and Functions ---

void showStudentDashboard(const User& user) {
    int choice;
    while (true) {
        clearScreen();
        std::cout << "=====================================\n";
        std::cout << "        Student Dashboard - Welcome " << user.name << "\n";
        std::cout << "=====================================\n";
        std::cout << "1. View College Events\n";
        std::cout << "2. View Attendance\n";
        std::cout << "3. Submit Complaint\n";
        std::cout << "4. Submit Leave Notice\n";
        std::cout << "5. View Results\n";
        std::cout << "6. Logout\n";
        std::cout << "=====================================\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            ignoreLine();
            std::cout << "Press Enter to continue...";
            ignoreLine();
            continue;
        }
        ignoreLine();

        switch (choice) {
            case 1: displayCollegeEvents(); break;
            case 2: displayAttendance(); break;
            case 3: handleComplaintBox(user); break; // Pass user info
            case 4: handleLeaveNotice(user); break; // Pass user info
            case 5: displayResults(); break;
            case 6:
                std::cout << "Logging out...\n";
                std::cout << "Press Enter to return to main menu...";
                std::cin.get();
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
        if (choice != 6) {
             std::cout << "\nPress Enter to return to the Student Dashboard...";
             std::cin.get();
        }
    }
}

void displayCollegeEvents() {
    clearScreen();
    std::cout << "--- Upcoming College Events ---\n";
    std::cout << "1. Tech Fest 'Innovate 2024'\n";
    std::cout << "   Date: 20 March, 2024\n";
    std::cout << "   Venue: College Auditorium\n\n";
    std::cout << "2. Annual Sports Day\n";
    std::cout << "   Date: 05 April, 2024\n";
    std::cout << "   Venue: College Sports Ground\n\n";
    std::cout << "3. Guest Lecture on AI Ethics\n";
    std::cout << "   Date: 15 April, 2024\n";
    std::cout << "   Venue: Seminar Hall B\n";
    std::cout << "--------------------------------\n";
}

void displayAttendance() {
    clearScreen();
    std::cout << "--- Your Attendance (Dummy Data) ---\n";
    std::cout << "Overall Attendance: 85%\n\n";
    std::cout << "Subject Wise:\n";
    std::cout << "- Data Structures: 90%\n";
    std::cout << "- Operating Systems: 82%\n";
    std::cout << "- Database Management: 78%\n";
    std::cout << "- Computer Networks: 91%\n";
     std::cout << "------------------------------------\n";
     // In a real system, this data would be fetched based on teacher uploads
}

void handleComplaintBox(const User& student) {
    clearScreen();
    Complaint newComplaint;
    newComplaint.studentEmail = student.email; // Store student email
    newComplaint.studentName = student.name;   // Store student name
    std::cout << "--- Submit Complaint ---\n";
    std::cout << "Please type your complaint below and press Enter:\n";
    std::cout << "----------------------------------------------\n";
    std::getline(std::cin, newComplaint.message);
    std::cout << "\n----------------------------------------------\n";

    studentComplaints.push_back(newComplaint); // Add to global list

    std::cout << "Thank you. Your complaint has been recorded.\n";
    std::cout << "----------------------------------------------\n";
}

void handleLeaveNotice(const User& student) {
    clearScreen();
    LeaveNotice newNotice;
    newNotice.studentEmail = student.email;
    newNotice.studentName = student.name;

    std::cout << "--- Submit Leave Notice ---\n";
    std::cout << "Enter Date(s) of Leave (e.g., 25 Feb 2024 or 25-27 Feb 2024): ";
    std::getline(std::cin, newNotice.dates);
    std::cout << "Enter Reason for Leave:\n";
    std::cout << "---------------------------\n";
    std::getline(std::cin, newNotice.reason);
    std::cout << "\n---------------------------\n";

    studentLeaveNotices.push_back(newNotice); // Add to global list

    std::cout << "Leave notice submitted for "<< newNotice.dates << ".\n";
    std::cout << "---------------------------\n";
}

void displayResults() {
    clearScreen();
    std::cout << "--- Your Results (Dummy Data - Last Semester) ---\n";
    std::cout << "Subject             | Grade\n";
    std::cout << "--------------------|-------\n";
    std::cout << "Data Structures     | A\n";
    std::cout << "Mathematics III     | B+\n";
    std::cout << "Digital Logic       | A-\n";
    std::cout << "Economics           | B\n";
    std::cout << "--------------------|-------\n";
    std::cout << "SGPA: 8.8\n";
    std::cout << "---------------------------\n";
    // In a real system, this data would be fetched based on teacher uploads
}


// --- Teacher Dashboard and Functions ---

void showTeacherDashboard(const User& user) {
    int choice;
    while (true) {
        clearScreen();
        std::cout << "=====================================\n";
        std::cout << "        Teacher Dashboard - Welcome " << user.name << "\n";
        std::cout << "        Position: " << user.roleSpecificData << "\n";
        std::cout << "=====================================\n";
        std::cout << "1. View/Announce Meetings\n";
        std::cout << "2. Upload Student Results\n";
        std::cout << "3. Take/Upload Attendance\n";
        std::cout << "4. View Student Complaints (" << studentComplaints.size() << ")\n"; // Show count
        std::cout << "5. View Student Leave Notices (" << studentLeaveNotices.size() << ")\n"; // Show count
        std::cout << "6. Logout\n";
        std::cout << "=====================================\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            ignoreLine();
            std::cout << "Press Enter to continue...";
            ignoreLine();
            continue;
        }
        ignoreLine();

        switch (choice) {
            case 1: displayTeacherMeetings(); break;
            case 2: handleResultUpload(); break;
            case 3: handleAttendanceTaking(); break;
            case 4: viewStudentComplaints(); break;
            case 5: viewStudentLeaveNotices(); break;
            case 6:
                std::cout << "Logging out...\n";
                 std::cout << "Press Enter to return to main menu...";
                std::cin.get();
                return; // Exit the teacher dashboard loop
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
         if (choice != 6) {
             std::cout << "\nPress Enter to return to the Teacher Dashboard...";
             std::cin.get();
        }
    }
}

void displayTeacherMeetings() {
    clearScreen();
    std::cout << "--- Scheduled/Announced Meetings ---\n";
    std::cout << "1. Department Faculty Meeting\n";
    std::cout << "   Date: 18 February, 2024\n";
    std::cout << "   Time: 03:00 PM\n";
    std::cout << "   Venue: Staff Room\n\n";
    std::cout << "2. Curriculum Review Committee\n";
    std::cout << "   Date: 25 February, 2024\n";
    std::cout << "   Time: 11:00 AM\n";
    std::cout << "   Venue: Conference Hall\n\n";
    std::cout << "3. [Placeholder for adding new meetings]\n";
    std::cout << "-------------------------------------\n";
    // Could add functionality here for teachers to add new meetings
}

void handleResultUpload() {
    clearScreen();
    std::string subjectName;
    std::string filePath;
    std::cout << "--- Upload Student Results ---\n";
    std::cout << "Enter Subject Name: ";
    std::getline(std::cin, subjectName);
    std::cout << "Enter path to results file (e.g., C:\\results\\DS_results.csv): ";
    std::getline(std::cin, filePath);
    std::cout << "\n--- Simulation ---\n";
    std::cout << "Simulating upload of results for subject '" << subjectName << "' from file '" << filePath << "'.\n";
    std::cout << "In a real application, the file would be processed and data stored.\n";
    std::cout << "Results are now considered 'uploaded' for students to view (in this simulation).\n";
     std::cout << "-----------------------------\n";
}

void handleAttendanceTaking() {
    clearScreen();
    std::string subjectName;
    std::string date;
    char presentChoice;
    std::map<std::string, bool> attendanceRecord; // Map email to present status

    std::cout << "--- Take Attendance ---\n";
    std::cout << "Enter Subject Name: ";
    std::getline(std::cin, subjectName);
    std::cout << "Enter Date (e.g., 15 Feb 2024): ";
    std::getline(std::cin, date);
    std::cout << "----------------------\n";
    std::cout << "Mark Attendance (P=Present, A=Absent):\n";

    int studentCount = 0;
    for (const auto& user : users) {
        if (user.role == Role::STUDENT) {
            studentCount++;
            std::cout << "- " << user.name << " (" << user.roleSpecificData << "): ";
            while (true) {
                std::cin >> presentChoice;
                presentChoice = tolower(presentChoice);
                if (presentChoice == 'p' || presentChoice == 'a') {
                    attendanceRecord[user.email] = (presentChoice == 'p'); // Store true if present, false if absent
                    ignoreLine(); // Consume newline after single char input
                    break;
                } else {
                    std::cout << "Invalid input. Enter 'p' or 'a': ";
                    std::cin.clear();
                    ignoreLine();
                }
            }
        }
    }

    if (studentCount == 0) {
        std::cout << "No students found to take attendance for.\n";
    } else {
        std::cout << "\n--- Attendance Summary (" << subjectName << " - " << date << ") ---\n";
        for(const auto& pair : attendanceRecord) {
            std::string studentName = "Unknown";
            // Find student name again (could optimize this)
            for(const auto& user : users) {
                if(user.email == pair.first) {
                    studentName = user.name;
                    break;
                }
            }
            std::cout << studentName << ": " << (pair.second ? "Present" : "Absent") << "\n";
        }
        std::cout << "--------------------------------------------------------\n";
        std::cout << "Attendance Recorded (Simulated Save).\n";
        // In a real system, this map would be saved to a database/file.
    }
     std::cout << "----------------------\n";
}


void viewStudentComplaints() {
    clearScreen();
    std::cout << "--- Received Student Complaints ---\n";
    if (studentComplaints.empty()) {
        std::cout << "No complaints have been submitted yet.\n";
    } else {
        int count = 1;
        for (const auto& complaint : studentComplaints) {
            std::cout << count++ << ". From: " << complaint.studentName << " (" << complaint.studentEmail << ")\n";
            std::cout << "   Complaint: " << complaint.message << "\n";
            std::cout << "---------------------------------------\n";
        }
    }
     std::cout << "-----------------------------------\n";
}

void viewStudentLeaveNotices() {
    clearScreen();
    std::cout << "--- Received Student Leave Notices ---\n";
    if (studentLeaveNotices.empty()) {
        std::cout << "No leave notices have been submitted yet.\n";
    } else {
        int count = 1;
        for (const auto& notice : studentLeaveNotices) {
            std::cout << count++ << ". From: " << notice.studentName << " (" << notice.studentEmail << ")\n";
            std::cout << "   Dates: " << notice.dates << "\n";
            std::cout << "   Reason: " << notice.reason << "\n";
            std::cout << "----------------------------------------\n";
        }
    }
     std::cout << "--------------------------------------\n";
}


// --- Non-Teaching Staff Dashboard ---

void showNonTeachingStaffDashboard(const User& user) {
    clearScreen();
    std::cout << "=====================================\n";
    std::cout << "   Non-Teaching Staff Dashboard - Welcome " << user.name << "\n";
    std::cout << "        Role: " << user.roleSpecificData << "\n";
    std::cout << "=====================================\n";
    // Add staff-specific options here based on roleSpecificData if needed
    if (user.roleSpecificData == "Librarian") {
         std::cout << "1. Manage Books (Not Implemented)\n";
         std::cout << "2. View Overdue Books (Not Implemented)\n";
    } else {
        std::cout << "General staff features not implemented yet.\n";
    }
     std::cout << "\nPress Enter to Logout...";
    std::cin.get(); // Wait for user
}