/*****************************************************
 * @file   main.c                                    *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Main Function for Data Structures (CS240b) *
 * Project: Winter 2023						         *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "Movie.h"

#define BUFFER_SIZE 1024  /**< Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

int hashtable_size ; 	/** The size of the users hashtable (>0) */
int max_users ;         /** The maximum number of registrations (users) */
int max_id ;            /** The maximum user ID */

/* This is a very conservative progress on the hashtable. Our purpose */
/* is to force many rehashes to check the stability of the code. */
int primes_g[170] = 		{  5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
                               41,  43,  47,  53,  59,  61,  67,  71,  73,  79,
                               83,  89,  97, 101, 103, 107, 109, 113, 127, 131,
                              137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
                              191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
                              241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
                              307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
                              367, 373, 379, 383, 389, 397, 401, 409, 419, 421,
                              431, 433, 439, 443, 449, 457, 461, 463, 467, 479,
                              487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
                              563, 569, 571, 577, 587, 593, 599, 601, 607, 613,
                              617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
                              677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
                              751, 757, 761, 769, 773, 787, 797, 809, 811, 821,
                              823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
                              883, 887, 907, 911, 919, 929, 937, 941, 947, 953,
                          	  967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021 };

user_t **user_hashtable_p;	/* The users hashtable. This is an array of chains (pinakas katakermatismoy xrhstwn)*/
new_movie_t *new_releases;     /* New releases simply-linked binary tree*/
movieCategory_t *categoryArray[6];  /* The categories array (pinakas kathgoriwn)*/
movie_t *movie_head;		/* The head of the movies simply-linked binary tree*/
userMovie_t *user_movie_head;	/* The head of the user's history simply-linked binary tree*/

void init_structures(){
	struct new_movie* new_movie = (struct new_movie*)malloc(sizeof(struct new_movie));
	if(new_movie == NULL){
		printf("Error allocating memory for new_movie\n");
		exit(1);
	}	
	new_movie->movieID = -1;
	new_movie->category = -1;
	new_movie->year = -1;
	new_movie->sumScore = -1;
	new_movie->watchedCounter = -1;
	new_movie->lc = NULL;
	new_movie->rc = NULL;
	new_releases = new_movie;

	struct movie* movie = (struct movie*)malloc(sizeof(struct movie));
	if(movie == NULL){
		printf("Error allocating memory for movie\n");
		exit(1);
	}
	movie->movieID = -1;
	movie->year = -1;
	movie->sumScore = -1;
	movie->watchedCounter = -1;
	movie->lc = NULL;
	movie->rc = NULL;
	movie_head = movie;

	struct user_movie* user_movie = (struct user_movie*)malloc(sizeof(struct user_movie));
	if(user_movie == NULL){
		printf("Error allocating memory for user_movie\n");
		exit(1);
	}
	user_movie->movieID = -1;
	user_movie->category = -1;
	user_movie->score = -1;
	user_movie->parent = NULL;
	user_movie->lc = NULL;
	user_movie->rc = NULL;
	user_movie_head = user_movie;

	for(int i=0; i<6; i++){
		struct movie_category* movie_category = (struct movie_category*)malloc(sizeof(struct movie_category));
		if(movie_category == NULL){
			printf("Error allocating memory for movie_category\n");
			exit(1);
		}
		movie_category->movie = NULL;
		movie_category->sentinel = NULL;
		categoryArray[i] = movie_category;
	}

	user_hashtable_p = (user_t **) malloc(hashtable_size * sizeof(user_t *));
	if (user_hashtable_p == NULL) {
		printf("Error allocating memory for user_hashtable_p\n");
		exit(1);
	}
	for (int i = 0; i < hashtable_size; i++) {
		user_hashtable_p[i] = NULL;
	}
	
}

/******************************************************************************************************************************************/

int hash_function(int userID) {
	int p = primes_g[169];
    int a = 313; 
    int b = 439;

    int hash = ((a * userID + b) % p) % hashtable_size;
    return hash;
}

user_t* find_user_by_id(int userID) {
    /* Compute the hash index for the given userID */
    int hash_index = hash_function(userID);

    /* Traverse the linked list at the computed hash index */
    user_t* current_user = user_hashtable_p[hash_index];
    while (current_user != NULL) {
        if (current_user->userID == userID) {
            /* User found */
            return current_user;
        }
        /* Move to the next user in the chain */
        current_user = current_user->next;
    }

    /* User not found */
    return NULL;
}

