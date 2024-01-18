#include <fstream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <iomanip>

using namespace std;

//TODO 
// destructors
// emergence, chemotaxis
// dump dat every n intervals, but dump all board data

// global variables
const int RESOLUTION = 50; // board size, used in for loops
const int spawnCount = 5; // initial population
int MAX_FRAMES = 2000; // maximum number of frames to generate
const int MAX_PLOTS = 150; // number of data points to generate for plotting
const int plt_mod = MAX_FRAMES/MAX_PLOTS;

void print_progress(int frame, int max){
    // printing progress for visual output
    double perc = float(frame)/float(max)*100;
    if(static_cast<int>(perc) == perc){
        printf("\33[2K\r");
        perc = int(perc);
        cout << "Simulation: [" << perc << "%]";
    }
}

int spawn_point(){
    // getting a random point for determining initial spawn point
    int random = rand()% RESOLUTION;
    return random;
}

class Prey
{
private:
    
public:
    int position_i;
    int position_j;
    bool alive;
    int food_consumed = 15 + (rand()%20 - 10); // defualt starting food plus or minus five
    char live = 'A';
    const char dead = 'D';
    const int DEATH = 0;
    int generation = 0;

    // mutate-able attributes
    int food_quality = 5; // MUTATE             -done
    int reproduction_limit = 100; // MUTATE      -done
    float birth_penalty = 2; // MUTATE     -done       ** make sure no division by zero prior to altering **
    int food_rate = 2; // MUTATE   rate of energy consumption             -done
    int sight_distance = 1; // MUTATE           ** alter edge detection prior to altering **

    Prey(){
        // class constructor
        position_i = spawn_point();
        position_j = spawn_point();
        alive = true;
    }

    bool search_colony(int i, int j, Prey Colony[][RESOLUTION]){
        // find if a colony member is on a given set of indices
        for (int x = 0; x < RESOLUTION; x++)
        {
            for (int y = 0; y < RESOLUTION; y++)
            {
                if(Colony[x][y].position_i == i && Colony[x][y].position_j == j){
                    return true;
                }
            }   
        }
        return false;
    }

    void food_search(int board[][RESOLUTION], Prey Colony[][RESOLUTION]){
        // search within a local boundary area for food
        // set boundary search conditions
        int lower_i = position_i-sight_distance;
        int lower_j = position_j-sight_distance;
        int upper_i = position_i+sight_distance;
        int upper_j = position_j+sight_distance;

        // adjust  search parameters for gameboard boarders
        if(lower_i < 0){
            lower_i = position_i;
        }
        if(lower_j < 0){
            lower_j = position_j;
        }
        if(upper_i > RESOLUTION){
            upper_i = position_i;
        }
        if(upper_j > RESOLUTION){
            upper_j = position_j;
        }

        // search for food
        int indices_i = position_i;
        int indices_j = position_j;
        vector<vector<int>> food_vect;
        vector<int> food_indices;
        int n = 0;
        // search the local area
        for (int i = lower_i; i <= upper_i; i++)
        {
            for (int j = lower_j; j <= upper_j; j++)
            {
                if (board[i][j] > 0)
                {
                    if (search_colony(i, j, Colony) == false)
                    {
                        // if a member is on the given indices, don't move to it
                        n = n + 1;  
                    }
                    else{
                        // if the indices has food, and no member, move to it
                        food_indices.push_back(i);
                        food_indices.push_back(j);
                        food_vect.push_back(food_indices);
                        n = n + 1;
                    }
                }
                else
                {   
                    // if the indices has no food, ignore it
                    n = n + 1;
                }
            }
        }

        // get the indices of the maximum food value present
        if (food_vect.size() > 0)
        {
            int MAX_FOOD = 0;
            int MAX_X = 0;
            int MAX_Y = 0;
            int buffer = 0;
            int buf_x = 0;
            int buf_y = 0;
            for (int i = 0; i < food_vect.size(); i++)
            {
                buf_x = food_vect[i][0];
                buf_y = food_vect[i][1];
                buffer = board[buf_x][buf_y];
                if(buffer > MAX_FOOD){
                    MAX_FOOD = board[MAX_X][MAX_Y];
                    MAX_X = buf_x;
                    MAX_Y = buf_y;
                }
            }
            position_i = MAX_X;
            position_j = MAX_Y;
        }
        else
        {
            // pass and do nothing
        }
    }

