#pragma once  // 防止头文件被多次包含
#include <Windows.h>  // Windows API 头文件
#include <thread>     // 线程支持
#include <conio.h>    // 控制台输入输出支持
#include <mutex>      // 线程互斥锁
#include <regex>      // 正则表达式支持
#include <string.h>   // 字符串操作
#include <fstream>    // 文件读写支持
#include <iostream>   // 输入输出流支持
#include <assert.h>   // 断言支持
#include "LinkedList.h"
#include "NoteList.h"
#define DEBUG 0       // 调试标志，0 表示关闭调试信息

//音符列表类（乐谱类）
class MusicList{
public:
	int noteDelayMs=500; //四分音符延迟
	LinkedList list; //存储音符的链表
	
	~MusicList(){}

	//添加音符
	void add(std::string s){
		list.append(s);
	}

	//清除乐谱
	void clear(){
		list.clear();
	}
	
	//设置音符持续时间
	void setDelay(int _noteDelayMs){
		noteDelayMs= _noteDelayMs;
	}

	//读取乐谱，读取相应文件名的文件，如无参数，默认为空
	void readFile(std::string fileName=""){
		//清除当前乐谱
		clear();
		//打开文件
		std::ifstream in(fileName);
		//读取第一行的音符节奏
		in>>noteDelayMs;
		
		std::string noteLine;
		//读取空格之间的字符串作为音符，并加入乐谱
		while (getline(in,noteLine)) add(noteLine);
		//关闭io流
		in.close();
	}
	//构造函数
	MusicList(std::string fileName=""){
		list.clear();
		if (fileName!="") readFile(fileName);
	}
};
//判断是否为纯数字的节奏行
bool isNumeric(std::string const &str){
    // return regex_match(str,std::regex("[(-|+)|][0-9]+"));
	return regex_match(str,std::regex("[0-9]+"));
}

class MusicPlayer{
private:
	enum scale{
		//休止符
		Rest=0,
		//不同音调的音符，与midi音符编号对应
		C8=108,
		B7=107,A7s=106,A7=105,G7s=104,G7=103,F7s=102,F7=101,E7=100,D7s=99, D7=98, C7s=97, C7=96,
		B6=95, A6s=94, A6=93, G6s=92, G6=91, F6s=90, F6=89, E6=88, D6s=87, D6=86, C6s=85, C6=84,
		B5=83, A5s=82, A5=81, G5s=80, G5=79, F5s=78, F5=77, E5=76, D5s=75, D5=74, C5s=73, C5=72,
		B4=71, A4s=70, A4=69, G4s=68, G4=67, F4s=66, F4=65, E4=64, D4s=63, D4=62, C4s=61, C4=60,
		B3=59, A3s=58, A3=57, G3s=56, G3=55, F3s=54, F3=53, E3=52, D3s=51, D3=50, C3s=49, C3=48,
		B2=47, A2s=46, A2=45, G2s=44, G2=43, F2s=42, F2=41, E2=40, D2s=39, D2=38, C2s=37, C2=36,
		B1=35, A1s=34, A1=33, G1s=32, G1=31, F1s=30, F1=29, E1=28, D1s=27, D1=26, C1s=25, C1=24,
		B0=23, A0s=22, A0=21	
	};
	//八度音阶（钢琴白键），使用二维数组存储
	const int C_Scale[7][7]={{C1,D1,E1,F1,G1,A1,B1},
							 {C2,D2,E2,F2,G2,A2,B2},
							 {C3,D3,E3,F3,G3,A3,B3},
							 {C4,D4,E4,F4,G4,A4,B4},
							 {C5,D5,E5,F5,G5,A5,B5},
							 {C6,D6,E6,F6,G6,A6,B6},
							 {C7,D7,E7,F7,G7,A7,B7}};
							 