/******************************************************************************************************************************************/

int register_user(int userID) {
    /* Allocate memory for the new user */
    user_t *new_user = (user_t *) malloc(sizeof(user_t));
    if (new_user == NULL) {
        printf("Error allocating memory for new_user\n");
        exit(1); /* Exit the program if memory allocation fails */
    }

    /* Initialize the fields of the new user */
    new_user->history = NULL; /* Initialize the user's movie history to NULL */
    new_user->next = NULL;    /* Set the next element in the chain to NULL */
    new_user->userID = userID;/* Set the user's ID */

    /* Calculate the hash index for the userID */
    int hash_index = hash_function(userID);
    
    /* Insert the new user into the hash table */
    new_user->next = user_hashtable_p[hash_index]; /* Point the new user to the current first element of the chain */
    user_hashtable_p[hash_index] = new_user; /* Set the new user as the first element of the chain at this index */
    printf("Chain %d of Users:\n", hash_index);
    while(new_user){
        printf("%d\n", new_user->userID);
        new_user = new_user->next;
    }

    printf("DONE\n");
    return 1; /* Return success */
}

/******************************************************************************************************************************************/

userMovie_t* mostLeftChild(userMovie_t* node) {
    userMovie_t* current = node;

    /* Find the most left child */
    while (current && current->lc != NULL)
        current = current->lc;

    return current;
}

void delete_movie_history(userMovie_t **root, int key) {
    if (*root == NULL) return;

    /* Search for the node to delete */
    if (key < (*root)->movieID) {
        delete_movie_history(&(*root)->lc, key);
    } else if (key > (*root)->movieID) {
        delete_movie_history(&(*root)->rc, key);
    } else {
        /* Node to delete is found */
        /* Case 1 or 2: Node with up to one child */
        if ((*root)->lc == NULL || (*root)->rc == NULL) {
            userMovie_t *temp;
			if ((*root)->lc) {
				temp = (*root)->lc;
			} else {
				temp = (*root)->rc;
			}

            if (temp == NULL) {
                /* No children exist */
                temp = *root;
                *root = NULL;
            } else {
                /* Replace the node with its child */
                **root = *temp;
            }
            free(temp);
        } else {
            /* Case 3: Node with two children */
            userMovie_t *temp = mostLeftChild((*root)->rc);

            /* Copy the value of the inorder successor */
            (*root)->movieID = temp->movieID;

            /* Delete the inorder successor */
            delete_movie_history(&(*root)->rc, temp->movieID);
        }
    }
}

int unregister_user(int userID) {
    /* Check if the user exists */
    user_t *user_to_remove = find_user_by_id(userID);
    if (user_to_remove == NULL) {
        printf("User %d not found\n", userID);
        return 0; /* User not found */
    }

    /* Calculate the hash index for the userID */
    int hash_index = hash_function(userID);

    /* Pointers to traverse the linked list at the hash_index */
    user_t *current = user_hashtable_p[hash_index];
    user_t *prev = NULL;

    /* Traverse the linked list to find and remove the user */
    while (current != NULL) {
        if (current->userID == userID) {
            /* If user is found, delete their movie history if it exists */
            delete_movie_history(&current->history, current->userID);

            if (prev == NULL) {
                /* If the user is the first node in the chain */
                user_hashtable_p[hash_index] = current->next;
            } else {
                /* If the user is in the middle or end of the chain */
                prev->next = current->next;
            }

            /* Free the memory allocated for the user */
            free(current);
            printf("User %d unregistered\n", userID);
            return 1; /* Successful deletion */
        }

        /* Move to the next node */
        prev = current;
        current = current->next;
    }

	/*Print that user was not found*/
	printf("User %d not found\n", userID);
    return 0; /* User not found */
}

/******************************************************************************************************************************************/

void inorderTraversal(new_movie_t* root) {
    if (root != NULL && root->movieID != -1) {
        inorderTraversal(root->lc);
        printf("Movie ID: %d\n", root->movieID);
        inorderTraversal(root->rc);
    }
}

