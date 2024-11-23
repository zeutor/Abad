#include "Application.hpp"


int main() {
	Application app;
	setlocale(LC_ALL, "rus");
	app.INIT();
	app.RUN();
	app.CLOSE();
	return 0;
}