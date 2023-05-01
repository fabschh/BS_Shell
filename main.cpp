#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main(){
    time_t start = time(nullptr);

    while(true){
        cout << ">";
        string input;
        getline(cin, input);
        vector<string> arguments;

        istringstream iss(input);
        string word;

        while (iss >> word) {
            arguments.push_back(word);
        }

        int id = fork();

        if(id == 0){
            //cout << "id child " << getpid() << endl;
            //cout << "id parent " << getppid() << endl;
            if(arguments[0] == "date" && arguments.size() == 1){
                time_t now = time(NULL);
                tm* local_time = localtime(&now);
                char date_str[100];
                strftime(date_str, sizeof(date_str), "%a %b %d %H:%M:%S %Z %Y", local_time);
                cout << date_str << endl;
            }
            else if(arguments[0] == "pwd" && arguments.size() == 1){
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) != nullptr) {
                    cout << cwd << endl;
                }
            }
            else if(arguments[0] == "echo"){
                for(int i = 1; i < arguments.size(); i++){
                    cout << arguments[i] << " ";
                }
                cout << endl;
            }
            else if(arguments[0] == "sleep" && arguments.size() <= 3 && arguments.size() > 1){
                int time = stoi(arguments[1]);
                if(arguments.size() == 3 && arguments[2] == "&"){
                    //befehl im hintergrund ausführen
                }
                usleep(time * 1000000);
            }
            else if(arguments[0] == "ps" && arguments.size() == 1){
                FILE* stream = popen("ps", "r");
                if (stream == nullptr){
                    perror("popen() failed");
                    return 0;
                }

                char buffer[256];
                while (fgets(buffer, sizeof(buffer), stream)){
                    printf("%s", buffer);
                }

                pclose(stream);
            }
            else if(arguments[0] ==  "^C" && arguments.size() == 1){

            }
            else if(arguments[0] == "exit" && arguments.size() == 1){

            }
            else{
                cout << "minishell: command not found: " << arguments[0] << endl;
            }
            exit(0);
        }
        else{
            cout << "f>";
            string father_input;
            getline(cin, father_input);
            vector<string> father_arguments;

            istringstream father_iss(input);
            string father_word;

            while (father_iss >> father_word) {
                father_arguments.push_back(word);
            }


            if(father_arguments[0] ==  "^C" && father_arguments.size() == 1){
                time_t end = time(nullptr);
                double elapsed_time = difftime(end, start);
                int hours = elapsed_time / 3600;
                elapsed_time -= hours * 3600;
                int minutes = elapsed_time / 60;
                elapsed_time -= minutes * 60;
                cout << "time elapsed: " << hours << "h " << minutes << "m " << elapsed_time << "s";
                return 0;
            }
            else if(father_arguments[0] == "exit" && father_arguments.size() == 1){
                string quit;
                bool correct = false;
                while(!correct){
                    cout << "Do you really want to quit (y/n): ";
                    getline(cin, quit);
                    if(quit == "y"){
                        cout << "Das Programm wird beendet!" << endl;
                        return 0;
                    }
                    else if (quit == "n") {
                        correct = true;
                    }
                    else {
                        cout << "Invalid input. Please enter 'y' or 'n'." << endl;
                    }
                }
            }
            else{
                wait(NULL);
                //cout << "parent id done waiting: " << getpid() << endl;
            }
        }
    }
    return 0;
}












