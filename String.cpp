#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>


class MyString {
private:
	char* string;
public:
	MyString(): string(nullptr) {};

	MyString(const char* new_string) {
		if (new_string) {
			string = new char[strlen(new_string) + 1];
			strcpy(string, new_string);
		}
		else {
			string = nullptr;
		}
	}

	MyString(const MyString& other) {
		if (other.string) {
			string = new char[strlen(other.string) + 1];
			strcpy(string, other.string);
		}
		else {
			string = nullptr;
		}
	}

	MyString(MyString&& other) noexcept {
		string = other.string;
		other.string = nullptr;
	}

/////////////////////////////////////////////////////
/// INITIALIZER_LIST
	MyString(std::initializer_list<char> it) {
		int size = it.size();
		if (!size) {
			string = nullptr;
		}
		else {
			string = new char[size + 1];
			int i = 0;
			for (char c: it) {
				string[i++] = c;
			}
			string[i] = '\0';
		}
	}
///////////////////////////////////////////////////

	void set_string(const char* new_string) {
		delete[] string;
		if (new_string) {
			string = new char[strlen(new_string) + 1];
			strcpy(string, new_string);
		}
		else {
			string = nullptr;
		}
	}


	const char* get_string() const {
		return string;
	}

	int length() const {
		if (string) {
			int length = 0;
			while (*(string + length) != '\0') {
				length++;
			}
			return length;
		}
		else {
			return 0;
		}
	}


	void append(const char* str) {
		if (!str) return;

		if (!string) {
			string = new char[strlen(str) + 1];
			strcpy(string, str);
			return;
		}

		int stringlen = strlen(string);
		int str_len = strlen(str);
		char* newstr = new char[stringlen + str_len + 1];

		int i = 0;
		while (i < stringlen) {
			newstr[i] = string[i];
			i++;
		}
		int j = 0;
		while (i < stringlen + str_len) {
			newstr[i] = str[j];
			i++;
			j++;
		}
		newstr[i] = '\0';

		delete[] string;
		string = newstr;
	}

	MyString substring(int start, int length) {
		if (!string) {
			return MyString("");
		}
		int len = strlen(string);
		if (start < 0 || start >= len || length < 0 || start + length > len) {
			return MyString("");
		}


		char* newstr = new char[length + 1];
		int j = 0;
		for (int i = start; i < start + length; i++) {
			newstr[j] = string[i];
			j++;
		}
		newstr[length] = '\0';
		MyString result(newstr);
		delete[] newstr;

		return result;
	}


	int find(const char* substr) const {
		if (!substr || !string) return -1;
		int sublen = strlen(substr);
		int len = strlen(string);
		if (sublen > len) return -1;

		int j = 0;
		for (int i = 0; i <= len - sublen; i++) {
			j = 0;
			while (j < sublen && string[i + j] == substr[j]) {
				j++;
			}
			if (j == sublen) {
				return i;
			}
		}
		return -1;
	}

	void to_upper() {
		if (!string) return;

		int len = strlen(string);
		for (int i = 0; i < len; i++) {
			if (string[i] >= 'a' && string[i] <= 'z') {
				string[i] -= 32;
			}
		}
	}

	void show_string() const  {
		if (string) std::cout << string;
	}


	MyString& operator=(const MyString& other) {
		if (this == &other)  return *this;
		delete [] string;
		set_string(other.get_string());

		return *this;
	}


	MyString& operator=(MyString&& other) noexcept {
		if (this == &other)  return *this;
		delete [] string;
		string = other.string;
		other.string = nullptr;
		return *this;
	}


	MyString operator+(const MyString& other) {
		if (!string) return MyString("");
		if (!other.string) return MyString("");

		int len = strlen(other.string);
		int len2 = strlen(string);

		char* newstr = new char[len + len2 + 1];
		strcpy(newstr, string);
		strcat(newstr, other.string);

		MyString result(newstr);
		delete[] newstr;
		return result;
	}

	MyString& operator+=(const MyString& other) {
		if (!other.string) return *this;
		append(other.string);
		return *this;
	}

	bool operator==(const MyString& other) const {
		if (!string && !other.string) return true;
		if (!string || !other.string) return false;
		return strcmp(string, other.string) == 0;
	}

	bool operator!=(const MyString& other) const {
		if (!string && !other.string) return false;
		if (!string || !other.string) return true;
		return strcmp(string, other.string) != 0;
	}


