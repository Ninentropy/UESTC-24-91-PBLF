#pragma once  // 防止头文件被多次包含
#include <qlogging.h>
#include <string.h>   // 字符串操作
#include <sstream>
#include "NoteList.h"
#include <QDebug>
class MusicDeserializer{
	private:
	enum scale{
		//休止符
		Rest=0,
		//不同音调的音符，与midi音符编号对应
		B7=107,A7s=106,A7=105,G7s=104,G7=103,F7s=102,F7=101,E7=100,D7s=99, D7=98, C7s=97, C7=96,
		B6=95, A6s=94, A6=93, G6s=92, G6=91, F6s=90, F6=89, E6=88, D6s=87, D6=86, C6s=85, C6=84,
		B5=83, A5s=82, A5=81, G5s=80, G5=79, F5s=78, F5=77, E5=76, D5s=75, D5=74, C5s=73, C5=72,
		B4=71, A4s=70, A4=69, G4s=68, G4=67, F4s=66, F4=65, E4=64, D4s=63, D4=62, C4s=61, C4=60,
		B3=59, A3s=58, A3=57, G3s=56, G3=55, F3s=54, F3=53, E3=52, D3s=51, D3=50, C3s=49, C3=48,
		B2=47, A2s=46, A2=45, G2s=44, G2=43, F2s=42, F2=41, E2=40, D2s=39, D2=38, C2s=37, C2=36,
		B1=35, A1s=34, A1=33, G1s=32, G1=31, F1s=30, F1=29, E1=28, D1s=27, D1=26, C1s=25, C1=24,
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
							 
	public:
	std::vector<NoteList> deserialize(const std::string& line,double defaultDuration) {
		std::vector <NoteList>noteTable;
		std::istringstream iss(line);
		//token为以空行为单位分割的字符串
		std::string token;
		//目标字符，主要是用于匹配节奏字符
		std::string targetChars = "?*&%.-";
		
		while (iss >> token){
			// std::cout << "Current token: " << token << std::endl; // 调试输出
			// std::cout << "iss.eof(): " << iss.eof() 
			// 	<< ", iss.fail(): " << iss.fail() 
			// 	<< ", iss.bad(): " << iss.bad() << std::endl;
			double duration = defaultDuration;
			NoteList currentList;
			int vol = 0x7f;
			int pitch = Rest;
			//忽略分节符
			if(token == "|") continue;
			//while循环，结束条件是字符串末尾不再包含节奏字符
			while (targetChars.find(token.back()) != std::string::npos){
				// std::cout << "Token inside while: " << token << std::endl;
				//读取最后一个字符
				char c = token.back();
				//匹配并处理
				switch (c){
					case '?':{
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
						duration += defaultDuration;
						break;
					}
				}
				//删除最后一个字符
				token.pop_back();
			}
			//设置持续时间
			currentList.setDuration(duration);
			//重置为节奏行定义的时间，如果不重置的话，duration会递减至0
			duration = defaultDuration;
			if (token.front() == '[' && token.back() == ']') {
				// 解析和弦，去掉中括号
				std::string chordNotes = token.substr(1, token.size() - 2);
				//剩下的代码就全部都是解析字符串，并存储在链表中
				int lastIndex = chordNotes.size();
				int index = 0;
				for (int i = 1; i < chordNotes.size(); i++)
				{
					if (chordNotes[i] >= '1' && chordNotes[i] <= '7'){
						std::string str = chordNotes.substr(index,i - index);
						Note note = parseNote(str);
						currentList.append(note);
						// currentList.display();
						index = i;
						str.clear();
					}
					if (i == chordNotes.size() - 1)
					{
						std::string str = chordNotes.substr(index,chordNotes.size() - index);
						Note note = parseNote(str);
						currentList.append(note);
						// currentList.display();
					}
				}
			} else {
				// 解析单独的音符
				Note singleNote = parseNote(token);
				currentList.append(singleNote);
			}
			// std::cout << "pass" << std::endl;
			//最后将链表存储在动态数组，成为一个map
			noteTable.push_back(currentList);
		}
		// std::cout << "helloworld while\n";
		return noteTable;
	}