    void random_step(int board[][RESOLUTION]){

        // handle edge of the map
        if(position_i < 1){
            // 0 edge
            int a = rand()%2;
            position_i = position_i + a;
        }
        else if (position_i >= (RESOLUTION-2))
        {
            // upper edge
            int a = rand()%2-1;
            position_i = position_i + a;
        }
        else{
            // central area
            int a = rand()%3-1;
            position_i = position_i + a;
        }

        if(position_j < 1){
            int a = rand()%2;
            position_j = position_j + a;
        }
        else if (position_j > (RESOLUTION-2))
        {
            int a = rand()%2-1;
            position_j = position_j + a;
        }
        else{
            int a = rand()%3-1;
            position_j = position_j + a;
        }
        return;
    }

    void consume_food(int board[][RESOLUTION]){
        // consuming food on a specified indices
        board[position_i][position_j] = board[position_i][position_j] - 1;
        food_consumed = food_consumed + food_quality;
    }

    char alter_genID(char gen_id){
        // altering the generation ID for mutation
        char id = gen_id;
        char buff = id + 1;  // character buffer
        if(buff == 'D'){
            id = gen_id + 2; // skip death indicator
        }
        else
        {
            id = gen_id + 1;
        }

        if (gen_id == '[')
        {
            id = 'A'; // circle back to start of ascii table capital letters
        }
        
        return id;
    }

    void mutate(vector<Prey> productive_members, Prey colony[][RESOLUTION], int x, int y, int i){
        // // MUTATIONS
        // food rate mutation
        colony[x][y].live = alter_genID(productive_members[i].live);
        int a = rand()%3-1;
        if ((productive_members[i].food_rate + (a)) < 2)
        {
            colony[x][y].food_rate = 1;
        }
        else
        {
            colony[x][y].food_rate = productive_members[i].food_rate + (a);
        }
        // food quality mutation
        a = rand()%3-1;
        if ((productive_members[i].food_quality + (a))<2)
        {
            colony[x][y].food_quality = 2;
        }
        else
        {
            colony[x][y].food_quality = productive_members[i].food_quality + (a);
        }
        // reproduction mutation
        a = rand()%3-1;
        if ((productive_members[i].reproduction_limit + (a))<1)
        {
            colony[x][y].reproduction_limit = 1;
        }
        else
        {
            colony[x][y].reproduction_limit = productive_members[i].reproduction_limit + (a);
        }
        // birth penalty mutation
        a = rand()%3-1;
        if ((productive_members[i].birth_penalty + (a))<2)
        {
            colony[x][y].birth_penalty = 1;
        }
        else
        {
            colony[x][y].birth_penalty = productive_members[i].birth_penalty + (a);
        }
    }

    bool mutation_generator(){
        // randomly generate a true or false with an approximately 2% chance of returning true
        int counter = 0;
        int a = rand()%25;
        if (a < 3)
        {
            return true;
        }
        return false;
    }

