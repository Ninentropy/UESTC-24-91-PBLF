#pragma once  // 防止头文件被多次包含
#define byte rpcndr_byte_alias
#include <Windows.h>  // Windows API 头文件
#undef byte
#include <thread>     // 线程支持
#include <conio.h>    // 控制台输入输出支持
#include <mutex>      // 线程互斥锁
#include <regex>      // 正则表达式支持
#include <string.h>   // 字符串操作
#include <fstream>    // 文件读写支持
#include <iostream>   // 输入输出流支持
#include <assert.h>   // 断言支持
#include <filesystem>
#include "LinkedList.h"
#include "NoteList.h"
#include "MusicProcess.h"
#define DEBUG 0       // 调试标志，0 表示关闭调试信息

namespace fs = std::filesystem;
//音符列表类（乐谱类）
class MusicManager{
private:
	// 定义目录路径和文件扩展名。
    const fs::path dirPath = "../songs/";
	const std::string ext = ".txt";
	std::string curMusic;
public:
	LinkedList currentMusicList; //存储音符的链表
	MusicSerializer musicSerializer;
	MusicDeserializer musicDeserializer;
	
	~MusicManager(){}
	//添加文本行至链表
	void add(std::string s){
		currentMusicList.Sappend(s);
	}

	//清除乐谱
	void clear(){
		setMusic("");
		currentMusicList.clear();
	}
	void setMusic(const std::string& music){
		this->curMusic = music;
	}
	std::string getMusic(){
		return this->curMusic;
	}

	//读取乐谱，读取相应文件名的文件，如无参数，默认为空
    void load(const std::string& music){
        // 创建原始文件的完整路径。
        fs::path filePath = dirPath / (music + ext);
        //清除当前乐谱
        clear();
        //打开文件
        std::ifstream in(filePath);
        if (!in.is_open()) {
            std::cout << "无法打开文件" << filePath << std::endl;
            return;
        }
        std::string noteLine;
        //读取每一行的音符
        while (getline(in,noteLine)){
            if (noteLine.find_first_not_of("\n") == std::string::npos) {
                // 保留空行
                add("\n\n");
            } else {
                // 否则，添加非空行
                add(noteLine + "\n");
            }
        }
        //关闭io流
        setMusic(music);
        in.close();
    }
	void setDuration(double duration){
        currentMusicList.Sappend(std::to_string(duration) + "\n");
    }
    void saveLine(std::vector<NoteList> noteTable,double defaultDuration,bool isEndChord = 1){
        std::string musicStr =  musicSerializer.serialize(noteTable,defaultDuration);
        if(isEndChord){
            currentMusicList.Sappend(musicStr);
            currentMusicList.Sappend("\n");
        }else {currentMusicList.Sappend(musicStr);
		currentMusicList.Sappend("");
		}

    }
    void saveMusic(const std::string& music){
		// 创建原始文件的完整路径。
		fs::path filePath = dirPath / (music + ext);
		std::ofstream outFile(filePath);
		for (int i = 0; i < currentMusicList.size(); i++)
		{
			std::string musicStr = currentMusicList.findAt(i);
			// 检查文件是否成功打开
			if (outFile.is_open()) {
				// 将字符串写入文件
				outFile << musicStr;
			} else {
				// 如果文件未能打开，打印错误信息
				std::cerr << "无法打开文件: " << filePath << std::endl;
			}
		}
		// 关闭文件
		outFile.close();
	}
    void create(const std::string& music){
        // 创建原始文件的完整路径。
        fs::path filePath = dirPath / (music + ext);
        std::ofstream file(filePath);
        load(music);
    }
	void remove(const std::string& music){
		
		// 构造文件路径
		fs::path filePath = dirPath / (music + ext);
		
		// 尝试删除文件，并检查操作是否成功
		if (fs::exists(filePath)) {
			if (fs::remove(filePath)) {
				std::cout << "文件已删除: " << filePath << std::endl;
			} else {
				std::cerr << "无法删除文件: " << filePath << std::endl;
			}
		} else {
			std::cerr << "文件不存在: " << filePath << std::endl;
		}
	}
	//查找并加载歌曲
	bool query(const std::string& music){
		// 构造文件路径
		fs::path filePath = dirPath / (music + ext);
		if(fs::exists(filePath)){
			return 1;
		}else{
			return 0;
		}
	}
	//构造函数
	MusicManager(std::string music=""){
		currentMusicList.clear();
		if (music!=""){
			this->curMusic = music;
			load(music);
		}
	}
};
//判断是否为纯数字的节奏行
bool isNumeric(std::string const &str){
	return regex_match(str,std::regex("[0-9]+"));
}