void ascending_insert(new_movie_t **root, new_movie_t *node) {
    if (*root == NULL) {
        /* If the tree is empty, set the node as the root */
        *root = node;
        return;
    }

    /* If the tree is not empty, find the correct position to insert the node */
    if (node->movieID < (*root)->movieID) {
        /* If the node's movieID is smaller than the root's movieID, insert it in the left subtree */
        ascending_insert(&(*root)->lc, node);
    } else if (node->movieID > (*root)->movieID) {
        /* If the node's movieID is larger than the root's movieID, insert it in the right subtree */
        ascending_insert(&(*root)->rc, node);
    } else {
        /* If the node's movieID is equal to the root's movieID, do nothing */
        return;
    }
}

int add_new_movie(int movieID, int category, int year) {
    new_movie_t* new_movie = (new_movie_t*)malloc(sizeof(new_movie_t));

    if (new_movie == NULL) {
        printf("Error allocating memory for new_movie\n");
        exit(1);
    }

    /* Initialize the attributes of the new movie */
    new_movie->movieID = movieID;
    new_movie->year = year;
    new_movie->sumScore = 0;
    new_movie->watchedCounter = 0;
    new_movie->category = category;
    new_movie->lc = NULL;
    new_movie->rc = NULL;

    /* Insert the new movie into the list of new releases in ascending order */
    ascending_insert(&new_releases, new_movie);
    printf("New releases tree:\n");
    inorderTraversal(new_releases);

    printf("DONE\n");
    return 1;
}

/******************************************************************************************************************************************/

int find_min (int a, int b){
    if(a < b){
        return a;
    }
    else{
        return b;
    }
}

int find_max (int a, int b){
    if(a > b){
        return a;
    }
    else{
        return b;
    }
}

void inorderTraversal2(movie_t* root) {
    if (root != NULL) {
        inorderTraversal2(root->lc);
        printf("Movie ID: %d\n", root->movieID);
        inorderTraversal2(root->rc);
    }
}

void insert_into_bst(movie_t **root, movie_t *movie) {
    if (*root == NULL) {
        /* If the root is NULL, assign the movie as the new root */
        *root = movie;
    } else if (movie->movieID < (*root)->movieID) {
        /* If the movie ID is smaller than the current root's movie ID, insert into the left subtree */
        insert_into_bst(&(*root)->lc, movie);
    } else if (movie->movieID > (*root)->movieID) {
        /* If the movie ID is larger than the current root's movie ID, insert into the right subtree */
        insert_into_bst(&(*root)->rc, movie);
    } else {
        /* If the movie ID is equal to the current root's movie ID, handle collision */
        movie_t *new_node = (movie_t *)malloc(sizeof(movie_t));
        movie_t *new_leaf = (movie_t *)malloc(sizeof(movie_t));
        if (new_node == NULL || new_leaf == NULL) {
            perror("Failed to allocate memory for new nodes");
            exit(EXIT_FAILURE);
        }

        /* Create a new node with a linked list to handle collisions */
        new_node->movieID = (*root)->movieID;
        new_node->lc = new_leaf;
        new_node->rc = *root; 

        /* Update the movie IDs of the root and the new leaf */
        new_leaf->movieID = find_min(movie->movieID, (*root)->movieID);
        (*root)->movieID = find_max(movie->movieID, (*root)->movieID);

        *root = new_node;
    }
}

void insert_movie_into_category_tree(movie_t *movie, int category) {
    if (category < 0 || category > 7) {
        printf("Invalid category: %d\n", category);
        return;
    }
    /* Insert the movie into the category tree using the insert_into_bst function */
    insert_into_bst(&(categoryArray[category]->movie), movie);
}

new_movie_t* mostLeftChild2(new_movie_t* node) {
    new_movie_t* current = node;

    /* Loop down to find the leftmost leaf */
    while (current && current->lc != NULL) {
        current = current->lc;
    }

    return current;
}

void delete_movie(new_movie_t **root, int key) {
    if (*root == NULL) {
        /* Node not found, base case */
        return;
    }

    /* Search for the node */
    if (key < (*root)->movieID) {
        /* If the key is smaller, search in the left subtree */
        delete_movie(&(*root)->lc, key);
    } else if (key > (*root)->movieID) {
        /* If the key is larger, search in the right subtree */
        delete_movie(&(*root)->rc, key);
    } else {
        /* Node with the matching key is found */

        /* Node with only one child or no child */
        if ((*root)->lc == NULL) {
            /* If there is no left child */
            new_movie_t *temp = (*root)->rc;
            free(*root);
            *root = temp;
        } else if ((*root)->rc == NULL) {
            /* If there is no right child */
            new_movie_t *temp = (*root)->lc;
            free(*root);
            *root = temp;
        } else {
            /* Node with two children: Get the inorder successor (smallest in the right subtree) */
            new_movie_t *temp = mostLeftChild2((*root)->rc);

            /* Copy the inorder successor's content to this node */
            (*root)->movieID = temp->movieID;
            (*root)->year = temp->year;
            (*root)->sumScore = temp->sumScore;
            (*root)->watchedCounter = temp->watchedCounter;

            /* Delete the inorder successor */
            delete_movie(&(*root)->rc, temp->movieID);
        }
    }
}

