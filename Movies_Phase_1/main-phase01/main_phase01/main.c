/*
 * ============================================
 * file: main.c
 * @Author John Malliotakis (jmal@csd.uoc.gr)
 * @Version 23/10/2023
 *
 * @e-mail hy240@csd.uoc.gr
 *
 * @brief Main function
 *        for CS240 Project Phase 1,
 *        Winter Semester 2023-2024
 * @see   Compile using supplied Makefile by running: make
 * ============================================
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "streaming_service.h"

/* Maximum input line size */
#define MAX_LINE 1024

/* 
 * Uncomment the following line to
 * enable debugging prints
 * or comment to disable it
 */
/* #define DEBUG */
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else
#define DPRINT(...)
#endif /* DEBUG */

struct user *head_user;
struct user *sentinel;
struct movie *head_movie;
struct movie *category_pointers[6];
struct new_movie *head_new_movie;
struct suggested_movie *head_suggested_movie;
struct suggested_movie *tail_suggested_movie;
struct movie *top_stack;
struct movie_filter *head_movie_filter;

void init_structures(void)
{
	struct user *new_user = (struct user*)malloc(sizeof(struct user));
	if(new_user == NULL){
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	new_user->uid = 0;
	new_user->suggestedHead = NULL;
	new_user->suggestedTail = NULL;
	new_user->watchHistory = NULL;
	new_user->next = NULL;
	head_user = new_user;

	struct movie *new_movie = (struct movie*)malloc(sizeof(struct movie));
	if(new_movie == NULL){
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	new_movie->info.mid = 0;
	new_movie->info.year = 0;
	new_movie->next = NULL;
	head_movie = new_movie;

	struct new_movie *new_new_movie = (struct new_movie*)malloc(sizeof(struct new_movie));
	if(new_new_movie == NULL){
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	new_new_movie->info.mid = 0;
	new_new_movie->info.year = 0;
	new_new_movie->category = 0;
	new_new_movie->next = NULL;
	head_new_movie = new_new_movie;

	struct suggested_movie *new_suggested_movie = (struct suggested_movie*)malloc(sizeof(struct suggested_movie));
	if(new_suggested_movie == NULL){
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	new_suggested_movie->info.mid = 0;
	new_suggested_movie->info.year = 0;
	new_suggested_movie->prev = NULL;
	new_suggested_movie->next = NULL;
	head_suggested_movie = new_suggested_movie;

	struct suggested_movie *new_tail_suggested_movie = (struct suggested_movie*)malloc(sizeof(struct suggested_movie));
	if(new_tail_suggested_movie == NULL){
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	new_tail_suggested_movie->info.mid = 0;
	new_tail_suggested_movie->info.year = 0;
	new_tail_suggested_movie->prev = NULL;
	new_tail_suggested_movie->next = NULL;
	tail_suggested_movie = new_tail_suggested_movie;

	struct movie *new_top_stack = (struct movie*)malloc(sizeof(struct movie));
	if(new_top_stack == NULL){
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	new_top_stack->info.mid = 0;
	new_top_stack->info.year = 0;
	new_top_stack->next = NULL;
	top_stack = new_top_stack;

	struct movie_filter *new_movie_filter = (struct movie_filter*)malloc(sizeof(struct movie_filter));
	if(new_movie_filter == NULL){
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	new_movie_filter->movie_filtered.info.mid = 0;
	new_movie_filter->movie_filtered.info.year = 0;
	new_movie_filter->next = NULL;
	new_movie_filter->prev = NULL;
	head_movie_filter = new_movie_filter;
}

/*----------------------------------------------------------------------------------------------------------------------*/

int register_user(int uid) {
    struct user *current = head_user;
    printf("R<%d>\n", uid);

    /* Check if the user already exists */
    while (current != NULL) {
        if (current->uid == uid) {
            printf("Error: User already exists\n");
            return 0;
        }
        if (current->next == NULL) {
            break;
        }
        current = current->next;
    }

    /* Create a sentinel node if the list is initially empty */
    if (current == NULL) {
        sentinel = (struct user*)malloc(sizeof(struct user));
        if (sentinel == NULL) {
            printf("Error allocating memory\n");
            exit(EXIT_FAILURE);
        }
        sentinel->uid = -1;
        sentinel->suggestedHead = NULL;
        sentinel->suggestedTail = NULL;
        sentinel->watchHistory = NULL;
        sentinel->next = NULL;
    }

    /* Create a new user node */
    struct user* new_user = (struct user*)malloc(sizeof(struct user));
    if (new_user == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    new_user->uid = uid;
    new_user->suggestedHead = NULL;
    new_user->suggestedTail = NULL;
    new_user->watchHistory = NULL;
    new_user->next = NULL;

    /* Add the new user to the list */
    if (current != NULL) {
        current->next = new_user;
    }

    /* Print the list of users */
    current = head_user;
    printf("  Users =");
    while (current != NULL) {
        printf("<%d>, ", current->uid);
        current = current->next;
    }

    printf("\nDONE\n\n");
    return 1;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void unregister_user(int uid) {
    struct user* current = head_user;
    struct user* temp_user;

    printf("U<%d>\n", uid);

    /* Iterate through the list to find the user with the specified UID */
    while (current->next != NULL) {
        /* Check if the next user's UID matches the specified UID */
        if (current->next->uid == uid) {
            struct suggested_movie* temp_suggested;

            /* Free the suggested movies associated with the user */
            while (current->next->suggestedHead != NULL) {
                temp_suggested = current->next->suggestedHead;
                current->next->suggestedHead = current->next->suggestedHead->next;
                free(temp_suggested);
            }

            /* Remove the user from the list and free the memory */
            temp_user = current->next;
            current->next = current->next->next;
            free(temp_user);
        }

        current = current->next;
    }

    /* Print the list of users */
    current = head_user;
    printf("  Users =");
    while (current != NULL) {
        printf("<%d>, ", current->uid);
        current = current->next;
    }

    printf("\n");
    printf("DONE\n\n");
}


/*----------------------------------------------------------------------------------------------------------------------*/

int add_new_movie(unsigned mid, movieCategory_t category, unsigned year) {
    struct new_movie *current;
    struct new_movie *new_node = (struct new_movie *)malloc(sizeof(struct new_movie));

    printf("A<%u><%d><%u>\n", mid, category, year);

    /* Check for memory allocation error */
    if (new_node == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the new movie node with the provided information */
    new_node->category = category;
    new_node->info.mid = mid;
    new_node->info.year = year;

    /* Insert the new movie node into the sorted list based on movie ID */
    if (head_new_movie == NULL || head_new_movie->info.mid >= mid) {
        new_node->next = head_new_movie;
        head_new_movie = new_node;
    } else {
        current = head_new_movie;
        while (current->next != NULL && current->next->info.mid < mid) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }

    /* Print the list of new movies */
    current = head_new_movie->next;
    printf("  New movies =");
    while (current != NULL) {
        printf("<%u>,<%d>,<%u>  ", current->info.mid, current->category, current->info.year);
        current = current->next;
    }
    printf("\nDONE\n\n");

    return 0;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void distribute_new_movies(void) {
    struct new_movie *current_new_movie = head_new_movie;
    struct movie *current_movie = head_movie;
    int i = 1;

    /* Process each new movie in the list */
    while (current_new_movie != NULL) {
        struct movie *new_movie = (struct movie*)malloc(sizeof(struct movie));
        if (new_movie == NULL) {
            printf("Error allocating memory\n");
            exit(EXIT_FAILURE);
        }

        /* Initialize the new movie node with information from new_movie list */
        new_movie->info.mid = current_new_movie->info.mid;
        new_movie->info.year = current_new_movie->info.year;
        new_movie->next = NULL;

        /* Insert the new movie into the sorted list of all movies based on movie ID */
        if (head_movie == NULL || head_movie->info.mid >= current_new_movie->info.mid) {
            new_movie->next = head_movie;
            head_movie = new_movie;
        } else {
            current_movie = head_movie;
            while (current_movie->next != NULL && current_movie->next->info.mid < current_new_movie->info.mid) {
                current_movie = current_movie->next;
            }
            new_movie->next = current_movie->next;
            current_movie->next = new_movie;
        }

        struct movie *new_category_movie = (struct movie*)malloc(sizeof(struct movie));
        if (new_category_movie == NULL) {
            printf("Error allocating memory\n");
            exit(EXIT_FAILURE);
        }

        /* Initialize the new movie node for the specific category */
        new_category_movie->info.mid = current_new_movie->info.mid;
        new_category_movie->info.year = current_new_movie->info.year;
        new_category_movie->next = NULL;

        /* Insert the new movie into the category-specific list based on category */
        if (current_new_movie->category == HORROR) {
            new_category_movie->next = category_pointers[0];
            category_pointers[0] = new_category_movie;
        } else if (current_new_movie->category == SCIFI) {
            new_category_movie->next = category_pointers[1];
            category_pointers[1] = new_category_movie;
        } else if (current_new_movie->category == DRAMA) {
            new_category_movie->next = category_pointers[2];
            category_pointers[2] = new_category_movie;
        } else if (current_new_movie->category == ROMANCE) {
            new_category_movie->next = category_pointers[3];
            category_pointers[3] = new_category_movie;
        } else if (current_new_movie->category == DOCUMENTARY) {
            new_category_movie->next = category_pointers[4];
            category_pointers[4] = new_category_movie;
        } else if (current_new_movie->category == COMEDY) {
            new_category_movie->next = category_pointers[5];
            category_pointers[5] = new_category_movie;
        }

        current_new_movie = current_new_movie->next;
    }

    /* Print the categorized movies */

	printf("Categorized Movies:\n");
	current_movie = category_pointers[0];
	printf("Horror:");
	while(current_movie != NULL){
		printf("<%u>,<%u>  ", current_movie->info.mid, i);
		current_movie = current_movie->next;
		i++;
	}
	printf("\n");
	current_movie = category_pointers[1];
	printf("Sci-fi:");
	while(current_movie != NULL){
		printf("<%u>,<%u>  ", current_movie->info.mid, i);
		current_movie = current_movie->next;
		i++;
	}
	printf("\n");
	current_movie = category_pointers[2];
	printf("Drama:");
	while(current_movie != NULL){
		printf("<%u><%u>  ", current_movie->info.mid, i);
		current_movie = current_movie->next;
		i++;
	}
	printf("\n");
	current_movie = category_pointers[3];
	printf("Romance:");
	while(current_movie != NULL){
		printf("<%u><%u>  ", current_movie->info.mid, i);
		current_movie = current_movie->next;
		i++;
	}
	printf("\n");
	current_movie = category_pointers[4];
	printf("Documentary:");
	while(current_movie != NULL){
		printf("<%u><%u>  ", current_movie->info.mid, i);
		current_movie = current_movie->next;
		i++;
	}
	printf("\n");
	current_movie = category_pointers[5];
	printf("Comedy:");
	while(current_movie != NULL){
		printf("<%u><%u>  ", current_movie->info.mid, i);
		current_movie = current_movie->next;
		i++;
	}
	printf("\nDONE\n\n");

}

/*----------------------------------------------------------------------------------------------------------------------*/

void push(struct movie* current, struct movie** top) {
    if (current == NULL || top == NULL) {
        printf("Error: Invalid arguments to push function\n");
        return;
    }

    struct movie* history = (struct movie *)malloc(sizeof(struct movie));
    if (history == NULL) {
        printf("Error: Could not allocate memory for new movie\n");
        return;
    }

    history->info.mid  = current->info.mid;
    history->info.year = current->info.year;
    history->next = *top;
    *top = history;
}

/*----------------------------------------------------------------------------------------------------------------------*/

struct movie* pop(struct movie** top) {
    if (*top == NULL ){
        printf("Error: Invalid argument to pop function or stack is empty\n");
        return NULL;
    }

    struct movie *popped_movie = *top;
    *top = (*top)->next;
    popped_movie->next = NULL;

    return popped_movie;
}

/*----------------------------------------------------------------------------------------------------------------------*/

int watch_movie(int uid, unsigned mid) {
    struct user *current_user = head_user;
    struct movie *current_movie = head_movie;
    current_user->watchHistory = top_stack;

    printf("W<%d> <%u>\n", uid, mid);
    int i = 0;

    /* Find the user with the specified UID */
    while (current_user != NULL && current_user->uid != uid) {
        current_user = current_user->next;
    }

    /* Check if the user was not found */
    if (current_user == NULL) {
        printf("Error: User not found\n");
        return -1;
    }

    /* Find the movie with the specified MID */
    while (current_movie != NULL && current_movie->info.mid != mid) {
        current_movie = current_movie->next;
    }

    /* Check if the movie was not found */
    if (current_movie == NULL) {
        printf("Error: Movie not found\n");
        return -1;
    }

    /* Push the watched movie onto the user's watch history stack */
    push(current_movie, &current_user->watchHistory);

    /* Print the updated watch history for the user */
    printf("User %d: Watch History =", current_user->uid);
    struct movie *current = current_user->watchHistory;
    while (current != NULL) {
        printf("%u ", current->info.mid);
        current = current->next;
    }
    printf("\nDONE\n\n");

    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------*/

int suggest_movies(int uid) {
    struct user *current_user = head_user->next;
    struct suggested_movie *current_head = head_suggested_movie;
    struct suggested_movie *current_tail = tail_suggested_movie;

    printf("S<%d>\n", uid);

    /* Iterate through all users */
    while (current_user != NULL) {
        /* Check if the user is not the target user (uid) */
        if (current_user->uid != uid) {
            /* Pop a movie from the user's watch history stack */
            struct movie *current_movie = pop(&current_user->watchHistory);

            /* Check if the watch history is empty for the current user */
            if (current_movie == NULL) {
                printf("Error: Watch history is empty for user %d\n", current_user->uid);
                return -1;
            }

            /* Create a suggested movie node for the popped movie */
            struct suggested_movie *current_suggested_movie = (struct suggested_movie*)malloc(sizeof(struct suggested_movie));
            
            /* Check if memory allocation for suggested_movie is successful */
            if (current_suggested_movie == NULL) {
                printf("Error allocating memory for suggested_movie\n");
                free(current_movie);
                return -1;
            }

            /* Initialize the suggested movie node with movie information */
            current_suggested_movie->info.mid = current_movie->info.mid;
            current_suggested_movie->info.year = current_movie->info.year;

            /* Update the suggested movie list */
            if (current_head != NULL) {
                current_suggested_movie->next = current_head->next;
                current_head->next = current_suggested_movie;
                current_suggested_movie->prev = current_head;

                if (current_suggested_movie->next != NULL) {
                    current_suggested_movie->next->prev = current_suggested_movie;
                }

                current_head = current_head->next;
            } else {
                current_suggested_movie->prev = current_tail->prev;
                tail_suggested_movie->prev = current_suggested_movie;
                current_suggested_movie->next = current_tail;

                if (current_suggested_movie->prev != NULL) {
                    current_suggested_movie->prev->next = current_suggested_movie;
                } else {
                    current_user->suggestedTail = current_suggested_movie;
                }

                current_tail = current_tail->prev;
            }

            /* Free the memory allocated for the popped movie */
            free(current_movie);
        } else {
            return 0;
        }

        current_user = current_user->next;
    }

    /* Print the suggested movies for the target user */
    printf("User %d: Suggested =", uid);
    current_user = head_user->next;
    while (current_user != NULL) {
        if (current_user->uid == uid) {
            struct suggested_movie *current_suggested_movie = current_user->suggestedTail->next;
            while (current_suggested_movie != NULL) {
                printf("%u ", current_suggested_movie->info.mid);
                current_suggested_movie = current_suggested_movie->next;
            }
        }
        current_user = current_user->next;
    }

    printf("\nDONE\n\n");

    return 0;
}

/*----------------------------------------------------------------------------------------------------------------------*/

int filtered_movie_search(int uid, movieCategory_t category1, movieCategory_t category2, unsigned year){

	struct user *current_user = head_user;
	printf("F<%d><%d><%d><%u>\n", uid, category1, category2, year);

	/*I want to find the user*/
	while (current_user != NULL && current_user->uid != uid) {
		current_user = current_user->next; 
	}

	if (current_user == NULL) {
		printf("Error: User not found\n");
		return -1;
	}

	if(category1 == HORROR || category2 == HORROR){
		struct movie *current_movie = category_pointers[0];
		while(current_movie != NULL){
			if (current_movie->info.year >= year) {
				struct movie_filter *new_movie_filter = (struct movie_filter*)malloc(sizeof(struct movie_filter));
				struct movie_filter *current;
				if (new_movie_filter == NULL) {
					printf("Error allocating memory\n");
					exit(EXIT_FAILURE);
				}

				new_movie_filter->movie_filtered.info.mid = current_movie->info.mid;
				new_movie_filter->movie_filtered.info.year = current_movie->info.year;

				if(head_movie_filter == NULL){
					head_movie_filter = new_movie_filter;
				}
				else if((head_movie_filter)->movie_filtered.info.mid >= new_movie_filter->movie_filtered.info.mid){
					new_movie_filter->next = head_movie_filter;
					new_movie_filter->next->prev = new_movie_filter;
					head_movie_filter = new_movie_filter;
				}
				else{
					current = head_movie_filter;

					while(current->next != NULL && current->next->movie_filtered.info.mid < new_movie_filter->movie_filtered.info.mid){
						current = current->next;
					}
					new_movie_filter->next = current->next;
					if(current->next != NULL){
						new_movie_filter->next->prev = new_movie_filter;
					}
					current->next = new_movie_filter;
					new_movie_filter->prev = current;
				}

			}

			current_movie = current_movie->next;
		}

	}
	else if(category1 == SCIFI || category2 == SCIFI){
		struct movie *current_movie = category_pointers[1];
		while(current_movie != NULL){
			if (current_movie->info.year >= year) {
				struct movie_filter *new_movie_filter = (struct movie_filter*)malloc(sizeof(struct movie_filter));
				struct movie_filter *current;
				if (new_movie_filter == NULL) {
					printf("Error allocating memory\n");
					exit(EXIT_FAILURE);
				}

				new_movie_filter->movie_filtered.info.mid = current_movie->info.mid;
				new_movie_filter->movie_filtered.info.year = current_movie->info.year;

				if(head_movie_filter == NULL){
					head_movie_filter = new_movie_filter;
				}
				else if((head_movie_filter)->movie_filtered.info.mid >= new_movie_filter->movie_filtered.info.mid){
					new_movie_filter->next = head_movie_filter;
					new_movie_filter->next->prev = new_movie_filter;
					head_movie_filter = new_movie_filter;
				}
				else{
					current = head_movie_filter;

					while(current->next != NULL && current->next->movie_filtered.info.mid < new_movie_filter->movie_filtered.info.mid){
						current = current->next;
					}
					new_movie_filter->next = current->next;
					if(current->next != NULL){
						new_movie_filter->next->prev = new_movie_filter;
					}
					current->next = new_movie_filter;
					new_movie_filter->prev = current;
				}
			}
			current_movie = current_movie->next;
		}
	}
	else if(category1 == DRAMA || category2 == DRAMA){
		struct movie *current_movie = category_pointers[2];
		while(current_movie != NULL){
			if (current_movie->info.year >= year) {
				struct movie_filter *new_movie_filter = (struct movie_filter*)malloc(sizeof(struct movie_filter));
				struct movie_filter *current;
				if (new_movie_filter == NULL) {
					printf("Error allocating memory\n");
					exit(EXIT_FAILURE);
				}

				new_movie_filter->movie_filtered.info.mid = current_movie->info.mid;
				new_movie_filter->movie_filtered.info.year = current_movie->info.year;

				if(head_movie_filter == NULL){
					head_movie_filter = new_movie_filter;
				}
				else if((head_movie_filter)->movie_filtered.info.mid >= new_movie_filter->movie_filtered.info.mid){
					new_movie_filter->next = head_movie_filter;
					new_movie_filter->next->prev = new_movie_filter;
					head_movie_filter = new_movie_filter;
				}
				else{
					current = head_movie_filter;

					while(current->next != NULL && current->next->movie_filtered.info.mid < new_movie_filter->movie_filtered.info.mid){
						current = current->next;
					}
					new_movie_filter->next = current->next;
					if(current->next != NULL){
						new_movie_filter->next->prev = new_movie_filter;
					}
					current->next = new_movie_filter;
					new_movie_filter->prev = current;
				}
			}
			current_movie = current_movie->next;
		}
	}
	else if(category1 == ROMANCE || category2 == ROMANCE){
		struct movie *current_movie = category_pointers[3];
		while(current_movie != NULL){
			if (current_movie->info.year >= year) {
				struct movie_filter *new_movie_filter = (struct movie_filter*)malloc(sizeof(struct movie_filter));
				struct movie_filter *current;
				if (new_movie_filter == NULL) {
					printf("Error allocating memory\n");
					exit(EXIT_FAILURE);
				}

				new_movie_filter->movie_filtered.info.mid = current_movie->info.mid;
				new_movie_filter->movie_filtered.info.year = current_movie->info.year;

				if(head_movie_filter == NULL){
					head_movie_filter = new_movie_filter;
				}
				else if((head_movie_filter)->movie_filtered.info.mid >= new_movie_filter->movie_filtered.info.mid){
					new_movie_filter->next = head_movie_filter;
					new_movie_filter->next->prev = new_movie_filter;
					head_movie_filter = new_movie_filter;
				}
				else{
					current = head_movie_filter;

					while(current->next != NULL && current->next->movie_filtered.info.mid < new_movie_filter->movie_filtered.info.mid){
						current = current->next;
					}
					new_movie_filter->next = current->next;
					if(current->next != NULL){
						new_movie_filter->next->prev = new_movie_filter;
					}
					current->next = new_movie_filter;
					new_movie_filter->prev = current;
				}
			}
			current_movie = current_movie->next;
		}
	}
	else if(category1 == DOCUMENTARY || category2 == DOCUMENTARY){
		struct movie *current_movie = category_pointers[4];
		while(current_movie != NULL){
			if (current_movie->info.year >= year) {
				struct movie_filter *new_movie_filter = (struct movie_filter*)malloc(sizeof(struct movie_filter));
				struct movie_filter *current;
				if (new_movie_filter == NULL) {
					printf("Error allocating memory\n");
					exit(EXIT_FAILURE);
				}

				new_movie_filter->movie_filtered.info.mid = current_movie->info.mid;
				new_movie_filter->movie_filtered.info.year = current_movie->info.year;

				if(head_movie_filter == NULL){
					head_movie_filter = new_movie_filter;
				}
				else if((head_movie_filter)->movie_filtered.info.mid >= new_movie_filter->movie_filtered.info.mid){
					new_movie_filter->next = head_movie_filter;
					new_movie_filter->next->prev = new_movie_filter;
					head_movie_filter = new_movie_filter;
				}
				else{
					current = head_movie_filter;

					while(current->next != NULL && current->next->movie_filtered.info.mid < new_movie_filter->movie_filtered.info.mid){
						current = current->next;
					}
					new_movie_filter->next = current->next;
					if(current->next != NULL){
						new_movie_filter->next->prev = new_movie_filter;
					}
					current->next = new_movie_filter;
					new_movie_filter->prev = current;
				}
			}
			current_movie = current_movie->next;
		}
	}
	else if(category1 == COMEDY || category2 == COMEDY){
		struct movie *current_movie = category_pointers[5];
		while(current_movie != NULL){
			if (current_movie->info.year >= year) {
				struct movie_filter *new_movie_filter = (struct movie_filter*)malloc(sizeof(struct movie_filter));
				struct movie_filter *current;
				if (new_movie_filter == NULL) {
					printf("Error allocating memory\n");
					exit(EXIT_FAILURE);
				}

				new_movie_filter->movie_filtered.info.mid = current_movie->info.mid;
				new_movie_filter->movie_filtered.info.year = current_movie->info.year;

				if(head_movie_filter == NULL){
					head_movie_filter = new_movie_filter;
				}
				else if((head_movie_filter)->movie_filtered.info.mid >= new_movie_filter->movie_filtered.info.mid){
					new_movie_filter->next = head_movie_filter;
					new_movie_filter->next->prev = new_movie_filter;
					head_movie_filter = new_movie_filter;
				}
				else{
					current = head_movie_filter;

					while(current->next != NULL && current->next->movie_filtered.info.mid < new_movie_filter->movie_filtered.info.mid){
						current = current->next;
					}
					new_movie_filter->next = current->next;
					if(current->next != NULL){
						new_movie_filter->next->prev = new_movie_filter;
					}
					current->next = new_movie_filter;
					new_movie_filter->prev = current;
				}
			}
			current_movie = current_movie->next;
		}
	}

	printf("User %d Suggested Movies= ", current_user->uid);
	struct movie_filter *current_movie_filter = head_movie_filter->next;
	while(current_movie_filter != NULL){
		printf("%u ", current_movie_filter->movie_filtered.info.mid);
		current_movie_filter = current_movie_filter->next;
	}

	printf("\nDONE\n\n");

	return 0;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void deleteNode(struct suggested_movie **head_ref, struct suggested_movie *del) {
    /* Check if the list or the node to be deleted is NULL */
    if (*head_ref == NULL || del == NULL) {
        return;
    }

    /* If the node to be deleted is the head of the list, update the head */
    if (*head_ref == del) {
        *head_ref = del->next;
    }

    /* If the node to be deleted has a next node, update its previous pointer */
    if (del->next != NULL) {
        del->next->prev = del->prev;
    }

    /* If the node to be deleted has a previous node, update its next pointer */
    if (del->prev != NULL) {
        del->prev->next = del->next;
    }

    /* Free the memory of the node to be deleted */
    free(del);
    return;
}

/*----------------------------------------------------------------------------------------------------------------------*/

int deleteNode2(struct movie** head_ref, int mid, int i) {
    struct movie* temp = *head_ref;
    struct movie* prev = NULL;
    const char *genre_names[] = {"Horror", "Sci-fi", "Drama", "Romance", "Documentary", "Comedy"};

    /* Check if the list is not empty and the first node matches the key */
    if (temp != NULL && temp->info.mid == mid) {
        *head_ref = temp->next;
        free(temp);
        printf("%d Removed from category %s\n", mid, genre_names[i]);
        return 1;
    }

    /* Search for the key in the rest of the list */
    while (temp != NULL && temp->info.mid != mid) {
        prev = temp;
        temp = temp->next;
    }

    /* If the key is not found in the list */
    if (temp == NULL) {
        return 0;
    }

    /* Unlink the node with the given key */
    prev->next = temp->next;

    printf("%d Removed from category %s\n", mid, genre_names[i]);
    free(temp);
    return 1;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void take_off_movie(unsigned mid) {
  struct user *current_user = head_user;
  int i;
  int k = 0;
  int deleted = -1;
  printf("T<%u>\n", mid);

  /* Iterate through each user */
  while (current_user) {
    struct suggested_movie *current_suggested_movie_head = current_user->suggestedHead;
    
    /* Iterate through suggested movies for the current user */
    while (current_suggested_movie_head) {
      /* Delete suggested movie if it matches the specified mid */
      if (current_suggested_movie_head->info.mid == mid) {
        deleteNode(&current_user->suggestedHead, current_suggested_movie_head);
      }
      current_suggested_movie_head = current_suggested_movie_head->next;
    }
    
    current_user = current_user->next;
  }

  /* Iterate through each movie category */
  for (i = 0; i < 6; i++) {
    struct movie *current_movie = category_pointers[i];
    
    /* Delete movie from category if it matches the specified mid */
    k = deleteNode2(&current_movie, mid, i);
    if (k == 1) {
      deleted = i;
      break;
    }
  }

  /* Print the category list if a movie is deleted, otherwise print a message */
  if (deleted != -1) {
    printf("Category list: ");
    struct movie *current_category_movie = category_pointers[deleted];
    while (current_category_movie) {
      printf("%u ", current_category_movie->info.mid);
      current_category_movie = current_category_movie->next;
    }
    printf("\n");
  } else {
    printf("Movie with mid=%u not found in any category.\n", mid);
  }

  printf("DONE\n\n");
}


/*----------------------------------------------------------------------------------------------------------------------*/

void print_movies(void){

	struct movie *current_movie = category_pointers[0];
	printf("Categorized Movies\n");
	printf("Horror:");
	while(current_movie != NULL){
		printf("<%u> ", current_movie->info.mid);
		if(current_movie->next == NULL){
			current_movie->next;
		}
		current_movie = current_movie->next;
	}
	printf("\n");
	current_movie = category_pointers[1];
	printf("Sci-fi:");
	while(current_movie != NULL){
		printf("<%u> ", current_movie->info.mid);
		current_movie = current_movie->next;
	}
	printf("\n");
	current_movie = category_pointers[2];
	printf("Drama:");
	while(current_movie != NULL){
		printf("<%u> ", current_movie->info.mid);
		current_movie = current_movie->next;
	}
	printf("\n");
	current_movie = category_pointers[3];
	printf("Romance:");
	while(current_movie != NULL){
		printf("<%u> ", current_movie->info.mid);
		current_movie = current_movie->next;
	}
	printf("\n");
	current_movie = category_pointers[4];
	printf("Documentary:");
	while(current_movie != NULL){
		printf("<%u> ", current_movie->info.mid);
		current_movie = current_movie->next;
	}
	printf("\n");
	current_movie = category_pointers[5];
	printf("Comedy:");
	while(current_movie != NULL){
		printf("<%u> ", current_movie->info.mid);
		current_movie = current_movie->next;
	}
	printf("\nDONE\n\n");
	
}

/*----------------------------------------------------------------------------------------------------------------------*/

void print_users(void) {
	
	struct user *current_user = head_user;
	printf("Users:\n");
	while(current_user != NULL){

		printf("<%d>\n", current_user->uid);
		printf("Suggested:");
		struct suggested_movie *current_suggested_movie = current_user->suggestedHead;
		while(current_suggested_movie != NULL){
			printf("<%u>, ", current_suggested_movie->info.mid);
			current_suggested_movie = current_suggested_movie->next;
		}
		printf("\n");
		printf("Watch History:");
		struct movie *current_movie = current_user->watchHistory;
		while(current_movie != NULL){
			printf("<%u>, ", current_movie->info.mid);
			current_movie = current_movie->next;
		}
		printf("\n");

		current_user = current_user->next;
	}
	printf("DONE\n\n");

}


/*----------------------------------------------------------------------------------------------------------------------*/

void destroy_structures(void)
	{
		struct user *current_user = head_user;
		struct movie *current_movie = head_movie;
		struct new_movie *current_new_movie = head_new_movie;
		struct suggested_movie *current_suggested_movie = head_suggested_movie;
		struct movie *current_top_stack = top_stack;
		struct movie_filter *current_movie_filter = head_movie_filter;

		while(current_user != NULL){
			struct user *temp_user = current_user;
			current_user = current_user->next;
			free(temp_user);
		}

		while(current_new_movie != NULL){
			struct new_movie *temp_new_movie = current_new_movie;
			current_new_movie = current_new_movie->next;
			free(temp_new_movie);
		}

		while(current_suggested_movie != NULL){
			struct suggested_movie *temp_suggested_movie = current_suggested_movie;
			current_suggested_movie = current_suggested_movie->next;
			free(temp_suggested_movie);
		}

		while(current_top_stack != NULL){
			struct movie *temp_top_stack = current_top_stack;
			current_top_stack = current_top_stack->next;
			free(temp_top_stack);
		}

		while(current_movie_filter != NULL){
			struct movie_filter *temp_movie_filter = current_movie_filter;
			current_movie_filter = current_movie_filter->next;
			free(temp_movie_filter);
		}

}

/*----------------------------------------------------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	FILE *event_file;
	char line_buffer[MAX_LINE];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	event_file = fopen(argv[1], "r");
	if (!event_file) {
		perror("fopen error for event file open");
		exit(EXIT_FAILURE);
	}

	init_structures();
	while (fgets(line_buffer, MAX_LINE, event_file)) {
		char *trimmed_line;
		char event;
		int uid;
		unsigned mid, year;
		movieCategory_t category1, category2;
		/*
		 * First trim any whitespace
		 * leading the line.
		 */
		trimmed_line = line_buffer;
		while (trimmed_line && isspace(*trimmed_line))
			trimmed_line++;
		if (!trimmed_line)
			continue;
		/* 
		 * Find the event,
		 * or comment starting with #
		 */
		if (sscanf(trimmed_line, "%c", &event) != 1) {
			fprintf(stderr, "Could not parse event type out of input line:\n\t%s",
					trimmed_line);
			fclose(event_file);
			exit(EXIT_FAILURE);
		}

		switch (event) {
			/* Comment, ignore this line */
			case '#':
				break;
			case 'R':
				if (sscanf(trimmed_line, "R %d", &uid) != 1) {
					fprintf(stderr, "Event R parsing error\n");
					break;
				}
				register_user(uid);
				break;
			case 'U':
				if (sscanf(trimmed_line, "U %d", &uid) != 1) {
					fprintf(stderr, "Event U parsing error\n");
					break;
				}
				unregister_user(uid);
				break;
			case 'A':
				if (sscanf(trimmed_line, "A %u %d %u", &mid, &category1,
							&year) != 3) {
					fprintf(stderr, "Event A parsing error\n");
					break;
				}
				add_new_movie(mid, category1, year);
				break;
			case 'D':
				distribute_new_movies();
				break;
			case 'W':
				if (sscanf(trimmed_line, "W %d %u", &uid, &mid) != 2) {
					fprintf(stderr, "Event W parsing error\n");
					break;
				}
				watch_movie(uid, mid);
				break;
			case 'S':
				if (sscanf(trimmed_line, "S %d", &uid) != 1) {
					fprintf(stderr, "Event S parsing error\n");
					break;
				}
				suggest_movies(uid);
				break;
			case 'F':
				if (sscanf(trimmed_line, "F %d %d %d %u", &uid, &category1,
							&category2, &year) != 4) {
					fprintf(stderr, "Event F parsing error\n");
					break;
				}
				filtered_movie_search(uid, category1, category2, year);
				break;
			case 'T':
				if (sscanf(trimmed_line, "T %u", &mid) != 1) {
					fprintf(stderr, "Event T parsing error\n");
					break;
				}
				take_off_movie(mid);
				break;
			case 'M':
				print_movies();
				break;
			case 'P':
				print_users();
				break;
			default:
				fprintf(stderr, "WARNING: Unrecognized event %c. Continuing...\n",
						event);
				break;
		}
	}
	fclose(event_file);
	destroy_structures();
	return 0;
}
