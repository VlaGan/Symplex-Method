#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <conio.h>
#include <stdlib.h>

void print_array(float** mass, const int h, const int w, std::vector<float> xiv, int x_count) {
	std::cout << std::endl;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
			std::cout << mass[i][j] << "\t";
		std::cout << std::endl;
	}
	std::cout << std::endl << "X"<<x_count<<"(";
	for (int i = 0; i < w - 1; i++) {
		if (i < w - 2)
			std::cout << xiv[i] << ", ";
		else
			std::cout << xiv[i];
	}
	std::cout << ")" << std::endl;
}

void get_i_j(float** mass, const int h, const int w, int* pi, int* pj) {
	int p1 = 0;
	int p2 = 0;

	float minc = mass[h - 1][0];
	for (int i = 0; i < w; i++) {
		if (minc > mass[h - 1][i]) {
			minc = mass[h - 1][i];
			p2 = i;
		}
	}

	float* q = new float[h - 1];
	for (int i = 0; i < h - 1;i++) {
		if (mass[i][p2] > 0) {
			q[i] = mass[i][w - 1] / mass[i][p2];
		}
		else
			q[i] = -100;
	}

	float min = mass[0][w - 1] / mass[0][p2];
	for (int i = 0; i < h - 1;i++) {
		if (q[i] >= 0 && min > q[i]) {
			min = q[i];
			p1 = i;
		}
	}

	delete[] q;
	(*pi) = p1;
	(*pj) = p2;
}

void symplex_method(float** mas, const int hight, const int width, std::vector<std::string> xi,
	std::vector<float> xiv, int x_count) {
	int p1 = 0;
	int p2 = 0;
	get_i_j(mas, hight, width, &p1, &p2);

	if (mas[p1][p2] != 0) {

		if (mas[p1][p2] != 1 && mas[p1][p2] != 0) {
			float det = mas[p1][p2];
			for (int j = 0; j < width; j++) {
				if (p1 != j)
					mas[p1][j] /= det;
			}
			mas[p1][p2] /= det;
		}
			
		for (int i = 0; i < hight; i++) {
			if (i != p1) {
				for (int j = 0; j < width; j++) {
					if (j != p2) {
						mas[i][j] = mas[i][j] + (mas[p1][j] * (-mas[i][p2]));
					}
				}
				mas[i][p2] = 0;
			}
		}

		std::string tmp = xi.at(p1);
		xi.at(p1) = xi.at(width - hight + p2 + 1);
		xi.at(width - hight + p2 + 1) = tmp;

		for (int i = 0; i < width - 1; i++) {
			int index = std::stoi(xi[i])-1;
			if (i <= width - hight)
				xiv[index] = mas[i][width - 1];
			else
				xiv[index] = 0;
		}

	}
	print_array(mas, hight, width, xiv, x_count);
}

void get_cond(float** mas, const int hight, const int width, const std::string target, std::vector<std::string> xi, 
	std::vector<float> xiv, int x_count) {
	bool not_minus_c = true;
	if (target == "max"){
		for (int k = 0; k < width - 1;k++) {
			if (mas[hight - 1][k] < 0) {
				not_minus_c = false;
			}
		}
	}
	else if (target == "min") {
		for (int k = 0; k < width - 1;k++) {
			if (mas[hight - 1][k] > 0) {
				not_minus_c = false;
			}
		}
	}

	if (!not_minus_c) {
		x_count++;
		symplex_method(mas, hight, width, xi, xiv, x_count);
		return get_cond(mas, hight, width, target, xi, xiv, x_count);
	}else
		std::cout << std::endl << "F(x) = " << mas[hight - 1][width - 1] << std::endl;
}

int main(int argc, char* argv[]) {
	int h, w;
	std::string target;

	std::cout<<"Enter the target (max or min): ";
	std::cin >> target;
	std::cout << "Enter the num of rows: ";
	std::cin >> h;
	std::cout << "Enter the num of colls: ";
	std::cin >> w;

	assert((target == "max" || target == "min") && w - h == 2);

	float** arr = new float* [h];
	for (int i = 0; i < h; i++) {
		arr[i] = new float[w];
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			std::cout << "Enter a[" << i + 1 << "][" << j + 1 << "]: ";
			std::cin >> arr[i][j];
		}
	}
	std::cout << std::endl;

	char clr;
	std::cout << "Clear console? (y/n): ";
	std::cin >> clr;
	if(true ? clr == 'y': false)
		system("cls");

	std::vector<std::string> xi(w - 1);
	std::vector<float> xiv(w - 1);
	int rsz = w - h;
	for (int i = 0; i < w - 1; i++) {
		if (i > rsz)						
			xi[i] = std::to_string(i - rsz);
		else
			xi[i] = std::to_string(i + rsz + 1);

		if (i > 1 && i-rsz < h - 1)
			xiv[i] = arr[i-rsz][w - 1];
		else
			xiv[i] = 0;
	}

	int x_cnt = 0;
	print_array(arr, h, w, xiv, x_cnt);
	get_cond(arr, h, w, target, xi, xiv, x_cnt);

	for (int i = 0; i < h; i++)
		delete[] arr[i];
	delete[] arr;

	return _getch();
}