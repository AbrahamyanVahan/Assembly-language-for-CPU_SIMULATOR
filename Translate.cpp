
#include "Translate.h"

Translate::Translate(std::string fileName) :source_1{ "0000" }, source_2{ "0000" }, dest_or_jmp{ "0000" }, command{ "00000" }
{
	//set comands names
	this->set_commands_s();
	this->set_commands_C();
	this->set_Reg_R_numbers();
	this->set_Reg_r_numbers();
	std::fstream read;
	read.open(fileName, std::fstream::in);
	if (!read.is_open())
	{
		std::cout << "file isnt open" << std::endl;
	}
	else
	{
		std::string line;
		int line_number = 0;
		std::string temp_cmd;
		while (!read.eof())
		{
			std::getline(read, line);
			//  check line empty or not
			std::string line_1;
			for (int i = 0; i < line.size(); i++)
			{
				if (line[i] != ' ' && line[i] != '\t')
				{
					line_1 += line[i];

				}
				else
				{
					continue;
				}

			}
			int empty_line = line_1.size();
			if (empty_line == 0)
			{
				++line_number;
				continue;
			}
			else
			{
				if (empty_line >= 2)
				{
					//    it is a comment
					if (line_1[0] == '/' && line_1[1] == '/')
					{
						++line_number;
						continue;
					}
					else
					{
						int error_code = Read(line_1, line_number);
						++line_number;
						if (error_code == 1)
						{
							if (source_1 == "00001111" && source_2 == "00001111")
							{
								std::cout << " only source_1 or source_2 can be BX " << std::endl;
								break;
							}

							temp_cmd += this->command;
							temp_cmd += this->source_1;
							temp_cmd += this->source_2;
							temp_cmd += this->dest_or_jmp;
							this->cmd += temp_cmd;
							this->cmd += "\n";

							temp_cmd = "";
							// overrideing 
							this->source_1 = "0000";
							this->source_2 = "0000";
							this->dest_or_jmp = "0000";
							this->command = "00000";
							continue;
						}
						else
						{
							decode_errors(error_code, line_number);
							break;
						}
					}
				}
				else
				{
					std::cout << "error in line  " << line_number << std::endl;
					break;
				}
			}
		}
	}
	read.close();
}


int Translate::Read(const std::string& line, int& line_number)
{
	int line_size = line.size();

	if (line_size == 0)
	{
		return 10;    // empty line
	}
	else
	{
		int error_code = correct_line_or_not(line);
		if (error_code == 1)
		{
			return 1;  //  everything ok
		}
		else
		{
			return error_code;
		}
	}

}