	//变音音阶（钢琴黑键），使用二维数组存储
	const int C_Scale_s[7][7]={{C1s,D1s,-1,F1s,G1s,A1s,-1},
							   {C2s,D2s,-1,F2s,G2s,A2s,-1},
							   {C3s,D3s,-1,F3s,G3s,A3s,-1},
							   {C4s,D4s,-1,F4s,G4s,A4s,-1},
							   {C5s,D5s,-1,F5s,G5s,A5s,-1},
							   {C6s,D6s,-1,F6s,G6s,A6s,-1},
							   {C7s,D7s,-1,F7s,G7s,A7s,-1}};
	//midi输出句柄
	HMIDIOUT handle;
	//四分音符延迟
	int noteDelayMs=500;
	//音量
	int volume=0x7f;

public:
	//音乐播放状态（是否结束）
	bool ENDMUSIC=0;
	//停止播放命令
	int STOP;
	//构造函数，当创建MusicPlayer对象时，自动打开midi输出设备
	MusicPlayer(){
		midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
	}
	//对象被销毁时，关闭midi设备
	~MusicPlayer(){
		midiOutClose(handle);
	}
	//以下为set方法
	void setVolume(int _vol){
		volume=_vol;
	}
	void setDelay(int _noteDelayMs){
		noteDelayMs=_noteDelayMs;
	}

	//互斥锁，保护多线程共享资源
	std::mutex mu;

	int ttag=0;

	int tick1,tick2;

	void play_single(std::string s,bool isMain){

		std::vector <int> noteBuffer;

		s=s+' ';int n=s.size();
		//时值
		int ctn=32*21,vol=volume;
		//是否为和弦
		bool isChord=0;
		noteBuffer.clear();
		int start=clock(),tick=0;
		for (int i=0;i<n;++i){
			//如果音乐结束，终止循环
			if (ENDMUSIC) break;

			char c=s[i];
			switch (c){
				//开始和弦
				case '[':case '{':{
					//使用断言，如果在此之前已有和弦正在进行，那么终止程序
					assert(isChord==0);
					//设置和弦正在进行
					isChord=1;
					break;
				}
				//结束和弦
				case ']':case '}':{
					//使用断言，如果在此之前和弦已经结束，那么终止程序
					assert(isChord==1);
					//设置和弦结束
					isChord=0;
					break;
				}
				//处理空格
				case ' ':{
					//如果不是和弦且音符不为空
					if (!(isChord || noteBuffer.empty())){
						//遍历每个音符
						for (int noteInfo : noteBuffer)
							//如果不是休止符
    						if (noteInfo != 0)
								//midi设备输出
        						midiOutShortMsg(handle, noteInfo);
						//遍历完成，清空音符字符串
						noteBuffer.clear();
						//延迟指定时间
						while ((clock()-start)*1000.0/CLOCKS_PER_SEC < noteDelayMs/32.0/21*(tick+ctn));
						tick+=ctn;
						ctn=32*21;
					}
					break;
				}
				//小节线
				case '|':break;
				//分音，延迟减半
				case '_':{
					ctn/=2;
					break;
				}
				//延迟减少为三分之一
				case '*':{
					ctn/=3;
					break;
				}
				//延迟减少为七分之一
				case '&':{
					ctn/=7;
					break;
				}
				//延迟减少为五分之一
				case '%':{
					ctn/=5;
					break;
				}
				//附点，延迟增半
				case '.':{
					ctn*=1.5;
					break;
				}
				//延音，每个符号多出一个四分音符
				case '-':{
					ctn+=32*21;
					break;
				}
				//休止符
				case '0':{
					noteBuffer.push_back(Rest);
					break;
				}
				default:{
					//如果音符不符合要求，终止程序
					assert(c>='1' && c<='7');
					//定义x由音符映射为0-6的数字；定义lvl为音符的八度
					int x=(int)c-49,lvl=3;
					//是否升音
					bool isSharp=0;
					//读取音符后面的符号，作特殊处理
					for (int j=i+1;j<n;++j){
						if (s[j]=='^') lvl++;
						else if (s[j]==',') lvl--;
						else if (s[j]=='#') isSharp=1; 
						else break;
						i++;
					}
					//根据midi协议发送相应数据
					if (isSharp) noteBuffer.push_back((vol<<16)+(C_Scale_s[lvl][x]<<8)+0x90);
					else noteBuffer.push_back((vol<<16)+(C_Scale[lvl][x]<<8)+0x90);
					break;
				}
			}
		}
		//如果本行是主旋律，则本行的时间戳为主时间戳
		if (isMain) tick1=tick;else tick2=tick;
		mu.lock();
		STOP++;
		mu.unlock();
		return;
	}
	void play(std::string s1,std::string s2=""){
		STOP=0;tick1=0;tick2=0;
		//设置曲子线程，分别对应两段旋律
		std::thread tune1(&MusicPlayer::play_single,this,s1,1);tune1.detach();
		std::thread tune2(&MusicPlayer::play_single,this,s2,0);tune2.detach();
		while (STOP<2);
		if (DEBUG){
			if (tick1==tick2) puts("Succ");
			else printf("Warn: %d!=%d\n",tick1,tick2);			
		}
	}
	
