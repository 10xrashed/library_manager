#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>
#include <stdexcept>
#include <memory>
// Professional Library Management System
// Author: 10xRashed
using namespace std;
void displayWelcome();
void displayMenu();
void clearInputBuffer();
int getValidInteger(const string& prompt, int minValue = 0);
string getValidString(const string& prompt, bool allowEmpty = false);
string toLowerCase(const string& str);
void pauseScreen(const string& message = "Press Enter to continue...");
namespace Color {
    const string RESET = "\033[0m";
    const string RED = "\033[1;31m";
    const string GREEN = "\033[1;32m";
    const string YELLOW = "\033[1;33m";
    const string BLUE = "\033[1;34m";
    const string MAGENTA = "\033[1;35m";
    const string CYAN = "\033[1;36m";
    const string WHITE = "\033[1;37m";
    const string GRAY = "\033[1;90m";
    const string BG_BLUE = "\033[1;44m";
    const string BG_GREEN = "\033[1;42m";
    const string BG_YELLOW = "\033[1;43m";
    const string BG_MAGENTA = "\033[1;45m";
    const string BG_CYAN = "\033[1;46m";
    const string BG_RED = "\033[1;41m";
}
class Book {
private:
    int id;
    string title;
    string author;
    string genre;
    int publicationYear;
    bool available;
    string borrowerName;
    string borrowDate;
    string truncate(const string& str, size_t maxLen) const {
        return (str.length() > maxLen) ? str.substr(0, maxLen - 3) + "..." : str;
    }

public:
    Book() : id(0), title(""), author(""), genre(""), publicationYear(0), 
             available(true), borrowerName(""), borrowDate("") {}
    
    Book(int bookId, const string& bookTitle, const string& bookAuthor, 
         const string& bookGenre = "", int year = 0) 
         : id(bookId), title(bookTitle), author(bookAuthor), genre(bookGenre), 
           publicationYear(year), available(true), borrowerName(""), borrowDate("") {}
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getGenre() const { return genre; }
    int getPublicationYear() const { return publicationYear; }
    bool isAvailable() const { return available; }
    string getBorrowerName() const { return borrowerName; }
    string getBorrowDate() const { return borrowDate; }
    bool setId(int bookId) { 
        if (bookId <= 0) return false;
        id = bookId; 
        return true;
    }
    
    bool setTitle(const string& bookTitle) { 
        if (bookTitle.empty()) return false;
        title = bookTitle; 
        return true;
    }
    
    bool setAuthor(const string& bookAuthor) { 
        if (bookAuthor.empty()) return false;
        author = bookAuthor; 
        return true;
    }
    
    void setGenre(const string& bookGenre) { genre = bookGenre; }
    void setPublicationYear(int year) { publicationYear = year; }
    bool borrowBook(const string& borrower, const string& date) {
        if (!available || borrower.empty()) return false;
        available = false;
        borrowerName = borrower;
        borrowDate = date;
        return true;
    }

    bool returnBook() {
        if (available) return false;
        available = true;
        borrowerName = "";
        borrowDate = "";
        return true;
    }
    void displayBook() const {
        cout << Color::WHITE << left << setw(8) << id;  
        cout << Color::YELLOW << setw(25) << truncate(title, 24);
        cout << Color::CYAN << setw(20) << truncate(author, 19);
        cout << Color::MAGENTA << setw(15) << truncate(genre, 14);
        cout << Color::WHITE << setw(8) << (publicationYear == 0 ? "" : to_string(publicationYear));
        
        if (available) {
            cout << Color::GREEN << setw(12) << "Available";
            cout << Color::GRAY << setw(20) << "";
        } else {
            cout << Color::RED << setw(12) << "Borrowed";
            cout << Color::YELLOW << setw(20) << truncate(borrowerName, 19);
        }
        cout << Color::RESET << endl;
    }

