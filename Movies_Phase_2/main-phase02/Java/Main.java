/**
 * ***************************************************
 * @file   main.c
 *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr> * * @brief Main Function for Data
 * Structures (CS240b) * Project: Winter 2023	*
 * ***************************************************
 */
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Main {

    // Number of movie categories ( Horror, ScienceFiction, Drama, Romance, Documentary, Comedy)
    static final int CATEGORIES = 6;
    /**
     * The size of the users hashtable (>0)
     */
    static int hashtable_size;
    /**
     * The maximum number of registrations (users)
     */
    static int max_users;
    /**
     * The maximum user ID
     */
    static int max_id;

    // This is a very conservative progress on the hashtable. Our purpose
    // is to force many rehashes to check the stability of the code.
    static int primes_g[] = {5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
        41, 43, 47, 53, 59, 61, 67, 71, 73, 79,
        83, 89, 97, 101, 103, 107, 109, 113, 127, 131,
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

    /**
     * @brief Creates a new user. Creates a new user with userID as its
     * identification.
     *
     * @param userID The new user's identification
     *
     * @return true on success false on failure
     */
    public static boolean register_user(int userID) {
        return true;
    }

    /**
     * @brief Deletes a user. Deletes a user with userID from the system, along
     * with users' history tree.
     *
     * @param userID The new user's identification
     *
     * @return true on success false on failure
     */
    public static boolean unregister_user(int userID) {
        return true;
    }

    /**
     * @brief Add new movie to new release binary tree. Create a node movie and
     * insert it in 'new release' binary tree.
     *
     * @param movieID The new movie identifier
     * @param category The category of the movie
     * @param year The year movie released
     *
     * @return 0 on success 1 on failure
     */
    public static boolean add_new_movie(int movieID, int category, int year) {
        return true;
    }

    /**
     * @brief Categorize new releases to the appropriate category list
     *
     * @return true on success false on failure
     */
    public static boolean distribute_movies() {
        return true;
    }

    /**
     * @brief User rates the movie with identification movieID with score
     *
     * @param userID The identifier of the user
     * @param movieID The identifier of the movie
     * @param score The score that user rates the movie with id movieID
     *
     * @return true on success false on failure
     */
    public static boolean watch_movie(int userID,int category, int movieID, int score) {
        return true;
    }

    /**
     * @brief Identify the best rating score movie and cluster all the movies of
     * a category.
     *
     * @param category The category of the movie
     *
     * @return true on success false on failure
     */
    public static boolean filter_movies(int userID, int score) {
        return true;
    }

    /**
     * @brief Find the median score that user rates movies.
     *
     * @param userID The identifier of the user
     *
     * @return true on success false on failure
     */
    public static boolean user_stats(int userID) {
        return true;
    }

    /**
     * @brief Search for a movie with identification movieID in a specific
     * category.
     *
     * @param movieID The identifier of the movie
     * @param category The category of the movie
     *
     * @return true on success false on failure
     */
    public static boolean search_movie(int movieID, int category) {
        return true;
    }

    /**
     * @brief Prints the movies in movies categories array.
     * @return true on success false on failure
     */
    public static boolean print_movies() {
        return true;
    }

    /**
     * @brief Prints the users hashtable.
     * @return true on success false on failure
     */
    public static boolean print_users() {
        return true;
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws FileNotFoundException, IOException {
        BufferedReader inputFile;
        String line;
        String[] params;

        /* Check command buff arguments */
        if (args.length != 1) {
            System.err.println("Usage: <executable-name> <input_file>");
            System.exit(0);
        }

        /* Open input file */
        inputFile = new BufferedReader(new FileReader(args[0]));

        /* Read input file and handle the events */
        while ((line = inputFile.readLine()) != null) {

            System.out.println("Event: " + line);
            params = line.split(" ");
            char eventID = line.charAt(0);

            switch (eventID) {

                /* max_users */
                case '0': {
                    max_users = Integer.parseInt(params[1]);
                    //System.out.println("max_users = " + max_users);
                    break;
                }
                /* max_id */
                case '1': {
                    max_id = Integer.parseInt(params[1]);
                    //System.out.println("max_id = " + max_id);
                    break;
                }
                /* Event R : R <userID> - Register user. */
                case 'R': {
                    int userID = Integer.parseInt(params[1]);

                    if (register_user(userID)) {
                        System.out.println("R " + userID + " succeeded");
                    } else {
                        System.err.println("R " + userID + " failed");
                    }
                    break;
                }
                /* Event U : U <userID> - Unregister user. */
                case 'U': {
                    int userID = Integer.parseInt(params[1]);

                    if (unregister_user(userID)) {
                        System.out.println("U " + userID + " succeeded");
                    } else {
                        System.err.println("U " + userID + " failed");
                    }
                    break;
                }

                /* Event A : A <movieID> <category> <year> - Add new movie. */
                case 'A': {
                    int movieID = Integer.parseInt(params[1]);
                    int category = Integer.parseInt(params[2]);
                    int year = Integer.parseInt(params[3]);

                    if (add_new_movie(movieID, category, year)) {
                        System.out.println("A " + movieID + " " + category + " " + year + " succeeded");
                    } else {
                        System.err.println("A " + movieID + " " + category + " " + year + " failed");
                    }
                    break;
                }

                /* Event D : D  - Distribute movies. */
                case 'D': {
                    if (distribute_movies()) {
                        System.out.println("D succeeded");
                    } else {
                        System.err.println("D failed");
                    }
                    break;
                }
                /* Event W : W <userID> <movieID> <score>  - Watch movie */
                case 'W': {
                    int userID = Integer.parseInt(params[1]);
                    int category = Integer.parseInt(params[2]);
                    int movieID = Integer.parseInt(params[3]);
                    int score = Integer.parseInt(params[4]);

                    if (watch_movie(userID,category, movieID, score)) {
                        System.out.println("W " + userID + " " + category + " " + movieID + " " + score + " succeeded");
                    } else {
                        System.err.println("W " + userID + " " + category + " " + movieID + " " + score + " failed");
                    }
                    break;
                }

                /* Event F : F <userID><score>  - Filter movies */
                case 'F': {
                    int userID = Integer.parseInt(params[1]);
                    int score = Integer.parseInt(params[2]);

                    if (filter_movies(userID,score)) {
                        System.out.println("F succeeded");
                    } else {
                        System.err.println("F failed");
                    }
                    break;
                }

                /* Event Q : Q <userID> - User statistics */
                case 'Q': {
                    int userID = Integer.parseInt(params[1]);
                    if (user_stats(userID)) {
                        System.out.println("Q succeeded");
                    } else {
                        System.err.println("Q failed");
                    }
                    break;
                }
                /* Event I : I <movieID> <category> - Search movie */
                case 'I': {
                    int movieID = Integer.parseInt(params[1]);
                    int category = Integer.parseInt(params[2]);
                    if (search_movie(movieID, category)) {
                        System.out.println("I " + movieID + category + " succeeded");
                    } else {
                        System.err.println("I " + movieID + category + " failed");
                    }
                    break;
                }

                /* Event M : M  - Print movies */
                case 'M': {
                    if (print_movies()) {
                        System.out.println("M succeeded");
                    } else {
                        System.err.println("M failed");
                    }
                    break;

                }

                /* Event P : P  - Print users */
                case 'P': {
                    if (print_users()) {
                        System.out.println("P succeeded");
                    } else {
                        System.err.println("P failed");
                    }
                    break;
                }
                /* Empty line */
                case '\n':
                    break;

                /* Ignore everything else */
                default:
                    System.out.println("Ignoring " + line);
                    break;
            }
        }

    }

}