	MyString operator/(int num) const {
		if (!string || !num) return MyString("");
		if (num >= strlen(string) || num < 0) return MyString("");

		int length = strlen(string) - num;
		char* newstr = new char[length + 1];

		for (int i = 0; i < length; i++) {
			newstr[i] = string[i];
		}
		newstr[length] = '\0';

		MyString result(newstr);
		delete[] newstr;
		return result;
	}


	MyString operator*(int num) {
		if (!string || num <= 0) return MyString("");

		int length = strlen(string);
		char* newstr = new char[length * num + 1];

		int k = 0;
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < length; j++) {
				newstr[k++] = string[j];
			}
		}
		newstr[k] = '\0';
		MyString result(newstr);
		delete[] newstr;
		return result;
	}

	const char& operator[](int index) const {
		static char chr = '\0';
		if (string == nullptr) return chr;
		if (index < 0 || index >= strlen(string)) return chr;

		return string[index];
	}

	char& operator[](int index) {
		static char chr = '\0';
		if (string == nullptr) return chr;
		if (index < 0 || index >= strlen(string)) return chr;

		return string[index];
	}

	const char& operator()(int index) const {
		static char chr = '\0';
		if (string == nullptr) return chr;
		if (index < 0 || index >= strlen(string)) return chr;

		return string[index];
	}

	char& operator()(int index) {
		static char chr = '\0';
		if (string == nullptr) return chr;
		if (index < 0 || index >= strlen(string)) return chr;

		return string[index];
	}

	operator float() {
		if (!string) return 0.0;
		int len = strlen(string);
		bool found = false;
		bool is_number = true;
		bool dote_repeat = false;
		int index = 0;
		bool is_negative = false;

		if (string[0] == '-') is_negative = true;

		for (int i = 0; i < len; i++) {
			if (found && string[i] == '.') {
				dote_repeat = true;
			}
			if (string[i] == '.' && i != len - 1) {
				index = i;
				found = true;
			}
			if ((string[i] > '9' || string[i] < '0') && string[i] != '.' && (i == 0 && string[i] == '-') == false) {
				is_number = false;
			}
		}

		std::cout << is_negative << std::endl;
		std::cout << is_number << std::endl;


		if (dote_repeat) return 0.0;
		if (!is_number) return 0.0;
		if (!found) return 0.0;

		float number = 0;
		float dev2 = 0.1;
		int dev1 = 1;
		int start = is_negative ? 1 : 0;
		for (int i = start; i < index - 1; i++) {
			dev1 *= 10;
		}

		for (int i = start; i < len; i++) {
			if (i < index) {
				number += dev1 * (string[i] - '0');
				dev1 /= 10;
			}
			if (i == index) {
				continue;
			}
			if (i > index) {
				number += dev2 * (string[i] - '0');
				dev2 /= 10;
			}
		}

		return is_negative ? -number : number;
	}


	operator int() {
		if (!string) return 0;

		bool is_number = true;
		int len = strlen(string);
		bool is_negative = false;

		if (string[0] == '-') is_negative = true;

		for (int i = 0; i < len; i++) {
			if ((string[i] > '9' || string[i] < '0') && (i == 0 && string[i] == '-') == false) {
				is_number = false;
			}
		}

		if (!is_number) return 0;

		int number = 0;
		int multiplier = 1;
		for (int j = is_negative; j < len - 1; j++) {
			multiplier *= 10;
		}
		for (int i = is_negative; i < len; i++) {
			number += multiplier * (string[i] - '0');
			multiplier /= 10;
		}
		return is_negative ? -number : number;
	}

	operator bool() const {
		return string != nullptr && string[0] != '\0';
	}


	friend std::ostream& operator<<(std::ostream& os, const MyString& str) {
		if (str.string) {
			os << str.string;
		}
		return os;
	}

	friend std::istream& operator>>(std::istream& is, MyString& str) {
		char buffer[1024];

		is.getline(buffer, 1024);
		delete[] str.string;

		str.string = new char[strlen(buffer) + 1];
		strcpy(str.string, buffer);
		return is;
	}


	~MyString() {
		delete[] string;
	}
};


// int main() {
// 	MyString num("-30014");
// 	std::cout << int(num) << std::endl;
// 	return 0;
// }