void distribute_new_movies(new_movie_t* root) {
    int i;
    if (root == NULL) {
        return; /* If the root is NULL, no further action is needed. */
    }

    /* Distribute left subtree first */
    distribute_new_movies(root->lc);

    /* Process the current movie */
    movie_t *movie = (movie_t *) malloc(sizeof(movie_t));
    if (movie == NULL) {
        printf("Error allocating memory for movie\n");
        exit(1);
    }
    movie->movieID = root->movieID;
    movie->year = root->year;
    movie->sumScore = root->sumScore;
    movie->watchedCounter = root->watchedCounter;
    movie->lc = NULL;
    movie->rc = NULL;

    insert_movie_into_category_tree(movie, root->category);

    /* Save the right subtree pointer before deleting the current movie */
    new_movie_t *rightSubtree = root->rc;
    
    /* Delete the current movie from the new releases tree */
    delete_movie(&new_releases, root->movieID);

    /* Now distribute the right subtree */
    distribute_new_movies( root->rc);
}

int distribute_movies(void){
/* Initialize pointers to the root of new movies and the root of movies */
    new_movie_t* new_movies_root = new_releases;
    int i;

    /* Check if there are no new movies to distribute */
    if(new_movies_root == NULL){
        printf("No new movies to distribute\n");
        return 0;
    }

    /* Loop through the categories */
    for(i = 0; i < 6; i++){
        /* Get the category at index i */
        movieCategory_t *category = categoryArray[i];

        /* Initialize sentinel for the category */
        movie_t *sentinel = (movie_t *) malloc(sizeof(movie_t));
        if (sentinel == NULL) {
            printf("Error allocating memory for sentinel\n");
            exit(1);
        }
        sentinel->movieID = -1;
        sentinel->year = -1;
        sentinel->sumScore = -1;
        sentinel->watchedCounter = -1;
        sentinel->lc = NULL;
        sentinel->rc = NULL;

        /* Set the category's sentinel */
        categoryArray[i]->sentinel = sentinel;
        categoryArray[i]->movie = NULL;
    }

    /* Distribute new movies into categories */
    distribute_new_movies(new_movies_root);

    printf("Movie category array:\n");
     for(i = 0; i < 6; i++){
        /* Get the category at index i */
        movieCategory_t *category = categoryArray[i]; 
        printf("Category %d\n", i);
        inorderTraversal2(category->movie);
    }

    printf("DONE\n");
    return 1;
}

/******************************************************************************************************************************************/

int find_movie(movie_t *current, int movieID) {
    if (current == NULL) {
        /* Movie not found */
        printf("Movie not found\n");
        return 0;
    }
    if (current->movieID == movieID) {
        /* Movie found */
        printf("Movie found\n");
        printf("Year: %d\n", current->year);
        return 1;
    }
    if (movieID < current->movieID) {
        return find_movie(current->lc, movieID);
    } else {
        return find_movie(current->rc, movieID);
    }
}

void insert_into_bst_2(userMovie_t **root, userMovie_t *movie) {
    if (*root == NULL) {
        *root = movie;
    } else if (movie->movieID < (*root)->movieID) {
        insert_into_bst_2(&(*root)->lc, movie);
    } else if (movie->movieID > (*root)->movieID) {
        insert_into_bst_2(&(*root)->rc, movie);
    } else {
        userMovie_t *new_node = (userMovie_t *)malloc(sizeof(userMovie_t));
        userMovie_t *new_leaf = (userMovie_t *)malloc(sizeof(userMovie_t));
        if (new_node == NULL || new_leaf == NULL) {
            perror("Failed to allocate memory for new nodes");
            exit(EXIT_FAILURE);
        }

        new_node->movieID = (*root)->movieID;
        new_node->lc = new_leaf;
        new_node->rc = *root; 

        new_leaf->movieID = find_min(movie->movieID, (*root)->movieID);
        (*root)->movieID = find_max(movie->movieID, (*root)->movieID);

        *root = new_node;
    }
}

