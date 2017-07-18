#ifndef _PARSER_H
#define _PARSER_H

#include <cstdint>
#include <string>
#include <regex>
#include <unordered_map>
#include <fstream>

#include <iostream>

#include "utils.h"
#include "symboltlb.h"

class parser
{
    using opcode = std::unordered_map<std::string, std::string>;

    std::ifstream &_file;
    symbol_table _table;

    const opcode DEST {
        { "M",    "001" },
        { "D",    "010" },
        { "MD",   "011" },
        { "A",    "100" },
        { "AM",   "101" },
        { "AD",   "110" },
        { "AMD",  "111" }
    };

    const opcode JUMP {
        { "JGT", "001" },
        { "JEQ", "010" },
        { "JGE", "011" },
        { "JLT", "100" },
        { "JNE", "101" },
        { "JLE", "110" },
        { "JMP", "111" }
    };

    const opcode OPCODE {
        { "0",   "101010" },
        { "1",   "111111" },
        { "-1",  "111010" },
        { "D",   "001100" },
        { "A",   "110000" },
        { "M",   "110000" },
        { "!D",  "001101" },
        { "!A",  "110001" },
        { "!M",  "110001" },
        { "-D",  "001111" },
        { "-A",  "110011" },
        { "-M",  "110011" },
        { "D+1", "011111" },
        { "A+1", "110111" },
        { "M+1", "110111" },
        { "D-1", "001110" },
        { "A-1", "110010" },
        { "M-1", "110010" },
        { "D+A", "000010" },
        { "D+M", "000010" },
        { "D-A", "010011" },
        { "D-M", "010011" },
        { "A-D", "000111" },
        { "M-D", "000111" },
        { "D&A", "000000" },
        { "D&M", "000000" },
        { "D|A", "010101" },
        { "D|M", "010101" },
    };

    uint16_t get_value(const std::string &line, size_t &current)
    {
        bool is_symbol = false;
        std::string token;
        while (current < line.size()) {
            if (!::isdigit(line[current])) {
                is_symbol = true;
            }
            token += line[current++];
        }

        if (is_symbol) {
            if (_table.has_key(token)) {
                return _table.get(token);
            }
            auto val = _table.available_spot();;
            _table.add(token, val);
            return val;
        }
        return std::stoi(token);
    }

    std::string get_dest(const std::string &token)
    {
        auto it = DEST.find(token);;
        if (it != DEST.end()) {
            return it->second;
        }
        std::cerr << "ERR: " << token << " invalid destination\n";
        return "000";
    }

    std::string get_jump(const std::string &token)
    {
        auto it = JUMP.find(token);;
        if (it != JUMP.end()) {
            return it->second;
        }
        std::cerr << "ERR: " << token << " invalid jump\n";
        return "000";
    }

    std::string get_opcode(const std::string &token, bool &is_mem)
    {
        auto it = OPCODE.find(token);;
        if (it != OPCODE.end()) {
            for (auto c : token) {
                if (c == 'M') {
                    is_mem = true;
                }
            }
            return it->second;
        }
        std::cerr << "ERR: " << token << " invalid opcode\n";
        return "000000";
    }

    char look_after(const std::string &line, size_t current)
    {
        if (current + 1 < line.size()) {
            return line[current + 1];
        }
        return '\n';
    }

    public:
    parser(std::ifstream &file) :
        _file(file)
    {
    }

    void get_symbols()
    {
        std::string line;
        size_t lineno = 0;

        while (getline(_file, line)) {
            bool found_symbol = false;
            std::string name;
            bool skip = false;

            line.erase(line.size() - 1, 1);

            if (line.empty()) {
                continue;
            }

            for (size_t i = 0; i <= line.size(); ++i) {
                // ignore empty spaces
                if (::isspace(line[i])) {
                    continue;
                }

                if (line[i] == '/' &&
                    look_after(line, i) == '/') {
                    skip = true;
                    break;
                }

                if (line[i] == '(') {
                    found_symbol = true;
                    continue;
                }

                if (line[i] == ')') {
                    if (!found_symbol) {
                        std::cerr << "Parse error: unexpected )\n";
                        return;
                    }
                    _table.add(name, lineno);
                    skip = true;
                    break;
                }

                if (!found_symbol) {
                    continue;
                }
                name += line[i];
            }

            if (!skip) {
                ++lineno;
                skip = false;
            }
        }
        _file.clear();
        _file.seekg(std::ios_base::beg);
    }

    void parse()
    {
        std::string line;

        while (getline(_file, line)) {
            std::string token;
            std::string bin;
            std::string dest = "000";
            std::string jmp = "000";
            std::string opcode = "000";
            bool is_memory_op = false;
            bool has_jmp = false;
            int inst_type = 0;

            line.erase(line.size() - 1, 1);

            if (line.empty()) {
                continue;
            }

            for (size_t current = 0; current <= line.size(); ++current) {
                // ignore empty spaces
                if (::isspace(line[current])) {
                    continue;
                }

                if (line[current] == '/' &&
                    look_after(line, current) == '/') {
                    break;
                }

                if (line[current] == '(') {
                    break;
                }

                if (line[current] == '@') {
                    current++;
                    bin = int_to_binary(get_value(line, current));
                    bin[0] = '0';
                    inst_type = 1;
                    break;
                }

                if (line[current] == '=') {
                    dest = get_dest(token);
                    token = "";
                    continue;
                }

                if (line[current] == ';') {
                    has_jmp = true;
                    opcode = get_opcode(token, is_memory_op);
                    inst_type = 2;
                    token = "";
                    continue;
                }

                if (current == line.size()) {
                    if (has_jmp) {
                        jmp = get_jump(token);
                        token = "";
                        continue;
                    }
                    else {
                        opcode = get_opcode(token, is_memory_op);
                        inst_type = 2;
                        token = "";
                        continue;
                    }
                }
                token += line[current];
            }

            if (inst_type == 2) {
                bin = "1110";
                if (is_memory_op) {
                    bin = "1111";
                }
                bin += opcode + dest + jmp;
                std::cout << bin << std::endl;
            }
            else if (inst_type == 1) {
                std::cout << bin << std::endl;
            }
        }
    }

    uint16_t parse_number(const std::string &line)
    {
        return 0;
    }
};

#endif
