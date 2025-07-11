# Movies_Project

# 🎥 Streaming Service System

## 📝 Description
A complete C implementation of a streaming service platform featuring user management, movie cataloging, and recommendation systems using optimized data structures.

## ✨ Features

### 👥 User Management
| Feature          | Command | Example             | Description                     |
|------------------|---------|---------------------|---------------------------------|
| Register User    | R       | `R 123`             | Add new user with unique ID     |
| Unregister User  | U       | `U 123`             | Remove user and clean up data   |

### 🎬 Movie Operations
| Feature          | Command | Example                 | Description                     |
|------------------|---------|-------------------------|---------------------------------|
| Add Movie        | A       | `A 456 2 2023`          | Add movie (ID, category, year)  |
| Distribute Movies| D       | `D`                     | Categorize new releases         |
| Remove Movie     | T       | `T 456`                 | Take movie off service          |

### 🍿 User Experience
| Feature          | Command | Example                 | Description                     |
|------------------|---------|-------------------------|---------------------------------|
| Watch Movie      | W       | `W 123 456`             | Record viewing history          |
| Get Suggestions  | S       | `S 123`                 | Movie recommendations           |
| Filter Search    | F       | `F 123 1 2 2020`        | Find movies by criteria         |

## 🏗️ Data Structures
```c
Core Structures
struct user {
    int uid;
    struct suggested_movie *suggestedHead;
    struct movie *watchHistory; // Stack
};

struct movie {
    unsigned mid;
    unsigned year;
    movieCategory_t category;
};

// Alternate BST Implementation (Movie.h)
typedef struct movie {
    int movieID;
    int year;
    struct movie *lc, *rc; // BST pointers
} movie_t;

🚀 Installation & Usage

bash
# Clone and build
git clone https://github.com/yourrepo/streaming-service.git
cd streaming-service
make

# Run with input file
./streaming_service input_commands.txt

📂 Sample Input File

# User operations
R 101
R 102
U 101

# Movie operations
A 1001 0 2022  # Horror
A 1002 1 2023  # Sci-Fi
D

# User actions
W 102 1001
S 102
F 102 0 1 2020

📊 Performance

Operation	Time Complexity	Space Complexity
User Register	O(n)	O(1)
Movie Add	O(log n)	O(1)
Movie Search	O(log n)	O(1)

🛠️ Build Options

CC = gcc
CFLAGS = -Wall -Wextra
DEBUG_FLAGS = -DDEBUG # Enable debug prints

all: streaming_service

streaming_service: main.c streaming_service.h
    $(CC) $(CFLAGS) $^ -o $@