void inorderTraversal3(userMovie_t* root) {
    if (root != NULL && root->movieID != -1) {
        inorderTraversal3(root->lc);
        printf("Movie ID: %d\n", root->movieID);
        inorderTraversal3(root->rc);
    }
}

int watch_movie(int userID, int category, int movieID, int score) {
    user_t *user = find_user_by_id(userID);
    movie_t *current = categoryArray[category]->movie;
    userMovie_t *current_userMovie = user_movie_head;
    if (user == NULL) {
        /* User not found */
        printf("User %d not found\n", userID);
        return 0;
    }
    if (current == NULL) {
        /* Movie not found */
        printf("Movie %d not found\n", movieID);
        return 0;
    }

    if (find_movie(current, movieID) == 0) {
        /* Movie not found */
        printf("Movie %d not found\n", movieID);
        return 0;
    } else {
        current->sumScore = current->sumScore + score;
    }

    userMovie_t *user_movie = (userMovie_t *)malloc(sizeof(userMovie_t));
    if (user_movie == NULL) {
        printf("Error allocating memory for user_movie\n");
        exit(1);
    }

    user_movie->movieID = movieID;
    user_movie->category = category;
    user_movie->score = score;
    user_movie->parent = NULL;
    user_movie->lc = NULL;
    user_movie->rc = NULL;

    printf("History tree of %d\n", userID);
    insert_into_bst_2(&(user->history), user_movie);
    inorderTraversal3(user->history);

    printf("DONE\n");
    return 1;
}


/******************************************************************************************************************************************/

int sumScored = 0;
int watchedCounter = 0;
int num_movies = 0;
int index_arr = 0;

void traverse_through_category_tree(movie_t *current, int score) {
    if (current == NULL) {
        return;
    }
    
    /* Recursively traverse the left subtree */
    traverse_through_category_tree(current->lc, score);

    /* Check if the current movie's sumScore meets the criteria */
    if (current->sumScore >= score) {
        sumScored += score;
        watchedCounter++;
    }

    /* Recursively traverse the right subtree */
    traverse_through_category_tree(current->rc, score);
}

void traverse_through_category_tree2(movie_t *current, int avg) {
    if (current == NULL) {
        return;
    }

    /* Recursively traverse the left subtree */
    traverse_through_category_tree2(current->lc, avg);

    /* Check if the current movie's sumScore meets the criteria */
    if (current->sumScore >= avg) {
        num_movies++;
    }

    /* Recursively traverse the right subtree */
    traverse_through_category_tree2(current->rc, avg);
}

void traverse_through_category_tree3(movie_t *current, movie_t *array, int avg) {
    if (current == NULL) {
        return;
    }

    /* Recursively traverse the left subtree */
    traverse_through_category_tree3(current->lc, array, avg);

    /* Check if the current movie's sumScore meets the criteria */
    if (current->sumScore >= avg) {
        array[index_arr] = *current;
        index_arr++;
    }

    /* Recursively traverse the right subtree */
    traverse_through_category_tree3(current->rc, array, avg);
}


