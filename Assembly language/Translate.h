#pragma once
#include <string>;
#include<iostream>
#include <fstream>
#include <map>
class Translate
{
public:
	Translate(const std::string file_name);
private:
	int Read(const std::string& line, int& line_number);
	//   decoders 
	int correct_source_1(const std::string& line, int& index_point);
	int correct_source_2(const std::string& line, int& index_point);
	int correct_line_or_not(const std::string& line);
	int correct_destination(const std::string& line, int& index_point);
	//
	int decode_commands_first_byte(const std::string& line);
	int decode_dec_to_binary(int num);
	void decode_errors(int error_code, int line_number);
	bool is_Number_or_not(const std::string& line);
	//  Assemby language commands and registors
	void set_commands_C();
	void set_commands_s();
	void set_Reg_R_numbers();
	void set_Reg_r_numbers();
	//   write binary  code in file which name is binary_code.txt
public:
	int write();
	std::string get_commands();
private:
	std::map<std::string, std::pair<std::string, std::string> >  command_id;
	std::map<std::string, std::string > reg_id_R;
	std::map<std::string, std::string > reg_id_r;
	bool alu = false;
	bool cond = false;
	int count_comma = 2;
	std::string command;
	std::string source_1;
	std::string source_2;
	std::string dest_or_jmp;
	std::string cmd;
};