int Translate::correct_line_or_not(const std::string& line)
{
	if (line[line.size() - 1] != ';')
	{
		//  ;  not found
		return 2;
	}
	bool is_double_dot = false;       //  :
	int storaket = 0;
	std::string temp_line;
	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			//  command type
			if (line[i] == ':' && is_double_dot == false)
			{
				if (this->decode_commands_first_byte(line) == 1)
				{
					is_double_dot = true;
					temp_line += line[i];
					continue;
				}
				else
				{
					// command not found

					return 3;
				}

			}
			//  if command is copy or move count of , must be 1 in command
			else if (line[i] == ',' && is_double_dot == true)
			{
				++storaket;
			}
			temp_line += line[i];
		}
		else
		{
			continue;
		}
	}
	// implementation error
	if (storaket == 0 || storaket >= 3)
	{
		return 4;
	}
	else if (is_double_dot == false)
	{
		return 5;
	}
	// 
	int index = 0;
	int error_code_s1 = correct_source_1(temp_line, index);
	if (error_code_s1 == 1)
	{
		if (storaket == 2)
		{
			int error_code_s2 = correct_source_2(temp_line, index);
			if (error_code_s2 == 1)
			{
				int error_code_dst = correct_destination(temp_line, index);
				if (error_code_dst == 1)
				{
					return 1;
				}
				else
				{
					return error_code_dst;
				}
			}
			else
			{
				return error_code_s2;
			}
		}
		//    Copy or move   only sourc1  and destination
		else if (storaket == 1)
		{
			//   source 2 is immidate value 0
			this->command[1] = '1';
			int error_code_dst = correct_destination(temp_line, index);
			if (error_code_dst == 1)
			{
				return 1;
			}
			else
			{
				return error_code_dst;
			}
		}
	}
	else
	{
		return error_code_s1;
	}
}
// decoders
int Translate::decode_commands_first_byte(const std::string& line)
{
	int i = 0;
	std::string command;
	while (line[i] != ':')
	{
		command += line[i];
		++i;
	}

	auto iterat = this->command_id.find(command);
	if (iterat != this->command_id.end())
	{
		if (iterat->second.first == "ALU")
		{
			this->command[4] == '0';
			this->command += iterat->second.second;
			//   copy and mov equal OR with 0
			if (iterat->first == "copy" || iterat->first == "COPY" || iterat->first == "mov"|| iterat->first == "MOV")
			{
				this->count_comma = 1;
				//  source2 is ia immidiate
				this->command[1] = '1';
				//
				this->source_2 = "00000000";
				return 1;

			}
			else
			{
				return 1;
			}

		}
		else if (iterat->second.first == "CONDITION")
		{
			this->command[4] = '1';
			this->command += iterat->second.second;
			this->command[4] = '1';
			return 1;
		}

	}
	//  if command not found
	else
	{
		return 3;
	}

}
int Translate::correct_source_2(const std::string& line, int& index_point)
{

	std::string temp_source_2;
	while (line[index_point] != ',')
	{
		temp_source_2 += line[index_point];
		++index_point;
	}
	++index_point;
	auto iter = this->reg_id_R.find(temp_source_2);
	auto iter1 = this->reg_id_r.find(temp_source_2);
	if (iter != this->reg_id_R.end())
	{
		this->command[1] = '0';
		this->source_2 += iter->second;
		return 1;
	}
	else if (iter1 != this->reg_id_r.end())
	{
		this->command[1] = '0';     // it is a register  
		this->source_1 += iter->second;
		return 1;
	}
	else
	{
		std::string str1 = temp_source_2;
		bool is_number = is_Number_or_not(str1);

		if (is_number)
		{
			this->command[1] = '1';     //  it is a immidiate value   
			int num = std::stoi(str1);
			if (num >= 0 && num <= 255)
			{
				//  convert to biinary 
				int binary = decode_dec_to_binary(num);
				std::string bn = std::to_string(binary);
				if (bn.size() <= 8)
				{
					int interval = 8 - bn.size();
					std::string bn1;
					for (int i = 0; i < interval; i++)
					{
						bn1 += '0';
					}
					bn1 += bn;
					this->source_2 = bn1;
					return 1;

				}
				else

				{
					return 1;
				}


			}
			else
			{
				return 7;    //    number size is greater then 255 
			}
		}
		else
		{
			return 6;
		}
	}
}
int Translate::correct_source_1(const std::string& line, int& index_point)
{
	// index where comamnd ends
	int index_c = 0;
	int k = 0;
	while (line[k] != ':')
	{
		++index_c;
		++k;
	}
	++index_c;
	// read source 1
	std::string temp_source_1;
	int a = index_c;
	while (line[a] != ',')
	{
		temp_source_1 += line[a];
		++a;
	}
	++a;
	index_point = a;
	//   decode source 1  
	auto iter = this->reg_id_R.find(temp_source_1);
	auto iter1 = this->reg_id_r.find(temp_source_1);
	if (iter != this->reg_id_R.end())
	{


		this->command[0] = '0';   //  it is a register
		this->source_1 += iter->second;
		return 1;
	}
	else if (iter1 != this->reg_id_r.end())
	{
		this->command[0] = '0';     // it is a register  
		this->source_1 += iter->second;
		return 1;
	}
	else
	{
		std::string str1 = temp_source_1;
		bool is_number = is_Number_or_not(str1);

		if (is_number)
		{
			this->command[0] = '1';     //  it is a immidiate value   
			int num = std::stoi(str1);
			if (num >= 0 && num <= 255)
			{
				//  convert to biinary 
				int binary = decode_dec_to_binary(num);
				std::string bn = std::to_string(binary);
				if (bn.size() <= 8)
				{
					int interval = 8 - bn.size();
					std::string bn1;
					for (int i = 0; i < interval; i++)
					{
						bn1 += '0';
					}
					bn1 += bn;
					this->source_1 = bn1;
					return 1;

				}
				else

				{
					this->source_1 = bn;
					return 1;
				}


			}
			else
			{
				return 7;    //    number size is greater then 255 
			}
		}
		else
		{
			return 6;
		}
	}
	return 222;
}
int Translate::correct_destination(const std::string& line, int& index_point)
{
	//   instructon for ALU
	if (this->command[4] == '0')
	{
		std::string dstn;
		int index = index_point;
		while (line[index] != ';')
		{
			dstn += line[index];
			++index;
		}
		auto iter = this->reg_id_R.find(dstn);
		//   destn is a Reg or if it BX it is a RAM memory addres 255 byte
		if (iter != this->reg_id_R.end())
		{
			this->dest_or_jmp += iter->second;
			return 1;
		}
		else
		{
			//  error destination must be register
			return 8;
		}
	}
	//   instrocution for condition --  jump-- if true // Counter number  
	else if (this->command[4] == '1')
	{
		std::string jmp;
		int index = index_point;
		while (line[index] != ';')
		{
			jmp += line[index];
			++index;
		}
		bool is_number = this->is_Number_or_not(jmp);
		if (is_number == true)
		{
			//convert to int
			int num = std::stoi(jmp);
			int num_bin = this->decode_dec_to_binary(num);
			std::string num_bin_str = std::to_string(num_bin);
			int interval = 8 - num_bin_str.size();
			if (interval == 0)
			{
				this->dest_or_jmp = num_bin_str;
				return 1;
			}
			else if (interval < 8)
			{
				std::string tmp_str;
				for (int i = 0; i < interval; i++)
				{
					tmp_str += '0';
				}
				tmp_str += num_bin_str;
				this->dest_or_jmp = tmp_str;
				return 1;
			}

		}
		else
		{
			return 9;
		}
	}
}
void Translate::decode_errors(int error_code, int line_number)
{
	switch (error_code)
	{
	case 2:
		std::cout << "; not found in line in line  " << line_number << std::endl;
		break;
	case 3:
		std::cout << "command not found in line  " << line_number << std::endl;
		break;
	case 4:
		std::cout << "wrong count of , in line " << line_number << std::endl;
		break;
	case 5:
		std::cout << "after command must be : in line " << line_number << std::endl;
		break;
	case 6:
		std::cout << "source_1 or source_2 not found in line " << line_number << std::endl;
		break;
	case 7:
		std::cout << "immidiate value must be less 255 and greater 0 in line " << line_number << std::endl;
		break;
	case 8:
		std::cout << "desination must be number name of register  in line " << line_number << std::endl;
		break;
	case 9:
		std::cout << "jump addres must be number of line where you want jump\njump address not found it must be number of reg_counter in line-" << line_number << std::endl;
		break;
	case 10:
		std::cout << "empty line in line " << line_number << std::endl;
		break;
	default:
		break;
	}
}
//
std::string Translate::get_commands() 
{
	return this->cmd;
}
//   write in file
int Translate::write()
{
std:std::fstream write;
	std::string file_name = "BINARY_OUTPUT.txt";
	write.open(file_name, std::ios::out);
	if (!write.is_open())
	{
		std::cout << "BINARY_OUTPUT.txt  file isnt open " << std::endl;
		return -1;
	}
	else
	{
		write << this->cmd;
		write.close();
		return 1;
	}

}


