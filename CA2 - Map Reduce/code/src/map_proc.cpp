#include "../includes/map_proc.hpp"

using namespace std;

MapProc::MapProc(string dataLine_) {
    dataLine = dataLine_;
}

void MapProc::startMap() {
    
    string partDir = Utils::parse_line(dataLine,'#');
    string partNum = Utils::parse_line(dataLine,'#');
    string genersData = Utils::parse_line(dataLine,'#');
    string fileData = Utils::read_from_file(partDir); 
    vector<string> genersList = Utils::make_list_from_line(genersData,',');
    vector<int> countTable(genersList.size(), 0);
    vector<string> partLines = Utils::make_list_from_line(fileData,'\n');
    
    for (unsigned int i = 0; i < partLines.size(); i++)
    {
        string bookName = Utils::parse_line(partLines[i], ',');
        vector<string> bookGeners = Utils::make_list_from_line(partLines[i], ',');
        int index = 0;
        for (unsigned int i = 0; i < bookGeners.size(); i++)
        {
            index = Utils::get_index(bookGeners[i], genersList);
            countTable[index]++;
        }
        
    }

    char buff_[MAX_SIZE_STR];
    int write_fifo;
    for (unsigned int i = 0; i < countTable.size(); i++)
    {
        string dir = Utils::make_pipe_dir(genersList[i], stoi(partNum));
        write_fifo = open(&dir[0], O_WRONLY);
        buff_[MAX_SIZE_STR] = {0};
        strcpy(buff_, to_string(countTable[i]).c_str());
        write(write_fifo, buff_, MAX_SIZE_STR);
        close(write_fifo);
    }
}

