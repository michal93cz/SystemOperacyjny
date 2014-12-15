#include "interpreter.h"
#include "string"

bool czy(char i) { if (i == ':')	return true; 
else return false;
}

bool Interpreter::poprawny_rejestr(string name) {
	return name == "A" ||
		name == "B" ||
		name == "C" ||
		name == "D";
}

void Interpreter::interpret_code(string blob) {
	//uzywany kod: nr_operacji,l_parametrow,w_parametrow,parametry

	vector<unsigned int> line_lengths;
	vector<unsigned int> param_lengths;
	//podzial na operacje
	vector<string> lines = split(blob, '|');
	op_count = lines.size();
	total_length = lines.size();
	unsigned int length, line_length;

	for (vector<string>::iterator it = lines.begin(); it != lines.end(); ++it) {
		//podzial operacji na czesci
		vector<string> parts = split(*it, ':');
		line_length = 1 + parts.size();
		//liczenie dl kodu
		for (unsigned int i = 1; i < parts.size(); i++) {
			length = 0;
			if (parts[0] == "OUT") {
				length = parts[i].length() + 1;
			}
			else if (parts[0] == "SET" || parts[0] == "ADD" ||
				parts[0] == "SUB" || parts[0] == "MUL" || parts[0] == "DIV") {

				length = i == 1 || (i == 2 && poprawny_rejestr(parts[i])) ? 1 : sizeof(unsigned int);
			}
			else if (parts[0] == "JUMP" || parts[0] == "JMPZ" || parts[0] == "JPNZ") {
				length = i == 1 && poprawny_rejestr(parts[i]) ? 1 : sizeof(unsigned int);
			}
			param_lengths.push_back(length);
			line_length += length;
		}
		total_length += line_length;
		line_lengths.push_back(line_length);
	}

	buffer = new char[total_length];
	unsigned int c = 0, p = 0;
	OpCode opcode;
	int jmp_ON = 0;
	vector<string>jmp_wr;
	//wpisywanie kodu do tablicy char
	for (vector<string>::size_type i = 0; i != lines.size(); i++) {
		vector<string> parts = split(lines[i], ':');
		string op = parts[0];
		if (op == "SET") {
			opcode = OpCode::SET;
		}
		else if (op == "ADD") {
			opcode = OpCode::ADD;
		}
		else if (op == "SUB") {
			opcode = OpCode::SUB;
		}
		else if (op == "MUL") {
			opcode = OpCode::MUL;
		}
		else if (op == "DIV") {
			opcode = OpCode::DIV;
		}
		else if (op == "JUMP") {
			opcode = OpCode::JUMP;
		}
		else if (op == "JMPZ") {
			opcode = OpCode::JMPZ;
		}
		else if (op == "JPNZ") {
			opcode = OpCode::JPNZ;
		}
		else if (op == "OUT") {
			opcode = OpCode::OUT;
		}
		else if (op == "BYE") {
			opcode = OpCode::BYE;
		}
		if (op != ""){
			string temp2 = "OUT";
			//wpisanie kodu
			buffer[c++] = (char)opcode;
			if (op == temp2){
				if (parts[1] == "0"){
					const char* tmp = parts[1].c_str();
					buffer[c++] = *tmp;
					buffer[c++] = parts[2].length();
					tmp = parts[2].c_str();
					memcpy(buffer + c, tmp, parts[2].length() + 1);
					c += parts[2].length() + 1;
				}
				else{
					const char* tmp = parts[1].c_str();
					buffer[c++] = *tmp;
				}
			}
			else if (op == "JUMP" )
			{
				unsigned int target = atoi(parts[1].c_str()) - 1;
				if (target != 0)
				{
					int tmp = 0;
					for (int j = 0; j <target; j++)
					{
						if (lines[j][0] == 'O')
						{
							if (lines[j][4] != '0')tmp += 2;
							else tmp += lines[j].length() - 3;
						}
						else if (lines[j][0] == 'J')
						{
							if (lines[j][1] == 'U'){
								if (jmp_ON != 0)
									tmp += jmp_wr[jmp_ON - 1].length() + 1;
								else
									tmp += 2;
								jmp_ON++;
								temp2 = to_string(tmp);
								jmp_wr.push_back(temp2);
							}
							else
							{
								if (jmp_ON != 0)
									tmp += jmp_wr[jmp_ON - 1].length() + 1;
								else
									tmp += lines[j].length() - 4;
								jmp_ON++;
								temp2 = to_string(tmp);
								jmp_wr.push_back(temp2);								
							}
						}
						else tmp += lines[j].length() - 4;
					}
					string b = to_string(tmp);
					for (int i = 0; i < b.length(); i++)
						buffer[c++] = b[i];
				}
				else
					buffer[c++] = '0';
			}
			else if (op == "JMPZ" || op == "JPNZ")
			{
				const char* tmp = parts[1].c_str();
				buffer[c++] = *tmp;
				unsigned int target = atoi(parts[2].c_str()) - 1;
				if (target != 0)
				{
					int tmp = 0;
					for (int j = 0; j <target; j++)
					{
						if (lines[j][0] == 'O')
						{
							if (lines[j][4] != '0')tmp += 2;
							else tmp += lines[j].length() - 3;
						}
						else if (lines[j][0] == 'J')
						{
							if (lines[j][1] == 'U'){
								if (jmp_ON != 0)
									tmp += jmp_wr[jmp_ON - 1].length() + 1;
								else
									tmp += 2;
								jmp_ON++;
								temp2 = to_string(tmp);
								jmp_wr.push_back(temp2);
							}
							else
							{
								if (jmp_ON != 0)
									tmp += jmp_wr[jmp_ON - 1].length() + 1;
								else
									tmp += lines[j].length() - 4;
								jmp_ON++;
								temp2 = to_string(tmp);
								jmp_wr.push_back(temp2);
							}
						}
						else tmp += lines[j].length() - 4;
					}
					string b = to_string(tmp);
					for (int i = 0; i < b.length(); i++)
						buffer[c++] = b[i];
				}
				else
					buffer[c++] = '0';
			}
			else if (op == "BYE")
			{
			}
			else{
				const char* tmp = parts[1].c_str();
				buffer[c++] = *tmp;
				tmp = parts[2].c_str();
				memccpy(c+buffer, tmp, c, parts[2].length());
				c =c+ parts[2].length();
			}
		}
	}
}