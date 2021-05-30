#include <stdio.h>
#include <webpress.hpp>

pServer serv(8080);

int main() {
	serv.start();
}