class MusicPlayer{
private:
	//midi输出句柄
	HMIDIOUT handle;

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


	//互斥锁，保护多线程共享资源
	std::mutex mu;

	int tick1,tick2;
	MusicDeserializer musicDeserializer;
	void play(std::string s1,std::string s2="",double duration = 500){
		STOP=0;tick1=0;tick2=0;
		//设置曲子线程，分别对应两段旋律
		std::thread tune1(&MusicPlayer::playSingleLine,this,musicDeserializer.deserialize(s1,duration),1);tune1.detach();
		std::thread tune2(&MusicPlayer::playSingleLine,this,musicDeserializer.deserialize(s2,duration),0);tune2.detach();
		while (STOP<2);
		if (DEBUG){
			if (tick1==tick2) puts("Succ");
			else printf("Warn: %d!=%d\n",tick1,tick2);			
		}
	}
	
	void classifyList(MusicManager &m){
		ENDMUSIC=0;
		double duration = 500;
		for (int i=0;i<m.currentMusicList.size() && !ENDMUSIC;++i){
			
			//解析节奏行，跳过空行
			while (i<m.currentMusicList.size() && (m.currentMusicList.findAt(i) == "" || isNumeric(m.currentMusicList.findAt(i)))){
				if (isNumeric(m.currentMusicList.findAt(i))){
					duration = stod(m.currentMusicList.findAt(i));
				}
				i++;
			}
			//结束循环，防止越界
			if (i>=m.currentMusicList.size()) break;
			//获取当前行存储至至s1，如果下一行也不为空，解析并存储至s2；s1和s2分别为两段旋律
			std::string s1=m.currentMusicList.findAt(i),s2="";
			if (i<m.currentMusicList.size()-1 && m.currentMusicList.findAt(i+1)!="") s2=m.currentMusicList.findAt(i+1),i++;
			//播放s1和s2
			play(s1,s2,duration);
			// duration = 500;
		}
	}

	void playSingleLine(std::vector<NoteList> noteTable, bool isMain){
		//获取播放开始的时间戳
		long start = clock();
		//定义记录时间戳，跟随进度
		double tick = 0;
		//遍历Note中的所有和弦的音符，发送至midi统一播放
		for(NoteList noteList : noteTable){
			std::vector<int> noteBuffer = noteList.transformNoteBuffer();
			for(int noteInfo : noteBuffer){
				if(noteInfo != 0){
					midiOutShortMsg(handle, noteInfo);
				}
			}
			noteBuffer.clear();
			// std::string tune = isMain ? "tune1" : "tune2";
			// std::cout << "duration " + tune + "\t" << tick + noteList.getDuration() + '\n'<<std::endl;
			//设置音符播放持续时间
			while ((clock() - start) * 1.0 < tick + noteList.getDuration());
			//更新进度条
			tick += noteList.getDuration(); 
		}
		//如果本行是主旋律，则本行的时间戳为主时间戳
		if (isMain) tick1=tick;else tick2=tick;
		//线程锁，等待下一个线程就绪
		mu.lock();
		//两个线程就绪（stop=2），开始解锁
		STOP++;
		mu.unlock();
	}
};
class BGM{
public:
	MusicPlayer player;
	MusicManager nowList;
	BGM(std::string name){
		nowList.load(name);
	}
	~BGM(){
		player.ENDMUSIC=1;
	}
	void setMusic(std::string name){
		nowList.load(name);
	}
	void play_thread(){
		while (1){
			player.classifyList(nowList);
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

