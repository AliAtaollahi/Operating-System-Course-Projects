#include "../includes/system.hpp"

#include "../includes/utils.hpp"
#include "../includes/consts.hpp"

using namespace std;

System::System(string assetsDirectory_) {
    assetsDirectory = assetsDirectory_;
}

void System::reduces(const vector<string>& genersList, int numberOfParts) {
    int reducesUnnamedPipedFd[2];
    mkdir(PIPES_DIR.c_str(), 0777);

    if (pipe(reducesUnnamedPipedFd) == -1) // pipe fail
        Utils::error("pipe fail");

    for (unsigned int i = 0; i < genersList.size(); i++)
    {
        int pid = fork();
        if (pid == -1) // fork fail
            Utils::error("fork fail");

        else if (pid == 0) // child process
        {
            char buff[MAX_SIZE_STR] = {0};
            read(reducesUnnamedPipedFd[READ], buff, MAX_SIZE_STR);
            string partCount(buff), generName = genersList[i];
            string message = Utils::make_message({generName, partCount});  

            for (int j = 0; j < stoi(partCount); j++)
            {
                string dir = Utils::make_pipe_dir(genersList[i], j + 1);
                const char* dir_ = dir.c_str();
                mkfifo(dir_, 0666);
            }
            
            if (execlp("./reduce_proc.out", message.c_str(), NULL) == -1)
                Utils::error("Exec Failed");

            return;
        }
        else if (pid > 0) // parent process
        {
            char buff[MAX_SIZE_STR] = {0};
            string message = to_string(numberOfParts);
            strcpy(buff , message.c_str());
            write(reducesUnnamedPipedFd[WRITE], buff, MAX_SIZE_STR);
        }
    }
}

void System::maps(int numberOfParts, string genersFileData) {
    int mapsUnnamedPipedFd[2];
    
    if (pipe(mapsUnnamedPipedFd) == -1) // pipe fail
        Utils::error("pipe fail");
    
    for (int i = 1; i < numberOfParts + 1; i++)
    {
        int pid = fork();
        if (pid == -1) // fork fail
            Utils::error("fork fail");

        else if (pid == 0) // child process
        {
            char buff[MAX_SIZE_STR] = {0};
            read(mapsUnnamedPipedFd[READ], buff, MAX_SIZE_STR);
            string genersData(buff);
            string partDir = Utils::make_part_dir(assetsDirectory, i);
            string message = Utils::make_message({partDir, to_string(i), genersData});
            if (execlp("./map_proc.out", message.c_str(), NULL) == -1)
                Utils::error("Exec Failed");

            return;
        }
        else if (pid > 0) // parent process
        {
            char buff[MAX_SIZE_STR] = {0};
            string message = genersFileData;
            strcpy(buff , message.c_str());
            write(mapsUnnamedPipedFd[WRITE], buff, MAX_SIZE_STR);
        }
    }
}

void System::categorize_and_print(const vector<string>& genersList,
                                  int numberOfParts, string genersFileData) {
    reduces(genersList, numberOfParts);
    maps(numberOfParts, genersFileData);
    Utils::wait(WAIT_FOR_END);
}

void System::start_program() {
    string genersFileData = Utils::read_from_file(assetsDirectory + GENERS_DIRECTORY);
    genersFileData = Utils::delete_last_char(genersFileData);
    vector<string> genersList = Utils::make_list_from_line(genersFileData,',');
    int numberOfParts = Utils::get_number_of_parts();
    categorize_and_print(genersList, numberOfParts, genersFileData);
}