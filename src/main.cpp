#include "headers/Utils.h"
#include "headers/Repo.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printError("Usage: syvc <command>\n");
        return 1;
    }

    Repo repo;

    std::string commandString = argv[1];
    Command command = parseCommand(commandString);

    switch (command) {
        case INIT:
            repo.init();
            break;
        case ADD:
            if (argc < 3) {
                printError("Usage: syvc add <file>\n");
                return 1;
            } else {
                std::string filename = argv[2];
                repo.add(filename);
                break;
            }
        case REMOVE:
            if (argc < 3) {
                printError("Usage: syvc rm <file>\n");
                return 1;
            } else {
                std::string filename = argv[2];
                repo.remove(filename);
                break;
            }
        case LOG:
            repo.displayCommitLog();
            break;
        case STATUS:
            //TODO: Realize
            break;
        case COMMIT:
            if (argc < 3) {
                printError("Usage: syvc commit <message>\n");
                return 1;
            } else {
                std::string message = argv[2];
                repo.commit(message);
                break;
            }
        case REVERT:
            break;
        case HELP:
            printHelp();
            break;
        case UNKNOWN:
            printError("Unknown command: " + commandString + "\n");
            return 1;
    }

    return 0;
}