    void displayDetailed() const {
        cout << "\n" << Color::BG_BLUE << Color::WHITE;
        cout << string(60, '=');
        cout << "\n         BOOK DETAILS         ";
        cout << "\n" << string(60, '=');
        cout << Color::RESET << "\n\n";
        
        cout << Color::YELLOW << "ID:               " << Color::WHITE << id << Color::RESET << endl;
        cout << Color::YELLOW << "Title:            " << Color::WHITE << title << Color::RESET << endl;
        cout << Color::YELLOW << "Author:           " << Color::WHITE << author << Color::RESET << endl;
        cout << Color::YELLOW << "Genre:            " << Color::WHITE << (genre.empty() ? "N/A" : genre) << Color::RESET << endl;
        cout << Color::YELLOW << "Publication Year: " << Color::WHITE << (publicationYear == 0 ? "Unknown" : to_string(publicationYear)) << Color::RESET << endl;
        cout << Color::YELLOW << "Status:           ";
        
        if (available) {
            cout << Color::GREEN << "Available" << Color::RESET << endl;
        } else {
            cout << Color::RED << "Borrowed" << Color::RESET << endl;
            cout << Color::YELLOW << "Borrowed by:      " << Color::CYAN << borrowerName << Color::RESET << endl;
            cout << Color::YELLOW << "Borrow Date:      " << Color::CYAN << borrowDate << Color::RESET << endl;
        }
        
        cout << "\n" << Color::BG_BLUE << Color::WHITE << string(60, '=') << Color::RESET << "\n";
    }
    void save(ostream& os) const {
        os << id << "|" 
           << title << "|" 
           << author << "|" 
           << genre << "|"
           << publicationYear << "|" 
           << available << "|" 
           << borrowerName << "|"
           << borrowDate << "\n";
    }

    static Book load(istream& is) {
        Book book;
        string line;
        
        if (getline(is, line)) {
            stringstream ss(line);
            string segment;

            try {
                if (getline(ss, segment, '|')) book.id = stoi(segment);
                if (getline(ss, segment, '|')) book.title = segment;
                if (getline(ss, segment, '|')) book.author = segment;
                if (getline(ss, segment, '|')) book.genre = segment;
                if (getline(ss, segment, '|')) book.publicationYear = stoi(segment);
                if (getline(ss, segment, '|')) book.available = (segment == "1");
                if (getline(ss, segment, '|')) book.borrowerName = segment;
                if (getline(ss, segment, '|')) book.borrowDate = segment;
            } catch (const invalid_argument& e) {
                cerr << Color::RED << "Error parsing book data: Invalid format" << Color::RESET << endl;
            } catch (const out_of_range& e) {
                cerr << Color::RED << "Error parsing book data: Value out of range" << Color::RESET << endl;
            }
        }
        return book;
    }
    bool operator<(const Book& other) const {
        return id < other.id;
    }
};

class Library {
private:
    vector<Book> books;
    string libraryName;
    const string DATA_FILENAME = "library_data.txt";
    const string BACKUP_FILENAME = "library_backup.txt";

    string getCurrentDate() const {
        auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        stringstream ss;
        ss << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S"); 
        return ss.str();
    }

    void createBackup() const {
        try {
            ifstream src(DATA_FILENAME, ios::binary);
            ofstream dst(BACKUP_FILENAME, ios::binary);
            dst << src.rdbuf();
        } catch (const exception& e) {
            cerr << Color::YELLOW << "Warning: Could not create backup: " << e.what() << Color::RESET << endl;
        }
    }

public:
    Library(const string& name = "City Library") : libraryName(name) {
        loadFromFile(DATA_FILENAME);
    }

    ~Library() {
        saveToFile(DATA_FILENAME);
    }

    bool isEmpty() const { return books.empty(); }
    size_t getBookCount() const { return books.size(); }
    string getLibraryName() const { return libraryName; }
    void setLibraryName(const string& name) { libraryName = name; }

    bool addBook(int id, const string& title, const string& author, 
                 const string& genre = "", int year = 0) {
        if (id <= 0) {
            cout << Color::RED << "Error: Book ID must be a positive number." << Color::RESET << endl;
            return false;
        }
        
        if (title.empty() || author.empty()) {
            cout << Color::RED << "Error: Title and author cannot be empty." << Color::RESET << endl;
            return false;
        }
        
        if (findBookIndex(id) != -1) {
            cout << Color::RED << "Error: Book with ID " << id << " already exists." << Color::RESET << endl;
            return false;
        }
        
        books.emplace_back(id, title, author, genre, year);
        
        cout << "\n" << Color::GREEN << "SUCCESS: " << Color::RESET;
        cout << "Book '" << Color::YELLOW << title << Color::RESET << "' added to the library." << endl;
        
        return true;
    }