//  
bool Translate::is_Number_or_not(const std::string& line)
{
	for (char const& c : line) {
		if (std::isdigit(c) == 0)
		{
			return false;
		}

	}
	return true;
}
int Translate::decode_dec_to_binary(int number)
{
	int  bin = 0, num_rem, x = 1;

	while (number != 0)
	{
		num_rem = number % 2;
		bin = bin + (num_rem * x);
		number = number / 2;
		x *= 10;
	}
	return bin;
}
// commands and regs names
void Translate::set_commands_s()
{
	// ALU commands  //  11
	this->command_id.emplace("add", std::make_pair("ALU", "000"));
	this->command_id.emplace("or", std::make_pair("ALU", "001"));
	this->command_id.emplace("not", std::make_pair("ALU", "010"));
	this->command_id.emplace("and", std::make_pair("ALU", "100"));
	this->command_id.emplace("sub", std::make_pair("ALU", "011"));
	this->command_id.emplace("mult", std::make_pair("ALU", "110"));
	this->command_id.emplace("copy", std::make_pair("ALU", "001"));
	this->command_id.emplace("mov", std::make_pair("ALU", "001"));
	this->command_id.emplace("shl", std::make_pair("ALU", "101"));
	this->command_id.emplace("shr", std::make_pair("ALU", "111"));
	//   condition comands
	this->command_id.emplace("je", std::make_pair("CONDITION", "000"));             //  jump when equal  
	this->command_id.emplace("jne", std::make_pair("CONDITION", "001"));            //  jump when not equal 
	this->command_id.emplace("jg", std::make_pair("CONDITION", "010"));             //  jump when greater then
	this->command_id.emplace("jge", std::make_pair("CONDITION", "100"));            //  jump when greater then or equal
	this->command_id.emplace("jl", std::make_pair("CONDITION", "011"));             //  jump when less then
	this->command_id.emplace("jle", std::make_pair("CONDITION", "110"));            //  jump when less equal 

}
void Translate::set_commands_C()
{
	// ALU commands  //  11
	this->command_id.emplace("ADD", std::make_pair("ALU", "000"));
	this->command_id.emplace("OR", std::make_pair("ALU", "001"));
	this->command_id.emplace("NOT", std::make_pair("ALU", "010"));
	this->command_id.emplace("AND", std::make_pair("ALU", "100"));
	this->command_id.emplace("SUB", std::make_pair("ALU", "011"));
	this->command_id.emplace("MULT", std::make_pair("ALU", "110"));
	this->command_id.emplace("COPY", std::make_pair("ALU", "001"));
	this->command_id.emplace("MOV", std::make_pair("ALU", "001"));
	this->command_id.emplace("SHL", std::make_pair("ALU", "101"));
	this->command_id.emplace("SHR", std::make_pair("ALU", "111"));
	//   condition comands
	this->command_id.emplace("JE", std::make_pair("CONDITION", "000"));             //  jump when equal  
	this->command_id.emplace("JNE", std::make_pair("CONDITION", "001"));            //  jump when not equal 
	this->command_id.emplace("JG", std::make_pair("CONDITION", "010"));             //  jump when greater then
	this->command_id.emplace("JGE", std::make_pair("CONDITION", "100"));            //  jump when greater then or equal
	this->command_id.emplace("JL", std::make_pair("CONDITION", "011"));             //  jump when less then
	this->command_id.emplace("JLE", std::make_pair("CONDITION", "110"));            //  jump when less equal 

}
//  Regs
void Translate::set_Reg_R_numbers()
{
	this->reg_id_R.emplace("R0", "0000");
	this->reg_id_R.emplace("R1", "0001");
	this->reg_id_R.emplace("R2", "0010");
	this->reg_id_R.emplace("R3", "0011");
	this->reg_id_R.emplace("R4", "0100");
	this->reg_id_R.emplace("R5", "0101");
	this->reg_id_R.emplace("R6", "0110");
	this->reg_id_R.emplace("R7", "0111");
	this->reg_id_R.emplace("R8", "1000");
	this->reg_id_R.emplace("R9", "1001");
	this->reg_id_R.emplace("R10", "1010");
	this->reg_id_R.emplace("R11", "1011");
	this->reg_id_R.emplace("R12", "1100");
	this->reg_id_R.emplace("R13", "1101");
	this->reg_id_R.emplace("RM", "1110");
	this->reg_id_R.emplace("BX", "1111");

}
void Translate::set_Reg_r_numbers()
{
	this->reg_id_R.emplace("r0", "0000");
	this->reg_id_R.emplace("r1", "0001");
	this->reg_id_R.emplace("r2", "0010");
	this->reg_id_R.emplace("r3", "0011");
	this->reg_id_R.emplace("r4", "0100");
	this->reg_id_R.emplace("r5", "0101");
	this->reg_id_R.emplace("r6", "0110");
	this->reg_id_R.emplace("r7", "0111");
	this->reg_id_R.emplace("r8", "1000");
	this->reg_id_R.emplace("r9", "1001");
	this->reg_id_R.emplace("r10", "1010");
	this->reg_id_R.emplace("r11", "1011");
	this->reg_id_R.emplace("r12", "1100");
	this->reg_id_R.emplace("r13", "1101");
	this->reg_id_R.emplace("rm", "1110");
	this->reg_id_R.emplace("bx", "1111");
}