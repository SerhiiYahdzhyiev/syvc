#include "headers/Utils.h"
#include "headers/Repo.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: syvc <command>\n";
        return 1;
    }

    std::string command = argv[1];
    Repo repo;

    if (command == "init") {
        repo.init();
    } else if (command == "add") {
        if (argc < 3) {
            std::cerr << "Usage: syvc add <file>\n";
            return 1;
        }
        std::string filename = argv[2];
        repo.add(filename);
    } else if (command == "rm") {
        if (argc < 3) {
            std::cerr << "Usage: syvc rm <file>\n";
            return 1;
        }
        std::string filename = argv[2];
        repo.remove(filename);
    } else if (command == "commit") {
        if (argc < 3) {
            std::cerr << "Usage: syvc commit <message>\n";
            return 1;
        }
        std::string message = argv[2];
        repo.commit(message);
    } else if (command == "log") {
        repo.displayCommitLog();
    } else {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }

    return 0;
}
