#include "music.h"
using namespace std;
int main(){
    BGM bgm("../songs/7.txt");
    bgm.play();
    while (1);
    bgm.stop();
    // std::string str = "[5^2^1^5]_. 1^_. 5^_ 2^_. [6^6]_. [7^7]_ | [1^^1^]_. [7^7]_. [5^5]_ [2^5]_ 1^_ 1^_ 5_ | 6_. 1^_. 2^_ 5^_. 2^^_. [5^^2^^]_ | [5^^^5^^]--- 5_ |";
    // MusicPlayer musicPlayer;
    // std::vector <NoteList>listVector = musicPlayer.parseLineToListVector(str);
    // musicPlayer.playSingleLine(listVector);
    return 0;
}
