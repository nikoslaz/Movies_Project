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