    void reproduce(Prey colony[][RESOLUTION]){
        // search the colony to see if any member meets the condition for reproduction
        vector<Prey> productive_members; // empty vector to build a list of members who meet reproduction requirements
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                // condition for reproduction
                if(colony[i][j].alive == true && food_consumed > reproduction_limit){ // conditions for reproduction
                    productive_members.push_back(colony[i][j]);
                }
            }
        }
        // get the indices of the last living member
        // for setting currently dead members, in colony[][RESOLUTION], to alive
        int x = 0;
        int y = 0;
        int lock = -1;
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                // lock prevents overwrite of first non-living member indices
                if (lock == -1 && colony[i][j].alive == false)
                {
                    x = i; // indices in colony[][] of the first non living member
                    y = j;
                    lock = 0;
                }
            }
        }

        // getting the parent to spawn the child
        for (int i = 0; i < productive_members.size(); i++)
        {
            colony[x][y].alive = true;
            colony[x][y].food_consumed = 15 + (rand()%20 - 9);
            bool gen = mutation_generator();
            if (gen==true)
            {
                // creating a new mutated generation
                colony[x][y].generation = productive_members[i].generation + 1;
                mutate(productive_members, colony, x, y, i);
            }
            else
            {
                // continuing the current generation
                colony[x][y].generation = productive_members[i].generation;
                colony[x][y].food_rate = productive_members[i].food_rate;
                colony[x][y].food_quality = productive_members[i].food_quality;
                colony[x][y].birth_penalty = productive_members[i].birth_penalty;
                colony[x][y].reproduction_limit = productive_members[i].reproduction_limit;
            }
            productive_members[i].food_consumed = productive_members[i].food_consumed/birth_penalty;
            
            // spawn a new member near it's parent
            // set indices & handle the edge of the map
            if(productive_members[i].position_i < 1){
                // 0 edge
                int a = rand()%3;
                colony[x][y].position_i = productive_members[i].position_i + a;
            }
            else if (productive_members[i].position_i >= (RESOLUTION-2))
            {
                // upper edge
                int a = rand()%2-1;
                colony[x][y].position_i = productive_members[i].position_i + a;
            }
            else{
                // central area
                int a = rand()%3-1;
                colony[x][y].position_i = productive_members[i].position_i + a;
            }

            if(productive_members[i].position_j < 1){
                int a = rand()%2;
                colony[x][y].position_j = productive_members[i].position_j + a;
            }
            else if (productive_members[i].position_j > (RESOLUTION-2))
            {
                int a = rand()%2-1;
                colony[x][y].position_j = productive_members[i].position_j + a;
            }
            else{
                int a = rand()%3-1;
                colony[x][y].position_j = productive_members[i].position_j + a;
            }

            // increment across the colony matrix, and down.
            x = x + 1;
            if(x == RESOLUTION){
                y = y + 1;
                x = 0;
            }
            if(y == RESOLUTION){
                y = 0;
            }
        }
    }
    
    void prey_cycle(int board[][RESOLUTION], Prey Colony[][RESOLUTION]){
        // check current position for food
        int original_i = position_i;
        int original_j = position_j;
        if(alive == true){
            if(board[position_i][position_j] > 0){
            // if food present, consume one then move
                board[position_i][position_j] = board[position_i][position_j] - 1;
                food_consumed = food_consumed + food_quality;
                food_search(board, Colony);
                // if(original_i == position_i && original_j == position_j){
                //     random_step(board);
                // }
            }
            else{
                // if no food present, move
                food_search(board, Colony);
                if(original_i == position_i && original_j == position_j){
                    random_step(board);
                }
            }
                // check if death condition is set
            if (food_consumed <= DEATH){
                alive = false;
            }
            reproduce(Colony);

            // burn energy KEEP AT END
            food_consumed = food_consumed - food_rate;
        }  
    }

    int colony_status(Prey colony[][RESOLUTION]){
        // get the number of living colony members
        int sum = 0;
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                if(colony[i][j].alive == true){
                    sum = sum + 1;
                }
            }
        }
        return sum;
    }

    void generate_data(Prey colony[][RESOLUTION], int cycle){
        // generate a data dump of the colony for each cycle
        ofstream FILE_DUMP;
        string FILENAME = "csv/Colony_data_" + to_string(cycle) + ".csv";
        FILE_DUMP.open(FILENAME);
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                if (colony[i][j].alive == true)
                {
                    FILE_DUMP << cycle;
                    FILE_DUMP << ',';
                    FILE_DUMP << colony[i][j].generation;
                    FILE_DUMP << ',';
                    FILE_DUMP << colony[i][j].food_consumed;
                    FILE_DUMP << ',';
                    FILE_DUMP << colony[i][j].food_quality;
                    FILE_DUMP << ',';
                    FILE_DUMP << colony[i][j].food_rate;
                    FILE_DUMP << ',';
                    FILE_DUMP << colony[i][j].birth_penalty;
                    FILE_DUMP << ',';
                    FILE_DUMP << colony[i][j].reproduction_limit;
                    FILE_DUMP << endl;
                }
            }
        }
        FILE_DUMP.close();
    }

    void set_spawncount(int spawnCount, Prey colony[][RESOLUTION]){
        // set colony members to dead, default colony member status is alive
        int live_counter = 0;
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                if (live_counter<spawnCount)
                {
                    live_counter = live_counter + 1;
                }
                else
                {
                    colony[i][j].alive = false;
                }
            }
        }
    }
};

class GameBoard
{
private:
public:
    int board[RESOLUTION][RESOLUTION];
    int max_food = 20;

    void start_board(){
        // getting the board set up
        int set = initialize_board();
        if (set == 1){
        }
        else
        {
            int food = add_food();
        }
    }