    bool deleteBook(int id) {
        if (isEmpty()) {
            cout << Color::RED << "Error: Library is empty." << Color::RESET << endl;
            return false;
        }
        
        int index = findBookIndex(id);
        if (index == -1) {
            cout << Color::RED << "Error: Book with ID " << id << " not found." << Color::RESET << endl;
            return false;
        }
        
        string title = books[index].getTitle();
        books.erase(books.begin() + index);
        
        cout << Color::GREEN << "Success: Book '" << title << "' deleted." << Color::RESET << endl;
        return true;
    }

    bool updateBook(int id, const string& newTitle, const string& newAuthor, 
                    const string& newGenre, int newYear) {
        int index = findBookIndex(id);
        if (index == -1) {
            cout << Color::RED << "Error: Book with ID " << id << " not found." << Color::RESET << endl;
            return false;
        }
        
        if (!books[index].setTitle(newTitle) || !books[index].setAuthor(newAuthor)) {
            cout << Color::RED << "Error: Title and author cannot be empty." << Color::RESET << endl;
            return false;
        }
        
        books[index].setGenre(newGenre);
        books[index].setPublicationYear(newYear);
        
        cout << Color::GREEN << "Success: Book updated." << Color::RESET << endl;
        return true;
    }

    int findBookIndex(int id) const {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].getId() == id) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    void findBook(int id) const {
        int index = findBookIndex(id);
        if (index != -1) {
            books[index].displayDetailed();
        } else {
            cout << Color::RED << "Error: Book with ID " << id << " not found." << Color::RESET << endl;
        }
    }

    void searchBooks(const string& keyword) const {
        vector<size_t> matches;
        string lowerKeyword = toLowerCase(keyword);
        
        for (size_t i = 0; i < books.size(); i++) {
            string title = toLowerCase(books[i].getTitle());
            string author = toLowerCase(books[i].getAuthor());
            string genre = toLowerCase(books[i].getGenre());
            
            if (title.find(lowerKeyword) != string::npos || 
                author.find(lowerKeyword) != string::npos ||
                genre.find(lowerKeyword) != string::npos) {
                matches.push_back(i);
            }
        }
        
        if (matches.empty()) {
            cout << Color::RED << "No books found matching '" << keyword << "'" << Color::RESET << endl;
        } else {
            cout << "\n" << Color::YELLOW << "Search Results for '" << keyword << "': " 
                 << matches.size() << " book(s) found" << Color::RESET << endl;
            displayHeader();
            for (size_t index : matches) {
                books[index].displayBook();
            }
            displayFooter();
        }
    }

    void sortBooks(const string& criteria = "id") {
        if (criteria == "title") {
            sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
                return toLowerCase(a.getTitle()) < toLowerCase(b.getTitle());
            });
        } else if (criteria == "author") {
            sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
                return toLowerCase(a.getAuthor()) < toLowerCase(b.getAuthor());
            });
        } else if (criteria == "year") {
            sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
                return a.getPublicationYear() < b.getPublicationYear();
            });
        } else {
            sort(books.begin(), books.end());
        }
        cout << Color::GREEN << "Books sorted by " << criteria << "." << Color::RESET << endl;
    }

    void displayAllBooks() const {
        if (isEmpty()) {
            cout << "\n" << Color::RED << "The library is empty." << Color::RESET << endl;
            return;
        }
        
        cout << "\n" << Color::BG_GREEN << Color::WHITE;
        cout << " " << libraryName << " - Complete Collection ";
        cout << Color::RESET << endl;  
        cout << Color::CYAN << "Total Books: " << Color::WHITE << books.size() << Color::RESET << endl;
        
        displayHeader(); 
        for (const auto& book : books) {
            book.displayBook();
        } 
        displayFooter();
    }

    void displayHeader() const {
        const int tableWidth = 108;
        cout << Color::BG_CYAN << Color::WHITE;
        cout << string(tableWidth, '=') << Color::RESET << endl;
        cout << Color::BG_CYAN << Color::WHITE;
        cout << left 
             << setw(8) << "ID" 
             << setw(25) << "TITLE" 
             << setw(20) << "AUTHOR"
             << setw(15) << "GENRE"
             << setw(8) << "YEAR"
             << setw(12) << "STATUS"
             << setw(20) << "BORROWER"
             << Color::RESET << endl;
        cout << Color::BG_CYAN << Color::WHITE;
        cout << string(tableWidth, '=') << Color::RESET << endl;
    }

    void displayFooter() const {
        cout << Color::BG_CYAN << Color::WHITE;
        cout << string(108, '=') << Color::RESET << endl;
    }
    bool borrowBook(int id, const string& borrowerName) {
        int index = findBookIndex(id);
        if (index == -1) {
            cout << Color::RED << "Error: Book with ID " << id << " not found." << Color::RESET << endl;
            return false;
        }
        
        if (!books[index].isAvailable()) {
            cout << Color::RED << "Error: Book already borrowed by " 
                 << books[index].getBorrowerName() << Color::RESET << endl;
            return false;
        }
        
        if (borrowerName.empty()) {
            cout << Color::RED << "Error: Borrower name cannot be empty." << Color::RESET << endl;
            return false;
        }
        
        if (books[index].borrowBook(borrowerName, getCurrentDate())) {
            cout << Color::GREEN << "Success: Book '" << books[index].getTitle() 
                 << "' borrowed by " << borrowerName << Color::RESET << endl;
            return true;
        }
        return false;
    }

    bool returnBook(int id) {
        int index = findBookIndex(id);
        if (index == -1) {
            cout << Color::RED << "Error: Book with ID " << id << " not found." << Color::RESET << endl;
            return false;
        }
        
        if (books[index].isAvailable()) {
            cout << Color::RED << "Error: Book '" << books[index].getTitle() 
                 << "' is not borrowed." << Color::RESET << endl;
            return false;
        }
        
        string borrower = books[index].getBorrowerName();
        if (books[index].returnBook()) {
            cout << Color::GREEN << "Success: Book '" << books[index].getTitle() 
                 << "' returned by " << borrower << Color::RESET << endl;
            return true;
        }
        return false;
    }

    void displayAvailableBooks() const {
        cout << "\n" << Color::GREEN << "=== Available Books ===" << Color::RESET << endl;
        displayHeader();
        
        int availableCount = 0;
        for (const auto& book : books) {
            if (book.isAvailable()) {
                book.displayBook();
                availableCount++;
            }
        }
        
        displayFooter();
        
        if (availableCount == 0) {
            cout << Color::YELLOW << "No books currently available." << Color::RESET << endl;
        } else {
            cout << Color::CYAN << "Available: " << availableCount << "/" << books.size() << Color::RESET << endl;
        }
    }

    void displayBorrowedBooks() const {
        cout << "\n" << Color::RED << "=== Borrowed Books ===" << Color::RESET << endl;
        displayHeader();
        
        int borrowedCount = 0;
        for (const auto& book : books) {
            if (!book.isAvailable()) {
                book.displayBook();
                borrowedCount++;
            }
        }
        
        displayFooter();
        
        if (borrowedCount == 0) {
            cout << Color::YELLOW << "No books currently borrowed." << Color::RESET << endl;
        } else {
            cout << Color::CYAN << "Borrowed: " << borrowedCount << "/" << books.size() << Color::RESET << endl;
        }
    }

    void clearLibrary() {
        createBackup();
        books.clear();
        cout << Color::GREEN << "Success: Library cleared. Backup created." << Color::RESET << endl;
    }

    void displayStatistics() const {
        int available = 0, borrowed = 0;
        for (const auto& book : books) {
            if (book.isAvailable()) available++;
            else borrowed++;
        }
        
        cout << "\n" << Color::BG_MAGENTA << Color::WHITE;
        cout << string(60, '=');
        cout << "\n       LIBRARY STATISTICS       ";
        cout << "\n" << string(60, '=');
        cout << Color::RESET << "\n\n";
        
        cout << Color::CYAN << "Library Name:     " << Color::WHITE << libraryName << Color::RESET << endl;
        cout << Color::CYAN << "Total Books:      " << Color::WHITE << books.size() << Color::RESET << endl;
        cout << Color::GREEN << "Available Books:  " << Color::WHITE << available << Color::RESET << endl;
        cout << Color::RED << "Borrowed Books:   " << Color::WHITE << borrowed << Color::RESET << endl;
        
        if (!books.empty()) {
            double borrowRate = (static_cast<double>(borrowed) / books.size()) * 100;
            cout << Color::YELLOW << "Borrow Rate:      " << Color::WHITE << fixed << setprecision(1) 
                 << borrowRate << "%" << Color::RESET << endl;
        }
        
        cout << "\n" << Color::BG_MAGENTA << Color::WHITE << string(60, '=') << Color::RESET << "\n";
    }
    bool saveToFile(const string& filename) const {
        try {
            ofstream outFile(filename);
            if (!outFile.is_open()) {
                throw runtime_error("Cannot open file for writing");
            }
            
            outFile << books.size() << "\n";
            for (const auto& book : books) {
                book.save(outFile);
            }
            outFile.close();
            
            cout << Color::GREEN << "Data saved to " << filename << Color::RESET << endl;
            return true;
        } catch (const exception& e) {
            cerr << Color::RED << "Error saving data: " << e.what() << Color::RESET << endl;
            return false;
        }
    }

    bool loadFromFile(const string& filename) {
        try {
            ifstream inFile(filename);
            if (!inFile.is_open()) {
                cout << Color::YELLOW << "No existing data found. Starting fresh." << Color::RESET << endl;
                return false;
            }

            books.clear();
            string countStr;
            
            if (getline(inFile, countStr)) {
                int count = stoi(countStr);
                books.reserve(count);
                
                for (int i = 0; i < count; ++i) {
                    books.push_back(Book::load(inFile));
                }
            }
            inFile.close();
            
            cout << Color::GREEN << "Data loaded from " << filename << Color::RESET << endl;
            return true;
        } catch (const exception& e) {
            cerr << Color::RED << "Error loading data: " << e.what() << Color::RESET << endl;
            return false;
        }
    }

    bool exportToCSV(const string& filename) const {
        try {
            ofstream outFile(filename);
            if (!outFile.is_open()) {
                throw runtime_error("Cannot open file for writing");
            }
            
            outFile << "ID,Title,Author,Genre,Year,Status,Borrower,BorrowDate\n";
            for (const auto& book : books) {
                outFile << book.getId() << ","
                        << "\"" << book.getTitle() << "\","
                        << "\"" << book.getAuthor() << "\","
                        << "\"" << book.getGenre() << "\","
                        << book.getPublicationYear() << ","
                        << (book.isAvailable() ? "Available" : "Borrowed") << ","
                        << "\"" << book.getBorrowerName() << "\","
                        << "\"" << book.getBorrowDate() << "\"\n";
            }
            outFile.close();
            
            cout << Color::GREEN << "Data exported to " << filename << Color::RESET << endl;
            return true;
        } catch (const exception& e) {
            cerr << Color::RED << "Error exporting data: " << e.what() << Color::RESET << endl;
            return false;
        }
    }
};
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidInteger(const string& prompt, int minValue) {
    int value;
    while (true) {
        cout << Color::CYAN << prompt << Color::RESET;
        if (cin >> value) {
            clearInputBuffer();
            if (value >= minValue) {
                return value;
            } else {
                cout << Color::RED << "Error: Value must be at least " << minValue << Color::RESET << endl;
            }
        } else {
            cout << Color::RED << "Error: Please enter a valid number." << Color::RESET << endl;
            clearInputBuffer();
        }
    }
}