	void playList(MusicList &m){
		//设置默认延迟
		noteDelayMs=m.noteDelayMs;
		ENDMUSIC=0;
		for (int i=0;i<m.list.size() && !ENDMUSIC;++i){
			//解析节奏行，跳过空行
			while (i<m.list.size() && (m.list.findAt(i) == "" || isNumeric(m.list.findAt(i)))){
				if (isNumeric(m.list.findAt(i))) setDelay(stoi(m.list.findAt(i)));
				i++;
			}
			//结束循环，防止越界
			if (i>=m.list.size()) break;
			//获取当前节奏行存储至至s1，如果下一行也不为空，解析并存储至s2；s1和s2分别为两段旋律
			std::string s1=m.list.findAt(i),s2="";
			if (i<m.list.size()-1 && m.list.findAt(i+1)!="") s2=m.list.findAt(i+1),i++;
			//播放s1和s2
			play(s1,s2);
		}
	}

	Note parseNote(const std::string& noteStr) {
		double duration = 500;
		int vol = 0x7f;
		int pitch = Rest;
		int size = noteStr.size();
		int lvl = 3;
		bool isSharp = 0;
		int x = 0;
		//分割字符串

		for (int i = 0; i < size; i++){
			char c = noteStr[i];
			switch (c){
				case '_':{
					duration /= 2;
					break;
				}
				case '*':{
					duration /= 3;
					break;
				}
				case '&':{
					duration /= 7;
					break;
				}
				case '%':{
					duration /= 5;
					break;
				}
				case '.':{
					duration *= 1.5;
					break;
				}
				case '-':{
					duration += duration;
					break;
				}
				case '0':{
					break;
				}
				case '^':{
					lvl++;
					break;
				}
				case ',':{
					lvl--;
					break;
				}
				case '#':{
					isSharp = 1;
					break;
				}
				default:{
					assert(c >= '1' && c <= '7');
					x = c - 49;
					break;
				}
			}
		}
		pitch = (isSharp  == 0 ? C_Scale[lvl][x] : C_Scale_s[lvl][x]);
		return Note(pitch,duration,vol);
}

void parseLineToNoteList(NoteList& noteList, const std::string& line) {
    std::istringstream iss(line);
    std::string token;
	//链表动态数组
	std::vector<NoteList> noteLists;
    while (iss >> token) { // 使用空格分割
        if (token == "|") continue; // 忽略小节线

        if (token.front() == '[' && token.back() == ']') {
            // 解析和弦
            std::string chordNotes = token.substr(1, token.size() - 2);
            std::istringstream chordStream(chordNotes);
            std::string chordToken;

            while (getline(chordStream, chordToken, ',')) {
                Note chordNote = parseNote(chordToken);
                noteList.append(chordNote);
            }
        } else {
            // 解析单独的音符
            Note singleNote = parseNote(token);
            noteList.append(singleNote);
        }
    }
}

};
class BGM{
public:
	MusicPlayer player;
	MusicList nowList;
	BGM(std::string name,int volume=0x7f){
		nowList.readFile(name);player.setVolume(volume);
	}
	~BGM(){
		player.ENDMUSIC=1;
	}
	void setMusic(std::string name){
		nowList.readFile(name);
	}
	void play_thread(){
		while (1){
			player.playList(nowList);
			if (player.ENDMUSIC) break;	
		}
	}
	void play(){
		player.ENDMUSIC=0;
		std::thread bgm(&BGM::play_thread,this);bgm.detach();
	}
	void stop(){
		player.ENDMUSIC=1;
	}
};
