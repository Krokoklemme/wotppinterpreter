#include <iostream>
#include <fstream>
#include <vector>
#include <array>

int temp_var = 0;
uint8_t func_ptr_index = 0, cells_x = 0, cells_y = 0;
std::array<std::array<int32_t, 256>, 256> cells;
std::array<std::vector<char>, 256> functions;

void execute(const std::vector<char>& code) {
	for (auto i = 0u; i < code.size(); i++) {
		switch (code[i])
		{
		case '{': { // defines a function and save it to the index currently pointed to by 'func_ptr_index'
			i++;

			std::vector<char> new_func(0);

			while (code[i] != '}') {
				new_func.push_back(code[i]);
				i++;
			}

			functions[func_ptr_index] = new_func;
			break;
		}
		
		case '(': { // Code inside parenthesis is executed if current cell is not zero
			i++;

			std::vector<char> new_func(0);

			while (code[i] != ')') {
				new_func.push_back(code[i]);
				i++;
			}

			if (cells[cells_y][cells_x] != 0)
				execute(new_func);

			break;
		}

		case '/': { // Code inside slashes is executed if current cell is zero
			i++;

			std::vector<char> new_func(0);

			while (code[i] != '\\') {
				new_func.push_back(code[i]);
				i++;
			}

			if (cells[cells_y][cells_x] == 0)
				execute(new_func);

			break;
		}

		case '[': { // Code inside is looped while current cell is > zero
			i++;

			std::vector<char> new_func(0);

			while (code[i] != ']') {
				new_func.push_back(code[i]);
				i++;
			}

			while (cells[cells_y][cells_x] > 0)
				execute(new_func);

			break;
		}

		case '`': { // skips n instructions, whereas n is equal to the value at the currently pointed cell
			i += cells[cells_y][cells_x];
			break;
		}

		case '!': { // Bitwise not the current cell
			cells[cells_y][cells_x] = ~cells[cells_y][cells_x];
			break;
		}

		case '~': { // Set current cell to zero
			cells[cells_y][cells_x] = 0;
			break;
		}

		case '>': { // Move right
			cells_x++;
			break;
		}

		case '<': { // Move left
			cells_x--;
			break;
		}

		case '^': { // Move up
			cells_y--;
			break;
		}

		case 'v': { // Move down
			cells_y++;
			break;
		}

		case '+': { // Add one to current cell
			cells[cells_y][cells_x]++;
			break;
		}

		case '-': { // Subtract one from current cell
			cells[cells_y][cells_x]--;
			break;
		}

		case '*': { // Multiply current cell by two
			cells[cells_y][cells_x] *= 2;
			break;
		}

		case '|': { // Divide current cell by two
			cells[cells_y][cells_x] /= 2;
			break;
		}

		case '%': { // Divide current cell by two and store remainder
			cells[cells_y][cells_x] %= 2;
			break;
		}

		case '@': { // Output raw value
			std::cout << cells[cells_y][cells_x];
			break;
		}

		case '.': { // Ouput ASCII equivalent value
			std::cout << (char)cells[cells_y][cells_x];
			break;
		}

		case ',': { // Input a single char as ASCII value to current cell
			cells[cells_y][cells_x] = getchar();
			break;
		}

		case '?': { // Get temporary variable and copy to current cell
			cells[cells_y][cells_x] = temp_var;
			break;
		}

		case '&': { // Set temporary variable to current cells value
			temp_var = cells[cells_y][cells_x];
			break;
		}

		case ':': { // Move function pointer to the right
			func_ptr_index++;
			break;
		}

		case ';': { // Move function pointer to the left
			func_ptr_index--;
			break;
		}

		case '#': { // Execute currently pointed to function
			execute(functions[func_ptr_index]);
			break;
		}

		case '$': { // Goto cell zero on current row
			cells_x = 0;
			break;
		}

		default: { // Everything else is considered a comment, therefore can be skipped
			break;
		}
		}
	}
}

int main(int argc, char** argv) {
	if (argc == 1) {
		std::cerr << "Usage:\nwot++ <file>" << std::endl;
		return -1;
	}

	std::ifstream file(argv[1]);

	if (!file.is_open()) {
		std::cerr << "Failed to open file \'" << argv[1] << '\'' << std::endl;
		return -1;
	}

	std::vector<char> code((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	execute(code);

	return 0;
}