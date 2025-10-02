# library_manager: A C++ Library Management System


## About

**library_manager** is a console-based Library Management System implemented in C++. It provides a comprehensive set of functionalities for managing a collection of books, including adding, deleting, updating, searching, and tracking the borrowing status of each item. The system prioritizes a clear and interactive user experience through color-coded console output and robust error handling.

## Project Context

This project was developed as part of a **Data Structures Lab** coursework. It demonstrates the practical application of fundamental data structures like `std::vector` to store and manage `Book` objects, along with algorithms for searching, sorting, and data manipulation. The focus was on implementing core library operations efficiently and reliably within a C++ environment.

## Features

-   **Book Management:**
    -   Add new books with unique IDs, titles, authors, genres, and publication years.
    -   Delete existing books by ID.
    -   Update details (title, author, genre, year) of any book.
-   **Search & Retrieval:**
    -   Find specific book details by its unique ID.
    -   Search for books using keywords across title, author, and genre fields.
-   **Borrowing System:**
    -   Mark books as borrowed, recording the borrower's name and the borrow date.
    -   Mark books as returned, making them available again.
    -   Display separate lists for available and borrowed books.
-   **Collection Overview:**
    -   Display a complete list of all books in a formatted table.
    -   Sort the book collection by various criteria: ID, Title, Author, or Publication Year.
    -   View detailed library statistics (total books, available, borrowed, borrow rate).
-   **Data Handling:**
    -   **Persistence:** Automatically saves library data to `library_data.txt` and loads it on startup.
    -   **Backup:** Creates `library_backup.txt` before critical operations (e.g., clearing the library).
    -   **Export:** Allows exporting the entire library collection to a CSV file (`.csv`).
    -   **Clear Library:** Option to completely clear all book records with a confirmation prompt.
-   **User Interface:**
    -   Utilizes ANSI escape codes for colored text and background highlights, enhancing readability and user interaction within the console.

## Technical Details

The system is built entirely in C++ and leverages various components of the C++ Standard Library:

-   **Core Data Structure:** `std::vector<Book>` is used to dynamically store and manage the collection of `Book` objects.
-   **Object-Oriented Design:** The `Book` class encapsulates book attributes and behaviors, while the `Library` class manages the collection of `Book` objects and implements library-specific operations.
-   **Input/Output:** `iostream` for console interaction; `fstream` for file operations (saving, loading, CSV export).
-   **String Manipulation:** `string` for text handling; `stringstream` for parsing file data; `algorithm` for `toLowerCase` and sorting.
-   **Date & Time:** `chrono` and `ctime` for generating current timestamps for borrow dates.
-   **Error Handling:** Basic error checking for invalid user input and file operations, utilizing `std::runtime_error` for exceptions.
-   **Console Coloring:** Custom `Color` namespace defines ANSI escape codes for colored output, improving the visual appeal and clarity of the console application.


## Usage

The application presents a command-line interface. Follow the prompts in the main menu to perform various library management tasks. Input is validated to ensure data integrity.

 ## Data Persistence

The system automatically manages data through files:
-   `library_data.txt`: Stores the main library collection. This file is created/updated automatically on program exit and loaded on startup.
-   `library_backup.txt`: A safety backup created automatically when clearing the entire library.
-   CSV export functionality allows saving data to a user-specified `.csv` file.

## Author

-   **10xRashed** - Data Structures Lab Project
