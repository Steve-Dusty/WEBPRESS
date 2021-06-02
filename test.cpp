#include <stdio.h>
#include <webpress.hpp>

pServer serv(8080, 1);

int main() {
	serv.start();
}
