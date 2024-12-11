#include "music.h"
using namespace std;
int main(){
    BGM bgm("../songs/3.txt");
    bgm.play();
    while (1);
    bgm.stop();
}