string getValidString(const string& prompt, bool allowEmpty) {
    string input;
    while (true) {
        cout << Color::CYAN << prompt << Color::RESET;
        getline(cin, input);
        
        if (!input.empty() || allowEmpty) {
            return input;
        } else {
            cout << Color::RED << "Error: Input cannot be empty." << Color::RESET << endl;
        }
    }
}

string toLowerCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void pauseScreen(const string& message) {
    cout << "\n" << Color::YELLOW << message << Color::RESET;
    cin.get();
}

void displayWelcome() {
    system("cls||clear");
    cout << Color::CYAN;
    cout << R"(
╔══════════════════════════════════════════════════════════════════════╗
║                                                                      ║
║    ██╗     ██╗██████╗ ██████╗  █████╗ ██████╗ ██╗   ██╗              ║
║    ██║     ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝              ║
║    ██║     ██║██████╔╝██████╔╝███████║██████╔╝ ╚████╔╝               ║
║    ██║     ██║██╔══██╗██╔══██╗██╔══██║██╔══██╗  ╚██╔╝                ║
║    ███████╗██║██████╔╝██║  ██║██║  ██║██║  ██║   ██║                 ║
║    ╚══════╝╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝                 ║
║                                                                      ║
║    ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗██████╗     ║
║    ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝██╔══██╗    ║
║    ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██████╔╝    ║
║    ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██╔══██╗    ║
║    ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║  ██║    ║
║    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝    ║
║                                                                      ║
╚══════════════════════════════════════════════════════════════════════╝
    )";
    cout << Color::RESET;
    cout << "\n" << Color::YELLOW << "           Professional Library Management System\n" << Color::RESET;
    cout << "\n" << Color::GREEN << "Initializing system";
    for(int i = 0; i < 5; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    cout << " Ready" << Color::RESET << "\n\n";
}