void swap(movie_t *a, movie_t *b) {
    movie_t temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(movie_t *arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].sumScore > arr[largest].sumScore) {
        largest = left;
    }

    if (right < n && arr[right].sumScore > arr[largest].sumScore) {
        largest = right;
    }

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(movie_t *arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int filter_movies(int userID, int score) {
    int avg;
    sumScored = 0;
    watchedCounter = 0;
    user_t *user = find_user_by_id(userID);
    
    /* Check if the user exists */
    if (user == NULL) {
        printf("User %d not found\n", userID);
        return 0;
    }

    /* Calculate average score of watched movies */
    for (int i = 0; i < 6; i++) {
        traverse_through_category_tree(categoryArray[i]->movie, score);
    }

    avg = sumScored / watchedCounter;

    /* Count movies with sumScore greater than or equal to average */
    for (int i = 0; i < 6; i++) {
        traverse_through_category_tree2(categoryArray[i]->movie, avg);
    }

    /* Allocate memory for an array to store qualified movies */
    movie_t *array = (movie_t *)malloc(num_movies * sizeof(movie_t));
    if (array == NULL) {
        printf("Error allocating memory for array\n");
        exit(1);
    }

    index_arr = 0;
    /* Collect qualified movies into the array */
    for (int i = 0; i < 6; i++) {
        traverse_through_category_tree3(categoryArray[i]->movie, array, avg);
    }

    /* Sort the array of movies based on sumScore using heap sort */
    heapSort(array, num_movies);

    /* Display the qualified movies */
    printf("Movies with score >= %d\n", avg);
    for (int i = 0; i < num_movies; i++) {
        if (array[i].movieID > 0) {
            printf("Movie ID: %d\n", array[i].movieID);
        }
    }

    printf("DONE\n");
    return 1;
}

/******************************************************************************************************************************************/

int ScoreSum = 0;
int counter = 0;

userMovie_t* mostLeftChild3(userMovie_t* node) {
    while (node && node->lc != NULL) {
        node = node->lc;
    }
    return node;
}

userMovie_t* findNextLeaf(userMovie_t* node) {
    if (node == NULL) {
        return NULL;
    }

    if (node->rc != NULL) {
        return mostLeftChild3(node->rc);
    }

    userMovie_t* parent = node->parent;
    while (parent != NULL && node == parent->rc) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

int user_stats(int userID) {
    user_t *user = find_user_by_id(userID);
    if (user == NULL) {
        printf("User %d not found\n", userID);
        return 0;
    }

    int scoreSum = 0;
    int counter = 0;
    userMovie_t *current = mostLeftChild3(user->history);

    while (current != NULL) {
        scoreSum += current->score;
        counter++;
        current = findNextLeaf(current);
    }

    if (counter > 0) {
        float averageScore = (float)scoreSum / counter;
        printf("User %d's average score: %.2f\n", userID, averageScore);
    } else {
        printf("User %d has not rated any movies\n", userID);
    }

    printf("DONE\n");
    return 1;
}

/******************************************************************************************************************************************/

int search_movie(int movieID, int category){
    if (category < 0 || category >= 7 || categoryArray[category] == NULL) {
        printf("Invalid category\n");
        return 0;
    }
    movie_t *current = categoryArray[category]->movie;
    return find_movie(current, movieID);
}

/******************************************************************************************************************************************/

int print_movies(void) {
    printf("Movie Category Array:\n");
    for (int i = 0; i < 6; i++) {
        printf("category%d: \n", i);
        inorderTraversal2(categoryArray[i]->movie);
        printf("\n");
    }

    printf("DONE\n");
    return 1;
}

/******************************************************************************************************************************************/

void print_user_history(userMovie_t *root) {
    if (root == NULL) {
        return;
    }
    print_user_history(root->lc);
    printf("%d %d\n", root->movieID, root->score);
    print_user_history(root->rc);
}

int print_users(void) {
    for (int j = 0; j < hashtable_size; j++) {
        printf("Chain %d of Users:\n", j);
        user_t *current_user = user_hashtable_p[j];
        while (current_user != NULL) {
            printf("%d\n", current_user->userID);
            printf("History Tree:\n");
            print_user_history(current_user->history);
            current_user = current_user->next;
        }
    }

    printf("DONE\n");
    return 1;
}

/******************************************************************************************************************************************/

void destroyStructs(){
    for (int i = 0; i < 6; i++) {
        movie_t *current = categoryArray[i]->movie;
        while (current != NULL) {
            movie_t *temp = current;
            current = current->rc;
            free(temp);
        }
        free(categoryArray[i]->sentinel);
        free(categoryArray[i]);
    }

    for (int i = 0; i < hashtable_size; i++) {
        user_t *current = user_hashtable_p[i];
        while (current != NULL) {
            user_t *temp = current;
            current = current->next;
            free(temp);
        }
    }

    new_movie_t *current = new_releases;
    while (current != NULL) {
        new_movie_t *temp = current;
        current = current->rc;
        free(temp);
    }

}

/******************************************************************************************************************************************/


int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if ( argc != 2 ) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    unsigned int users, id;
    if (fscanf(file, "0 %u\n", &users) != 1) {
        perror("Error reading users");
        fclose(file);
        return 1;
    }

    if (fscanf(file, "1 %u\n", &id) != 1) {
        perror("Error reading id");
        fclose(file);
        return 1;
    }

    fclose(file); 

    max_users = users;
    max_id = id;
    hashtable_size = max_users * 2;


	init_structures();
	/* Read input file buff-by-buff and handle the events */
	while ( fgets(buff, BUFFER_SIZE, fin) ) {

		DPRINT("Event: %s \n", buff);

		switch(buff[0]) {

		/* Comment */
		case '#':
			break;
		/* max_users */
		case '0': {
			sscanf(buff, "%c %u", &event, &max_users);
			DPRINT("max users: %u\n", max_users);
			break;
		}
		/* max_id */
		case '1': {
			sscanf(buff, "%c %u", &event, &max_id);
			DPRINT("max id: %u\n", max_id);
			break;
		}		
		/* Event R : R <userID> - Register user. */
		case 'R':
		{
			int userID;
			sscanf(buff, "%c %d", &event, &userID);
			DPRINT("%c %d\n", event, userID);
			if ( register_user(userID) ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Event U : U <userID> - Unregister user. */
		case 'U':
		{
			int userID;
			sscanf(buff, "%c %d", &event, &userID);
			DPRINT("%c %d\n", event, userID);

			if ( unregister_user(userID) ) {
				DPRINT("%c %d succeeded\n", event, userID);
			} else {
				fprintf(stderr, "%c %d failed\n", event, userID);
			}

			break;
		}
		/* Event A : A <movieID> <category> <year> - Add new movie. */
		case 'A':
		{
			int movieID,category, year;
			sscanf(buff, "%c %d %d %d", &event, &movieID, &category, &year);
			DPRINT("%c %d %d %d\n", event, movieID, category, year);

			if ( add_new_movie(movieID, category, year) ) {
				DPRINT("%c %d %d %d succeeded\n", event, movieID, category, year);
			} else {
				fprintf(stderr, "%c %d %d %d failed\n", event, movieID, category, year);
			}

			break;
		}
		/* Event D : D  - Distribute movies. */
		case 'D':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( distribute_movies() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Event W : W <userID ><category><movieID><score> - Watch movie */
		case 'W':
		{
			int userID, movieID,category,score;

			sscanf(buff, "%c %d %d %d %d", &event,&userID,&category, &movieID, &score);
			DPRINT("%c %d %d %d %d\n", event,userID, category, movieID, score);

			if ( watch_movie(userID,category, movieID, score) ) {
				DPRINT("%c %d %d %d %d succeeded\n", event,userID, category, movieID, score);
			} else {
				fprintf(stderr, "%c %d %d %d %d failed\n", event, userID,category, movieID, score);
			}

			break;
		}
		/* Event –	F  <userID ><category1><category2><score> Filter movies */
		case 'F':
		{
			int userID, score;
			sscanf(buff, "%c %d %d\n", &event, &userID,&score);
			DPRINT("%c %d %d\n", event, userID,score);

			if (filter_movies(userID,score) ) {
				DPRINT("%c %d %d succeeded\n", event, userID,score);
			} else {
				fprintf(stderr, "%c %d %d failed\n", event, userID,score);
			}

			break;
		}
		/* Event Q : Q <userID> - User statistics */
		case 'Q':
		{
			int userID;
			sscanf(buff, "%c %d\n", &event, &userID);
			DPRINT("%c %d\n", event, userID);

			if ( user_stats(userID) ) {
				DPRINT("%c %d succeeded\n", event, userID);
			} else {
				fprintf(stderr, "%c %d failed\n", event, userID);
			}

			break;
		}
		/* Event I : I <movieID> <category> - Search movie */
		case 'I':
		{
			int movieID,category;
			sscanf(buff, "%c %d %d\n", &event, &movieID, &category);
			DPRINT("%c %d %d\n", event, movieID, category);

			if ( search_movie(movieID, category) ) {
				DPRINT("%c %d %d succeeded\n", event, movieID, category);
			} else {
				fprintf(stderr, "%c %d %d failed\n", event, movieID, category);
			}

			break;
		}
		/* Event M : M  - Print movies */
		case 'M':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_movies() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Event P : P  - Print users */
		case 'P':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_users() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Empty line */
		case '\n':
			break;

		/* Ignore everything else */
		default:
			DPRINT("Ignoring buff: %s \n", buff);
			break;
		}
	}

    destroyStructs();
	return (EXIT_SUCCESS);
}
