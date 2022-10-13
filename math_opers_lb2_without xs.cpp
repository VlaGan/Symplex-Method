#include <iostream>
#include <string>
#include <cassert>
#include <conio.h>

void print_array(float** mass, const int h, const int w) {
	std::cout << std::endl;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
			std::cout << mass[i][j] << "\t";
		std::cout << std::endl;
	}
}

void get_i_j(float**& mass, const int h, const int w, int* pi, int* pj) {
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

void symplex_method(float**& mas, const int hight, const int width) {
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
	}
	print_array(mas, hight, width);
}

void get_cond(float**& mas, const int hight, const int width, std::string target) {
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
		symplex_method(mas, hight, width);
		return get_cond(mas, hight, width, target);
	}else
		std::cout << std::endl << "F(x) = " << mas[hight - 1][width - 1]<<std::endl;
}

int main() {
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

	print_array(arr, h, w);
	get_cond(arr, h, w, target);

	for (int i = 0; i < h; i++)
		delete[] arr[i];
	delete[] arr;

	return _getch();
}