void displayMenu() {
    cout << "\n" << Color::BLUE;
    cout << "╔═════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                         MAIN MENU                           ║" << endl;
    cout << "╠═════════════════════════════════════════════════════════════╣" << endl;
    cout << "║  " << Color::GREEN << "1." << Color::BLUE << "  Add New Book                                          ║" << endl;
    cout << "║  " << Color::GREEN << "2." << Color::BLUE << "  Delete Book                                           ║" << endl;
    cout << "║  " << Color::GREEN << "3." << Color::BLUE << "  Update Book                                           ║" << endl;
    cout << "║  " << Color::GREEN << "4." << Color::BLUE << "  Find Book by ID                                       ║" << endl;
    cout << "║  " << Color::GREEN << "5." << Color::BLUE << "  Search Books                                          ║" << endl;
    cout << "║  " << Color::GREEN << "6." << Color::BLUE << "  Display All Books                                     ║" << endl;
    cout << "║  " << Color::GREEN << "7." << Color::BLUE << "  Display Available Books                               ║" << endl;
    cout << "║  " << Color::GREEN << "8." << Color::BLUE << "  Display Borrowed Books                                ║" << endl;
    cout << "║  " << Color::GREEN << "9." << Color::BLUE << "  Borrow a Book                                         ║" << endl;
    cout << "║  " << Color::GREEN << "10." << Color::BLUE << " Return a Book                                         ║" << endl;
    cout << "║  " << Color::GREEN << "11." << Color::BLUE << " Sort Books                                            ║" << endl;
    cout << "║  " << Color::GREEN << "12." << Color::BLUE << " Library Statistics                                    ║" << endl;
    cout << "║  " << Color::GREEN << "13." << Color::BLUE << " Export to CSV                                         ║" << endl;
    cout << "║  " << Color::GREEN << "14." << Color::BLUE << " Clear Library                                         ║" << endl;
    cout << "║  " << Color::RED << "0." << Color::BLUE << "  Exit Program                                            ║" << endl;
    cout << "╚═════════════════════════════════════════════════════════════╝" << Color::RESET << endl;
    
    cout << "\n" << Color::CYAN << "> Enter your choice (0-14): " << Color::RESET;
}

