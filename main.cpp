#include <iostream>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <set>
#include <queue>


typedef std::chrono::system_clock Clock;

// Loads the dictionary words of size word_len into an unordered map.
void loadDictionary(std::unordered_map<std::string, bool>& dict, int word_len)
{
    // File stream
    std::ifstream infile("dictionary.txt");

    // Copy all strings into the dictionary that are of length word_len
    std::string word;
    while (infile >> word) {
        if (word.length() == word_len)
            dict[word] = true;
    }
}

// Swapping letters from source to target.
// Returns -1 if words could not be matched.
int swapLetters(std::unordered_map<std::string, bool>& dict,
                std::string source,
                std::string target)
{
    int steps = 0;
    char orig;
    for (int i = 0; i < source.size(); i++) {
        if (target[i] == source[i])
            continue;
            
        orig = source[i];
        source[i] = target[i];

        if ((source == target))
            return steps;

        if (dict.find(source) != dict.end())
            steps++;
        else
            source[i] = orig;

    }
    return -1;
}


// Bidirectional search.
int shortestLadderGram(std::unordered_map<std::string, bool>& dict,
                       const std::string& source,
                       const std::string& target)
{
    std::queue<std::string> start;
    std::queue<std::string> end;
    start.push(source);
    end.push(target);

    // Stores which states have been visited and at what level of the tree.
    std::unordered_map<std::string, int> visited_end;
    std::unordered_map<std::string, int> visited_start;
    visited_start[target] = 0;
    visited_end[target] = 0;

    int start_level = 0, end_level = 0;

    const std::string alphabet = "etaoinsrhdlucmfywgpbvkxqjz";

    while (!start.empty() && !end.empty()) {
        // For the Start tree.
        start_level++;
        int q_size = start.size();

        // The queue is being updated while traversed.
        // So only traverse the words that are currently
        // in the queue (no updated words).
        for (int j = 0; j < q_size; j++) {
            std::string word = start.front();
            start.pop();

            for (int i = 0; i < word.size(); i++) {
                char original_chr = word[i];

                for (char chr : alphabet) {
                    word[i] = chr;

                    if (dict.find(word) != dict.end()
                        && visited_start.find(word) == visited_start.end()) {

                        if (word == target)
                            return start_level;

                        // Check if it is found on the end tree.
                        if (visited_end.find(word) != visited_end.end())
                            return start_level + visited_end[word];

                        visited_start[word] = start_level;
                        start.push(word);
                    }
                }
                word[i] = original_chr;
            }
        }

        // For the End tree.
        end_level++;
        q_size = end.size();

        // The queue is being updated while traversed.
        // So only traverse the words that are currently
        // in the queue (no updated words).
        for (int j = 0; j < q_size; j++) {
            std::string word = end.front();
            end.pop();

            for (int i = 0; i < word.size(); i++) {
                char original_chr = word[i];

                for (char chr : alphabet) {
                    word[i] = chr;

                    if (dict.find(word) != dict.end()
                        && visited_end.find(word) == visited_end.end()) {

                        if (word == target)
                            return end_level;

                        // Check if it is found on the start tree.
                        if (visited_start.find(word) != visited_start.end())
                            return end_level + visited_start[word];

                        visited_end[word] = end_level;
                        end.push(word);
                    }
                }
                word[i] = original_chr;
            }
        }
    }

    return 0;
}

int main(int argc, char * argv[])
{
    if (argc < 3) {
        std::string msg = "ERROR: expected 2 arguments <source> <target>; received: "
                  + argc-1;
        throw std::runtime_error(msg + " arguments.\n");
    }

    std::string source(argv[1]);
    std::string target(argv[2]);

    if (source.size() != target.size()) {
        std::string msg = "ERROR: source and target must be equal length.\n";
         throw std::runtime_error(msg);
    }

    std::unordered_map<std::string, bool> dict;

    auto start_time = Clock::now();

    loadDictionary(dict, source.size());

    // First confirm that the target string is in the dictionary.
    if (dict.find(target) == dict.end()) { // O(1)
        std::string msg = "Target \"" + target + "\" is not in the dictionary.\n";
        throw std::runtime_error(msg);
    }

    // Firstly try to swapping letters from source to target.
    int result = swapLetters(dict, source, target);
    // Secondly try from target to source.
    if (result == -1)
        result = swapLetters(dict, target, source);
    // Thirdly try Bi directional BFS.
    if (result == -1)
        result = shortestLadderGram(dict, source, target);

    std::chrono::duration<double> elapsed_time = Clock::now() - start_time;

    std::cout << source << "->" << target;
    if (result == 0)
        std::cout << " no such path exists.\n";
    else
        std::cout << " found in: " << result << " steps\n";

    std::cout << "Elapsed time: " << elapsed_time.count() << " seconds" << std::endl;


    return 0;
}