	Note parseNote(const std::string& noteStr) {
		// duration = 500;
		int vol = 0x7f;
		int pitch = Rest;
		int size = noteStr.size();
		int lvl = 3;//音高（也是确定数组下标）
		bool isSharp = 0;
		int x = 0;//转换后的数组下标
		
		//遍历字符串
		for (int i = 0; i < size; i++){
			char c = noteStr[i];
			switch (c){
				case '0':{
					return Note(Rest,vol);
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
					if (!(c >= '1' && c <= '7')) {
    					std::cerr << "Invalid character: " << c << std::endl;
					}
					x = c - 49;
					// std::cout << "add:" << c - 49; 
					break;
				}
			}
		}
		//三目运算符进行赋值
		pitch = (isSharp  == 0 ? C_Scale[lvl][x] : C_Scale_s[lvl][x]);
		return Note(pitch,vol);
	}
};

class MusicSerializer{
	public:
    std::string serialize(std::vector<NoteList> noteTable,double defaultDuration){
        std::string result = "";
        for(NoteList noteList : noteTable){
            std::string noteStr;
            double duration = noteList.getDuration();
            if (noteList.size() > 1){
                for (int i = 0; i < noteList.size(); i++)
                {
                    Note note = noteList.findAt(i);
                    noteStr += parseString(note);

                }
                result.append("[" + noteStr + "]");
            }else{
                result.append(parseString(noteList.findAt(0)));
            }
            duration = noteList.getDuration();
            // 如果duration不是默认值，则添加相应的修饰符
            if (duration != defaultDuration) {
                if (duration > defaultDuration) {
                    // 计算-的数量
                    int additions = static_cast<int>((duration - defaultDuration) / defaultDuration);
                    result.append(additions, '-'); // 用'-'表示增加的时长
                } else {
                    double ratio = defaultDuration / duration;
                    while (ratio >= 1.9) {
                        result.append("?"); // 用'?'表示一半的时长
                        ratio /= 2;
                        qDebug() <<"fuck";
                    }
                }
            }
            result.append(" ");
        }
        return result;
	}
	std::string parseString(Note note){
		std::string str;
		int pitch = note.pitch;
		int basePitch = 60; // C4 
		//八度偏移
		int octaveOffset = (pitch - basePitch) / 12;
		//音符偏移
		int noteOffset = (pitch - basePitch) % 12;

		// 将 MIDI 偏移映射到击键音节，将 C 视为 1 (do)
		const std::vector<std::string> noteChars{"", "1", "2", "3", "4", "5", "6", "7"};

		if(pitch == 0) return "0";
		if (noteOffset < 0) {
			noteOffset += 12;
			octaveOffset--;
		}
		// 顺序为: C, C#, D, D#, E, F, F#, G, G#, A, A#, B
		// 将其映射到: 1, 1#, 2, 2#, 3, 4, 4#, 5, 5#, 6, 6#, 7
		switch (noteOffset) {
			case 0: str = "1"; break;   // C
			case 1: str = "1#"; break;  // C#
			case 2: str = "2"; break;   // D
			case 3: str = "2#"; break;  // D#
			case 4: str = "3"; break;   // E
			case 5: str = "4"; break;   // F
			case 6: str = "4#"; break;  // F#
			case 7: str = "5"; break;   // G
			case 8: str = "5#"; break;  // G#
			case 9: str = "6"; break;   // A
			case 10: str = "6#"; break; // A#
			case 11: str = "7"; break;  // B
		}

		// 根据偏移量添加逗号或 ^
		if (octaveOffset < 0) {
			for (int i = 0; i > octaveOffset; --i) {
				str += ",";
			}
		} else if (octaveOffset > 0) {
			for (int i = 0; i < octaveOffset; ++i) {
				str += "^";
			}
		}
		return str;

	}
};