int main() {
    Library library("Central City Library");
    
    displayWelcome();
    cout << "Welcome to " << library.getLibraryName() << endl;
    
    int choice;
    
    while (true) {
        displayMenu();
        
        if (!(cin >> choice)) {
            cout << Color::RED << "Error: Invalid input. Please enter a number (0-14)." << Color::RESET << endl;
            clearInputBuffer();
            this_thread::sleep_for(chrono::milliseconds(800));
            continue;
        }
        clearInputBuffer();
        
        try {
            switch (choice) {
                case 1: { 
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " ADD NEW BOOK ";
                    cout << Color::RESET << "\n\n";
                    
                    int id = getValidInteger("Enter Book ID: ", 1);
                    string title = getValidString("Enter Book Title: ");
                    string author = getValidString("Enter Book Author: ");
                    string genre = getValidString("Enter Book Genre (optional): ", true);
                    int year = getValidInteger("Enter Publication Year (0 if unknown): ", 0);
                    
                    library.addBook(id, title, author, genre, year);
                    pauseScreen();
                    break;
                }
                
                case 2: {
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " DELETE BOOK ";
                    cout << Color::RESET << "\n\n";
                    
                    int id = getValidInteger("Enter Book ID to delete: ", 1);
                    library.deleteBook(id);
                    pauseScreen();
                    break;
                }
                
                case 3: { 
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " UPDATE BOOK ";
                    cout << Color::RESET << "\n\n";
                    
                    int id = getValidInteger("Enter Book ID to update: ", 1);
                    
                    if (library.findBookIndex(id) != -1) {
                        string title = getValidString("Enter New Title: ");
                        string author = getValidString("Enter New Author: ");
                        string genre = getValidString("Enter New Genre (optional): ", true);
                        int year = getValidInteger("Enter New Publication Year (0 if unknown): ", 0);
                        
                        library.updateBook(id, title, author, genre, year);
                    } else {
                        cout << Color::RED << "Error: Book not found." << Color::RESET << endl;
                    }
                    pauseScreen();
                    break;
                }
                
                case 4: { 
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " FIND BOOK ";
                    cout << Color::RESET << "\n\n";
                    
                    int id = getValidInteger("Enter Book ID to find: ", 1);
                    library.findBook(id);
                    pauseScreen();
                    break;
                }
                
                case 5: { 
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " SEARCH BOOKS ";
                    cout << Color::RESET << "\n\n";
                    
                    string keyword = getValidString("Enter search keyword (title/author/genre): ");
                    library.searchBooks(keyword);
                    pauseScreen();
                    break;
                }
                
                case 6: { 
                    library.displayAllBooks();
                    pauseScreen();
                    break;
                }
                
                case 7: { 
                    library.displayAvailableBooks();
                    pauseScreen();
                    break;
                }
                
                case 8: { 
                    library.displayBorrowedBooks();
                    pauseScreen();
                    break;
                }
                
                case 9: { 
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " BORROW BOOK ";
                    cout << Color::RESET << "\n\n";
                    
                    int id = getValidInteger("Enter Book ID to borrow: ", 1);
                    string borrower = getValidString("Enter borrower name: ");
                    library.borrowBook(id, borrower);
                    pauseScreen();
                    break;
                }
                
                case 10: { 
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " RETURN BOOK ";
                    cout << Color::RESET << "\n\n";
                    
                    int id = getValidInteger("Enter Book ID to return: ", 1);
                    library.returnBook(id);
                    pauseScreen();
                    break;
                }
                
                case 11: { 
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " SORT BOOKS ";
                    cout << Color::RESET << "\n\n";
                    
                    cout << "Sort by:\n";
                    cout << "1. ID\n";
                    cout << "2. Title\n";
                    cout << "3. Author\n";
                    cout << "4. Year\n";
                    
                    int sortChoice = getValidInteger("Enter choice: ", 1);
                    
                    switch (sortChoice) {
                        case 1: library.sortBooks("id"); break;
                        case 2: library.sortBooks("title"); break;
                        case 3: library.sortBooks("author"); break;
                        case 4: library.sortBooks("year"); break;
                        default: 
                            cout << Color::RED << "Invalid choice." << Color::RESET << endl;
                    }
                    pauseScreen();
                    break;
                }
                
                case 12: { 
                    library.displayStatistics();
                    pauseScreen();
                    break;
                }
                
                case 13: { 
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " EXPORT TO CSV ";
                    cout << Color::RESET << "\n\n";
                    
                    string filename = getValidString("Enter filename (e.g., library.csv): ");
                    library.exportToCSV(filename);
                    pauseScreen();
                    break;
                }
                
                case 14: { 
                    cout << "\n" << Color::BG_YELLOW << Color::WHITE;
                    cout << " CLEAR LIBRARY ";
                    cout << Color::RESET << "\n\n";
                    
                    cout << Color::RED << "WARNING: This will delete ALL books!" << Color::RESET << endl;
                    string confirm = getValidString("Type 'yes' to confirm: ");
                    
                    if (toLowerCase(confirm) == "yes") {
                        library.clearLibrary();
                    } else {
                        cout << Color::YELLOW << "Operation cancelled." << Color::RESET << endl;
                    }
                    pauseScreen();
                    break;
                }
                
                case 0: { 
                    cout << "\n" << Color::BG_RED << Color::WHITE;
                    cout << " GOODBYE ";
                    cout << Color::RESET << "\n\n";
                    
                    cout << Color::YELLOW << "Thank you for using " << library.getLibraryName() 
                         << " Management System" << Color::RESET << endl;
                    cout << Color::GREEN << "Saving data";
                    for(int i = 0; i < 3; i++) {
                        cout << ".";
                        cout.flush();
                        this_thread::sleep_for(chrono::milliseconds(200));
                    }
                    cout << " Done" << Color::RESET << "\n\n";
                    
                    return 0;
                }
                
                default: {
                    cout << Color::RED << "Error: Invalid choice. Please select 0-14." << Color::RESET << endl;
                    this_thread::sleep_for(chrono::milliseconds(800));
                    break;
                }
            }
        } catch (const exception& e) {
            cerr << Color::RED << "An error occurred: " << e.what() << Color::RESET << endl;
            pauseScreen();
        }
    }
    
    return 0;
}