    int initialize_board(){
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                board[i][j] = 0;
            }
            
        }
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                if (board[i][j] != 0){
                    return 1;
                }
            }
        }
        return 0;
    }

    int add_food(){
        int counter = 0; // food counter
        // populate the board with food
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                int a = rand()%2; // get a random 1 or 0
                board[i][j] = board[i][j] + a;
            }
        }

        // counts the total amount of food on the board
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                if(board[i][j] != 0){
                    counter = counter + board[i][j];
                }
            }
        }
        return counter;
    }

    void Wumbo(){
        // Wumbology, the study of wumbo and wumbo-ing
    }

    int get_food(){
        int counter = 0;
        // counts the total amount of food on the board
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                if(board[i][j] != 0){
                    counter = counter + board[i][j];
                }
            }
        }
        return counter;
    }

    void trickle_food(){

        //generate a random number of indices to add food to
        int total_indices = rand()%max_food+1; 
        // generate random indices
        int rand_i = rand()%(RESOLUTION-3)+2;
        int rand_j = rand()%(RESOLUTION-3)+2;
        for (int i = 0; i < total_indices; i++)
        {
            do
            {
                // if the random indices are not in the domain, recalculate
                int a = rand()%5;
                rand_i = rand()%RESOLUTION;
                rand_j = rand()%RESOLUTION; 
                if (board[rand_i][rand_j] + a < 10)
                {
                    board[rand_i][rand_j] = board[rand_i][rand_j] + a;
                }
                
            } while (rand_i < 1 || rand_i > (RESOLUTION-1) || rand_j < 1 || rand_j > (RESOLUTION-1));
        }
    }

    void print_board(Prey prey[][RESOLUTION], ofstream& FILE_DUMP){
        // dumping the board data into .dat file
        bool prey_found = false;
        int a = 0;
        int b = 0;
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                // search for a live animal on the indices
                for (int x = 0; x < RESOLUTION; x++)
                {
                    for (int y = 0; y < RESOLUTION; y++)
                    {
                        if(prey[x][y].alive == true && prey[x][y].position_i == i && prey[x][y].position_j == j){
                            prey_found = true;
                            a = x;
                            b = y;
                            break;
                        }
                        else{
                            prey_found = false;
                        }
                    }
                    if(prey_found == true){
                        break;
                    }
                }
                
                // print
                if(prey_found == true){
                    if (prey[a][b].food_consumed > 1)
                    {
                        FILE_DUMP << ("%c ", prey[a][b].live);
                    }
                    else
                    {
                        FILE_DUMP << ("%c ", prey[a][b].dead);
                    }
                        
                }
                else{   
                    FILE_DUMP << ("%d ", board[i][j]);
                }
            } 
            FILE_DUMP << ("\n");
        }
    }
};

void dump_data_foodNpopulation(ofstream& FILE, GameBoard board, Prey colony[][RESOLUTION], int n){
    // generate total food and total population data files
    string pop_food_data = "dat/foodPopulation_" + to_string(n) + ".csv";
    FILE.open(pop_food_data);
    int population = colony[0][0].colony_status(colony);
    int total_food = board.get_food();
    FILE << population << ',' << total_food << endl;
    FILE.close();
}

int main() {

    cout << endl << "Input number of cycles to simulate [default is 2000 cycles]:" << endl;
    cin >> MAX_FRAMES;
    
    // file init
    ofstream FILE;
 
    // board init
    srand(time(NULL)); // seed for randomization
    GameBoard board;
    board.start_board();

    // generate a limited starting colony equal to spawncount
    Prey colony[RESOLUTION][RESOLUTION];
    colony[0][0].set_spawncount(spawnCount, colony);

    // life cycle variables
    int n = 0; // used for generating data files, and food addition
    bool colony_alive = true;

     // life cycle loop
    while (colony_alive)
    {
        bool limt_data = true;
        print_progress(n, MAX_FRAMES);
        if(limt_data == true && n%plt_mod == 0){
            // generate data file for the total food and population
            dump_data_foodNpopulation(FILE, board, colony, n);

            // generate data files for statistics about the colony per cycle
            colony[0][0].generate_data(colony, n); 
        }
        else
        {
            // generate data file for the total food and population
            dump_data_foodNpopulation(FILE, board, colony, n);

            // generate data files for statistics about the colony per cycle
            colony[0][0].generate_data(colony, n); 
        }
        
        //generate the data of the board for the current cycle
        if(MAX_FRAMES <= 8000){
            // prevents generating gifs if more than eight thousand images are going to be generated
            string FILENAME = "dat/Colony_output_" + to_string(n) + ".dat";
            FILE.open(FILENAME);
            board.print_board(colony, FILE);
            FILE.close();
        }
        // update each colony member
        for (int i = 0; i < RESOLUTION; i++)
        {
            for (int j = 0; j < RESOLUTION; j++)
            {
                // update each living colony member
                colony[i][j].prey_cycle(board.board, colony);
            }
        }
        // update variable, and add food to the board
        if(n%4 == 0){
            board.trickle_food();
        }
        n = n + 1;

        // EXIT CONDITIONS 
        if (colony[0][0].colony_status(colony) == 0)
        {
            // if there are no members alive anymore, exit the loop
            colony_alive = false;
            cout << endl << "Colony failed to survive..." << endl;
        }
        if(n == MAX_FRAMES){
            // if maximum frames are generated, exit the loop
            printf("\33[2K\r");
            cout << "Simulation: [100%]" << endl;
            break;
        }
    }
    return 0;
}